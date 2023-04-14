#ifndef macro_h
#define macro_h

#include <memory>

// SIMPLE CLASSES ---------------------------------------------------------------

// define a shared pointer
#define DEF_SH_PTR( className ) \
typedef std::shared_ptr<className> className##ShPtr; \

// define a class and an associated shared pointer
#define DEF_CLASS_SH_PTR( className ) \
class className; \
DEF_SH_PTR( className ) \

// define a weak pointer
#define DEF_WK_PTR( className ) \
typedef std::weak_ptr<className> className##WkPtr; \

// define a class and associated shared and weak pointers
#define DEF_CLASS_SMART_PTR( className ) \
class className; \
DEF_SH_PTR( className ) \
DEF_WK_PTR( className ) \

// TEMPLATED CLASSES ------------------------------------------------------------

// define a templated class
#define DEF_TEMP_CLASS( className, typeName ) \
template <typename typeName> \
class className; \

// define a templated class and associated shared and weak pointers
#define DEF_TEMP_CLASS_SMART_PTR( className, typeName ) \
DEF_TEMP_CLASS( className, typeName ) \
template <typename typeName> \
using className##ShPtr = std::shared_ptr<className<typeName>>; \
template <typename typeName> \
using className##WkPtr = std::weak_ptr<className<typeName>>; \

//-------------------------------------------------------------------------------

#endif /* macro_h */
