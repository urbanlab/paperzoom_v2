#ifndef blur_h
#define blur_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Blur)

//-------------------------------------------------------------------------------

class Blur
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static BlurShPtr create();
	virtual ~Blur();

protected:

	Blur();

public:

	BlurWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void process(std::vector<ofTexture> & tex);
    ofTexture & getFboTexture();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ofFbo _fbo;
    ofShader _shader;
    ofPlanePrimitive _plane;
};

#endif /* blur_h */
