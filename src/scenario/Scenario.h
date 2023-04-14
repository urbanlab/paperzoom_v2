#ifndef scenario_h
#define scenario_h

#include "ofMain.h"

//-------------------------------------------------------------------------------

#include "tools/Macro.h"
DEF_CLASS_SMART_PTR(Scenario)

//-------------------------------------------------------------------------------

class Scenario
{
	//-- CONSTRUCTION / DESTRUCTION ---------------------------------------------

public:

	static ScenarioShPtr create(const std::string & path);
	virtual ~Scenario();

protected:

	Scenario(const std::string & path);

public:

	ScenarioWkPtr _weak_ptr;

	//-- METHODS ----------------------------------------------------------------
    
public:
    
    void bindImages(ofShader & shader);
    void unbindImages();

	//-- ATTRIBUTES -------------------------------------------------------------

private:
    
    std::map<int,ofImage> _images;
    std::map<int,ofVideoPlayer> _videos;

};

#endif /* scenario_h */
