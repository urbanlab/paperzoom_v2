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
    
    void setMaskTLx(float value);
    void setMaskTLy(float value);
    void setMaskTRx(float value);
    void setMaskTRy(float value);
    void setMaskBLx(float value);
    void setMaskBLy(float value);
    void setMaskBRx(float value);
    void setMaskBRy(float value);
    
    void setOffsetTexX(float value);
    void setOffsetTexY(float value);

    void setDetectionScale(float value);
    void setDetectionX(float value);
    void setDetectionY(float value);
    void setDetectionRot(float value);

    void setDetectionMin(float value);
    void setDetectionMax(float value);
    void setDetectionSteps(int value);
    
    void save();
    
    //-- ATTRIBUTES -------------------------------------------------------------

public :

    bool _projection;

    //

    std::string _dataset;

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
    float _mask_tlx, _mask_tly;
    float _mask_trx, _mask_try;
    float _mask_blx, _mask_bly;
    float _mask_brx, _mask_bry;
    
    float _offset_tex_x;
    float _offset_tex_y;

    // KINECT
    
    int _kinect_width;
    int _kinect_height;
    
    // DETECTION
    
    float _detection_x;
    float _detection_y;
    float _detection_scale;
    float _detection_rot;

    float _detection_min;
    float _detection_max;
    int _detection_steps;
    
    // OSC
    
    std::string _osc_mode;
    std::string _osc_host;
    int _osc_port;

private :
    
    ofxXmlSettings _xml;
    
};

#endif /* config_h */
