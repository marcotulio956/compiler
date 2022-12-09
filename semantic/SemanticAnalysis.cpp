#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SemanticAnalysis.h"

SemanticAnalysis::SemanticAnalysis(SymbolTable* st) : symbolTable(st){
}

SemanticAnalysis::~SemanticAnalysis(){}

bool SemanticAnalysis::equalTypes(Lexeme l1, Lexeme l2){
	if(l1.type == l2.type){
		return true;
	}
	return false;
}

bool SemanticAnalysis::checkLexeme(Lexeme lex, TokenType dtype){
	if(lex.type != TokenType::TKN_ID){
		return false;
		//printf("smntc error : Lexeme Token not an identifier [%s]\n", lex.token.c_str());
	}
	if(symbolTable->contains(lex.token) && symbolTable->findDataType(lex.token) == TKN_UNASSINGD){
		printf("smntc error: Lexeme Token dType already declared [%s]\n", lex.token.c_str());
		symbolTable->print();
		exit(1);
	}else if(symbolTable->findDataType(lex.token) != TKN_UNASSINGD){
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
	if(tt1!=tt2)
		symbolTable->print();
	return tt1==tt2;
}

TokenType SemanticAnalysis::getTokenType(std::string token){
	return symbolTable->findDataType(token);
}