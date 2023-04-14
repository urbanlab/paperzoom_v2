#include "ofMain.h"
#include "ofApp.h"
#include "tools/Globals.h"

int DEBUG_WIDTH = 1512;
int DEBUG_HEIGHT = 855;

int PROJ_WIDTH = DEBUG_HEIGHT * (1024/848.f);
int PROJ_HEIGHT = DEBUG_HEIGHT;

//

const ofColor STEP_COLOR_0 = ofColor(255,0,0);
const ofColor STEP_COLOR_1 = ofColor(0,255,0);
const ofColor STEP_COLOR_2 = ofColor(0,0,255);
const ofColor STEP_COLOR_3 = ofColor(255,255,0);
const ofColor STEP_COLOR_4 = ofColor(0,255,255);
const ofColor STEP_COLOR_5 = ofColor(255,0,255);
const ofColor STEP_COLOR_6 = ofColor(96,192,0);
const ofColor STEP_COLOR_7 = ofColor(0,96,192);
const ofColor STEP_COLOR_8 = ofColor(96,0,192);
const ofColor STEP_COLOR_9 = ofColor(192,96,0);
const ofColor STEP_COLOR_10 = ofColor(0,192,96);
const ofColor STEP_COLOR_11 = ofColor(192,0,96);


//========================================================================
int main()
{
    ofSetLogLevel(OF_LOG_ERROR);
//    ofSetLogLevel(OF_LOG_VERBOSE);

    ofGLFWWindowSettings settings;
    settings.setPosition(glm::vec2(0,0));
    //settings.setSize(DEFAULT_INTERFACE_WIDTH, DEFAULT_INTERFACE_HEIGHT);
    settings.setGLVersion(3,2);
    settings.windowMode = OF_GAME_MODE;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    shared_ptr<ofApp> mainApp(new ofApp);

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
