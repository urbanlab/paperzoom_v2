#ifndef kinect2_h
#define kinect2_h

#include "ofMain.h"
//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Kinect2)
#ifdef KINECT_V2
DEF_CLASS_SH_PTR(ofxKinectV2)
#else
DEF_CLASS_SH_PTR(ofxKinect)
#endif
DEF_CLASS_SH_PTR(Config)

//-------------------------------------------------------------------------------

class Kinect2
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static Kinect2ShPtr create(ConfigShPtr config);
	virtual ~Kinect2();

protected:

	Kinect2(ConfigShPtr config);
    
public:

	Kinect2WkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void update();
    void drawRgb(int width, int height);
    ofTexture & getTextureRgb();
    ofTexture & getTextureDepth();
    
    bool isNew();

	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    #ifdef KINECT_V2
    ofxKinectV2ShPtr _kinect;
    #else
    ofxKinectShPtr _kinect;
    #endif
    
    bool _active;
    
    ofTexture _tex_rgb;
    ofTexture _tex_depth;
    
    bool _is_frame_new;
};

#endif /* kinect2_h */
