//===-- SimpleInputDialog.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/17
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Prompts user for input
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SIMPLEINPUTDIALOG_H_
#define SIMPLEINPUTDIALOG_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies

   // SimpleInputDialog

   class SimpleInputDialog : public Layer
   {
   public:

      typedef std::function<void(std::string)> FunType;

      static SimpleInputDialog* create(const char* req = "Input, please", const char* def = "");

      void SetCallback(FunType f) { fun = f; }

   protected:

      bool initWithRequast(const char* req = "Input, please", const char* def = "");

      void OnOK();
      void OnCancel();

   private:

      TextFieldTTF* field;

      void Terminate();

      FunType fun;

   };

}


#endif // SIMPLEINPUTDIALOG_H_
