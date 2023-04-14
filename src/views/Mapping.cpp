#include "Mapping.h"
#include "tools/Config.h"
#include "views/Parameter.h"
#include "filter/Blur.h"
#include "filter/Smooth.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

MappingShPtr Mapping::create(ConfigShPtr config)
{
	MappingShPtr Mapping_shptr(new Mapping(config));
	Mapping_shptr->_weak_ptr = Mapping_shptr;
	return Mapping_shptr;
}

Mapping::~Mapping()
{
	this->_weak_ptr.reset();
}

Mapping::Mapping(ConfigShPtr config)
{
    //
    
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
    
    fsettings.width = max(KINECT_WIDTH, KINECT_HEIGHT);
    fsettings.height = fsettings.width;
    
    this->_fbo_square.allocate(fsettings);

    this->_shader.load("shaders/mapping.vert", "shaders/mapping.frag");
    
    this->_plane.set(2, 2, 2, 2);
    this->_plane.mapTexCoords(0, 0, 1.f, 1.f);
    
    this->_mesh = _plane.getMesh();
    
    //
    
    this->_scale = Parameter::create("Scale", config->_detection_scale, 0, 500, "%");
    this->_pos_x = Parameter::create("Position — x ", config->_detection_x, 0, PROJECTION_WIDTH, "px");
    this->_pos_y = Parameter::create("Position — y ", config->_detection_y, 0, PROJECTION_HEIGHT, "px");
    this->_rot = Parameter::create("Rotation", config->_detection_rot, 0, 360, "°");
    
    //
    
    this->_tex_scale = config->_detection_scale / 100.f;
    this->_tex_x = config->_detection_x;
    this->_tex_y = config->_detection_y;
    this->_tex_rot = config->_detection_rot;
    
    //

    this->_smooth = Smooth::create();
    this->_smooth2 = Smooth::create();
    this->_blur = Blur::create();
    
    //this->_debug.load("out.png");
    
    //
    
    this->_tex_mode = false;
}

void Mapping::update()
{
    this->updateValues();
}

void Mapping::drawFbo(ofTexture & tex, bool is_new)
{
    this->_smooth->process(tex);
    this->_smooth2->process(this->_smooth->getFboTexture());

    if (is_new)
    {
        ofEnableArbTex();
        ofPixels px; this->_smooth2->getFboTexture().readToPixels(px);
        ofTexture tx; tx.allocate(px);

        _blur_textures.push_back(tx);
        if ( this->_blur_textures.size() > 10 )
        {
            this->_blur_textures.erase(this->_blur_textures.begin());
        }
    }
    
    this->_blur->process(this->_blur_textures);

    ofTexture & texture = this->_blur->getFboTexture();
    
    this->_mesh = _plane.getMesh();
    std::vector<glm::vec2> texcoords = this->_mesh.getTexCoords();

    float scale_x = this->_fbo_square.getWidth()/(float)KINECT_WIDTH;
    float scale_y = this->_fbo_square.getHeight()/(float)KINECT_HEIGHT;
    
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        texcoords[i] -= glm::vec2(0.5);
        texcoords[i] *= glm::vec2(scale_x, scale_y);
        texcoords[i] += glm::vec2(0.5);
        this->_mesh.setTexCoord(i, texcoords[i]);
    }

    // draw kinect in square for non deforming rotation (next step)

    this->_fbo_square.begin();
    this->_shader.begin();
    this->_shader.setUniformTexture("tex", texture, 1);
    this->_shader.setUniform2i("tex_resolution", KINECT_WIDTH, KINECT_HEIGHT);
    this->_shader.setUniform1i("vflip", 0);

    ofClear(0);
    ofBackground(0);
    ofSetColor(255);

    this->_mesh.draw();

    texture.unbind();

    this->_shader.end();
    this->_fbo_square.end();
    
    // draw kinect frame in projection space

    float pratio = PROJECTION_WIDTH/(float)PROJECTION_HEIGHT;
    float kratio = 1;//KINECT_WIDTH/(float)KINECT_HEIGHT;
    
    int w = 0;
    int h = 0;
    
    if (pratio > kratio)
    {
        w = PROJECTION_WIDTH;
        h = w / kratio;
    }
    else
    {
        h = PROJECTION_HEIGHT;
        w = h * kratio;
    }
        
    //
    
    scale_x = w/(float)PROJECTION_WIDTH;
    scale_y = h/(float)PROJECTION_HEIGHT;
    
    this->_mesh = _plane.getMesh();
    texcoords = this->_mesh.getTexCoords();
    
    // go to center
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        texcoords[i] -= glm::vec2(0.5);
    }

    // scale to projection size (contain mode)
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        texcoords[i] /= glm::vec2(scale_x, scale_y);
    }
    
    // config scale
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        texcoords[i] /= glm::vec2(this->_tex_scale);
    }
    
    // config transform
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        glm::vec2 center = glm::vec2(PROJECTION_WIDTH/2,PROJECTION_HEIGHT/2);
        
        float x = this->_tex_x;
        float y = this->_tex_y;
                
        x = (x-center.x)/(float)PROJECTION_WIDTH;
        y = (y-center.y)/(float)PROJECTION_HEIGHT;

        texcoords[i] += glm::vec2(x,y);
    }
    
    // config rotation
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        ofVec2f v = ofVec2f(texcoords[i].x,texcoords[i].y);
        v.rotate(this->_tex_rot);
        texcoords[i] = glm::vec2(v.x,v.y);
    }

    // go back to origin
    for ( int i = 0; i < texcoords.size(); i++ )
    {
        texcoords[i] += glm::vec2(0.5);
    }

    for ( int i = 0; i < texcoords.size(); i++ )
    {
        this->_mesh.setTexCoord(i, texcoords[i]);
    }
    
    //
