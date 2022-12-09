#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <string>

#include "TokenType.h"

class SymbolTable {
	public:
		SymbolTable();
		virtual ~SymbolTable();

		bool addEntry(std::string token, enum TokenType type, enum TokenType dtype=TKN_NONE);
		TokenType findType(std::string token);
		TokenType findDataType(std::string token);
		bool contains(std::string token);
		void updateDType(std::string token, TokenType dtype);
		void print();
	private:
		std::map<std::string, std::pair<enum TokenType, enum TokenType>> m_st;
};

#endif