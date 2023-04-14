#ifndef parameter_h
#define parameter_h

#include "tools/Globals.h"

#include "ofxOsc.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Parameter)

//-------------------------------------------------------------------------------

class Parameter
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static ParameterShPtr create(
        const std::string & name, float value, float min, float max, const std::string & suffix = "" );
	virtual ~Parameter();

protected:

	Parameter( const std::string & name, float value, float min, float max, const std::string & suffix );

public:

	ParameterWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
//    void update();
    int draw(int width, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    float mousePressed(int x, int y);
    float mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    float keyPressed(int key);
    void keyReleased(int key);

    float getValue();
    
    static void initOsc(const std::string & mode, const std::string & host, int port);
    
    static void setOscValue(const std::string & id, float value);
    
    static std::map<std::string,ParameterShPtr> _parameters;
    
    float incr(float value);

private:
    
    void setValue(float value);

    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    std::string _name;
    float _value;
    float _min;
    float _max;
    int _id;

    std::string _suffix;
    
    ofRectangle _slider;
    int _width;
    
    bool _hover;
    
    static ofTrueTypeFont _font;

    static int _id_count;
    
    std::map<int,bool> _keypressed;
    
public:
    
    static ofxOscSender _osc_sender;
    static ofxOscReceiver _osc_receiver;
    static std::string _osc_mode;
};

#endif /* parameter_h */
