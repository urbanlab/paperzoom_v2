#include "Smooth.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

SmoothShPtr Smooth::create()
{
	SmoothShPtr Smooth_shptr(new Smooth());
	Smooth_shptr->_weak_ptr = Smooth_shptr;
	return Smooth_shptr;
}

Smooth::~Smooth()
{
	this->_weak_ptr.reset();
}

Smooth::Smooth()
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
    
    this->_shader.load("shaders/smooth.vert", "shaders/smooth.frag");

    this->_plane.set(2, 2, 2, 2);
    this->_plane.mapTexCoords(0, 0, 1.f, 1.f);
}

//-- METHODS --------------------------------------------------------------------

void Smooth::process(ofTexture & tex)
{
    this->_fbo.begin();
    this->_shader.begin();
    this->_shader.setUniformTexture("tex", tex, 1);
    this->_shader.setUniform2i("tex_resolution", tex.getWidth(), tex.getHeight());
    
    ofClear(0);
    ofBackground(0);
    ofSetColor(255);

    this->_plane.draw();

    tex.unbind();

    this->_shader.end();
    this->_fbo.end();
}

ofTexture & Smooth::getFboTexture()
{
    return this->_fbo.getTexture();
}
