#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include <cstdio>
#include <string>

#include "../lexical/LexicalAnalysis.h"
#include "../util/TreeNode.h"
#include "../semantic/SemanticAnalysis.h"

class SyntaticAnalysis {
public:
    SyntaticAnalysis(LexicalAnalysis* lex, SemanticAnalysis* semantic);
    virtual ~SyntaticAnalysis();
    void start();
private:
    LexicalAnalysis* lexicalAnalysis;
    Lexeme m_current;
    TreeNode<std::string> m_derivation;
    SemanticAnalysis* semanticAnalysis;

    void eat(enum TokenType type, std::string tab);
    void advance();
    void showError(enum TokenType expected_type);

    bool is_identifier();
    bool is_constant();
    bool is_relop();
    bool is_addop();
    bool is_mulop();

    void  procProgram(std::string tab);
    void  procDeclList(std::string tab);
    void  procDecl(std::string tab);
    void  procIdentList(std::string tab);
    void  procType(std::string tab);
    void  procStmtList(std::string tab);
    void  procStmt(std::string tab);
    void  procAssignStmt(std::string tab);
    // void  procIfStmt(std::string tab);
    void  procIfStmtA(std::string tab);
    void  procIfStmtB(std::string tab);
    void  procCondition(std::string tab);
    void  procWhileStmt(std::string tab);
    void  procStmtSufix(std::string tab);
    void  procReadStmt(std::string tab);
    void  procWriteStmt(std::string tab);
    void  procWritable(std::string tab);
    // void  procExpression(std::string tab);
    void  procExpressionA(std::string tab);
    void  procExpressionB(std::string tab);
    // void  procSimpleExpr(std::string tab);
    void  procSimpleExprA(std::string tab);
    void  procSimpleExprB(std::string tab);
    // void  procTerm(std::string tab);
    void  procTermA(std::string tab);
    void  procTermB(std::string tab);
    void  procFactorA(std::string tab);
    void  procFactor(std::string tab);
    void  procRelOp(std::string tab);
    void  procAddOp(std::string tab);
    void  procMulOp(std::string tab);
    void  procConstant(std::string tab);
    void  procIntegerConst(std::string tab);
    void  procFloatConst(std::string tab);
    void  procLiteral(std::string tab);
    void  procIdentifier(std::string tab);
    void  procLetter(std::string tab);
    void  procDigit(std::string tab);
    void  procCharacter(std::string tab);
};

#endif