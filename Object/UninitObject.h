//
// Created by Igor Maximov on 03.05.2020.
//

#pragma once
#include "ClassObject.h"

class UninitObject : public Object {
 public:
  UninitObject() : Object(nullptr) {}
  
  int ToInt() override;
};

