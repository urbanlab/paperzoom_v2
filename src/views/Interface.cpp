#include "Interface.h"
#include "views/Projection.h"
#include "views/Mapping.h"
#include "views/Detection.h"
#include "views/Preview.h"
#include "tools/Config.h"
#include "kinect/Kinect2.h"

#include "views/Parameter.h"

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------


InterfaceShPtr Interface::create(ConfigShPtr config)
{
	InterfaceShPtr Interface_shptr(new Interface(config));
	Interface_shptr->_weak_ptr = Interface_shptr;
	return Interface_shptr;
}

Interface::~Interface()
{
	this->_weak_ptr.reset();
}

Interface::Interface(ConfigShPtr config)
{
    this->_config = config;
    
    this->_kinect = Kinect2::create(config);

    std::string fontnames[] =
    {
        /* 00 */ "fonts/Roboto/Roboto-Black.ttf",
        /* 01 */ "fonts/Roboto/Roboto-BlackItalic.ttf",
        /* 02 */ "fonts/Roboto/Roboto-Bold.ttf",
        /* 03 */ "fonts/Roboto/Roboto-BoldItalic.ttf",
        /* 04 */ "fonts/Roboto/Roboto-Italic.ttf",
        /* 05 */ "fonts/Roboto/Roboto-Light.ttf",
        /* 06 */ "fonts/Roboto/Roboto-LightItalic.ttf",
        /* 07 */ "fonts/Roboto/Roboto-Medium.ttf",
        /* 08 */ "fonts/Roboto/Roboto-MediumItalic.ttf",
        /* 09 */ "fonts/Roboto/Roboto-Regular.ttf",
        /* 10 */ "fonts/Roboto/Roboto-Thin.ttf",
        /* 11 */ "fonts/Roboto/Roboto-ThinItalic.ttf"
    };
    
    ofTrueTypeFontSettings settings_font(fontnames[2], 48 * WINDOW_RATIO);
    settings_font.antialiased = true;
    settings_font.contours = false;
    settings_font.dpi = 72;
    settings_font.addRange(ofUnicode::LatinA);
    settings_font.addRange(ofUnicode::Latin1Supplement);
    settings_font.addRanges(ofAlphabet::Latin);
    settings_font.addRange(ofUnicode::GeneralPunctuation);

    this->_font_h1.load(settings_font);
    this->_font_h1.setLineHeight(this->_font_h1.getSize() * 1.00f);

    settings_font.fontName = fontnames[7];
    settings_font.fontSize = 24 * WINDOW_RATIO;
    this->_font_h2.load(settings_font);
    this->_font_h2.setLineHeight(this->_font_h2.getSize() * 1.05f);
    
    settings_font.fontName = fontnames[9];
    settings_font.fontSize = 18 * WINDOW_RATIO;
    this->_font_p.load(settings_font);
    this->_font_p.setLineHeight(this->_font_p.getSize() * 1.05f);
    
    //
        
    for ( int i = 0; i < 4; i++ )
    {
        this->_buttons.push_back(ofRectangle(0,0,0,0));
        this->_buttons_hover.push_back(false);
    }
    
    this->_save_button = ofRectangle(0,0,0,0);
    this->_save_button_hover = false;

    //
    
    this->_mode = 3;
    this->_projection = Projection::create(config);
    this->_mapping = Mapping::create(config);
    this->_detection = Detection::create(config);
    this->_preview = Preview::create();
    
    Parameter::initOsc(
        this->_config->_osc_mode, this->_config->_osc_host, this->_config->_osc_port);
    
    _keypressed[103] = false;
    _keypressed[104] = false;
    _keypressed[100] = false;
    _keypressed[98] = false;
    _keypressed[38] = false;
    _keypressed[233] = false;
}

//-- METHODS --------------------------------------------------------------------

void Interface::update()
{
    this->_kinect->update();

    if ( this->_mode == 0 ) { this->_projection->update(); }
    if ( this->_mode == 1 ) { this->_mapping->update(); }
    if ( this->_mode == 2 ) { this->_detection->update(); }
    if ( this->_mode == 3 ) { this->_preview->update(); }
    
    if ( Parameter::_osc_mode == "receive" )
    {
        while(Parameter::_osc_receiver.hasWaitingMessages())
        {
            // get the next message
            ofxOscMessage m;
            Parameter::_osc_receiver.getNextMessage(m);

            std::string id = m.getAddress();

            if (id == "save" ) { this->save(); }
            else
            {
                int value = m.getArgAsInt(0);
                Parameter::setOscValue(id,value);
            }
        }
    }

}

