#ifndef projection_h
#define projection_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Projection)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(Parameter)

//-------------------------------------------------------------------------------

class Projection
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static ProjectionShPtr create(ConfigShPtr config);
	virtual ~Projection();

protected:

	Projection(ConfigShPtr config);

public:

	ProjectionWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
    void update();
    void draw(int width, int height, ofTrueTypeFont & font);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y);
    void mouseDragged(int x, int y);
    void mouseReleased(int x, int y);
    void keyPressed(int key);
    
    int getLeftTop();
    int getLeftBottom();
    int getTopLeft();
    int getTopRight();
    int getRightTop();
    int getRightBottom();
    int getBottomLeft();
    int getBottomRight();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
//    ConfigShPtr _config;
    
    ofMesh _left;
    ofMesh _top;
    ofMesh _right;
    ofMesh _bottom;

    int _thickness;
    
    ParameterShPtr _left_top;
    int _left_top_x, _left_top_y;
    
    ParameterShPtr _left_bottom;
    int _left_bottom_x, _left_bottom_y;
    
    ParameterShPtr _top_left;
    int _top_left_x, _top_left_y;
    
    ParameterShPtr _top_right;
    int _top_right_x, _top_right_y;
    
    ParameterShPtr _right_top;
    int _right_top_x, _right_top_y;
    
    ParameterShPtr _right_bottom;
    int _right_bottom_x, _right_bottom_y;
    
    ParameterShPtr _bottom_left;
    int _bottom_left_x, _bottom_left_y;
    
    ParameterShPtr _bottom_right;
    int _bottom_right_x, _bottom_right_y;
};

#endif /* projection_h */
