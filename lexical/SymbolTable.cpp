#include "SymbolTable.h"

SymbolTable::SymbolTable() {
    // SYMBOLS
    m_st[";"] = TKN_SEMICOLON;
    m_st["("] = TKN_OPEN_PAR;
    m_st[")"] = TKN_CLOSE_PAR;
    m_st["{"] = TKN_OPEN_CUR;
    m_st["}"] = TKN_CLOSE_CUR;
    m_st["."] = TKN_DOT;
    m_st[","] = TKN_COMMA;

    // OPERATORS
    m_st["="] = TKN_ASSIGN;
    m_st["=="] = TKN_EQ;
    m_st["<>"] = TKN_NE;
    m_st["!"] = TKN_NOT;
    m_st["+"] = TKN_PLUS;
    m_st["-"] = TKN_MINUS;
    m_st["*"] = TKN_MUL;
    m_st["/"] = TKN_DIV;
    m_st["&&"] = TKN_AND;
    m_st["||"] = TKN_OR;
    m_st[">"] = TKN_GT;
    m_st["<"] = TKN_LT;
    m_st[">="] = TKN_GE;
    m_st["<="] = TKN_LE;

    // KEYWORDS
    m_st["start"] = TKN_START;
    m_st["exit"] = TKN_EXIT;
    m_st["if"] = TKN_IF;
    m_st["then"] = TKN_THEN;
    m_st["else"] = TKN_ELSE;
    m_st["end"] = TKN_END;
    m_st["do"] = TKN_DO;
    m_st["while"] = TKN_WHILE;
    m_st["scan"] = TKN_SCAN;
    m_st["print"] = TKN_PRINT;

    // DATATYPES
    m_st["int"] = TKN_TYPE_INT;
    m_st["float"] = TKN_TYPE_FLOAT;
    m_st["string"] = TKN_TYPE_STRING;
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(std::string token) {
    return m_st.find(token) != m_st.end();
}

enum TokenType SymbolTable::find(std::string token) {
    return this->contains(token) ? m_st[token] : TKN_ID;
}