void Interface::draw(ScenarioShPtr scene)
{
    ofSetColor(255);
    
    int x = 0, y = 0;
    x += WINDOW_PADDING;
    y += WINDOW_PADDING;
    
    int max_width = 0;

    std::string str = "PaperZoom — Erasme";
    y += this->_font_h1.stringHeight(str);
    max_width = std::max(max_width, (int)this->_font_h1.stringWidth(str));
    this->_font_h1.drawString(str, x, y);
    
    str = "Studio Antho, 2022";
    y += this->_font_h1.getLineHeight();
    max_width = std::max(max_width, (int)this->_font_h1.stringWidth(str));
    this->_font_h1.drawString(str, x, y);
    
    y -= this->_font_h1.getLineHeight();
    x += max_width + WINDOW_PADDING*4;
    
    ofSetColor( (this->_mode == 0 || this->_buttons_hover[0]) ? 255 : 127 );
    
    str = "1 — Projection";
    this->_font_h2.drawString(str, x, y);
    
    this->_buttons[0].set(
        x, y-this->_font_h2.stringHeight(str),
        this->_font_h2.stringWidth(str), this->_font_h2.stringHeight(str) );

    x += this->_font_h2.stringWidth(str);
    x += WINDOW_PADDING;
    
    ofSetColor( (this->_mode == 1 || this->_buttons_hover[1]) ? 255 : 127 );

    str = "2 — Mapping";
    this->_font_h2.drawString(str, x, y);
    
    this->_buttons[1].set(
        x, y-this->_font_h2.stringHeight(str),
        this->_font_h2.stringWidth(str), this->_font_h2.stringHeight(str) );

    x += this->_font_h2.stringWidth(str);
    x += WINDOW_PADDING;
    
    ofSetColor( (this->_mode == 2 || this->_buttons_hover[2])  ? 255 : 127 );

    str = "3 — Détection";
    this->_font_h2.drawString(str, x, y);
    
    this->_buttons[2].set(
        x, y-this->_font_h2.stringHeight(str),
        this->_font_h2.stringWidth(str), this->_font_h2.stringHeight(str) );

    x += this->_font_h2.stringWidth(str);
    x += WINDOW_PADDING;
    
    ofSetColor( (this->_mode == 3 || this->_buttons_hover[3])  ? 255 : 127 );

    str = "4 — Preview";
    this->_font_h2.drawString(str, x, y);
    
    this->_buttons[3].set(
        x, y-this->_font_h2.stringHeight(str),
        this->_font_h2.stringWidth(str), this->_font_h2.stringHeight(str) );

    x += this->_font_h2.stringWidth(str);
    x += WINDOW_PADDING;
    
    ofSetColor( this->_save_button_hover  ? 255 : 127 );

    str = "Save";
    this->_font_h2.drawString(str, x, y);
    
    this->_save_button.set(
        x, y-this->_font_h2.stringHeight(str),
        this->_font_h2.stringWidth(str), this->_font_h2.stringHeight(str) );

//    x += this->_font_h2.stringWidth(str);
//    x += WINDOW_PADDING;
    
    y += this->_font_h1.getLineHeight();
    y += WINDOW_PADDING;
    x = WINDOW_PADDING;
    
    //
    
    ofSetColor(255);
    
    int width = INTERFACE_WIDTH - 2*WINDOW_PADDING;
    int height = INTERFACE_HEIGHT - WINDOW_PADDING - y;
    
    bool depth_new = this->_kinect->isNew();
    ofTexture & depth_tex = this->_kinect->getTextureDepth();
    ofTexture & rgb_tex = this->_kinect->getTextureRgb();

    ofPushMatrix();
    ofTranslate(x, y);
    this->_projection_x = x;
    this->_projection_y = y;
    if ( this->_mode == 0 ) { this->_projection->draw(width, height, this->_font_p); }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x, y);
    this->_mapping_x = x;
    this->_mapping_y = y;
    if ( this->_mode == 1 ) {
        this->_mapping->draw(width, height, depth_tex, rgb_tex, depth_new, this->_font_p); }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x, y);
    this->_detection_x = x;
    this->_detection_y = y;
    if ( this->_mode == 2 )
    {
        this->_mapping->drawFbo(depth_tex, depth_new);
        this->_detection->draw(width, height, this->_mapping->getFboTexture(), this->_font_p);

    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(x, y);
    if ( this->_mode == 3 )
    {
        this->_mapping->drawFbo(depth_tex, depth_new);

        this->_preview->draw(
            width, height, this->_detection->getSteps(),
            this->_detection->getMin()/100.f, this->_detection->getMax()/100.f,
            this->_projection->getOffsetTexX()/1000.f, this->_projection->getOffsetTexY()/1000.f,
            this->_mapping->getFboTexture(), scene);
    }
    ofPopMatrix();
}

