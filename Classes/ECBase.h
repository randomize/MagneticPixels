/* 
   Main header for all cocos games
   by Mihailenco E, TheEndlessCat Games, 2013-2014
*/

#pragma once
#ifndef ECBASE_H
#define ECBASE_H

//===-------------- Globals section -------------------------------------------===//

// Putting usings in headers is generally bad idea,
// I turn this off when project is finished
#define ECUSINGS_ON

// My own debuggint specification
#define ECDEBUG


/* Namespaces:
  EndlessCatLib -- user components 
  ECCore -- internal components
 */

//===----------- Standard libs section ----------------------------------------===//

// Standard libs
#include <cstdlib>
#include <cmath>
#include <list>
#include <forward_list>
#include <deque>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <typeinfo>

#ifdef ECUSINGS_ON

   using std::vector;
   using std::list;
   using std::deque;
   using std::string;
   using std::ostringstream;
   using std::ostream;
   using std::cout;
   using std::sort;
   using std::endl;
   using std::find_if;
   using std::istringstream;
   using std::remove_if;
   using std::function;
   using std::shared_ptr;
   using std::unordered_map;
   using std::unordered_multimap;
   using std::unordered_set;
   using std::make_shared;
   using std::make_pair;
   using std::dynamic_pointer_cast;
   using std::forward_list;

#endif // ECUSINGS_ON


//===----------- Cocos section ----------------------------------------===//

// Include cocos itself
//#include "cocos2d.h"

#define COCOS2D_DEBUG 1

#ifdef __clang__

   #pragma clang diagnostic push
   #pragma clang diagnostic ignored "-Wall"
   #pragma clang diagnostic ignored "-Wextra"
   #include <cocos2d.h>
   #include <cocos-ext.h>
   #include <cocostudio/CocoStudio.h>
   #include <external/tinyxml2/tinyxml2.h>
   #pragma clang diagnostic pop

#elif _MSC_VER
   #pragma warning (push, 0)
   #include "cocos2d.h"
   #include "cocos-ext.h"
   #include "cocostudio/CocoStudio.h"
   #include "external/tinyxml2/tinyxml2.h"
   #pragma warning (pop)

#elif __GNUG__

   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wall"
   #pragma GCC diagnostic ignored "-Wextra"
   #include "cocos2d.h"
   #include "extensions/cocos-ext.h"
   #include "editor-support/cocostudio/CocoStudio.h"
   #include "external/tinyxml2/tinyxml2.h"
   #pragma GCC diagnostic pop

#endif


#ifdef ECUSINGS_ON
   using namespace cocos2d;
   //using namespace CocosDenshion;
   using namespace cocos2d::extension;
   using namespace cocostudio;
#endif

//===----------- Helper functions ----------------------------------------===//

namespace EndlessCatLib {

   enum class ErrorCode {
      RET_OK,
      RET_FAIL,
      RET_YES,
      RET_NO
   };


   inline string StringToLowcase(string str)
   {
       std::transform(str.begin(), 
                      str.end(),
                      str.begin(),
                     [](char ch ){ return tolower(ch); } 
       );
       return str;
   }

   inline ErrorCode StringToBool(string str, bool& value)
   {
       str = StringToLowcase( str );
       if ( str == "true") {
           value = true;
           return  ErrorCode::RET_OK;
       } else if (str == "false") {
           value = false;
           return  ErrorCode::RET_OK;
       }
       return ErrorCode::RET_FAIL;
   }

   inline void ScaleToSize(Node* node, cocos2d::Size size )
   {
       auto contentSize = node->getContentSize();
       float xScale = size.width / contentSize.width;
       node->setScaleX(xScale);
       float yScale = size.height / contentSize.height;
       node->setScaleY(yScale);
   }

   template <typename T>
   string ToString ( T Number )
   {
      ostringstream ss;
      ss << Number;
      return ss.str();
   }

