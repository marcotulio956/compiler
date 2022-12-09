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
    return true;
  }
  return false;
}

// essa aqui é pra add valor na TS
void SemanticAnalysis:: appendValueST(std::string var, std::string value){
  symboltable.m_st.insert(value, symboltable.find(var));
}

//essa aqui é pra pegar Valor na TS
std:: string getIdValueST(std::string var){
  std::string lex = symboltable.find(var);
  return lex.value;
}
/* 
    node_type extract(const key_type& _Keyval) {
        const const_iterator _Where = find(_Keyval);
        if (_Where == end()) {
            return node_type{};
        }

        return extract(_Where);
    }*/


// essa aqui é pra add tipo na TS
void SemanticAnalysis:: appendTypeST(std::string var, int type){
symboltable.m_st.insert(type, symboltable.find(var));
}

