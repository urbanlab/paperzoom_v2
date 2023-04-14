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

    //

    this->_dataset = this->_xml.getValue("dataset", "");

    //
    
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
    
    this->_xml.pushTag("mask"); this->_xml.pushTag("top_left");
    this->_mask_tlx = this->_xml.getValue("x", 0.f);
    this->_mask_tly = this->_xml.getValue("y", 0.f);
    this->_xml.popTag(); this->_xml.pushTag("top_right");
    this->_mask_trx = this->_xml.getValue("x", 0.f);
    this->_mask_try = this->_xml.getValue("y", 0.f);
    this->_xml.popTag(); this->_xml.pushTag("bottom_left");
    this->_mask_blx = this->_xml.getValue("x", 0.f);
    this->_mask_bly = this->_xml.getValue("y", 0.f);
    this->_xml.popTag(); this->_xml.pushTag("bottom_right");
    this->_mask_brx = this->_xml.getValue("x", 0.f);
    this->_mask_bry = this->_xml.getValue("y", 0.f);
    this->_xml.popTag(); this->_xml.popTag();

    this->_offset_tex_x =  this->_xml.getValue("offset_tex_x", 0.f);
    this->_offset_tex_y =  this->_xml.getValue("offset_tex_y", 0.f);

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
    this->_detection_scale = this->_xml.getValue("scale", 100.f);
    this->_detection_rot = this->_xml.getValue("rotation", 0.f);

    this->_detection_min = this->_xml.getValue("min_depth", 100.f);
    this->_detection_max = this->_xml.getValue("max_depth", 100.f);
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

void Config::setMaskTLx(float value) { this->_mask_tlx = value; }
void Config::setMaskTLy(float value) { this->_mask_tly = value; }
void Config::setMaskTRx(float value) { this->_mask_trx = value; }
void Config::setMaskTRy(float value) { this->_mask_try = value; }
void Config::setMaskBLx(float value) { this->_mask_blx = value; }
void Config::setMaskBLy(float value) { this->_mask_bly = value; }
void Config::setMaskBRx(float value) { this->_mask_brx = value; }
void Config::setMaskBRy(float value) { this->_mask_bry = value; }

void Config::setOffsetTexX(float value) { this->_offset_tex_x = value; }
void Config::setOffsetTexY(float value) { this->_offset_tex_y = value; }

void Config::setDetectionScale(float value) { this->_detection_scale = value; }
void Config::setDetectionX(float value) { this->_detection_x = value; }
void Config::setDetectionY(float value) { this->_detection_y = value; }
void Config::setDetectionRot(float value) { this->_detection_rot = value; }

void Config::setDetectionMin(float value) { this->_detection_min = value; }
void Config::setDetectionMax(float value) { this->_detection_max = value; }
void Config::setDetectionSteps(int value)  { this->_detection_steps = value; }

void Config::save()
{
    this->_xml.pushTag("config");

    this->_xml.pushTag("projection");
    this->_xml.pushTag("mask");
    
    this->_xml.pushTag("top_left");
    this->_xml.setValue("x", (int)this->_mask_tlx);
    this->_xml.setValue("y", (int)this->_mask_tly);
    this->_xml.popTag();
    
    this->_xml.pushTag("top_right");
    this->_xml.setValue("x", (int)this->_mask_trx);
    this->_xml.setValue("y", (int)this->_mask_try);
    this->_xml.popTag();
    
    this->_xml.pushTag("bottom_left");
    this->_xml.setValue("x", (int)this->_mask_blx);
    this->_xml.setValue("y", (int)this->_mask_bly);
    this->_xml.popTag();
    
    this->_xml.pushTag("bottom_right");
    this->_xml.setValue("x", (int)this->_mask_brx);
    this->_xml.setValue("y", (int)this->_mask_bry);
    this->_xml.popTag();
    
    this->_xml.popTag();
    
    this->_xml.setValue("offset_tex_x", ofToString(this->_offset_tex_x,2));
    this->_xml.setValue("offset_tex_y", ofToString(this->_offset_tex_y,2));

    this->_xml.popTag();
    
    this->_xml.pushTag("detection");

    this->_xml.setValue("x", ofToString(this->_detection_x,2));
    this->_xml.setValue("y", ofToString(this->_detection_y,2));
    this->_xml.setValue("scale", ofToString(this->_detection_scale,2));
    this->_xml.setValue("rotation", ofToString(this->_detection_rot,2));

    this->_xml.setValue("min_depth", ofToString(this->_detection_min,2));
    this->_xml.setValue("max_depth", ofToString(this->_detection_max,2));
    this->_xml.setValue("n_steps", (int)this->_detection_steps);

    this->_xml.popTag();
            
    this->_xml.popTag(); // config
    this->_xml.saveFile("config-kinect1.xml");
}