   // Own PI constant is a musthave for any project :)
   const float PI_FLOAT = 3.141592654f;

   // Photoshop like OpenGL blend modes for cocos
   const BlendFunc MULTIPLY_FAKE = { GL_ZERO, GL_SRC_COLOR };
   const BlendFunc SCREEN = { GL_ONE, GL_ONE_MINUS_SRC_COLOR };
   const BlendFunc SCREEN2 = { GL_ONE_MINUS_DST_COLOR, GL_ONE };
   const BlendFunc LINEAR_DODGE = { GL_ONE, GL_ONE };

   // Random numbers

   // Return [0..0.999999] random number
   inline float RandomFloat() {
      return float(rand()) / RAND_MAX;
   };

   // Return [0..max) random
   inline float RandomFloat(float max) {
      return float(rand()) / RAND_MAX * max;
   };

   // Return [min, max)
   inline float RandomFloat(float min, float max) {
      return float(rand()) / RAND_MAX * (max - min) + min;
   };

} // namespace EndlessCatLib 

#ifdef ECUSINGS_ON
using namespace EndlessCatLib;
#endif // ECUSINGS_ON


//===----------- Logging system ----------------------------------------===//

// Fast version (can be globally shut down)
//#define ECLOG_OFF // Uncomment do off completely

// Choose filter for logging (ON = uncommented)
#define ECLOGGING_ERRORS
#define ECLOGGING_WARNINGS
#define ECLOGGING_INFO
#define ECLOGGING_DEBUG

// Slow version with dynamic verb. control
//#define ECLOG_DYNAMIC

// Usage:
// ECLOG_ERROR("Got real error");
// ECLOG_INFO(42);
// ECLOG_WARNING("X="+101.6+" d="+string);
// ECLOG_DEBUG("A holds: " + instanceOfA ); // A has operator<< defined
// ECLOG( "This" + " is " + 'c' + 0 + "ol!" );

namespace ECCore {

   // This fast and dirty way of implementing + concatenation
   class ECChainConcatenator{
      std::ostringstream out;
   public:
      template <class T>
      inline ECChainConcatenator& operator+(const T &rhs){
         out << rhs;
         return *this;
      }
      std::string str(){
         return out.str();
      }
   };

   #ifdef ECLOG_DYNAMIC

   // Supporting class for dynamic logging
   class LogLevelHolder{
      public:
         LogLevelHolder() { logLevel = 4; }
         void SetLogLevel(int x) { logLevel = x; }
         int GetLogLevel() { return logLevel; }
      private:
         int logLevel;
   };
   LogLevelHolder& LogLevel(){
      static LogLevelHolder log;
      return log;
   }

   #endif // ECLOG_DYNAMIC

}

#ifdef ECLOG_DYNAMIC
   #define ECLOG_SETLEVEL(x) do { ECCore::LogLevel().SetLogLevel(x); } while(false)
   #define ECLOG_SET_OFF     EC_LOG_SETLEVEL(0)
   #define ECLOG_SET_ERRORS  EC_LOG_SETLEVEL(1)
   #define ECLOG_SET_WARNING EC_LOG_SETLEVEL(2)
   #define ECLOG_SET_INFO    EC_LOG_SETLEVEL(3)
   #define ECLOG_SET_DEBUG   EC_LOG_SETLEVEL(4)
#else
   #define ECLOG_SET_OFF
   #define ECLOG_SET_ERRORS
   #define ECLOG_SET_WARNINGS
   #define ECLOG_SET_INFO
   #define ECLOG_SET_DEBUG
   #define PL_LOG_SET_LEVEL(x)
#endif

// Logging is muted entirely by default
#define ECLOG_ERROR(s)  
#define ECLOG_WARNING(s)
#define ECLOG_INFO(s)  
#define ECLOG_DEBUG(s)
#define ECLOG(s) 


