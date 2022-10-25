#pragma once

//#define KINECT_V2

#include "ofMain.h"
#include "ofxXmlSettings.h"

#ifdef KINECT_V2
#include "ofxKinectV2.h"
#else
#include "ofxKinect.h"
#endif

//

#include "tools/Macro.h"
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(AppWindow)
DEF_CLASS_SH_PTR(Interface)
DEF_CLASS_SH_PTR(Scenario)

//

class ofApp : public ofBaseApp{

	public:
  
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
        
    private:
    
    ConfigShPtr _config;
    AppWindowShPtr _window;

    InterfaceShPtr _interface;
    
    std::vector<ScenarioShPtr> _scenarios;

    //
    
    #ifdef KINECT_V2
    std::shared_ptr<ofxKinectV2> _kinect;
    #else
    std::shared_ptr<ofxKinect> _kinect;
    #endif

    ofTexture _tex_RGB;
    ofTexture _tex_IR;
    ofTexture _tex_Depth;
    ofTexture _tex_RGBRegistered;
    
    //
    
    ofPlanePrimitive _plane; 

    ofFbo _fbo;
    ofFbo _fbo2;
    ofFbo _fbo3;
    ofShader _shader;
    ofShader _shader2;

    //
    
    std::vector<ofImage> _images;
    
//    //
//    
//    int _cv_threshold;
//    bool _cv_learn_bg;
//    
//    ofxCvColorImage _cv_color;
//    ofxCvGrayscaleImage _cv_gray;
//    ofxCvGrayscaleImage _cv_bg;
//    ofxCvGrayscaleImage _cv_diff;
//    ofxCvContourFinder _cv_finder;
};