ofTexture & Interface::getProjectionTexture()
{
    return this->_preview->getFboTexture();
}

ProjectionShPtr Interface::getProjection()
{
    return this->_projection;
}


void Interface::mousePressed(int x, int y)
{
    for ( int i = 0; i < this->_buttons.size(); i++)
    {
        if ( this->_buttons[i].inside(x, y) )
        {
            this->_mode = i;
        }
    }
    
    if ( this->_save_button.inside(x, y) )
    {
        this->save();
    }
    
    if ( this->_mode == 0 )
    {
        this->_projection->mousePressed(x-this->_projection_x, y-this->_projection_y);
    }
    if ( this->_mode == 1 )
    {
        this->_mapping->mousePressed(x-this->_mapping_x, y-this->_mapping_y);
    }
    if ( this->_mode == 2 )
    {
        this->_detection->mousePressed(x-this->_detection_x, y-this->_detection_y);
    }
}

void Interface::mouseMoved(int x, int y)
{
    if ( this->_mode == 0 )
    {
        this->_projection->mouseMoved(x-this->_projection_x, y-this->_projection_y);
    }
    if ( this->_mode == 1 )
    {
        this->_mapping->mouseMoved(x-this->_mapping_x, y-this->_mapping_y);
    }
    if ( this->_mode == 2 )
    {
        this->_detection->mouseMoved(x-this->_detection_x, y-this->_detection_y);
    }
    
    for ( int i = 0; i < this->_buttons.size(); i++)
    {
        if ( this->_buttons[i].inside(x, y) )
        {
            this->_buttons_hover[i] = true;
        }
        else
        {
            this->_buttons_hover[i] = false;
        }
    }
    
    if ( this->_save_button.inside(x, y) )
    {
        this->_save_button_hover = true;
    }
    else
    {
        this->_save_button_hover = false;
    }
}

void Interface::mouseDragged(int x, int y)
{
    if ( this->_mode == 0 )
    {
        this->_projection->mouseDragged(x-this->_projection_x, y-this->_projection_y);
    }
    if ( this->_mode == 1 )
    {
        this->_mapping->mouseDragged(x-this->_mapping_x, y-this->_mapping_y);
    }
    if ( this->_mode == 2 )
    {
        this->_detection->mouseDragged(x-this->_detection_x, y-this->_detection_y);
    }
}

void Interface::mouseReleased(int x, int y)
{
    if ( this->_mode == 0 )
    {
        this->_projection->mouseReleased(x-this->_projection_x, y-this->_projection_y);
    }
    if ( this->_mode == 1 )
    {
        this->_mapping->mouseReleased(x-this->_mapping_x, y-this->_mapping_y);
    }
    if ( this->_mode == 2 )
    {
        this->_detection->mouseReleased(x-this->_detection_x, y-this->_detection_y);
    }
}

