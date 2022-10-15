#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include <cstdio>
#include <string>

#include "../lexical/LexicalAnalysis.h"

class SyntaticAnalysis {
public:
    SyntaticAnalysis(LexicalAnalysis& lex);
    virtual ~SyntaticAnalysis();
    void start();
private:
    LexicalAnalysis& m_lex;
    Lexeme m_current;

    void eat(enum TokenType type);
    void advance();
    void showError(enum TokenType expected_type);

    bool is_identifier();
    bool is_constant();
    bool is_relop();
    bool is_addop();
    bool is_mulop();
    
    void  procProgram();
    void  procDeclList();
    void  procDecl();
    void  procIdentList();
    void  procType();
    void  procStmtList();
    void  procStmt();
    void  procAssignStmt();
    void  procIfStmt();
    void  procCondition();
    void  procWhileStmt();
    void  procStmtSufix();
    void  procReadStmt();
    void  procWriteStmt();
    void  procWritable();
    void  procExpression();
    void  procSimpleExpr();
    void  procTerm();
    void  procFactorA();
    void  procFactor();
    void  procRelOp();
    void  procAddOp();
    void  procMulOp();
    void  procConstant();
    void  procIntegerConst();
    void  procFloatConst();
    void  procLiteral();
    void  procIdentifier();
    void  procLetter();
    void  procDigit();
    void  procCharacter();
};

#endif