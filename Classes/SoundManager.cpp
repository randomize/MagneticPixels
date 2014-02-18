#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "MPix.h"
#include "Cmd.h"

using namespace MPix;
using namespace CocosDenshion;

SoundManager::SoundManager()
{

   // Register BG/FG handling
   CmdDidEnterBG::listners["SoundManager"] = std::bind(&SoundManager::PauseEngine, this) ;
   CmdWillEnterFG::listners["SoundManager"] = std::bind(&SoundManager::ResumeEngine, this) ;
   CmdPlaySound::listners["SoundManager"] = std::bind(&SoundManager::PlaySoundSync, this, std::placeholders::_1) ;


   // set default volume to max
   // TODO: load from settings!
   SimpleAudioEngine::getInstance()->setEffectsVolume(1);
   SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);


   // TODO: load predefined packs


   EM_LOG_INFO("[ SoundManager initialized ]");

}

EmbossLib::ErrorCode MPix::SoundManager::PlayInBackground( const string& name )
{
   // TODO: implement
   EM_LOG_INFO("SoundManager is playing gentle music now");
   //SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(),true);
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::SoundManager::PlaySoundSync( const string& name )
{
   // TODO: implement different formats:
   // http://www.cocos2d-x.org/projects/cocos2d-x/wiki/Audio_formats_supported_by_CocosDenshion_on_different_platforms
   // WAV on windows
   EM_LOG_INFO("Playing sound file: " + name);
   SimpleAudioEngine::getInstance()->playEffect((name + ".wav").c_str());
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::SoundManager::UpdateSoundSystem( float deltha )
{
   // TODO: get time and perform playback
   return ErrorCode::RET_OK;
}

void MPix::SoundManager::InitSoundSystem()
{
   // Any kind of preparation here
}


ErrorCode SoundManager::PauseEngine()
{
   SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
   return ErrorCode::RET_OK;
}

ErrorCode SoundManager::ResumeEngine()
{
   SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
   return ErrorCode::RET_OK;
}

