#ifndef appwindow_h
#define appwindow_h

//-------------------------------------------------------------------------------

#include "ofAppGLFWWindow.h"
#include "Globals.h"
#include "Macro.h"

DEF_SH_PTR(ofAppGLFWWindow)
DEF_CLASS_SH_PTR(Config)
DEF_CLASS_SMART_PTR(AppWindow)

//-------------------------------------------------------------------------------

class AppWindow
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static AppWindowShPtr create(ConfigShPtr config, ofAppGLFWWindowShPtr window);
	virtual ~AppWindow();

protected:

	AppWindow(ConfigShPtr config, ofAppGLFWWindowShPtr window);

public:

	AppWindowWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void setup();
    
	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    ConfigShPtr _config;
	ofAppGLFWWindowShPtr _window;
};

#endif /* appwindow_h */
