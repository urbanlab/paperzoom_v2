#include "Preview.h"
#include "scenario/Scenario.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

PreviewShPtr Preview::create()
{
	PreviewShPtr Preview_shptr(new Preview());
	Preview_shptr->_weak_ptr = Preview_shptr;
	return Preview_shptr;
}

Preview::~Preview()
{
	this->_weak_ptr.reset();
}

Preview::Preview()
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

    this->_shader_background.load("shaders/background.vert", "shaders/background.frag");
    this->_shader_scene.load("shaders/scene.vert", "shaders/scene.frag");

    this->_plane.set(2, 2, 2, 2);
    this->_plane.mapTexCoords(0, 0, 1.f, 1.f);
}

//-- METHODS --------------------------------------------------------------------

void Preview::update()
{
}

//void Preview::drawFbo(ofTexture & tex)
//{
//    
//}

void Preview::draw(
                   int width, int height, int n_steps, float min_depth, float max_depth, float offset_x, float offset_y,
    ofTexture & tex, ScenarioShPtr scene )
{
    ofSetColor(255);
    
    this->_fbo.begin();
    this->_shader_background.begin();

    this->_shader_background.setUniformTexture("tex", tex, 1);
    this->_shader_background.setUniform2i("tex_resolution", PROJECTION_WIDTH, PROJECTION_HEIGHT);
    
    this->_shader_background.setUniform1i("n_steps", n_steps);
    this->_shader_background.setUniform1f("min_depth", min_depth);
    this->_shader_background.setUniform1f("max_depth", max_depth);
    this->_shader_background.setUniform1i("vflip", 1);

//    this->_shader.setUniform4f("color_0", STEP_COLOR_0);
//    this->_shader.setUniform4f("color_1", STEP_COLOR_1);
//    this->_shader.setUniform4f("color_2", STEP_COLOR_2);
//    this->_shader.setUniform4f("color_3", STEP_COLOR_3);
//    this->_shader.setUniform4f("color_4", STEP_COLOR_4);
//    this->_shader.setUniform4f("color_5", STEP_COLOR_5);
//    this->_shader.setUniform4f("color_6", STEP_COLOR_6);
//    this->_shader.setUniform4f("color_7", STEP_COLOR_7);
//    this->_shader.setUniform4f("color_8", STEP_COLOR_8);
//    this->_shader.setUniform4f("color_9", STEP_COLOR_9);
//    this->_shader.setUniform4f("color_10", STEP_COLOR_10);
//    this->_shader.setUniform4f("color_11", STEP_COLOR_11);

    scene->bindImages(this->_shader_background);
    
    this->_plane.draw();
    
    scene->unbindImages();
    this->_shader_background.end();
    this->_fbo.end();
    
    //
    
    this->_fbo.begin();
    this->_shader_scene.begin();

    this->_shader_scene.setUniformTexture("tex", tex, 1);
    this->_shader_scene.setUniform2i("tex_resolution", PROJECTION_WIDTH, PROJECTION_HEIGHT);
    
    this->_shader_scene.setUniform1i("n_steps", n_steps);
    this->_shader_scene.setUniform1f("min_depth", min_depth);
    this->_shader_scene.setUniform1f("max_depth", max_depth);
    this->_shader_scene.setUniform1i("vflip", 1);
    this->_shader_scene.setUniform2f("offset_tex", offset_x, offset_y);

//    this->_shader.setUniform4f("color_0", STEP_COLOR_0);
//    this->_shader.setUniform4f("color_1", STEP_COLOR_1);
//    this->_shader.setUniform4f("color_2", STEP_COLOR_2);
//    this->_shader.setUniform4f("color_3", STEP_COLOR_3);
//    this->_shader.setUniform4f("color_4", STEP_COLOR_4);
//    this->_shader.setUniform4f("color_5", STEP_COLOR_5);
//    this->_shader.setUniform4f("color_6", STEP_COLOR_6);
//    this->_shader.setUniform4f("color_7", STEP_COLOR_7);
//    this->_shader.setUniform4f("color_8", STEP_COLOR_8);
//    this->_shader.setUniform4f("color_9", STEP_COLOR_9);
//    this->_shader.setUniform4f("color_10", STEP_COLOR_10);
//    this->_shader.setUniform4f("color_11", STEP_COLOR_11);

    scene->bindImages(this->_shader_scene);
    
    this->_plane.draw();
    
    scene->unbindImages();
    this->_shader_scene.end();
    this->_fbo.end();
    
    //
    
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
}

ofTexture & Preview::getFboTexture()
{
    return this->_fbo.getTexture();
}
