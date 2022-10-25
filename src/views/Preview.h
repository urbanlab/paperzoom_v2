#ifndef preview_h
#define preview_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Preview)
DEF_CLASS_SH_PTR(Scenario)

//-------------------------------------------------------------------------------

class Preview
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static PreviewShPtr create();
	virtual ~Preview();

protected:

	Preview();

public:

	PreviewWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void update();
    
    void draw(
        int width, int height, int n_steps, float min_depth, float max_depth,
        ofTexture & tex, ScenarioShPtr scene
    );
    
//    void drawFbo(ofTexture & tex);
    ofTexture & getFboTexture();
    
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ofFbo _fbo;
    ofShader _shader_background;
    ofShader _shader_scene;
    ofPlanePrimitive _plane;
};

#endif /* preview_h */
