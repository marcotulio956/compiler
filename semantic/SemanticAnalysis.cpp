#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SemanticAnalysis.h"

SymbolTable symboltable;

SemanticAnalysis::SemanticAnalysis(SymbolTable st){
  symboltable = st;
}

SemanticAnalysis::~SemanticAnalysis(){}

bool SemanticAnalysis::equalTypes(Lexeme l1, Lexeme l2){
  if(l1.type == l2.type){
    return true;
  }
  return false;
}

bool SemanticAnalysis::varDeclared(std::string var){
  if(symboltable.contains(var)){
    return false;
  }
  return false;
}
void SemanticAnalysis:: appendValueST(string var, string value){
Lexeme lex = symboltable.get(lex);
lex.value = value;
symboltable.put(lex,value);
}