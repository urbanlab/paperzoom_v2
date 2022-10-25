#include "Parameter.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

std::map<std::string,ParameterShPtr> Parameter::_parameters;

ParameterShPtr Parameter::create(const std::string & name, int value, int min, int max, const std::string & suffix )
{
	ParameterShPtr Parameter_shptr(new Parameter(name, value, min, max, suffix));
	Parameter_shptr->_weak_ptr = Parameter_shptr;
    _parameters[ofToString(Parameter_shptr->_id)] = Parameter_shptr;
	return Parameter_shptr;
}

Parameter::~Parameter()
{
	this->_weak_ptr.reset();
}

Parameter::Parameter(const std::string & name, int value, int min, int max, const std::string & suffix)
{
    this->_name = name;
    this->setValue(value);
    this->_min = min;
    this->_max = max;
    this->_suffix = suffix;
    
    this->_hover = false;
    this->_width = 0;
    
    this->_id = Parameter::_id_count;
    Parameter::_id_count++;
}

ofxOscSender Parameter::_osc_sender;
ofxOscReceiver Parameter::_osc_receiver;
std::string Parameter::_osc_mode;
int Parameter::_id_count = 0;

void Parameter::initOsc(const std::string & mode, const std::string & host, int port)
{
    Parameter::_osc_mode = mode;
    if ( mode == "send" )
    {
        Parameter::_osc_sender.setup(host, port);
    }
    else
    {
        Parameter::_osc_receiver.setup(port);
    }
}

void Parameter::setOscValue(const std::string & id, int value)
{
    Parameter::_parameters[id]->setValue(value);
}

//-- METHODS --------------------------------------------------------------------

//void Parameter::update()
//{
//
//}

int Parameter::draw(int width, ofTrueTypeFont & font)
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

void Parameter::mouseMoved(int x, int y)
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

void Parameter::setValue(int value)
{
    this->_value = value;
    
    if (Parameter::_osc_mode == "send" )
    {
        ofxOscMessage mess;
        mess.setAddress(ofToString(this->_id));
        mess.addIntArg(this->_value);
        Parameter::_osc_sender.sendMessage(mess);
    }
}

int Parameter::mousePressed(int x, int y)
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

int Parameter::mouseDragged(int x, int y)
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

void Parameter::mouseReleased(int x, int y)
{
    this->_hover = false;
}

int Parameter::keyPressed(int key)
{
    if ( this->_hover )
    {
        if (key == OF_KEY_UP)
        {
            this->_value++;
            this->setValue( min(this->_value, this->_max) );
        }
        else if (key == OF_KEY_DOWN)
        {
            this->_value--;
            this->setValue( max(this->_value, this->_min) );
        }        
    }
    
    return this->_value;
}


int Parameter::getValue()
{
    return this->_value;
}
