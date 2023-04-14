#include "Projection.h"
#include "tools/Config.h"
#include "views/ParameterInt.h"
#include "views/Parameter.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

ProjectionShPtr Projection::create(ConfigShPtr config)
{
	ProjectionShPtr Projection_shptr(new Projection(config));
	Projection_shptr->_weak_ptr = Projection_shptr;
	return Projection_shptr;
}

Projection::~Projection()
{
	this->_weak_ptr.reset();
}

Projection::Projection(ConfigShPtr config)
{
//    this->_config = config;
    this->_thickness = 0;
    
    // LEFT / RIGHT
    
    ofPlanePrimitive plane; plane.set(this->_thickness, PROJECTION_HEIGHT, 2, 2);

    this->_left = plane.getMesh();
    std::vector<glm::vec3> vertices = this->_left.getVertices();
    for (int i = 0; i < vertices.size(); i++ )
    {
        vertices[i].x -= this->_thickness/2;
        vertices[i].y += PROJECTION_HEIGHT/2;
        this->_left.setVertex(i, vertices[i]);
    }
    
    this->_right = plane.getMesh();
    vertices = this->_right.getVertices();
    for (int i = 0; i < vertices.size(); i++ )
    {
        vertices[i].x += PROJECTION_WIDTH + this->_thickness/2;
        vertices[i].y += PROJECTION_HEIGHT/2;
        this->_right.setVertex(i, vertices[i]);
    }
    
    // TOP / BOTTOM
    
    plane.set(PROJECTION_WIDTH, this->_thickness, 2, 2);
    this->_top = plane.getMesh();

    vertices = this->_top.getVertices();
    for (int i = 0; i < vertices.size(); i++ )
    {
        vertices[i].x += PROJECTION_WIDTH/2;
        vertices[i].y -= this->_thickness/2;
        this->_top.setVertex(i, vertices[i]);
    }
    
    this->_bottom = plane.getMesh();
    vertices = this->_bottom.getVertices();
    
    for (int i = 0; i < vertices.size(); i++ )
    {
        vertices[i].x += PROJECTION_WIDTH/2;
        vertices[i].y += PROJECTION_HEIGHT + this->_thickness/2;
        this->_bottom.setVertex(i, vertices[i]);
    }
    
    // LEFT

    glm::vec3 v = this->_left.getVertex(1);
    v.x = config->_mask_tlx;
    v.y = config->_mask_tly;
    this->_left.setVertex(1, v);
    
    v = this->_left.getVertex(3);
    v.x = config->_mask_blx;
    v.y = PROJECTION_HEIGHT - config->_mask_bly;
    this->_left.setVertex(3, v);
    
    // TOP
    
    v = this->_top.getVertex(2);
    v.x = config->_mask_tlx;
    v.y = config->_mask_tly;
    this->_top.setVertex(2, v);
    
    v = this->_top.getVertex(3);
    v.x = PROJECTION_WIDTH - config->_mask_trx;
    v.y = config->_mask_try;
    this->_top.setVertex(3, v);
    
    // RIGHT
    
    v = this->_right.getVertex(0);
    v.x = PROJECTION_WIDTH - config->_mask_trx;
    v.y = config->_mask_try;
    this->_right.setVertex(0, v);
    
    v = this->_right.getVertex(2);
    v.x = PROJECTION_WIDTH - config->_mask_brx;
    v.y = PROJECTION_HEIGHT - config->_mask_bry;
    this->_right.setVertex(2, v);
    
    // BOTTOM
    
    v = this->_bottom.getVertex(0);
    v.x = config->_mask_blx;
    v.y = PROJECTION_HEIGHT - config->_mask_bly;
    this->_bottom.setVertex(0, v);
    
    v = this->_bottom.getVertex(1);
    v.x = PROJECTION_WIDTH - config->_mask_brx;
    v.y = PROJECTION_HEIGHT - config->_mask_bry;
    this->_bottom.setVertex(1, v);
    
    //
    
    this->_tlx = ParameterInt::create("Haut Gauche - X", config->_mask_tlx, 0, PROJECTION_WIDTH, "px");
    this->_tly = ParameterInt::create("Haut Gauche - Y", config->_mask_tly, 0, PROJECTION_HEIGHT, "px");

    this->_trx = ParameterInt::create("Haut Droite - X", config->_mask_trx, 0, PROJECTION_WIDTH, "px");
    this->_try = ParameterInt::create("Haut Droite - Y", config->_mask_try, 0, PROJECTION_HEIGHT, "px");

    this->_blx = ParameterInt::create("Bas Gauche - X", config->_mask_blx, 0, PROJECTION_WIDTH, "px");
    this->_bly = ParameterInt::create("Bas Gauche - Y", config->_mask_bly, 0, PROJECTION_HEIGHT, "px");

    this->_brx = ParameterInt::create("Bas Droite - X", config->_mask_brx, 0, PROJECTION_WIDTH, "px");
    this->_bry = ParameterInt::create("Bas Droite - Y", config->_mask_bry, 0, PROJECTION_HEIGHT, "px");
    
    
    //
    
    this->_offset_x = config->_offset_tex_x;
    this->_offset_y = config->_offset_tex_y;

    this->_offset_tex_x = Parameter::create("Offset Tex - X", config->_offset_tex_x, 0, 100, "%");
    this->_offset_tex_y = Parameter::create("Offset Tex - Y", config->_offset_tex_y, 0, 100, "%");
}

