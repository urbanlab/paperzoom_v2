#ifndef parameterint_h
#define parameterint_h

#include "tools/Globals.h"

#include "ofxOsc.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(ParameterInt)

//-------------------------------------------------------------------------------

class ParameterInt
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static ParameterIntShPtr create(
        const std::string & name, int value, int min, int max, const std::string & suffix = "" );
	virtual ~ParameterInt();

protected:

	ParameterInt( const std::string & name, int value, int min, int max, const std::string & suffix );

public:

	ParameterIntWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
//    void update();
    int draw(int width, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    int mousePressed(int x, int y);
    int mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    int keyPressed(int key);
    void keyReleased(int key);

    int getValue();
    
    static void initOsc(const std::string & mode, const std::string & host, int port);
    
    static void setOscValue(const std::string & id, int value);
    
    static std::map<std::string,ParameterIntShPtr> _parameters;
    
    int incr(int value);

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
    
    std::map<int,bool> _keypressed;
    
public:
    
    static ofxOscSender _osc_sender;
    static ofxOscReceiver _osc_receiver;
    static std::string _osc_mode;
};

#endif /* parameterint_h */
