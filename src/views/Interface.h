#ifndef interface_h
#define interface_h

#include "tools/Globals.h"

#include "ofxOsc.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Interface)
DEF_CLASS_SH_PTR(Projection)
DEF_CLASS_SH_PTR(Mapping)
DEF_CLASS_SH_PTR(Detection)
DEF_CLASS_SH_PTR(Preview)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(Kinect2)
DEF_CLASS_SH_PTR(Scenario)

//-------------------------------------------------------------------------------

class Interface
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static InterfaceShPtr create(ConfigShPtr config);
	virtual ~Interface();

protected:

	Interface(ConfigShPtr config);

public:

	InterfaceWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void update();
    void draw(ScenarioShPtr scene);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y);
    void mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    void keyPressed(int key);
    void keyReleased(int key);

    ofTexture & getProjectionTexture();
    ProjectionShPtr getProjection();
    
    void save();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ConfigShPtr _config;
    
    Kinect2ShPtr _kinect;

    ofTrueTypeFont _font_h1;
    ofTrueTypeFont _font_h2;
    ofTrueTypeFont _font_p;

    int _mode;
    std::vector<ofRectangle> _buttons;
    ofRectangle _save_button;
    std::vector<bool> _buttons_hover;
    bool _save_button_hover;
    
    ProjectionShPtr _projection;
    int _projection_x, _projection_y;

    MappingShPtr _mapping;
    int _mapping_x, _mapping_y;
    
    DetectionShPtr _detection;
    int _detection_x, _detection_y;
    
    PreviewShPtr _preview;

    ofxOscReceiver _osc_receiver;
    ofxOscSender _osc_sender;
    
    //
    
    std::map<int,bool> _keypressed;
};

#endif /* interface_h */
