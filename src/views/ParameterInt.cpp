#include "ParameterInt.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

std::map<std::string,ParameterIntShPtr> ParameterInt::_parameters;

ParameterIntShPtr ParameterInt::create(const std::string & name, int value, int min, int max, const std::string & suffix )
{
	ParameterIntShPtr ParameterInt_shptr(new ParameterInt(name, value, min, max, suffix));
	ParameterInt_shptr->_weak_ptr = ParameterInt_shptr;
    _parameters[ofToString(ParameterInt_shptr->_id)] = ParameterInt_shptr;
	return ParameterInt_shptr;
}

ParameterInt::~ParameterInt()
{
	this->_weak_ptr.reset();
}

ParameterInt::ParameterInt(const std::string & name, int value, int min, int max, const std::string & suffix)
{
    this->_name = name;
    this->setValue(value);
    this->_min = min;
    this->_max = max;
    this->_suffix = suffix;
    
    this->_hover = false;
    this->_width = 0;
    
    this->_id = ParameterInt::_id_count;
    ParameterInt::_id_count++;
}

ofxOscSender ParameterInt::_osc_sender;
ofxOscReceiver ParameterInt::_osc_receiver;
std::string ParameterInt::_osc_mode;
int ParameterInt::_id_count = 0;

void ParameterInt::initOsc(const std::string & mode, const std::string & host, int port)
{
    ParameterInt::_osc_mode = mode;
    if ( mode == "send" )
    {
        ParameterInt::_osc_sender.setup(host, port);
    }
    else
    {
        ParameterInt::_osc_receiver.setup(port);
    }
}

void ParameterInt::setOscValue(const std::string & id, int value)
{
    ParameterInt::_parameters[id]->setValue(value);
}

//-- METHODS --------------------------------------------------------------------

//void ParameterInt::update()
//{
//
//}

int ParameterInt::draw(int width, ofTrueTypeFont & font)
{
    ofSetColor(255);
    
    this->_width = width;
    int x = 0;
    int y = 0;
    
    y += font.stringHeight(this->_name);
    font.drawString(this->_name, x, y);
    
    x = width - font.stringWidth(ofToString(this->_value)+" "+this->_suffix);
    font.drawString(ofToString(this->_value)+" "+this->_suffix, x, y);
    
    x = 0;
    y += WINDOW_PADDING/4.f;

    if ( this->_hover ) { ofSetColor(255); }
    else { ofSetColor(127); }
    
    ofDrawRectangle(x-1, y-1, width+2, WINDOW_PADDING+2);
    
    if ( this->_hover ) { ofSetColor(64); }
    else { ofSetColor(0); }
    
    this->_slider = ofRectangle(x, y, width, WINDOW_PADDING);
    ofDrawRectangle(this->_slider);

    ofSetColor(255);

    int v = ofMap(this->_value, this->_min, this->_max, 0, width+1 );
    ofDrawRectangle(x, y, v, WINDOW_PADDING);
    
    y += WINDOW_PADDING;
    return y;
}

void ParameterInt::mouseMoved(int x, int y)
{
    if ( this->_slider.inside(x, y) )
    {
        this->_hover = true;
    }
    else
    {
        this->_hover = false;
    }
}

void ParameterInt::setValue(int value)
{
    this->_value = value;
    
    if (ParameterInt::_osc_mode == "send" )
    {
        ofxOscMessage mess;
        mess.setAddress(ofToString(this->_id));
        mess.addIntArg(this->_value);
        ParameterInt::_osc_sender.sendMessage(mess);
    }
}

int ParameterInt::incr(int value)
{
    this->_value += value;
    this->_value = min(this->_value, this->_max);
    this->_value = max(this->_value, this->_min);
    this->setValue(this->_value);
    
    return this->_value;
}

int ParameterInt::mousePressed(int x, int y)
{
    if ( this->_slider.inside(x, y) )
    {
        this->_hover = true;
        this->setValue( ofMap(x, 0, this->_width, this->_min, this->_max+1) );
        return this->_value;
    }
    else
    {
        this->_hover = false;
        return -1;
    }
}

int ParameterInt::mouseDragged(int x, int y)
{
    if ( this->_slider.inside(x, y) )
    {
        this->_hover = true;
        this->setValue( ofMap(x, 0, this->_width, this->_min, this->_max+1) );
        return this->_value;
    }
    else
    {
        this->_hover = false;
        return -1;
    }
}

void ParameterInt::mouseReleased(int x, int y)
{
    this->_hover = false;
}

int ParameterInt::keyPressed(int key)
{
    // 3680 > Shift
    // 3686 > Cmd
    
    if ( this->_hover )
    {        
        this->_keypressed[key] = true;
        float incr = 1;
        if ( _keypressed[3680] ) { incr = 0.1f; }
        if ( _keypressed[3686] ) { incr = 0.01f; }

        if (key == OF_KEY_UP)
        {
            this->_value+=incr;
            this->setValue( min(this->_value, this->_max) );
        }
        else if (key == OF_KEY_DOWN)
        {
            this->_value-=incr;
            this->setValue( max(this->_value, this->_min) );
        }        
    }
    
    return this->_value;
}

void ParameterInt::keyReleased(int key)
{
    this->_keypressed[key] = false;
}


int ParameterInt::getValue()
{
    return this->_value;
}
