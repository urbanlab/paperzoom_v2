#include "Scenario.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

ScenarioShPtr Scenario::create(const std::string & path)
{
	ScenarioShPtr Scenario_shptr(new Scenario(path));
	Scenario_shptr->_weak_ptr = Scenario_shptr;
	return Scenario_shptr;
}

Scenario::~Scenario()
{
	this->_weak_ptr.reset();
}

Scenario::Scenario(const std::string & path)
{
    ofDirectory dir("images/"+path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("jpeg");
    dir.allowExt("mp4");
    dir.allowExt("mov");

    dir.listDir();
    dir.sort();
    
    for (int i = 0; i < dir.size(); i++ )
    {
        std::string name = dir.getName(i);
        std::string ext = ofFilePath::getFileExt(name);

        if ( ext == "mov" || ext == "mp4" )
        {
            this->_videos.insert(std::pair<int,ofVideoPlayer>(i,ofVideoPlayer()));
            this->_videos[i].load("images/"+path+"/"+name);
            this->_videos[i].setLoopState(OF_LOOP_NORMAL);
            this->_videos[i].play();   
        }
        else
        {
            this->_images.insert(std::pair<int,ofImage>(i,ofImage("images/"+path+"/"+name)));
        }
    }
}

//-- METHODS --------------------------------------------------------------------

void Scenario::bindImages(ofShader & shader)
{
    for (std::map<int,ofVideoPlayer>::iterator it=this->_videos.begin(); it!=this->_videos.end(); ++it)
    {
        it->second.update();
        shader.setUniformTexture("img"+ofToString(it->first), it->second.getTexture(), it->first+2);
    }

    for (std::map<int,ofImage>::iterator it=this->_images.begin(); it!=this->_images.end(); ++it) 
    {
        shader.setUniformTexture("img"+ofToString(it->first), it->second.getTexture(), it->first+2);
    }
}

void Scenario::unbindImages()
{
    for (std::map<int,ofVideoPlayer>::iterator it=this->_videos.begin(); it!=this->_videos.end(); ++it)
    {
        it->second.getTexture().unbind();
    }

    for (std::map<int,ofImage>::iterator it=this->_images.begin(); it!=this->_images.end(); ++it) 
    {
        it->second.getTexture().unbind();
    }
}
