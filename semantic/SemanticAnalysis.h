#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <cstdio>
#include <string>
#include "../lexical/Lexeme.h"
#include "../lexical/SymbolTable.h"

class SemanticAnalysis{
public:
	SemanticAnalysis(SymbolTable* st);
	virtual ~SemanticAnalysis();
	bool equalTypes(Lexeme l1, Lexeme l2);
	//bool tknDeclared(std::string tkn, bool declare=false);
	//void appendValueST(std::string var, std::string value);
	bool checkLexeme(Lexeme lex, TokenType dtype);
	bool checkDataType(TokenType tt1, TokenType tt2);
	TokenType getTokenType(std::string token);
private:
	SymbolTable* symbolTable;
	std::map<std::pair<std::string, enum TokenType>, std::pair<std::string, enum TokenType>> m_errort;
};
#endif