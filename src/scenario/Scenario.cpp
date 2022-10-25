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
}

//-- METHODS --------------------------------------------------------------------

void Scenario::bindImages(ofShader & shader)
{
    for ( int i = 0; i < this->_images.size(); i++ )
    {
        shader.setUniformTexture("img"+ofToString(i), this->_images[i].getTexture(), i+2);
    }
}

void Scenario::unbindImages()
{
    for ( int i = 0; i < this->_images.size(); i++ )
    {
        this->_images[i].getTexture().unbind();
    }
}
