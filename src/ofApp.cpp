#include "ofApp.h"
#include "tools/Config.h"
#include "tools/AppWindow.h"
#include "views/Interface.h"
#include "scenario/Scenario.h"

void ofApp::setup()
{
    this->_config = Config::create("config-kinect1.xml");
    std::shared_ptr<ofAppGLFWWindow> glfw = static_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());
    this->_window = AppWindow::create(this->_config, glfw);

    this->_scenarios.push_back(Scenario::create("confluence"));
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
        ofPushMatrix();
        ofTranslate(PROJECTION_WIDTH, 0);
        this->_interface->draw(this->_scenarios[0]);
        ofPopMatrix();
    }
    else
    {
        this->_interface->draw(this->_scenarios[0]);
    }
    
    if ( this->_config->_projection )
    {
//        ofBackground(0);
        this->_interface->getProjectionTexture().draw(0,0, PROJECTION_WIDTH,PROJECTION_HEIGHT);;
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
void ofApp::keyReleased(int key){

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
