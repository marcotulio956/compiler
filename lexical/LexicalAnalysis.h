#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <cstdio>
#include <string>

#include "TokenType.h"
#include "SymbolTable.h"
#include "Lexeme.h"

class LexicalAnalysis {
public:
    LexicalAnalysis(const char* filename, bool DEBug, SymbolTable st);
    virtual ~LexicalAnalysis();

    int line() const;

    struct Lexeme nextToken();

    bool showPrints;

    int m_line;
private:
    FILE* m_file;
    SymbolTable symboltable;
};

#endif
