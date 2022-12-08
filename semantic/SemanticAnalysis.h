#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <cstdio>
#include <string>
#include "../lexical/Lexeme.h"

class SemanticAnalysis{
public:
  SemanticAnalysis();
  virtual ~SemanticAnalysis();
private:
  bool equalTypes(Lexeme l1, Lexeme l2);
  bool varDeclared(std::string var);
};
#endif