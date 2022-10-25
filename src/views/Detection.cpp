#include "Detection.h"
#include "tools/Config.h"
#include "views/Parameter.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

DetectionShPtr Detection::create(ConfigShPtr config)
{
	DetectionShPtr Detection_shptr(new Detection(config));
	Detection_shptr->_weak_ptr = Detection_shptr;
	return Detection_shptr;
}

Detection::~Detection()
{
	this->_weak_ptr.reset();
}

Detection::Detection(ConfigShPtr config)
{
    ofFboSettings fsettings;
    fsettings.width = PROJECTION_WIDTH;
    fsettings.height = PROJECTION_HEIGHT;
    fsettings.numColorbuffers = 1;
    fsettings.colorFormats = std::vector<GLint>({GL_RGB});
    fsettings.useDepth = false;
    fsettings.useStencil = false;
    fsettings.depthStencilAsTexture = false;
    fsettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    fsettings.internalformat = GL_RGB32F_ARB;
    fsettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fsettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    fsettings.minFilter = GL_LINEAR;
    fsettings.maxFilter = GL_LINEAR;
    fsettings.numSamples = 1;

    this->_fbo.allocate(fsettings);
    
    this->_shader.load("shaders/detection.vert", "shaders/detection.frag");

    this->_plane.set(2, 2, 2, 2);
    this->_plane.mapTexCoords(0, 0, 1.f, 1.f);
    
    //
    
    this->_steps = Parameter::create("Intervalles", config->_detection_steps, 1, N_MAX_STEPS, "");
    this->_min = Parameter::create("Distance — min", config->_detection_min, 0, 100, "%");
    this->_max = Parameter::create("Distance — max", config->_detection_max, 1, 100, "%");

    this->_n_steps = config->_detection_steps;
    this->_min_depth = config->_detection_min / 100.f;
    this->_max_depth = config->_detection_max / 100.f;
    
    //
    
//    this->_tex_scale = 1.f;
//    this->_tex_x = 960;
//    this->_tex_y = 540;
//
//    this->_scale = Parameter::create("Scale", 100, 0, 200, "%");
//    this->_pos_x = Parameter::create("Position — x ", 960, 0, PROJECTION_WIDTH, "px");
//    this->_pos_y = Parameter::create("Position — y ", 540, 0, PROJECTION_HEIGHT, "px");
}

//-- METHODS --------------------------------------------------------------------

void Detection::update()
{
}

void Detection::drawFbo(ofTexture & tex)
{    
    this->_fbo.begin();
    this->_shader.begin();
    this->_shader.setUniformTexture("tex", tex, 1);
    this->_shader.setUniform2i("tex_resolution", PROJECTION_WIDTH, PROJECTION_HEIGHT);
    this->_shader.setUniform1i("n_steps", this->_n_steps);
    this->_shader.setUniform1f("min_depth", this->_min_depth);
    this->_shader.setUniform1f("max_depth", this->_max_depth);

//    this->_shader.setUniform1f("scale", this->_tex_scale);
//    this->_shader.setUniform1i("tex_x", this->_tex_x);
//    this->_shader.setUniform1i("tex_y", this->_tex_y);
//
    this->_shader.setUniform4f("color_0", STEP_COLOR_0);
    this->_shader.setUniform4f("color_1", STEP_COLOR_1);
    this->_shader.setUniform4f("color_2", STEP_COLOR_2);
    this->_shader.setUniform4f("color_3", STEP_COLOR_3);
    this->_shader.setUniform4f("color_4", STEP_COLOR_4);
    this->_shader.setUniform4f("color_5", STEP_COLOR_5);
    this->_shader.setUniform4f("color_6", STEP_COLOR_6);
    this->_shader.setUniform4f("color_7", STEP_COLOR_7);
    this->_shader.setUniform4f("color_8", STEP_COLOR_8);
    this->_shader.setUniform4f("color_9", STEP_COLOR_9);
    this->_shader.setUniform4f("color_10", STEP_COLOR_10);
    this->_shader.setUniform4f("color_11", STEP_COLOR_11);
    
    ofClear(0);
    ofBackground(0);
    ofSetColor(255);

    this->_plane.draw();

    tex.unbind();

    this->_shader.end();
    this->_fbo.end();
}