#ifndef ECLOG_OFF

   #ifdef WIN32

   namespace ECCore {
   inline void SetConsoleDefault () { // Reset color
      SetConsoleTextAttribute(
         GetStdHandle(STD_OUTPUT_HANDLE), 
         /*FOREGROUND_INTENSITY | */FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
      );
   }

   inline void SetConsoleRed () { // ERROR
      SetConsoleTextAttribute(
         GetStdHandle(STD_OUTPUT_HANDLE), 
         FOREGROUND_INTENSITY | FOREGROUND_RED 
      );
   }

   inline void SetConsoleGreen () { // INFO
      SetConsoleTextAttribute(
         GetStdHandle(STD_OUTPUT_HANDLE), 
         FOREGROUND_INTENSITY | FOREGROUND_GREEN 
      );
   }
   inline void SetConsoleYellow () { // WARNING
      SetConsoleTextAttribute(
         GetStdHandle(STD_OUTPUT_HANDLE), 
         FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED 
      );
   }
   }

      #define __NAME_OF_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

      #define ECCONCATENATOR_HELPER(s) __NAME_OF_FILE__,  __LINE__,  (ECCore::ECChainConcatenator() + s).str().c_str()

      #undef ECLOG
      #define ECLOG(s) do { CCLOG("%s:%d %s", EC_CONCATENATOR_HELPER(s) ); } while(false)

      #ifdef ECLOG_DYNAMIC

         #define ECLOG_ERROR(s)                           \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 1 ) {     \
               CCLOG("%s:%d [EE] %s", ECCONCATENATOR_HELPER(s) ); \
            }                                              \
         } while(false)

         #define ECLOG_WARNING(s)                         \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 2 ) {     \
               CCLOG("%s:%d [WW] %s", ECCONCATENATOR_HELPER(s) ); \
            }                                              \
         } while(false)

         #define ECLOG_INFO(s)                            \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 3 ) {     \
               CCLOG("%s:%d [II] %s", ECCONCATENATOR_HELPER(s) ); \
            }                                              \
         } while(false)

         #define ECLOG_DEBUG(s)                           \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 4 ) {     \
               CCLOG("%s:%d [DD] %s", ECCONCATENATOR_HELPER(s) ); \
            }                                              \
         } while(false)

      #else // Else static

         #ifdef ECLOGGING_ERRORS
            #undef ECLOG_ERROR
            #define ECLOG_ERROR(s) \
               do { \
                  ECCore::SetConsoleRed();\
                  CCLOG("%s:%d: [EE] %s", ECCONCATENATOR_HELPER(s));\
                  ECCore::SetConsoleDefault();\
               } while(false)
         #endif

         #ifdef ECLOGGING_WARNINGS
            #undef ECLOG_WARNING
            #define ECLOG_WARNING(s) \
               do { \
                  ECCore::SetConsoleYellow();\
                  CCLOG("%s:%d: [WW] %s", ECCONCATENATOR_HELPER(s)); \
                  ECCore::SetConsoleDefault();\
               } while(false)
         #endif

         #ifdef ECLOGGING_INFO
            #undef ECLOG_INFO
            #define ECLOG_INFO(s) \
               do { \
                  ECCore::SetConsoleGreen();\
                  CCLOG("%s:%d: [II] %s", ECCONCATENATOR_HELPER(s)); \
                  ECCore::SetConsoleDefault();\
               } while(false)
         #endif

         #ifdef ECLOGGING_DEBUG
            #undef ECLOG_DEBUG
            #define ECLOG_DEBUG(s) do { CCLOG("%s:%d: [DD] %s", EC_CONCATENATOR_HELPER(s)); } while(false)
         #endif

      #endif

   #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

      // By default android SDK is compiled with GCC -- use PrettyFunction
      #define ECCONCATENATOR_HELPER(s) __FILE__,  __LINE__, __PRETTY_FUNCTION__, (ECCore::ECChainConcatenator() + s).str().c_str()

      #undef ECLOG
      #define ECLOG(s) do { CCLOG("%s:%d:%s %s", EC_CONCATENATOR_HELPER(s) ); } while(false)

      #ifdef ECLOG_DYNAMIC

         #define ECLOG_ERROR(s)                                   \
         do {                                                      \
            if ( ECCore::LogLevel().GetLogLevel() >= 1 ) {             \
               CCLOG("\033[1;31m%s:%d:%s [EE] %s\033[0m", ECCONCATENATOR_HELPER(s) ); \
            }                                                      \
         } while(false)

         #define ECLOG_WARNING(s)                                 \
         do {                                                      \
            if ( ECCore::LogLevel().GetLogLevel() >= 2 ) {             \
               CCLOG("\033[1;33m%s:%d:%s [WW] %s\033[0m", ECCONCATENATOR_HELPER(s) ); \
            }                                                      \
         } while(false)

         #define ECLOG_INFO(s)                                    \
         do {                                                      \
            if ( ECCore::LogLevel().GetLogLevel() >= 3 ) {             \
               CCLOG("\033[1;32m%s:%d:%s [II] %s\033[0m", ECCONCATENATOR_HELPER(s) ); \
            }                                                      \
         } while(false)

         #define ECLOG_DEBUG(s)                                   \
         do {                                                      \
            if ( ECCore::LogLevel().GetLogLevel() >= 4 ) {             \
               CCLOG("\033[1;35m%s:%d:%s [DD] %s\033[0m", ECCONCATENATOR_HELPER(s) ); \
            }                                                      \
         } while(false)

      #else // Else static

         #ifdef ECLOGGING_ERRORS
            #undef ECLOG_ERROR
            #define ECLOG_ERROR(s) do { CCLOG("%s:%d:%s [EE] %s", EC_CONCATENATOR_HELPER(s)); } while(false)
         #endif

         #ifdef ECLOGGING_WARNINGS
            #undef ECLOG_WARNING
            #define ECLOG_WARNING(s) do { CCLOG("%s:%d:%s [WW] %s",  EC_CONCATENATOR_HELPER(s)); } while(false)
         #endif

         #ifdef ECLOGGING_INFO
            #undef ECLOG_INFO
            #define ECLOG_INFO(s) do { CCLOG("%s:%d:%s [II] %s",  EC_CONCATENATOR_HELPER(s)); } while(false)
         #endif

         #ifdef ECLOGGING_DEBUG
            #undef ECLOG_DEBUG
            #define ECLOG_DEBUG(s) do { CCLOG("%s:%d:%s [DD] %s",  EC_CONCATENATOR_HELPER(s)); } while(false)
         #endif

      #endif

   #else // Linux and OS X and others

      #define __NAME_OF_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

      // Use escape sequences in nix consoles, also GCC can insert full Function names
      #define ECCONCATENATOR_HELPER(s) __NAME_OF_FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << ": " << (ECCore::ECChainConcatenator() + s).str() << "\033[0m" << endl;

      #undef ECLOG
      #define ECLOG(s) do { cout << EC_CONCATENATOR_HELPER(s) } while(false)

      #ifdef DYNAMIC_VERBOSITY

         #define ECLOG_ERROR(s)                           \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 1 ) {     \
                cout << "\033[1;31m" << ECCONCATENATOR_HELPER(s)  \
            }                                              \
         } while(false)

         #define ECLOG_WARNING(s)                         \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 2 ) {     \
                cout << "\033[1;33m" << ECCONCATENATOR_HELPER(s) \
            }                                              \
         } while(false)

         #define ECLOG_INFO(s)                            \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 3 ) {     \
                cout << "\033[1;32m" << ECCONCATENATOR_HELPER(s) \
            }                                              \
         } while(false)

         #define ECLOG_DEBUG(s)                           \
         do {                                              \
            if ( ECCore::LogLevel().GetLogLevel() >= 4 ) {     \
                cout << "\033[1;35m" << ECCONCATENATOR_HELPER(s) \
            }                                              \
         } while(false)

      #else

         #ifdef ECLOGGING_ERRORS
            #undef ECLOG_ERROR
            #define ECLOG_ERROR(s) do { cout << "\033[1;31m" << EC_CONCATENATOR_HELPER(s) } while(false)
         #endif

         #ifdef ECLOGGING_WARNINGS
            #undef ECLOG_WARNING
            #define ECLOG_WARNING(s) do { cout << "\033[1;33m" << EC_CONCATENATOR_HELPER(s) } while(false)
         #endif

         #ifdef ECLOGGING_INFO
            #undef ECLOG_INFO
            #define ECLOG_INFO(s) do { cout << "\033[1;32m" << EC_CONCATENATOR_HELPER(s) } while(false)
         #endif

         #ifdef ECLOGGING_DEBUG
            #undef ECLOG_DEBUG
            #define ECLOG_DEBUG(s) do { cout << "\033[1;35m" << EC_CONCATENATOR_HELPER(s) } while(false)
         #endif

      #endif

   #endif

