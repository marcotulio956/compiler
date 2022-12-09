#include <iostream>

#include "SymbolTable.h"


SymbolTable::SymbolTable() {
	// SYMBOLS
	this->addEntry(";", TKN_SEMICOLON);
	this->addEntry(";", TKN_SEMICOLON);
	this->addEntry("(", TKN_OPEN_PAR);
	this->addEntry(")", TKN_CLOSE_PAR);
	this->addEntry("{", TKN_OPEN_CUR);
	this->addEntry("}", TKN_CLOSE_CUR);
	this->addEntry(".", TKN_DOT);
	this->addEntry(",", TKN_COMMA);

	// OPERATORS
	this->addEntry("=", TKN_ASSIGN);
	this->addEntry("==", TKN_EQ);
	this->addEntry("<>", TKN_NE);
	this->addEntry("!", TKN_NOT);
	this->addEntry("+", TKN_PLUS);
	this->addEntry("-", TKN_MINUS);
	this->addEntry("*", TKN_MUL);
	this->addEntry("/", TKN_DIV);
	this->addEntry("&&", TKN_AND);
	this->addEntry("||", TKN_OR);
	this->addEntry(">", TKN_GT);
	this->addEntry("<", TKN_LT);
	this->addEntry(">=", TKN_GE);
	this->addEntry("<=", TKN_LE);

	// KEYWORDS
	this->addEntry("start", TKN_START);
	this->addEntry("exit", TKN_EXIT);
	this->addEntry("if", TKN_IF);
	this->addEntry("then", TKN_THEN);
	this->addEntry("else", TKN_ELSE);
	this->addEntry("end", TKN_END);
	this->addEntry("do", TKN_DO);
	this->addEntry("while", TKN_WHILE);
	this->addEntry("scan", TKN_SCAN);
	this->addEntry("print", TKN_PRINT);

	// DATATYPES
	this->addEntry("int", TKN_TYPE_INT);
	this->addEntry("float", TKN_TYPE_FLOAT);
	this->addEntry("string", TKN_TYPE_STRING);
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(std::string token) {
	return m_st.find(token) != m_st.end();
}

bool SymbolTable::addEntry(std::string token, enum TokenType type, enum TokenType dtype){
	printf("symboltable: new Lexeme Token [%s] type (%s, %s)\n", token.c_str(), tt2str(TokenType(type)).c_str(), tt2str(TokenType(dtype)).c_str());
	if(!this->contains(token)){
		printf("\t - init as TKN_NONE\n");

		m_st[token] = std::pair<TokenType, TokenType>(type, TKN_NONE);
		return true;
	}else if(this->findDataType(token)!=TKN_UNASSINGD){
		printf("\t - added as %s\n", tt2str(TokenType(dtype)).c_str());

		m_st[token] = std::pair<TokenType, TokenType>(type, dtype);
		return true;
	}
	printf("\t - returned\n");
	
	return false;
}

enum TokenType SymbolTable::findType(std::string token) {
	return this->contains(token) ? m_st[token].first : TKN_ID;
}

enum TokenType SymbolTable::findDataType(std::string token) {
	return this->contains(token) ? m_st[token].second : TKN_UNASSINGD;
}


void SymbolTable::print(){
	printf("symbolTable Identifiers\n");
	int varc = 1;
	for(
		std::map<std::string, std::pair<TokenType,TokenType>>::const_iterator it = m_st.begin();
		it != m_st.end();
		++it
	){
		if(it->second.first == TokenType::TKN_ID){
			//std::cout << it->first << it->second.first << it->second.second;
			std::cout << "." << varc << " " << it->first << " -:- (" << tt2str(TokenType(it->second.first)) <<", "<< tt2str(TokenType(it->second.second)) << ")\n";
			varc++;
		}
	}
}

void SymbolTable::updateDType(std::string token, TokenType dtype){
	m_st[token] = std::pair<TokenType, TokenType>(TKN_ID, dtype);
}