void Detection::draw(int width, int height, ofTexture & tex, ofTrueTypeFont & font)
{
    if ( !tex.isAllocated() ) { return; }
    
    this->drawFbo(tex);

    float ratio = width/(float)height;
    float pratio = PROJECTION_WIDTH/(float)PROJECTION_HEIGHT;
    
    int disp_width = 0;
    int disp_height = 0;
    
    if (ratio > pratio)
    {
        disp_height = height;
        disp_width = disp_height * pratio;
    }
    else
    {
        disp_width = width;
        disp_height = disp_width / pratio;
    }
    
    ofSetColor(127);
    ofDrawRectangle(-2, -2, disp_width+4, disp_height+4);
    
    ofSetColor(255);
    this->_fbo.draw(0, 0, disp_width, disp_height);
    
    //
    
    int x = disp_width + WINDOW_PADDING;
    int y = 0;
    int w = width - disp_width - WINDOW_PADDING;
   
    for ( int i = 0; i < this->_n_steps; i++ )
    {
        ofSetColor(255);
        std::string str = ofToString(i+1)+".";
        int h = font.stringHeight(str);
        y += h;
        font.drawString(str, x, y);
        y -= h;
        
        switch(i)
        {
            case 0 :  ofSetColor(STEP_COLOR_0); break;
            case 1 :  ofSetColor(STEP_COLOR_1); break;
            case 2 :  ofSetColor(STEP_COLOR_2); break;
            case 3 :  ofSetColor(STEP_COLOR_3); break;
            case 4 :  ofSetColor(STEP_COLOR_4); break;
            case 5 :  ofSetColor(STEP_COLOR_5); break;
            case 6 :  ofSetColor(STEP_COLOR_6); break;
            case 7 :  ofSetColor(STEP_COLOR_7); break;
            case 8 :  ofSetColor(STEP_COLOR_8); break;
            case 9 :  ofSetColor(STEP_COLOR_9); break;
            case 10 : ofSetColor(STEP_COLOR_10); break;
            case 11 : ofSetColor(STEP_COLOR_11); break;
        }
       
        int offset = font.stringWidth(str)+WINDOW_PADDING/4;
        int ww = w - offset;
        ofDrawRectangle(x+offset, y, ww, h);
        y += h + WINDOW_PADDING/4;
    }
    
    y += WINDOW_PADDING;
    
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_steps_x = x;
    this->_steps_y = y;
    y += this->_steps->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_min_x = x;
    this->_min_y = y;
    y += this->_min->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_max_x = x;
    this->_max_y = y;
    y += this->_max->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    //
    
//    ofPushMatrix();
//    ofTranslate(x,y);
//    this->_scale_x = x;
//    this->_scale_y = y;
//    y += this->_scale->draw(w, font) + WINDOW_PADDING/2;
//    ofPopMatrix();
//
//    ofPushMatrix();
//    ofTranslate(x,y);
//    this->_pos_x_x = x;
//    this->_pos_x_y = y;
//    y += this->_pos_x->draw(w, font) + WINDOW_PADDING/2;
//    ofPopMatrix();
//
//    ofPushMatrix();
//    ofTranslate(x,y);
//    this->_pos_y_x = x;
//    this->_pos_y_y = y;
//    y += this->_pos_y->draw(w, font) + WINDOW_PADDING/2;
//    ofPopMatrix();
}

ofTexture & Detection::getFboTexture()
{
    return this->_fbo.getTexture();
}


int Detection::getMin() { return this->_min->getValue(); }
int Detection::getMax() { return this->_max->getValue(); }
int Detection::getSteps() { return this->_steps->getValue(); }

