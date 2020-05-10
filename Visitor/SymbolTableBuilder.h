//
// Created by Igor Maximov on 04.05.2020.
//

#pragma once

#include "Visitor.h"
#include "ast_decl.h"
#include "SymbolTable/Scope.h"
#include "SymbolTable/ScopeTree.h"
#include "SymbolTable/Table.h"

#include <unordered_map>
#include <stack>

namespace Visitor {

class SymbolTableBuilder : public Visitor::Base {
 public:
  SymbolTableBuilder() = default;
  
  void Process(Program* program);
  
   void Visit(Program* program) override;
  
   void Visit(Class* class_decl) override;
   void Visit(ClassMethod* method) override;
   void Visit(ClassField* field) override;
   void Visit(ProgramBody* body) override;
  
   void Visit(Expr::BinaryOp* binary_op) override;
   void Visit(Expr::Const* const_expr) override;
   void Visit(Expr::Id* rvalue) override;
   void Visit(Expr::This* this_expr) override;
   void Visit(Expr::UnaryOp* unary_op) override;
  
   void Visit(Stmt::Assign* assn) override;
   void Visit(Stmt::Cond* cond) override;
   void Visit(Stmt::Print* print) override;
   void Visit(Stmt::Ret* ret) override;
   void Visit(Stmt::List* list) override;
   void Visit(Stmt::VarDecl* var_decl) override;
   void Visit(Stmt::ScopedList* scoped_list) override;
 
 private:
  void FindDefinition(Expr::Id* id);
  
 private:
  std::vector<ScopeTree*> trees_;
  ScopeTree* current_tree_{nullptr};
  std::stack<Scope*> scopes_;
};

} // namespace Visitor