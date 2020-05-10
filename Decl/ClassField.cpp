//
// Created by Igor Maximov on 08.05.2020.
//

#include "ClassField.h"

ClassField::ClassField(Type *type, std::string label) : type(type), name(std::move(label)) {
}

void ClassField::Accept(Visitor::Base *visitor) {
  visitor->Visit(this);
}