void Detection::mouseMoved(int x, int y)
{
    this->_steps->mouseMoved(x-this->_steps_x, y-this->_steps_y);
    this->_min->mouseMoved(x-this->_min_x, y-this->_min_y);
    this->_max->mouseMoved(x-this->_max_x, y-this->_max_y);
    
//    this->_scale->mouseMoved(x-this->_scale_x, y-this->_scale_y);
//    this->_pos_x->mouseMoved(x-this->_pos_x_x, y-this->_pos_x_y);
//    this->_pos_y->mouseMoved(x-this->_pos_y_x, y-this->_pos_y_y);
}

void Detection::mousePressed(int x, int y)
{
    int value = -1;
    
    if ( (value = this->_steps->mousePressed(x-this->_steps_x, y-this->_steps_y)) >= 0 )
    {
        this->_n_steps = value;
    }
    if ( (value = this->_min->mousePressed(x-this->_min_x, y-this->_min_y)) >= 0 )
    {
        this->_min_depth = value / 100.f;
    }
    if ( (value = this->_max->mousePressed(x-this->_max_x, y-this->_max_y)) >= 0 )
    {
        this->_max_depth = value / 100.f;
    }
//
//    if ( (value = this->_scale->mousePressed(x-this->_scale_x, y-this->_scale_y)) >= 0 )
//    {
//        this->_tex_scale = value / 100.f;
//    }
//    if ( (value = this->_pos_x->mousePressed(x-this->_pos_x_x, y-this->_pos_x_y)) >= 0 )
//    {
//        this->_tex_x = value;
//    }
//    if ( (value = this->_pos_y->mousePressed(x-this->_pos_y_x, y-this->_pos_y_y)) >= 0 )
//    {
//        this->_tex_y = value;
//    }
}

void Detection::mouseDragged(int x, int y)
{
    int value = -1;
    
    if ( (value = this->_steps->mousePressed(x-this->_steps_x, y-this->_steps_y)) >= 0 )
    {
        this->_n_steps = value;
    }
    if ( (value = this->_min->mousePressed(x-this->_min_x, y-this->_min_y)) >= 0 )
    {
        this->_min_depth = value / 100.f;
    }
    if ( (value = this->_max->mousePressed(x-this->_max_x, y-this->_max_y)) >= 0 )
    {
        this->_max_depth = value / 100.f;
    }
    
//    if ( (value = this->_scale->mousePressed(x-this->_scale_x, y-this->_scale_y)) >= 0 )
//    {
//        this->_tex_scale = value / 100.f;
//    }
//    if ( (value = this->_pos_x->mousePressed(x-this->_pos_x_x, y-this->_pos_x_y)) >= 0 )
//    {
//        this->_tex_x = value;
//    }
//    if ( (value = this->_pos_y->mousePressed(x-this->_pos_y_x, y-this->_pos_y_y)) >= 0 )
//    {
//        this->_tex_y = value;
//    }
}

void Detection::mouseReleased(int x, int y)
{
    this->_steps->mouseReleased(x-this->_steps_x, y-this->_steps_y);
    this->_min->mouseReleased(x-this->_min_x, y-this->_min_y);
    this->_max->mouseReleased(x-this->_max_x, y-this->_max_y);
    
//    this->_scale->mouseReleased(x-this->_scale_x, y-this->_scale_y);
//    this->_pos_x->mouseReleased(x-this->_pos_x_x, y-this->_pos_x_y);
//    this->_pos_y->mouseReleased(x-this->_pos_y_x, y-this->_pos_y_y);
}

void Detection::keyPressed(int key)
{
    int value = -1;
    
    if ( value = this->_steps->keyPressed(key) )
    {
        this->_n_steps = value;
    }
    
    if ( value = this->_min->keyPressed(key) )
    {
        this->_min_depth = value / 100.f;
    }
    
    if ( value = this->_max->keyPressed(key) )
    {
        this->_max_depth = value / 100.f;
    }
    
//    if ( value = this->_scale->keyPressed(key) )
//    {
//        this->_tex_scale = value / 100.f;
//    }
//
//    if ( value = this->_pos_x->keyPressed(key) )
//    {
//        this->_tex_x = value;
//    }
//
//    if ( value = this->_pos_y->keyPressed(key) )
//    {
//        this->_tex_y = value;
//    }
}
