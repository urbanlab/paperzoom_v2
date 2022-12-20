#include "AppWindow.h"
#include "Config.h"

int INTERFACE_WIDTH;
int INTERFACE_HEIGHT;
int PROJECTION_WIDTH;
int PROJECTION_HEIGHT;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
float WINDOW_RATIO;
glm::vec2 WINDOW_RATIO_2;
int WINDOW_PADDING;
float FPS;

//-- CONSTRUCTION | DESTRUCTION -------------------------------------------------

AppWindowShPtr AppWindow::create(ConfigShPtr config, ofAppGLFWWindowShPtr window)
{
	AppWindowShPtr AppWindow_shptr(new AppWindow(config, window));
	AppWindow_shptr->_weak_ptr = AppWindow_shptr;
	return AppWindow_shptr;
}

AppWindow::~AppWindow()
{
	this->_weak_ptr.reset();
}

AppWindow::AppWindow(ConfigShPtr config, ofAppGLFWWindowShPtr window)
{
    this->_config = config;
	this->_window = window;

    // max window dimension
    
	int screen_width = this->_window->getScreenSize().x;
	int screen_height = this->_window->getScreenSize().y;
    this->_window->setWindowShape(screen_width, screen_height);
    
    PROJECTION_WIDTH = this->_config->_projection_width;
    PROJECTION_HEIGHT = this->_config->_projection_height;
    
    if (this->_config->_projection )
    {
        this->_window->setFullscreen(true);
    }
}

void AppWindow::setup()
{
    int max_width = ofGetWidth();
    int max_height = ofGetHeight();

    if (this->_config->_projection )
    {
//        this->_window->setFullscreen(true);
//        INTERFACE_WIDTH = ofGetScreenWidth();
//        INTERFACE_HEIGHT = ofGetScreenHeight();
//        WINDOW_WIDTH = INTERFACE_WIDTH;
//        WINDOW_HEIGHT = INTERFACE_HEIGHT;
        INTERFACE_WIDTH = min(max_width, this->_config->_interface_width);
        INTERFACE_HEIGHT = min(max_height, this->_config->_interface_height);
        WINDOW_WIDTH = /*INTERFACE_WIDTH+*/PROJECTION_WIDTH;
        WINDOW_HEIGHT = PROJECTION_HEIGHT;//max(INTERFACE_HEIGHT,PROJECTION_HEIGHT);
    }
	else if (this->_config->_interface_fullscreen)
	{
//        INTERFACE_WIDTH = ofGetScreenWidth();
//        INTERFACE_HEIGHT = ofGetScreenHeight();
        INTERFACE_WIDTH = max_width;
        INTERFACE_HEIGHT = max_height;
		WINDOW_WIDTH = INTERFACE_WIDTH;
		WINDOW_HEIGHT = INTERFACE_HEIGHT;
	}
    else
    {
        INTERFACE_WIDTH = min(max_width, this->_config->_interface_width);
        INTERFACE_HEIGHT = min(max_height, this->_config->_interface_height);
        WINDOW_WIDTH = INTERFACE_WIDTH;
        WINDOW_HEIGHT = INTERFACE_HEIGHT;
    }
    
    std::cout << WINDOW_WIDTH << " " << WINDOW_HEIGHT << std::endl;
    this->_window->setWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (this->_config->_interface_center)
    {
        this->_window->setWindowPosition(
            (max_width - WINDOW_WIDTH)*0.5f, (max_height - WINDOW_HEIGHT)*0.5f );
    }
    else
    {
        this->_window->setWindowPosition(this->_config->_interface_x, this->_config->_interface_y);
    }
    
	WINDOW_RATIO_2 = glm::vec2(
        INTERFACE_WIDTH / (float)DEFAULT_INTERFACE_WIDTH,
        INTERFACE_HEIGHT / (float)DEFAULT_INTERFACE_HEIGHT
	);

	WINDOW_RATIO = min(WINDOW_RATIO_2.x, WINDOW_RATIO_2.y);

	WINDOW_PADDING = this->_config->_interface_padding * WINDOW_RATIO;

    FPS = this->_config->_interface_fps;
	ofSetFrameRate(FPS);
    
	this->_window->setVerticalSync(this->_config->_interface_vsync);
}

//-- METHODS --------------------------------------------------------------------
