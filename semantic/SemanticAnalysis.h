#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <cstdio>
#include <string>
#include "../lexical/Lexeme.h"
#include "../lexical/SymbolTable.h"

class SemanticAnalysis{
public:
  SemanticAnalysis(SymbolTable st);
  virtual ~SemanticAnalysis();
private:
  bool equalTypes(Lexeme l1, Lexeme l2);
  bool varDeclared(std::string var);
  void appendValueST(std::string var, std::string value);
  std:: string getIdValueST(std::string var);
  void SemanticAnalysis:: appendTypeST(std::string var, int type);
  int getIdTypeST(std::string var);
};
#endif