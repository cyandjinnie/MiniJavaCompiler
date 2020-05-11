//
// Created by Igor Maximov on 09.05.2020.
//

#include <Program.h>
#include <ast.h>
#include <log.h>
#include <iostream>

#include "TypeChecker.h"

void Visitor::TypeChecker::Run(Program *program) {
  Visit(program);
}

void Visitor::TypeChecker::Visit(Program *program) {
  for (auto* class_ : program->classes) {
    class_->Accept(this);
  }
  
  program->main_class->Accept(this);
}

void Visitor::TypeChecker::Visit(Class *class_decl) {
  for (auto* field : class_decl->fields) {
    field->Accept(this);
  }
  
  for (auto* method : class_decl->methods) {
    method->Accept(this);
  }
}

void Visitor::TypeChecker::Visit(ClassMethod *method) {
  current_method = method;
  method->statements->Accept(this);
  current_method = nullptr;
}

void Visitor::TypeChecker::Visit(ClassField *field) {
  if (field->type->type == Type::PossibleTypes::Void) {
    std::cerr << "Unable to declare a variable of type 'void'" << std::endl;
  }
}

void Visitor::TypeChecker::Visit(ProgramBody *body) {
  UNREACHABLE("TypeChecker::Visit(ProgramBody*)");
}

void Visitor::TypeChecker::Visit(MainClass *main_class) {
  main_class->GetMainFunction()->Accept(this);
}

void Visitor::TypeChecker::Visit(Expr::BinaryOp *binary_op) {
  binary_op->left->Accept(this);
  binary_op->right->Accept(this);
  
  if (!IsInt(binary_op->left->type) || !IsInt(binary_op->right->type)) {
    std::cerr << "No matching operator for operands " << Repr(binary_op->left->type) << " and " << Repr(binary_op->right->type) << std::endl;
  }
}

void Visitor::TypeChecker::Visit(Expr::Const *const_expr) {
  const_expr->type = new Int();
}

void Visitor::TypeChecker::Visit(Expr::Id *id) {
  id->type = GetVariableType(id->symbol);
}

void Visitor::TypeChecker::Visit(Expr::This *this_expr) {

}

void Visitor::TypeChecker::Visit(Expr::UnaryOp *unary_op) {
  unary_op->expr->Accept(this);
  
  unary_op->type = unary_op->expr->type;
}

void Visitor::TypeChecker::Visit(Stmt::Assign *assn) {
  assn->rhs->Accept(this);
  assn->lhs->Accept(this);
  
  if (!SameType(assn->lhs->type, assn->rhs->type)) {
    std::cerr << "Incompatible types: " << Repr(assn->lhs->type) << " <- " << Repr(assn->rhs->type) << std::endl;
  }
}

void Visitor::TypeChecker::Visit(Stmt::Cond *cond) {
  cond->condition->Accept(this);
  
  if (!IsBool(cond->condition->type)) {
    std::cerr << "Incompatible types: " << Repr(cond->condition->type) << " at 'if' condition" << std::endl;
  }
}

void Visitor::TypeChecker::Visit(Stmt::Print *print) {
  print->expr->Accept(this);
  
  if (!IsInt(print->expr->type)) {
    std::cerr << "Non-int expression passed to 'System.out.println" << std::endl;
  }
}

void Visitor::TypeChecker::Visit(Stmt::Ret *ret) {
  ret->expr->Accept(this);
  
  if (!SameType(ret->expr->type, current_method->out)) {
    std::cerr << "Couldn't cast expression to return type " << Repr(current_method->out) << std::endl;
  }
}

void Visitor::TypeChecker::Visit(Stmt::List *list) {
  for (auto* stmt : list->list) {
    stmt->Accept(this);
  }
}

void Visitor::TypeChecker::Visit(Stmt::VarDecl *var_decl) {
  AddVariable(var_decl->var_id->symbol, var_decl->var_id->type);
}

void Visitor::TypeChecker::Visit(Stmt::ScopedList *scoped_list) {
  for (auto* stmt : scoped_list->list->list) {
    stmt->Accept(this);
  }
}

void Visitor::TypeChecker::AddVariable(Symbol *symbol, Type *type) {
  symbol_types.insert({*symbol, type});
}

Type *Visitor::TypeChecker::GetVariableType(Symbol *symbol) {
  if (symbol_types.find(*symbol) == symbol_types.end()) {
    std::cerr << "[TypeChecker] Symbol not found" << std::endl;
  }
  
  return symbol_types.at(*symbol);
}

void Visitor::TypeChecker::Visit(Expr::New *new_expr) {
  UserType* type = new UserType(new_expr->class_name);
  type->instance_of = symbol_table_->FindClass(new_expr->class_name);
  
  new_expr->type = type;
  
}

void Visitor::TypeChecker::Visit(Expr::Call *call) {
  call->expr->Accept(this);
  
  if (IsPrimitive(call->expr->type)) {
    std::cerr << "Call on primitive type, no such method" << std::endl;
    exit(1);
  }
  
  UserType* user_type = static_cast<UserType*>(call->expr->type);
  ClassMethod* method = symbol_table_->FindMethod(user_type->instance_of, call->method_name);
  
  // TODO ::::: NOW CALL !!!!!
 }

void Visitor::TypeChecker::Visit(Stmt::ExprStmt *stmt_expr) {
  stmt_expr->expr->Accept(this);
}