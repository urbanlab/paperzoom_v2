#ifndef detection_h
#define detection_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Detection)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(Parameter)
DEF_CLASS_SH_PTR(ParameterInt)

//-------------------------------------------------------------------------------

class Detection
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static DetectionShPtr create(ConfigShPtr config);
	virtual ~Detection();

protected:

	Detection(ConfigShPtr config);

public:

	DetectionWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void update();
    void draw(int width, int height, ofTexture & tex, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y);
    void mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    void keyPressed(int key);
    void keyReleased(int key);

    void drawFbo(ofTexture & tex);
    ofTexture & getFboTexture();
    
    float getMin();
    float getMax();
    int getSteps();

    void updateValues();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ofFbo _fbo;
    ofShader _shader;
    ofMesh _mesh;

    ofPlanePrimitive _plane;
    
    //
        
    int _n_steps;
    float _min_depth;
    float _max_depth;

    ParameterIntShPtr _steps;
    int _steps_x, _steps_y;
    
    ParameterShPtr _min;
    int _min_x, _min_y;
    
    ParameterShPtr _max;
    int _max_x, _max_y;
    
//    float _tex_scale;
//    int _tex_x;
//    int _tex_y;
//    
//    ParameterShPtr _scale;
//    int _scale_x, _scale_y;
//    
//    ParameterShPtr _pos_x;
//    int _pos_x_x, _pos_x_y;
//    
//    ParameterShPtr _pos_y;
//    int _pos_y_x, _pos_y_y;
};

#endif /* detection_h */
