#include "ofApp.h"
#include "tools/Config.h"
#include "tools/AppWindow.h"
#include "views/Interface.h"
#include "scenario/Scenario.h"

#include "views/Projection.h"

void ofApp::setup()
{
    this->_config = Config::create("config-kinect1.xml");
    std::shared_ptr<ofAppGLFWWindow> glfw = static_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());
    this->_window = AppWindow::create(this->_config, glfw);

    this->_scenarios.push_back(Scenario::create(this->_config->_dataset));
    return;
}

void ofApp::update()
{
    if (ofGetFrameNum() == 0)
    {
        this->_window->setup();
        this->_interface = Interface::create(this->_config);
    }
    
    this->_interface->update();
}

void ofApp::draw()
{
    ofClear(0);
    ofBackground(0);
    ofSetColor(255);
    
    if ( this->_config->_projection )
    {
        //ofPushMatrix();
        //ofTranslate(PROJECTION_WIDTH, 0);
        this->_interface->draw(this->_scenarios[0]);
        //ofPopMatrix();
    }
    else
    {
        this->_interface->draw(this->_scenarios[0]);
    }
    
    if ( this->_config->_projection )
    {
        /*
        if ( this->_config->_interface_visible )
        {
            ofPushMatrix();
            ofTranslate(INTERFACE_WIDTH, 0);
            this->_interface->getProjectionTexture().draw(0,0, PROJECTION_WIDTH,PROJECTION_HEIGHT);
            ofPopMatrix();
        }
        else
        {
            ofBackground(0);
            this->_interface->getProjectionTexture().draw(0,0, PROJECTION_WIDTH,PROJECTION_HEIGHT);
        }
        */
        
        ofBackground(0);
        this->_interface->getProjectionTexture().draw(0,0, PROJECTION_WIDTH,PROJECTION_HEIGHT);
        
        ofSetColor(0);
        ofMesh left = this->_interface->getProjection()->getLeft();
        left.draw();

        ofMesh top = this->_interface->getProjection()->getTop();
        top.draw();
        
        ofMesh right = this->_interface->getProjection()->getRight();
        right.draw();
        
        ofMesh bottom = this->_interface->getProjection()->getBottom();
        bottom.draw();
       
        ofSetColor(255);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    this->_interface->mouseMoved(x-(this->_config->_projection?PROJECTION_WIDTH:0),y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    this->_interface->mousePressed(x-(this->_config->_projection?PROJECTION_WIDTH:0),y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    this->_interface->mouseDragged(x-(this->_config->_projection?PROJECTION_WIDTH:0),y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    this->_interface->mouseReleased(x-(this->_config->_projection?PROJECTION_WIDTH:0),y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    this->_interface->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    this->_interface->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
