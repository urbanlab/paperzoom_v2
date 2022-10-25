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
        const std::string & name, int value, int min, int max, const std::string & suffix = "" );
	virtual ~Parameter();

protected:

	Parameter( const std::string & name, int value, int min, int max, const std::string & suffix );

public:

	ParameterWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
//    void update();
    int draw(int width, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    int mousePressed(int x, int y);
    int mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    int keyPressed(int key);

    int getValue();
    
    static void initOsc(const std::string & mode, const std::string & host, int port);
    
    static void setOscValue(const std::string & id, int value);
    
    static std::map<std::string,ParameterShPtr> _parameters;
    
private:
    
    void setValue(int value);
    
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    std::string _name;
    int _value;
    int _min;
    int _max;
    int _id;

    std::string _suffix;
    
    ofRectangle _slider;
    int _width;
    
    bool _hover;
    
    static ofTrueTypeFont _font;

    static int _id_count;
    
public:
    
    static ofxOscSender _osc_sender;
    static ofxOscReceiver _osc_receiver;
    static std::string _osc_mode;
};

#endif /* parameter_h */