//    ofImage img;
//    img.load("hello.png");
    
    this->_fbo.begin();
    this->_shader.begin();
    this->_shader.setUniformTexture("tex", this->_fbo_square.getTexture(), 1);
    this->_shader.setUniform1i("vflip", 1);
    this->_shader.setUniform2i("tex_resolution", this->_fbo_square.getWidth(), this->_fbo_square.getHeight());
//    this->_shader.setUniformTexture("tex", tex, 1);
//    this->_shader.setUniform2i("tex_resolution", tex.getWidth(), tex.getHeight());
    ofClear(0);
    ofBackground(0);
    ofSetColor(255);

    this->_mesh.draw();

    this->_fbo_square.getTexture().unbind();

    this->_shader.end();
    this->_fbo.end();
}

void Mapping::draw(int width, int height, ofTexture & tex, ofTexture & tex2, bool is_new, ofTrueTypeFont & font)
{
    if (this->_tex_mode) { this->drawFbo(tex2, is_new); }
    else { this->drawFbo(tex, is_new); }
    
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
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_scale_x = x;
    this->_scale_y = y;
    y += this->_scale->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_pos_x_x = x;
    this->_pos_x_y = y;
    y += this->_pos_x->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_pos_y_x = x;
    this->_pos_y_y = y;
    y += this->_pos_y->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_rot_x = x;
    this->_rot_y = y;
    y += this->_rot->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(x,y);
    int strh = font.stringHeight("Mode");
    std::string str = (this->_tex_mode ? "Couleur" : "Profondeur");
    font.drawString("Mode — "+str, 0, strh);
    strh += WINDOW_PADDING/4.f;
    ofSetColor(127);
    _tex_mode_rect = ofRectangle(x,y+strh, w, WINDOW_PADDING);
    ofDrawRectangle(-1,-1+strh, w+2, WINDOW_PADDING+2);
    ofSetColor(0);
    if ( this->_tex_mode ) { ofSetColor(255); }
    ofDrawRectangle(0, 0+strh, w, WINDOW_PADDING);
    ofPopMatrix();
    
    ofSetColor(255);
}