//-- METHODS --------------------------------------------------------------------

void Projection::update()
{
    
}

void Projection::draw(int width, int height, ofTrueTypeFont & font)
{
    ofSetColor(255);

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
    
    ofDrawRectangle(0,0, disp_width, disp_height);
    
    //
    
    ofSetColor(32);
    
//    ofSetColor(255,0,0,127);

    float disp_ratio = disp_width / (float)PROJECTION_WIDTH;
    
    ofMesh left = this->_left;
    std::vector<glm::vec3> vertices = left.getVertices();
    vertices[1].x *= disp_ratio;
    vertices[1].y *= disp_ratio;
    vertices[3].x *= disp_ratio;
    vertices[3].y *= disp_ratio;
    
    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[2].x = 0;
    vertices[2].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;
    
    for (int i = 0; i < vertices.size(); i++ ) { left.setVertex(i, vertices[i]); }
    
    left.draw();
    
    //
    
//    ofSetColor(0,255,0,127);

    ofMesh right = this->_right;
    vertices = right.getVertices();
    vertices[0].x *= disp_ratio;
    vertices[0].y *= disp_ratio;
    vertices[2].x *= disp_ratio;
    vertices[2].y *= disp_ratio;
    
    vertices[1].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[1].y = 0;
    vertices[3].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[3].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;

    for (int i = 0; i < vertices.size(); i++ ) { right.setVertex(i, vertices[i]); }

    right.draw();
    
    //
    
//    ofSetColor(0,0,255,127);

    ofMesh top = this->_top;
    vertices = top.getVertices();
    vertices[2].x *= disp_ratio;
    vertices[2].y *= disp_ratio;
    vertices[3].x *= disp_ratio;
    vertices[3].y *= disp_ratio;
    
    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[1].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[1].y = 0;
    
    for (int i = 0; i < vertices.size(); i++ ) { top.setVertex(i, vertices[i]); }

    top.draw();
    
    //
        
//    ofSetColor(0,255,255,127);

    ofMesh bottom = this->_bottom;
    vertices = bottom.getVertices();

    vertices[0].x *= disp_ratio;
    vertices[0].y *= disp_ratio;
    vertices[1].x *= disp_ratio;
    vertices[1].y *= disp_ratio;
    
    vertices[2].x = 0;
    vertices[2].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;
    vertices[3].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[3].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;
    
    for (int i = 0; i < vertices.size(); i++ ) { bottom.setVertex(i, vertices[i]); }

    bottom.draw();
    
    //
    
    ofSetColor(255);
    
    int x = disp_width + WINDOW_PADDING;
    int y = 0;
    int w = width - disp_width - WINDOW_PADDING;
    
    // TOP LEFT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_tlx_x = x;
    this->_tlx_y = y;
    y += this->_tlx->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_tly_x = x;
    this->_tly_y = y;
    y += this->_tly->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    // TOP RIGHT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_trx_x = x;
    this->_trx_y = y;
    y += this->_trx->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_try_x = x;
    this->_try_y = y;
    y += this->_try->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
 
    // BOTTOM LEFT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_blx_x = x;
    this->_blx_y = y;
    y += this->_blx->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_bly_x = x;
    this->_bly_y = y;
    y += this->_bly->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    // BOTTOM RIGHT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_brx_x = x;
    this->_brx_y = y;
    y += this->_brx->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_bry_x = x;
    this->_bry_y = y;
    y += this->_bry->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    // OFFET TEX
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_offset_tex_x_x = x;
    this->_offset_tex_x_y = y;
    y += this->_offset_tex_x->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_offset_tex_y_x = x;
    this->_offset_tex_y_y = y;
    y += this->_offset_tex_y->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
}

