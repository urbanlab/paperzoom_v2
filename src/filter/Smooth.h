#ifndef smooth_h
#define smooth_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Smooth)

//-------------------------------------------------------------------------------

class Smooth
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static SmoothShPtr create();
	virtual ~Smooth();

protected:

	Smooth();

public:

	SmoothWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void process(ofTexture & tex);
    ofTexture & getFboTexture();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ofFbo _fbo;
    ofShader _shader;
    ofPlanePrimitive _plane;
};

#endif /* smooth_h */
