#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <cstdio>
#include <string>

#include "TokenType.h"
#include "SymbolTable.h"
#include "Lexeme.h"

class LexicalAnalysis {
public:
    LexicalAnalysis(const char* filename, bool DEBug);
    virtual ~LexicalAnalysis();

    int line() const;

    struct Lexeme nextToken();

private:
    int m_line;
    FILE* m_file;
    SymbolTable m_st;
    bool showPrints;
};

#endif