#endif // !ECLOG_OFF




#include <iterator>

template<class enum_type>
class enum_iterator {
private:
   enum_type value;
   typedef typename std::underlying_type<enum_type>::type under;
public:   
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef enum_type value_type;
   typedef enum_type reference;
   typedef enum_type* pointer;
   typedef std::random_access_iterator_tag iterator_category;

   enum_iterator() :value() {}
   enum_iterator(const enum_iterator& rhs) :value(rhs.value) {}
   explicit enum_iterator(enum_type value_) :value(value_) {}
   ~enum_iterator() {}

   enum_iterator& operator=(const enum_iterator& rhs) {value=rhs.value; return *this;}
   enum_iterator& operator++() {value = (enum_type)(under(value) + 1); return *this;}
   enum_iterator operator++(int) {enum_iterator r(*this); ++*this; return r;}
   enum_iterator& operator+=(size_type o) {value = (enum_type)(under(value) + o); return *this;}
   enum_iterator& operator--() {value = (enum_type)(under(value) - 1); return *this;}
   enum_iterator operator--(int) {enum_iterator r(*this); --*this; return r;}
   enum_iterator& operator-=(size_type o) {value = (enum_type)(under(value) + o); return *this;}
   reference operator*() const {return value;}
   reference operator[](size_type o) const {return (enum_type)(under(value) + o);}
   const enum_type* operator->() const {return &value;}