void Interface::keyPressed(int key)
{
    if (this->_mode == 0 ) { this->_projection->keyPressed(key); }
    if (this->_mode == 1 ) { this->_mapping->keyPressed(key); }
    if (this->_mode == 2 ) { this->_detection->keyPressed(key); }

    _keypressed[key] = true;

//    1 38
//    2 233
//    3 34
//    4 39
//    up 57357
//    down 57359
//    left 57356
//    right 57358
        
    if ( _keypressed[38] && _keypressed[57358] ) { this->_projection->incrTLX(1); }
    if ( _keypressed[38] && _keypressed[57356] ) { this->_projection->incrTLX(-1); }
    if ( _keypressed[38] && _keypressed[57357] ) { this->_projection->incrTLY(-1); }
    if ( _keypressed[38] && _keypressed[57359] ) { this->_projection->incrTLY(1); }

    if ( _keypressed[233] && _keypressed[57358] ) { this->_projection->incrTRX(-1); }
    if ( _keypressed[233] && _keypressed[57356] ) { this->_projection->incrTRX(1); }
    if ( _keypressed[233] && _keypressed[57357] ) { this->_projection->incrTRY(-1); }
    if ( _keypressed[233] && _keypressed[57359] ) { this->_projection->incrTRY(1); }
    
    if ( _keypressed[34] && _keypressed[57358] ) { this->_projection->incrBRX(-1); }
    if ( _keypressed[34] && _keypressed[57356] ) { this->_projection->incrBRX(1); }
    if ( _keypressed[34] && _keypressed[57357] ) { this->_projection->incrBRY(1); }
    if ( _keypressed[34] && _keypressed[57359] ) { this->_projection->incrBRY(-1); }
    
    if ( _keypressed[39] && _keypressed[57358] ) { this->_projection->incrBLX(1); }
    if ( _keypressed[39] && _keypressed[57356] ) { this->_projection->incrBLX(-1); }
    if ( _keypressed[39] && _keypressed[57357] ) { this->_projection->incrBLY(1); }
    if ( _keypressed[39] && _keypressed[57359] ) { this->_projection->incrBLY(-1); }
    
    
    float incr = 1;
    if ( _keypressed[3680] ) { incr = 0.1f; }
    if ( _keypressed[3686] ) { incr = 0.01f; }
    if ( _keypressed[120] && _keypressed[57357] ) { this->_projection->incrOffsetTexX(incr); }
    if ( _keypressed[120] && _keypressed[57359] ) { this->_projection->incrOffsetTexX(-incr); }
    
    if ( _keypressed[121] && _keypressed[57357] ) { this->_projection->incrOffsetTexY(incr); }
    if ( _keypressed[121] && _keypressed[57359] ) { this->_projection->incrOffsetTexY(-incr); }
    
    if ( key == 115 )
    {
        this->_config->setMaskTLx( this->_projection->getTopLeftX() );
        this->_config->setMaskTLy( this->_projection->getTopLeftY() );

        this->_config->setMaskTRx( this->_projection->getTopRightX() );
        this->_config->setMaskTRy( this->_projection->getTopRightY() );

        this->_config->setMaskBLx( this->_projection->getBottomLeftX() );
        this->_config->setMaskBLy( this->_projection->getBottomLeftY() );

        this->_config->setMaskBRx( this->_projection->getBottomRightX() );
        this->_config->setMaskBRy( this->_projection->getBottomRightY() );
        
        this->_config->setOffsetTexX( this->_projection->getOffsetTexX() );
        this->_config->setOffsetTexY( this->_projection->getOffsetTexY() );

        this->_config->save();
    }
}

void Interface::keyReleased(int key)
{
    _keypressed[key] = false;
    if (this->_mode == 0 ) { this->_projection->keyReleased(key); }
    if (this->_mode == 1 ) { this->_mapping->keyReleased(key); }
    if (this->_mode == 2 ) { this->_detection->keyReleased(key); }
}

void Interface::save()
{
    if (this->_mode == 0 || Parameter::_osc_mode == "receive" )
    {
        this->_config->setMaskTLx( this->_projection->getTopLeftX() );
        this->_config->setMaskTLy( this->_projection->getTopLeftY() );

        this->_config->setMaskTRx( this->_projection->getTopRightX() );
        this->_config->setMaskTRy( this->_projection->getTopRightY() );

        this->_config->setMaskBLx( this->_projection->getBottomLeftX() );
        this->_config->setMaskBLy( this->_projection->getBottomLeftY() );

        this->_config->setMaskBRx( this->_projection->getBottomRightX() );
        this->_config->setMaskBRy( this->_projection->getBottomRightY() );

        this->_config->setOffsetTexX( this->_projection->getOffsetTexX() );
        this->_config->setOffsetTexY( this->_projection->getOffsetTexY() );
        
        this->_config->save();
    }
    
    if (this->_mode == 1 || Parameter::_osc_mode == "receive" )
    {
        this->_config->setDetectionScale( this->_mapping->getScale() );
        this->_config->setDetectionX( this->_mapping->getX() );
        this->_config->setDetectionY( this->_mapping->getY() );
        this->_config->setDetectionRot( this->_mapping->getRot() );

        this->_config->save();        
    }
    
    if (this->_mode == 2 || Parameter::_osc_mode == "receive" )
    {
        this->_config->setDetectionMin( this->_detection->getMin() );
        this->_config->setDetectionMax( this->_detection->getMax() );
        this->_config->setDetectionSteps( this->_detection->getSteps() );

        this->_config->save();
    }
    
    if ( Parameter::_osc_mode == "send" )
    {
        ofxOscMessage mess;
        mess.setAddress("save");
        mess.addIntArg(1);
        Parameter::_osc_sender.sendMessage(mess);
    }
}
