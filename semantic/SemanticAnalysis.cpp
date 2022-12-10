#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SemanticAnalysis.h"

SymbolTable symboltable;

SemanticAnalysis::SemanticAnalysis(SymbolTable* st) : symbolTable(st){
}


SemanticAnalysis::~SemanticAnalysis(){}

bool SemanticAnalysis::checkLexeme(Lexeme lex, TokenType dtype){
	printf("smntc: checkLexeme [%s](%s) to type (%s)\n", 
		lex.token.c_str(), 
		tt2str(TokenType(symbolTable->findDataType(lex.token))).c_str(),
		tt2str(TokenType(dtype)).c_str()
	);

	if(lex.type != TokenType::TKN_ID){
		return false;
		printf("smntc error: Lexeme Token not an identifier [%s]\n", lex.token.c_str());
		exit(1);
	}
	if(!symbolTable->contains(lex.token)){
		printf("smntc error: Lexeme Token Identifier was not declared [%s]\n", lex.token.c_str());
		exit(1);
	}
	if(symbolTable->contains(lex.token) && symbolTable->findDataType(lex.token) != TKN_NONE){
		printf("smntc error: Lexeme Token dType already declared [%s]\n", lex.token.c_str());
		symbolTable->print();
		exit(1);
	}else if(symbolTable->findDataType(lex.token) == TKN_NONE){
		symbolTable->updateDType(lex.token, dtype);
	}else{
		if(symbolTable->addEntry(lex.token, lex.type, dtype))
			printf("smntc: new Lexeme Token [%s] type (%s, %s)\n", lex.token.c_str(), tt2str(TokenType(lex.type)).c_str(), tt2str(TokenType(dtype)).c_str());
		else{
			throw std::string("Symbol Table Entry Not Created");
		}
	}
	return true;
}

bool SemanticAnalysis::checkDataType(TokenType tt1, TokenType tt2){
	switch(tt1){
		case TKN_TYPE_STRING:
		case TKN_STRING:
			if(tt2==TKN_TYPE_STRING || tt2==TKN_STRING)
				return true;
		case TKN_TYPE_FLOAT:
		case TKN_FLOAT:
			if(tt2==TKN_TYPE_FLOAT || tt2==TKN_FLOAT)
				return true;
		case TKN_TYPE_INT:
		case TKN_INT:
			if(tt2==TKN_TYPE_INT || tt2==TKN_INT)
				return true;
		default:
			//symbolTable->print();
			return false;
	}
}

TokenType SemanticAnalysis::getTokenType(std::string token){
	return symbolTable->findDataType(token);
}