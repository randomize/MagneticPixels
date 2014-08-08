#include "PlatformManager.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

// Jni support for android
#include <jni.h>
#include <android_native_app_glue.h>

// Vector2er to android add, for JNI usage
extern struct android_app* a_app;

// Env for JNI calls
JNIEnv *env = nullptr;
JavaVM *jvm = nullptr;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

// For Desktop directory lookup
#include <shlobj.h>


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

// Some ios specific stuff

#endif

namespace ECCore {

   //////////////////////////////////////////////////////////////////////////
   // PlatformManager's multi platform methods
   //////////////////////////////////////////////////////////////////////////


   PlatformManager::PlatformManager()
   {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      jvm = a_app->activity->vm;

      jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
      if (ret != JNI_OK) {
         ret = a_app->activity->vm->AttachCurrentThread(&env, 0);
         if (ret != JNI_OK) {
            // TODO: report bug, show message to user

         }
      }
      ECLOG_INFO("=== Android platform manager ready ====");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
      ECLOG_INFO("=== Windows platform manager ready ====");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      ECLOG_INFO("=== IOS platform manager ready ====");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
      ECLOG_INFO("=== Linux platform manager ready ====");
#else
      ECLOG_ERROR("=== No platform manager for this platform ===");
      assert(false);
#endif
   }

   PlatformManager::~PlatformManager()
   {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      jvm->DetachCurrentThread();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#endif
   }

   string PlatformManager::GetSavingPath()
   {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      return GetSDCardPath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
      return GetDesktopPath();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      return "Sorry, I am iOS, you can't access file system";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
      return "/home/randy/Desktop";
#endif
   }

   void PlatformManager::ShowMessage( const char* title, const char* text )
   {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      MessageBox(text, title);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
      //auto inst = GetModuleHandle();
      MessageBoxA(0,text,title, MB_OK | MB_ICONINFORMATION);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      MessageBox(text, title);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
      MessageBox(text, title);
#endif
   }



   //////////////////////////////////////////////////////////////////////////
   // Platform specific methods
   //////////////////////////////////////////////////////////////////////////

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

   string PlatformManager::GetSDCardPath()
   {
      jclass env_class = env->FindClass("android/os/Environment"); 
      jmethodID getextstor_id = env->GetStaticMethodID(
         env_class,"getExternalStorageDirectory", "()Ljava/io/File;"
         ); 
      jobject file_obj = env->CallStaticObjectMethod(env_class, getextstor_id);
      jclass file_class = env->FindClass("java/io/File"); 
      jmethodID filetostr_id = env->GetMethodID(file_class, "getAbsolutePath", "()Ljava/lang/String;");
      jstring dirstr_obj = (jstring)env->CallObjectMethod(file_obj, filetostr_id);
      const char *nativeString = env->GetStringUTFChars(dirstr_obj, 0); 
      string s(nativeString);
      env->ReleaseStringUTFChars(dirstr_obj, nativeString);
      return s;
   }

   string PlatformManager::GetDeviceModel()
   {
      JNIEnv *env;
      a_app->activity->vm->AttachCurrentThread(&env, 0);

      jclass build_class = env->FindClass("android/os/Build"); 
      jfieldID brand_id = env->GetStaticFieldID(build_class, "MODEL", "Ljava/lang/String;"); 
      jstring brand_obj = (jstring)env->GetStaticObjectField(build_class, brand_id); 
      const char *nativeString = env->GetStringUTFChars(brand_obj, 0); 
      string s(nativeString);
      env->ReleaseStringUTFChars(brand_obj, nativeString);
      return s;
      return "Not android";
   }

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

   string PlatformManager::GetDesktopPath()
   {
      char path[MAX_PATH+1];
      if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_DESKTOPDIRECTORY, FALSE))
         return path;
      else
         return "ERROR";
   }

#endif

}
