#ifndef mapping_h
#define mapping_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Mapping)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(Parameter)

DEF_CLASS_SH_PTR(Blur)
DEF_CLASS_SH_PTR(Smooth)

//-------------------------------------------------------------------------------

class Mapping
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static MappingShPtr create(ConfigShPtr config);
	virtual ~Mapping();

protected:

	Mapping(ConfigShPtr config);

public:

	MappingWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
    void update();
    void draw(int width, int height, ofTexture & tex, ofTexture & tex2, bool is_new, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y);
    void mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    void keyPressed(int key);

    void drawFbo(ofTexture & tex, bool is_new);
    ofTexture & getFboTexture();

    int getScale();
    int getX();
    int getY();
    int getRot();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
  
    ofFbo _fbo;
    ofFbo _fbo_square;

    ofShader _shader;
    ofMesh _mesh;

    ofPlanePrimitive _plane;
    
    //
    
    SmoothShPtr _smooth;
    SmoothShPtr _smooth2;
    BlurShPtr _blur;
    
    std::vector<ofTexture> _blur_textures;

    //
    
    float _tex_scale;
    int _tex_x;
    int _tex_y;
    int _tex_rot;

    ParameterShPtr _scale;
    int _scale_x, _scale_y;
    
    ParameterShPtr _pos_x;
    int _pos_x_x, _pos_x_y;
    
    ParameterShPtr _pos_y;
    int _pos_y_x, _pos_y_y;
    
    ParameterShPtr _rot;
    int _rot_x, _rot_y;
    
    ofImage _debug;
    
    ofRectangle _tex_mode_rect;
    bool _tex_mode;
};

#endif /* mapping_h */
