//===-- AppDelegate.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Main cocos2dx application class
//
//===---------------------------------------------------------===//

#pragma once
#ifndef APPDELEGATE_H_
#define APPDELEGATE_H_

#include "ECBase.h"
#include "ECBaseMasterLoop.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
   ~AppDelegate() override;
   bool applicationDidFinishLaunching() override;
   void applicationDidEnterBackground() override;
   void applicationWillEnterForeground() override;
   MPix::ECBaseMasterLoop *master;
};

#endif // APPDELEGATE_H_
