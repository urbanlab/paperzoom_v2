#ifndef config_h
#define config_h

//-------------------------------------------------------------------------------

#include "ofxXmlSettings.h"

#include "Globals.h"
#include "Macro.h"
DEF_CLASS_SMART_PTR(Config)

//-------------------------------------------------------------------------------

class Config
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static ConfigShPtr create(const std::string & path);
	virtual ~Config();

protected:

	Config(const std::string & path);

public:

	ConfigWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void setMaskL1(int value);
    void setMaskL2(int value);
    void setMaskT1(int value);
    void setMaskT2(int value);
    void setMaskR1(int value);
    void setMaskR2(int value);
    void setMaskB1(int value);
    void setMaskB2(int value);

    void setDetectionScale(int value);
    void setDetectionX(int value);
    void setDetectionY(int value);
    void setDetectionRot(int value);

    void setDetectionMin(int value);
    void setDetectionMax(int value);
    void setDetectionSteps(int value);
    
    void save();
    
    //-- ATTRIBUTES -------------------------------------------------------------

public :

    bool _projection;

	// INTERFACE

    bool _interface_visible;
	int _interface_width;
	int _interface_height;
	bool _interface_fullscreen;
	int _interface_x;
	int _interface_y;
	bool _interface_center;
	float _interface_fps;
	bool _interface_vsync;
	int _interface_padding;

	// PROJECTION

    int _projection_width;
    int _projection_height;
    int _mask_l1, _mask_l2;
    int _mask_t1, _mask_t2;
    int _mask_r1, _mask_r2;
    int _mask_b1, _mask_b2;

    // KINECT
    
    int _kinect_width;
    int _kinect_height;
    
    // DETECTION
    
    int _detection_x;
    int _detection_y;
    int _detection_scale;
    int _detection_rot;

    int _detection_min;
    int _detection_max;
    int _detection_steps;
    
    // OSC
    
    std::string _osc_mode;
    std::string _osc_host;
    int _osc_port;

private :
    
    ofxXmlSettings _xml;
    
};

#endif /* config_h */
