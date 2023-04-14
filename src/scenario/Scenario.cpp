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

    dir.listDir();
    dir.sort();
    
    for (int i = 0; i < dir.size(); i++ )
    {
        std::string name = dir.getName(i);
        this->_images.push_back(ofImage("images/"+path+"/"+name));
    }

    this->_video.load("images/"+path+"/tuto.mov");
    this->_video.setLoopState(OF_LOOP_NORMAL);
    this->_video.play();
}

//-- METHODS --------------------------------------------------------------------

void Scenario::bindImages(ofShader & shader)
{
    this->_video.update(); 

    int img_index = 0;
    for ( int i = 0; i < this->_images.size()+1; i++ )
    {
        if (i == 1)
        {
            shader.setUniformTexture("img"+ofToString(i), this->_video.getTexture(), i+2);
        }
        else
        {
            shader.setUniformTexture("img"+ofToString(i), this->_images[img_index].getTexture(), i+2);
            img_index++;
        }
    }
}

void Scenario::unbindImages()
{
    for ( int i = 0; i < this->_images.size(); i++ )
    {
        this->_images[i].getTexture().unbind();
    }
}