   friend enum_iterator operator-(const enum_iterator& it, size_type o) {return enum_iterator((enum_type)(under(it)-o));}
   friend difference_type operator-(enum_iterator lhs, enum_iterator rhs) {return under(lhs.value)-under(rhs.value);}
   friend enum_iterator operator+(const enum_iterator& it, size_type o) {return enum_iterator((enum_type)(under(it)+o));}
   friend enum_iterator operator+(size_type o, const enum_iterator& it) {return enum_iterator((enum_type)(under(it)+o));}
   friend bool operator==(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value==rhs.value;}
   friend bool operator!=(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value!=rhs.value;} 
   friend bool operator<(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value<rhs.value;}
   friend bool operator>(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value>rhs.value;}
   friend bool operator<=(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value<=rhs.value;}
   friend bool operator>=(const enum_iterator& lhs, const enum_iterator& rhs) {return lhs.value>=rhs.value;}
   friend void swap(const enum_iterator& lhs, const enum_iterator& rhs) {std::swap(lhs.value, rhs.value);}
};

template<class enum_type> struct EnumRanger {
   EnumRanger(enum_type) {}
   EnumRanger() {}
   enum_iterator<enum_type> begin() { return enum_iterator<enum_type>(enum_type::First); }
   enum_iterator<enum_type> end() { return enum_iterator<enum_type>(enum_type::Last); }
};
   

#endif // ECBASE_H
