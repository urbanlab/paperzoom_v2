#include "Projection.h"
#include "tools/Config.h"
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
    this->_thickness = 4;
    
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
    v.x = config->_mask_l1;
    this->_left.setVertex(1, v);
    
    v = this->_left.getVertex(3);
    v.x = config->_mask_l2;
    this->_left.setVertex(3, v);
    
    // TOP
    
    v = this->_top.getVertex(2);
    v.y = config->_mask_t1;
    this->_top.setVertex(2, v);
    
    v = this->_top.getVertex(3);
    v.y = config->_mask_t2;
    this->_top.setVertex(3, v);
    
    // RIGHT
    
    v = this->_right.getVertex(0);
    v.x = PROJECTION_WIDTH - config->_mask_r1;
    this->_right.setVertex(0, v);
    
    v = this->_right.getVertex(2);
    v.x = PROJECTION_WIDTH - config->_mask_r2;
    this->_right.setVertex(2, v);
    
    // BOTTOM
    
    v = this->_bottom.getVertex(0);
    v.y = PROJECTION_HEIGHT - config->_mask_b1;
    this->_bottom.setVertex(0, v);
    
    v = this->_bottom.getVertex(1);
    v.y = PROJECTION_HEIGHT - config->_mask_b2;
    this->_bottom.setVertex(1, v);
    
    //
    
    this->_left_top = Parameter::create("Gauche - haut", config->_mask_l1, 0, PROJECTION_WIDTH, "px");
    this->_left_bottom = Parameter::create("Gauche - bas", config->_mask_l2, 0, PROJECTION_WIDTH, "px");

    this->_top_left = Parameter::create("Haut - gauche", config->_mask_t1, 0, PROJECTION_HEIGHT, "px");
    this->_top_right = Parameter::create("Haut - droite", config->_mask_t2, 0, PROJECTION_HEIGHT, "px");

    this->_right_top = Parameter::create("Droite - haut", config->_mask_r1, 0, PROJECTION_WIDTH, "px");
    this->_right_bottom = Parameter::create("Droite - bas", config->_mask_r2, 0, PROJECTION_WIDTH, "px");

    this->_bottom_left = Parameter::create("Bas - gauche", config->_mask_b1, 0, PROJECTION_HEIGHT, "px");
    this->_bottom_right = Parameter::create("Bas - droite", config->_mask_b2, 0, PROJECTION_HEIGHT, "px");
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
    
    float disp_ratio = disp_width / (float)PROJECTION_WIDTH;
    
    ofMesh left = this->_left;
    std::vector<glm::vec3> vertices = left.getVertices();
    vertices[1].x *= disp_ratio;
    vertices[2].y *= disp_ratio;
    vertices[3].x *= disp_ratio;
    vertices[3].y *= disp_ratio;
    for (int i = 0; i < vertices.size(); i++ ) { left.setVertex(i, vertices[i]); }
    
    left.draw();
    
    //
    
    ofMesh right = this->_right;
    vertices = right.getVertices();
    vertices[0].x *= disp_ratio;
    vertices[1].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[2].x *= disp_ratio;
    vertices[2].y *= disp_ratio;
    vertices[3].x = PROJECTION_WIDTH*disp_ratio+this->_thickness;
    vertices[3].y *= disp_ratio;
    for (int i = 0; i < vertices.size(); i++ ) { right.setVertex(i, vertices[i]); }

    right.draw();
    
    //
    
    ofMesh top = this->_top;
    vertices = top.getVertices();
    vertices[1].x *= disp_ratio;
    vertices[2].y *= disp_ratio;
    vertices[3].x *= disp_ratio;
    vertices[3].y *= disp_ratio;
    for (int i = 0; i < vertices.size(); i++ ) { top.setVertex(i, vertices[i]); }

    top.draw();
    
    //
        
    ofMesh bottom = this->_bottom;
    vertices = bottom.getVertices();
    vertices[0].y *= disp_ratio;
    vertices[1].x *= disp_ratio;
    vertices[1].y *= disp_ratio;
    vertices[2].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;
    vertices[3].x *= disp_ratio;
    vertices[3].y = PROJECTION_HEIGHT*disp_ratio+this->_thickness;
    for (int i = 0; i < vertices.size(); i++ ) { bottom.setVertex(i, vertices[i]); }

    bottom.draw();
    
    //
    
    ofSetColor(255);
    
    int x = disp_width + WINDOW_PADDING;
    int y = 0;
    int w = width - disp_width - WINDOW_PADDING;
    
    // LEFT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_left_top_x = x;
    this->_left_top_y = y;
    y += this->_left_top->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_left_bottom_x = x;
    this->_left_bottom_y = y;
    y += this->_left_bottom->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    // TOP
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_top_left_x = x;
    this->_top_left_y = y;
    y += this->_top_left->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_top_right_x = x;
    this->_top_right_y = y;
    y += this->_top_right->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
 
    // RIGHT
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_right_top_x = x;
    this->_right_top_y = y;
    y += this->_right_top->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_right_bottom_x = x;
    this->_right_bottom_y = y;
    y += this->_right_bottom->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    // BOTTOM
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_bottom_left_x = x;
    this->_bottom_left_y = y;
    y += this->_bottom_left->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x,y);
    this->_bottom_right_x = x;
    this->_bottom_right_y = y;
    y += this->_bottom_right->draw(w, font) + WINDOW_PADDING/2;
    ofPopMatrix();
    
}

