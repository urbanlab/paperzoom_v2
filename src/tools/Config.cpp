#include "Config.h"

const int DEFAULT_INTERFACE_WIDTH = 1512;
const int DEFAULT_INTERFACE_HEIGHT = 855;

const int DEFAULT_PROJECTION_WIDTH = 1920;
const int DEFAULT_PROJECTION_HEIGHT = 1080;

const int DEFAULT_KINECT_WIDTH = 1024;
const int DEFAULT_KINECT_HEIGHT = 848;

const int N_MAX_STEPS = 12;

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

ConfigShPtr Config::create(const std::string & path)
{
	ConfigShPtr Config_shptr(new Config(path));
	Config_shptr->_weak_ptr = Config_shptr;
	return Config_shptr;
}

Config::~Config()
{
	this->_weak_ptr.reset();
}

Config::Config(const std::string & path)
{
    this->_xml.load(path);
    
    this->_xml.pushTag("config");
    
    this->_projection = this->_xml.getValue("production", false);
    
    // INTERFACE
    
    this->_xml.pushTag("interface");

    this->_interface_visible = this->_xml.getValue("visible", true);
    this->_interface_width = this->_xml.getValue("width", DEFAULT_INTERFACE_WIDTH);
    this->_interface_height = this->_xml.getValue("height", DEFAULT_INTERFACE_HEIGHT);
    this->_interface_fullscreen = this->_xml.getValue("fullscreen", false);
    this->_interface_x = this->_xml.getValue("x", 0);
    this->_interface_y = this->_xml.getValue("y", 0);
    this->_interface_center = this->_xml.getValue("center", false);
    this->_interface_fps = this->_xml.getValue("fps", 30.f);
    this->_interface_vsync = this->_xml.getValue("vsync", false);
    this->_interface_padding = this->_xml.getValue("padding", 0);

    this->_xml.popTag();

    // PROJECTION
    
    this->_xml.pushTag("projection");

    this->_projection_width = this->_xml.getValue("width", DEFAULT_PROJECTION_WIDTH);
    this->_projection_height = this->_xml.getValue("height", DEFAULT_PROJECTION_HEIGHT);
    
    this->_xml.pushTag("mask"); this->_xml.pushTag("left");
    this->_mask_l1 = this->_xml.getValue("p1", 0);
    this->_mask_l2 = this->_xml.getValue("p2", 0);
    this->_xml.popTag(); this->_xml.pushTag("top");
    this->_mask_t1 = this->_xml.getValue("p1", 0);
    this->_mask_t2 = this->_xml.getValue("p2", 0);
    this->_xml.popTag(); this->_xml.pushTag("right");
    this->_mask_r1 = this->_xml.getValue("p1", 0);
    this->_mask_r2 = this->_xml.getValue("p2", 0);
    this->_xml.popTag(); this->_xml.pushTag("bottom");
    this->_mask_b1 = this->_xml.getValue("p1", 0);
    this->_mask_b2 = this->_xml.getValue("p2", 0);
    this->_xml.popTag(); this->_xml.popTag();

    this->_xml.popTag();

    // KINECT
    
    this->_xml.pushTag("kinect");

    #ifdef KINECT_V2
    this->_kinect_width = 512; //this->_xml.getValue("width", DEFAULT_KINECT_WIDTH);
    this->_kinect_height = 424; //this->_xml.getValue("height", DEFAULT_KINECT_HEIGHT);
    #else
    this->_kinect_width = 640; //this->_xml.getValue("width", DEFAULT_KINECT_WIDTH);
    this->_kinect_height = 480; //this->_xml.getValue("height", DEFAULT_KINECT_HEIGHT);
    #endif
    
    this->_xml.popTag();

    // DETECTION
    
    this->_xml.pushTag("detection");

    this->_detection_x = this->_xml.getValue("x", this->_kinect_width/2.f);
    this->_detection_y = this->_xml.getValue("y", this->_kinect_height/2.f);
    this->_detection_scale = this->_xml.getValue("scale", 100);
    this->_detection_rot = this->_xml.getValue("rotation", 0);

    this->_detection_min = this->_xml.getValue("min_depth", 100);
    this->_detection_max = this->_xml.getValue("max_depth", 100);
    this->_detection_steps = this->_xml.getValue("n_steps", N_MAX_STEPS);
    this->_detection_steps = std::min(this->_detection_steps, N_MAX_STEPS);
    
    this->_xml.popTag();

        
    // OSC
    
    this->_xml.pushTag("osc");
    
    this->_osc_mode = this->_xml.getValue("mode", "send");
    this->_osc_host = this->_xml.getValue("host", "localhost");
    this->_osc_port = this->_xml.getValue("port", 0);

    this->_xml.popTag();

    
    this->_xml.popTag(); // config
}

//-- METHODS --------------------------------------------------------------------

void Config::setMaskL1(int value) { this->_mask_l1 = value; }
void Config::setMaskL2(int value) { this->_mask_l2 = value; }
void Config::setMaskT1(int value) { this->_mask_t1 = value; }
void Config::setMaskT2(int value) { this->_mask_t2 = value; }
void Config::setMaskR1(int value) { this->_mask_r1 = value; }
void Config::setMaskR2(int value) { this->_mask_r2 = value; }
void Config::setMaskB1(int value) { this->_mask_b1 = value; }
void Config::setMaskB2(int value) { this->_mask_b2 = value; }

void Config::setDetectionScale(int value) { this->_detection_scale = value; }
void Config::setDetectionX(int value) { this->_detection_x = value; }
void Config::setDetectionY(int value) { this->_detection_y = value; }
void Config::setDetectionRot(int value) { this->_detection_rot = value; }

void Config::setDetectionMin(int value) { this->_detection_min = value; }
void Config::setDetectionMax(int value) { this->_detection_max = value; }
void Config::setDetectionSteps(int value)  { this->_detection_steps = value; }

void Config::save()
{
    this->_xml.pushTag("config");

    this->_xml.pushTag("projection");

    this->_xml.pushTag("mask"); this->_xml.pushTag("left");
    this->_xml.setValue("p1", this->_mask_l1);
    this->_xml.setValue("p2", this->_mask_l2);
    
    this->_xml.popTag(); this->_xml.pushTag("top");
    this->_xml.setValue("p1", this->_mask_t1);
    this->_xml.setValue("p2", this->_mask_t2);
    
    this->_xml.popTag(); this->_xml.pushTag("right");
    this->_xml.setValue("p1", this->_mask_r1);
    this->_xml.setValue("p2", this->_mask_r2);
    
    this->_xml.popTag(); this->_xml.pushTag("bottom");
    this->_xml.setValue("p1", this->_mask_b1);
    this->_xml.setValue("p2", this->_mask_b2);
    
    this->_xml.popTag(); this->_xml.popTag();
    this->_xml.popTag();
    
    this->_xml.pushTag("detection");

    this->_xml.setValue("x", this->_detection_x);
    this->_xml.setValue("y", this->_detection_y);
    this->_xml.setValue("scale", this->_detection_scale);
    this->_xml.setValue("rotation", this->_detection_rot);

    this->_xml.setValue("min_depth", this->_detection_min);
    this->_xml.setValue("max_depth", this->_detection_max);
    this->_xml.setValue("n_steps", this->_detection_steps);

    this->_xml.popTag();
            
    this->_xml.popTag(); // config
    this->_xml.saveFile("config-kinect1.xml");
}
