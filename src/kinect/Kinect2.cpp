#include "Kinect2.h"
#include "tools/Config.h"

#ifdef KINECT_V2
#include "ofxKinectV2.h"
#else
#include "ofxKinect.h"
#endif

int KINECT_WIDTH;
int KINECT_HEIGHT;

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

Kinect2ShPtr Kinect2::create(ConfigShPtr config)
{
	Kinect2ShPtr Kinect2_shptr(new Kinect2(config));
	Kinect2_shptr->_weak_ptr = Kinect2_shptr;
	return Kinect2_shptr;
}

Kinect2::~Kinect2()
{
	this->_weak_ptr.reset();
}

Kinect2::Kinect2(ConfigShPtr config)
{
    KINECT_WIDTH = config->_kinect_width;
    KINECT_HEIGHT = config->_kinect_height;

    #ifdef KINECT_V2
    this->_kinect = std::make_shared<ofxKinectV2>();
    std::vector<ofxKinectV2::KinectDeviceInfo> device_list = _kinect->getDeviceList();
    
    ofxKinectV2::Settings ksettings;
    ksettings.enableRGB = true;
    ksettings.enableIR = false;
    ksettings.enableDepth = true;
    ksettings.enableRGBRegistration = true;
    ksettings.config.MinDepth = 0.5;
    ksettings.config.MaxDepth = 8;//4.5;
    
    this->_active = false;
    if (device_list.size() > 0)
    {
        _kinect->open(device_list[0].serial, ksettings);
        this->_active = true;
    }
    #else
    _kinect = std::make_shared<ofxKinect>();
    _kinect->listDevices();
    _kinect->setRegistration(true);
    _kinect->init();
    _kinect->open();//"A00365910253107A");
    //_kinect->open("A00366810115040A");

    
    this->_active = true;
    #endif
    
    this->_is_frame_new = false;
}

//-- METHODS --------------------------------------------------------------------

void Kinect2::update()
{
    if ( this->_active )
    {
        this->_kinect->update();
        
        if (this->_kinect->isFrameNew())
        {
            this->_tex_depth.loadData(_kinect->getDepthPixels());
            #ifdef KINECT_V2
            this->_tex_rgb.loadData(_kinect->getRegisteredPixels());
            #else
            this->_tex_rgb.loadData(_kinect->getPixels());
            #endif
            
            this->_is_frame_new = true;
        }
    }
}

void Kinect2::drawRgb(int width, int height)
{
    ofSetColor(255);
        
    if ( this->_tex_rgb.isAllocated() )
    {
        this->_tex_rgb.draw(0, 0, width, height);
    }
}

ofTexture & Kinect2::getTextureRgb()
{
    return this->_tex_rgb;
}

ofTexture & Kinect2::getTextureDepth()
{
    this->_is_frame_new = false;
    return this->_tex_depth;
}

bool Kinect2::isNew()
{
    return this->_is_frame_new;
}
