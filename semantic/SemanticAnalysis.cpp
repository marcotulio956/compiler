#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SemanticAnalysis.h"
#include "../lexical/SymbolTable.h"

bool SemanticAnalysis::equalTypes(Lexeme l1, Lexeme l2){
  if(l1.type == l2.type){
    return true;
  }
  return false;
}

bool SemanticAnalysis::varDeclared(String var){
  if(symboltable.contains(var)){
    return false;
  }
  return false;
}