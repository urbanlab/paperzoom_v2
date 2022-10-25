#include "Blur.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

BlurShPtr Blur::create()
{
	BlurShPtr Blur_shptr(new Blur());
	Blur_shptr->_weak_ptr = Blur_shptr;
	return Blur_shptr;
}

Blur::~Blur()
{
	this->_weak_ptr.reset();
}

Blur::Blur()
{
    ofFboSettings fsettings;
    fsettings.width = KINECT_WIDTH;
    fsettings.height = KINECT_HEIGHT;
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
    
    this->_shader.load("shaders/blur.vert", "shaders/blur.frag");

    this->_plane.set(2, 2, 2, 2);
    this->_plane.mapTexCoords(0, 0, 1.f, 1.f);
}

//-- METHODS --------------------------------------------------------------------

void Blur::process(std::vector<ofTexture> & tex)
{
    this->_fbo.begin();
    this->_shader.begin();
    
    int location = 1;
    for ( int i = tex.size()-1; i >= 0 ; i-- )
    {
        this->_shader.setUniformTexture("tex"+ofToString(location), tex[i], location);
        location++;
    }
    
    this->_shader.setUniform1i("n_textures", tex.size());
    this->_shader.setUniform2i("tex_resolution", KINECT_WIDTH, KINECT_HEIGHT);

    
    ofClear(0);
    ofBackground(0);
    ofSetColor(255);

    this->_plane.draw();

    for ( int i = 0; i < tex.size(); i++ )
    {
        tex[i].unbind();
    }

    this->_shader.end();
    this->_fbo.end();
}

ofTexture & Blur::getFboTexture()
{
    return this->_fbo.getTexture();
}