void Projection::mouseMoved(int x, int y)
{
    this->_tlx->mouseMoved(x-this->_tlx_x, y-this->_tlx_y);
    this->_tly->mouseMoved(x-this->_tly_x, y-this->_tly_y);

    this->_trx->mouseMoved(x-this->_trx_x, y-this->_trx_y);
    this->_try->mouseMoved(x-this->_try_x, y-this->_try_y);

    this->_blx->mouseMoved(x-this->_blx_x, y-this->_blx_y);
    this->_bly->mouseMoved(x-this->_bly_x, y-this->_bly_y);

    this->_brx->mouseMoved(x-this->_brx_x, y-this->_brx_y);
    this->_bry->mouseMoved(x-this->_bry_x, y-this->_bry_y);
    
    this->_offset_tex_x->mouseMoved(x-this->_offset_tex_x_x, y-this->_offset_tex_x_y);
    this->_offset_tex_y->mouseMoved(x-this->_offset_tex_y_x, y-this->_offset_tex_y_y);

}

void Projection::mousePressed(int x, int y)
{
    int value = -1;
    
    // TOP LEFT
    
    if ( (value = this->_tlx->mousePressed(x-this->_tlx_x, y-this->_tlx_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.x = value;
        this->_top.setVertex(2, v);
    }
    
    if ( (value = this->_tly->mousePressed(x-this->_tly_x, y-this->_tly_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.y = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }
    
    // TOP RIGHT
    
    if ( (value = this->_trx->mousePressed(x-this->_trx_x, y-this->_trx_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.x = PROJECTION_WIDTH - value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( (value = this->_try->mousePressed(x-this->_try_x, y-this->_try_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.y = value;
        this->_right.setVertex(0, v);
    }
    
    // BOTTOM LEFT
    
    if ( (value = this->_blx->mousePressed(x-this->_blx_x, y-this->_blx_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.x = value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( (value = this->_bly->mousePressed(x-this->_bly_x, y-this->_bly_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.y = PROJECTION_HEIGHT - value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(0, v);
    }
    
    // BOTTOM RIGHT
    
    if ( (value = this->_brx->mousePressed(x-this->_brx_x, y-this->_brx_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.x = PROJECTION_WIDTH - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    if ( (value = this->_bry->mousePressed(x-this->_bry_x, y-this->_bry_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.y = PROJECTION_HEIGHT - value;
        this->_right.setVertex(2, v);
    }
    
    // OFFSET TEX

    if ( (value = this->_offset_tex_x->mousePressed(x-this->_offset_tex_x_x, y-this->_offset_tex_x_y)) >= 0 )
    {
        this->_offset_x = value;
    }
    
    if ( (value = this->_offset_tex_y->mousePressed(x-this->_offset_tex_y_x, y-this->_offset_tex_y_y)) >= 0 )
    {
        this->_offset_y = value;
    }
}

void Projection::mouseDragged(int x, int y)
{
    int value = -1;
    
    // TOP LEFT

    if ( (value = this->_tlx->mousePressed(x-this->_tlx_x, y-this->_tlx_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.x = value;
        this->_top.setVertex(2, v);
    }
    
    if ( (value = this->_tly->mousePressed(x-this->_tly_x, y-this->_tly_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.y = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }
    
    // TOP RIGHT
    
    if ( (value = this->_trx->mousePressed(x-this->_trx_x, y-this->_trx_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.x = PROJECTION_WIDTH - value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( (value = this->_try->mousePressed(x-this->_try_x, y-this->_try_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.y = value;
        this->_right.setVertex(0, v);
    }
    
    // BOTTOM LEFT
    
    if ( (value = this->_blx->mousePressed(x-this->_blx_x, y-this->_blx_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.x = value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( (value = this->_bly->mousePressed(x-this->_bly_x, y-this->_bly_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.y = PROJECTION_HEIGHT - value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(0, v);
    }
    
    // BOTTOM RIGHT
    
    if ( (value = this->_brx->mousePressed(x-this->_brx_x, y-this->_brx_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.x = PROJECTION_WIDTH - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    if ( (value = this->_bry->mousePressed(x-this->_bry_x, y-this->_bry_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.y = PROJECTION_HEIGHT - value;
        this->_right.setVertex(2, v);
    }
    
    // OFFSET TEX

    if ( (value = this->_offset_tex_x->mousePressed(x-this->_offset_tex_x_x, y-this->_offset_tex_x_y)) >= 0 )
    {
        this->_offset_x = value;
    }
    
    if ( (value = this->_offset_tex_y->mousePressed(x-this->_offset_tex_y_x, y-this->_offset_tex_y_y)) >= 0 )
    {
        this->_offset_y = value;
    }
}

void Projection::mouseReleased(int x, int y)
{
    this->_tlx->mouseReleased(x-this->_tlx_x, y-this->_tlx_y);
    this->_tly->mouseReleased(x-this->_tly_x, y-this->_tly_y);
    this->_trx->mouseReleased(x-this->_trx_x, y-this->_trx_y);
    this->_try->mouseReleased(x-this->_try_x, y-this->_try_y);
    this->_blx->mouseReleased(x-this->_blx_x, y-this->_blx_y);
    this->_bly->mouseReleased(x-this->_bly_x, y-this->_bly_y);
    this->_brx->mouseReleased(x-this->_brx_x, y-this->_brx_y);
    this->_bry->mouseReleased(x-this->_bry_x, y-this->_bry_y);
    
    this->_offset_tex_x->mouseReleased(x-this->_offset_tex_x_x, y-this->_offset_tex_x_y);
    this->_offset_tex_y->mouseReleased(x-this->_offset_tex_y_x, y-this->_offset_tex_y_y);

}

void Projection::keyPressed(int key)
{
    int value = -1;
    
    // TOP LEFT

    if ( value = this->_tlx->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.x = value;
        this->_top.setVertex(2, v);
    }
    
    if ( value = this->_tly->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.y = value;
        this->_left.setVertex(1, v);
        
        v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }

    // TOP RIGHT

    if ( value = this->_trx->keyPressed(key) )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.x = PROJECTION_WIDTH - value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( value = this->_try->keyPressed(key) )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
        
        v = this->_right.getVertex(0);
        v.y = value;
        this->_right.setVertex(0, v);
    }
    
    // BOTTOM LEFT

    if ( value = this->_blx->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.x = value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( value = this->_bly->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.y = PROJECTION_HEIGHT - value;
        this->_left.setVertex(3, v);
        
        v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(0, v);
    }
    
    // BOTTOM RIGHT

    if ( value = this->_brx->keyPressed(key) )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.x = PROJECTION_WIDTH - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    if ( value = this->_bry->keyPressed(key) )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT - value;
        this->_bottom.setVertex(1, v);
        
        v = this->_right.getVertex(2);
        v.y = PROJECTION_HEIGHT - value;
        this->_right.setVertex(2, v);
    }
    
    // OFFSET TEX

    if ( value = this->_offset_tex_x->keyPressed(key) )
    {
        this->_offset_x = value;
    }
    
    if ( value = this->_offset_tex_y->keyPressed(key) )
    {
        this->_offset_y = value;
    }
}

void Projection::keyReleased(int key)
{
    this->_tlx->keyReleased(key);
    this->_tly->keyReleased(key);
    this->_trx->keyReleased(key);
    this->_try->keyReleased(key);
    this->_blx->keyReleased(key);
    this->_bly->keyReleased(key);
    this->_brx->keyReleased(key);
    this->_bry->keyReleased(key);
    
    this->_offset_tex_x->keyReleased(key);
    this->_offset_tex_y->keyReleased(key);
}

int Projection::getTopLeftX() { return this->_tlx->getValue(); }
int Projection::getTopLeftY() { return this->_tly->getValue(); }
int Projection::getTopRightX() { return this->_trx->getValue(); }
int Projection::getTopRightY() { return this->_try->getValue(); }
int Projection::getBottomLeftX() { return this->_blx->getValue(); }
int Projection::getBottomLeftY() { return this->_bly->getValue(); }
int Projection::getBottomRightX() { return this->_brx->getValue(); }
int Projection::getBottomRightY() { return this->_bry->getValue(); }

float Projection::getOffsetTexX() { return this->_offset_tex_x->getValue(); }
float Projection::getOffsetTexY() { return this->_offset_tex_y->getValue(); }


void Projection::incrTLX(int v)
{
    int value =  this->_tlx->incr(v);
    glm::vec3 ve = this->_left.getVertex(1);
    ve.x = value;
    this->_left.setVertex(1, ve);
    
    ve = this->_top.getVertex(2);
    ve.x = value;
    this->_top.setVertex(2, ve);

}
void Projection::incrTLY(int ve)
{
    int value = this->_tly->incr(ve);

    glm::vec3 v = this->_left.getVertex(1);
    v.y = value;
    this->_left.setVertex(1, v);
    
    v = this->_top.getVertex(2);
    v.y = value;
    this->_top.setVertex(2, v);

}
void Projection::incrTRX(int ve)
{
    int value = this->_trx->incr(ve);

    glm::vec3 v = this->_top.getVertex(3);
    v.x = PROJECTION_WIDTH - value;
    this->_top.setVertex(3, v);
    
    v = this->_right.getVertex(0);
    v.x = PROJECTION_WIDTH - value;
    this->_right.setVertex(0, v);
}
void Projection::incrTRY(int ve)
{
    int value = this->_try->incr(ve);

    glm::vec3 v = this->_top.getVertex(3);
    v.y = value;
    this->_top.setVertex(3, v);
    
    v = this->_right.getVertex(0);
    v.y = value;
    this->_right.setVertex(0, v);
}

void Projection::incrBLX(int ve)
{
    int value = this->_blx->incr(ve);

    glm::vec3 v = this->_left.getVertex(3);
    v.x = value;
    this->_left.setVertex(3, v);
    
    v = this->_bottom.getVertex(0);
    v.x = value;
    this->_bottom.setVertex(0, v);
}
void Projection::incrBLY(int ve)
{
    int value = this->_bly->incr(ve);

    glm::vec3 v = this->_left.getVertex(3);
    v.y = PROJECTION_HEIGHT - value;
    this->_left.setVertex(3, v);
    
    v = this->_bottom.getVertex(0);
    v.y = PROJECTION_HEIGHT - value;
    this->_bottom.setVertex(0, v);
}

void Projection::incrBRX(int ve)
{
    int value = this->_brx->incr(ve);

    glm::vec3 v = this->_bottom.getVertex(1);
    v.x = PROJECTION_WIDTH - value;
    this->_bottom.setVertex(1, v);
    
    v = this->_right.getVertex(2);
    v.x = PROJECTION_WIDTH - value;
    this->_right.setVertex(2, v);
}
void Projection::incrBRY(int ve)
{
    int value = this->_bry->incr(ve);

    glm::vec3 v = this->_bottom.getVertex(1);
    v.y = PROJECTION_HEIGHT - value;
    this->_bottom.setVertex(1, v);
    
    v = this->_right.getVertex(2);
    v.y = PROJECTION_HEIGHT - value;
    this->_right.setVertex(2, v);
}

void Projection::incrOffsetTexX(float ve)
{
    int value = this->_offset_tex_x->incr(ve);
    this->_offset_x = value;
}


void Projection::incrOffsetTexY(float ve)
{
    int value = this->_offset_tex_y->incr(ve);
    this->_offset_y = value;
}

ofMesh & Projection::getLeft() { return this->_left; }
ofMesh & Projection::getTop() { return this->_top; }
ofMesh & Projection::getRight() { return this->_right; }
ofMesh & Projection::getBottom() { return this->_bottom; }
