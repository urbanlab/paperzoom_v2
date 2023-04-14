#ifndef projection_h
#define projection_h

#include "tools/Globals.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Projection)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SH_PTR(ParameterInt)
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
    void keyReleased(int key);

    int getTopLeftX();
    int getTopLeftY();
    int getTopRightX();
    int getTopRightY();
    int getBottomLeftX();
    int getBottomLeftY();
    int getBottomRightX();
    int getBottomRightY();

    float getOffsetTexX();
    float getOffsetTexY();

    void incrTLX(int v);
    void incrTLY(int v);
    
    void incrTRX(int v);
    void incrTRY(int v);
    
    void incrBLX(int v);
    void incrBLY(int v);
    
    void incrBRX(int v);
    void incrBRY(int v);
    
    void incrOffsetTexX(float v);
    void incrOffsetTexY(float v);
    
    ofMesh & getLeft();
    ofMesh & getTop();
    ofMesh & getRight();
    ofMesh & getBottom();

	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
//    ConfigShPtr _config;
    
    ofMesh _left;
    ofMesh _top;
    ofMesh _right;
    ofMesh _bottom;
    
    float _offset_x;
    float _offset_y;


    int _thickness;
    
    ParameterIntShPtr _tlx;
    int _tlx_x, _tlx_y;
    
    ParameterIntShPtr _tly;
    int _tly_x, _tly_y;
    
    ParameterIntShPtr _trx;
    int _trx_x, _trx_y;
    
    ParameterIntShPtr _try;
    int _try_x, _try_y;
    
    ParameterIntShPtr _blx;
    int _blx_x, _blx_y;
    
    ParameterIntShPtr _bly;
    int _bly_x, _bly_y;
    
    ParameterIntShPtr _brx;
    int _brx_x, _brx_y;
    
    ParameterIntShPtr _bry;
    int _bry_x, _bry_y;
    
    ParameterShPtr _offset_tex_x;
    int _offset_tex_x_x, _offset_tex_x_y;
    
    ParameterShPtr _offset_tex_y;
    int _offset_tex_y_x, _offset_tex_y_y;
};

#endif /* projection_h */