ofTexture & Mapping::getFboTexture()
{
    return this->_fbo.getTexture();
}


float Mapping::getScale() { return this->_scale->getValue(); }
float Mapping::getX() { return this->_pos_x->getValue(); }
float Mapping::getY() { return this->_pos_y->getValue(); }
float Mapping::getRot() { return this->_rot->getValue(); }

void Mapping::mouseMoved(int x, int y)
{
    this->_scale->mouseMoved(x-this->_scale_x, y-this->_scale_y);
    this->_pos_x->mouseMoved(x-this->_pos_x_x, y-this->_pos_x_y);
    this->_pos_y->mouseMoved(x-this->_pos_y_x, y-this->_pos_y_y);
    this->_rot->mouseMoved(x-this->_rot_x, y-this->_rot_y);
}

void Mapping::mousePressed(int x, int y)
{
    float value = -1;
    
    if ( (value = this->_scale->mousePressed(x-this->_scale_x, y-this->_scale_y)) >= 0 )
    {
        this->_tex_scale = value / 100.f;
    }
    if ( (value = this->_pos_x->mousePressed(x-this->_pos_x_x, y-this->_pos_x_y)) >= 0 )
    {
        this->_tex_x = value;
    }
    if ( (value = this->_pos_y->mousePressed(x-this->_pos_y_x, y-this->_pos_y_y)) >= 0 )
    {
        this->_tex_y = value;
    }
    if ( (value = this->_rot->mousePressed(x-this->_rot_x, y-this->_rot_y)) >= 0 )
    {
        this->_tex_rot = value;
    }
    
    if ( this->_tex_mode_rect.inside(x,y) )
    {
        this->_tex_mode = ! this->_tex_mode;
    }
}

void Mapping::mouseDragged(int x, int y)
{
    int value = -1;
    
    if ( (value = this->_scale->mousePressed(x-this->_scale_x, y-this->_scale_y)) >= 0 )
    {
        this->_tex_scale = value / 100.f;
    }
    if ( (value = this->_pos_x->mousePressed(x-this->_pos_x_x, y-this->_pos_x_y)) >= 0 )
    {
        this->_tex_x = value;
    }
    if ( (value = this->_pos_y->mousePressed(x-this->_pos_y_x, y-this->_pos_y_y)) >= 0 )
    {
        this->_tex_y = value;
    }
    if ( (value = this->_rot->mousePressed(x-this->_rot_x, y-this->_rot_y)) >= 0 )
    {
        this->_tex_rot = value;
    }
}

void Mapping::mouseReleased(int x, int y)
{
    this->_scale->mouseReleased(x-this->_scale_x, y-this->_scale_y);
    this->_pos_x->mouseReleased(x-this->_pos_x_x, y-this->_pos_x_y);
    this->_pos_y->mouseReleased(x-this->_pos_y_x, y-this->_pos_y_y);
    this->_rot->mouseReleased(x-this->_rot_x, y-this->_rot_y);
}

void Mapping::keyPressed(int key)
{
    int value = -1;
    
    if ( value = this->_scale->keyPressed(key) )
    {
        this->_tex_scale = value / 100.f;
    }
    
    if ( value = this->_pos_x->keyPressed(key) )
    {
        this->_tex_x = value;
    }
    
    if ( value = this->_pos_y->keyPressed(key) )
    {
        this->_tex_y = value;
    }
    
    if ( value = this->_rot->keyPressed(key) )
    {
        this->_tex_rot = value;
    }
}

void Mapping::keyReleased(int key)
{
    this->_scale->keyReleased(key);
    this->_pos_x->keyReleased(key);
    this->_pos_y->keyReleased(key);
    this->_rot->keyReleased(key);
}

void Mapping::updateValues()
{
    this->_tex_scale = this->_scale->getValue() / 100.f;
    this->_tex_x = this->_pos_x->getValue();
    this->_tex_y = this->_pos_y->getValue();
    this->_tex_rot = this->_rot->getValue();
}