void Projection::mouseMoved(int x, int y)
{
    this->_left_top->mouseMoved(x-this->_left_top_x, y-this->_left_top_y);
    this->_left_bottom->mouseMoved(x-this->_left_bottom_x, y-this->_left_bottom_y);

    this->_top_left->mouseMoved(x-this->_top_left_x, y-this->_top_left_y);
    this->_top_right->mouseMoved(x-this->_top_right_x, y-this->_top_right_y);

    this->_right_top->mouseMoved(x-this->_right_top_x, y-this->_right_top_y);
    this->_right_bottom->mouseMoved(x-this->_right_bottom_x, y-this->_right_bottom_y);

    this->_bottom_left->mouseMoved(x-this->_bottom_left_x, y-this->_bottom_left_y);
    this->_bottom_right->mouseMoved(x-this->_bottom_right_x, y-this->_bottom_right_y);

}

void Projection::mousePressed(int x, int y)
{
    int value = -1;
    
    // LEFT
    
    if ( (value = this->_left_top->mousePressed(x-this->_left_top_x, y-this->_left_top_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
    }
    
    if ( (value = this->_left_bottom->mousePressed(x-this->_left_bottom_x, y-this->_left_bottom_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
    }
    
    // TOP
    
    if ( (value = this->_top_left->mousePressed(x-this->_top_left_x, y-this->_top_left_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }
    
    if ( (value = this->_top_right->mousePressed(x-this->_top_right_x, y-this->_top_right_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
    }
    
    // RIGHT
    
    if ( (value = this->_right_top->mousePressed(x-this->_right_top_x, y-this->_right_top_y)) >= 0 )
    {
        glm::vec3 v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( (value = this->_right_bottom->mousePressed(x-this->_right_bottom_x, y-this->_right_bottom_y)) >= 0 )
    {
        glm::vec3 v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    // BOTTOM
    
    if ( (value = this->_bottom_left->mousePressed(x-this->_bottom_left_x, y-this->_bottom_left_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( (value = this->_bottom_right->mousePressed(x-this->_bottom_right_x, y-this->_bottom_right_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(1, v);
    }
}

void Projection::mouseDragged(int x, int y)
{
    int value = -1;
    
    // LEFT

    if ( (value = this->_left_top->mousePressed(x-this->_left_top_x, y-this->_left_top_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
    }
    
    if ( (value = this->_left_bottom->mousePressed(x-this->_left_bottom_x, y-this->_left_bottom_y)) >= 0 )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
    }
    
    // TOP
    
    if ( (value = this->_top_left->mousePressed(x-this->_top_left_x, y-this->_top_left_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }
    
    if ( (value = this->_top_right->mousePressed(x-this->_top_right_x, y-this->_top_right_y)) >= 0 )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
    }
    
    // RIGHT
    
    if ( (value = this->_right_top->mousePressed(x-this->_right_top_x, y-this->_right_top_y)) >= 0 )
    {
        glm::vec3 v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( (value = this->_right_bottom->mousePressed(x-this->_right_bottom_x, y-this->_right_bottom_y)) >= 0 )
    {
        glm::vec3 v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    // BOTTOM
    
    if ( (value = this->_bottom_left->mousePressed(x-this->_bottom_left_x, y-this->_bottom_left_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( (value = this->_bottom_right->mousePressed(x-this->_bottom_right_x, y-this->_bottom_right_y)) >= 0 )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(1, v);
    }
}

void Projection::mouseReleased(int x, int y)
{
    this->_left_top->mouseReleased(x-this->_left_top_x, y-this->_left_top_y);
    this->_left_bottom->mouseReleased(x-this->_left_bottom_x, y-this->_left_bottom_y);
    this->_top_left->mouseReleased(x-this->_top_left_x, y-this->_top_left_y);
    this->_top_right->mouseReleased(x-this->_top_right_x, y-this->_top_right_y);
    this->_right_top->mouseReleased(x-this->_right_top_x, y-this->_right_top_y);
    this->_right_bottom->mouseReleased(x-this->_right_bottom_x, y-this->_right_bottom_y);
    this->_bottom_left->mouseReleased(x-this->_bottom_left_x, y-this->_bottom_left_y);
    this->_bottom_right->mouseReleased(x-this->_bottom_right_x, y-this->_bottom_right_y);
}

void Projection::keyPressed(int key)
{
    int value = -1;
    
    // LEFT

    if ( value = this->_left_top->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(1);
        v.x = value;
        this->_left.setVertex(1, v);
    }
    
    if ( value = this->_left_bottom->keyPressed(key) )
    {
        glm::vec3 v = this->_left.getVertex(3);
        v.x = value;
        this->_left.setVertex(3, v);
    }

    // TOP

    if ( value = this->_top_left->keyPressed(key) )
    {
        glm::vec3 v = this->_top.getVertex(2);
        v.y = value;
        this->_top.setVertex(2, v);
    }
    
    if ( value = this->_top_right->keyPressed(key) )
    {
        glm::vec3 v = this->_top.getVertex(3);
        v.y = value;
        this->_top.setVertex(3, v);
    }
    
    // RIGHT

    if ( value = this->_right_top->keyPressed(key) )
    {
        glm::vec3 v = this->_right.getVertex(0);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(0, v);
    }
    
    if ( value = this->_right_bottom->keyPressed(key) )
    {
        glm::vec3 v = this->_right.getVertex(2);
        v.x = PROJECTION_WIDTH - value;
        this->_right.setVertex(2, v);
    }
    
    // BOTTOM

    if ( value = this->_bottom_left->keyPressed(key) )
    {
        glm::vec3 v = this->_bottom.getVertex(0);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(0, v);
    }
    
    if ( value = this->_bottom_right->keyPressed(key) )
    {
        glm::vec3 v = this->_bottom.getVertex(1);
        v.y = PROJECTION_HEIGHT-value;
        this->_bottom.setVertex(1, v);
    }
}

int Projection::getLeftTop() { return this->_left_top->getValue(); }
int Projection::getLeftBottom() { return this->_left_bottom->getValue(); }
int Projection::getTopLeft() { return this->_top_left->getValue(); }
int Projection::getTopRight() { return this->_top_right->getValue(); }
int Projection::getRightTop() { return this->_right_top->getValue(); }
int Projection::getRightBottom() { return this->_right_bottom->getValue(); }
int Projection::getBottomLeft() { return this->_bottom_left->getValue(); }
int Projection::getBottomRight() { return this->_bottom_right->getValue(); }