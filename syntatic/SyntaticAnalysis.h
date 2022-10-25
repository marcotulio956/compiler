#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include <cstdio>
#include <string>

#include "../lexical/LexicalAnalysis.h"
#include "../util/TreeNode.h"

class SyntaticAnalysis {
public:
    SyntaticAnalysis(LexicalAnalysis& lex);
    virtual ~SyntaticAnalysis();
    void start();
private:
    LexicalAnalysis& m_lex;
    Lexeme m_current;
    TreeNode<std::string> m_derivation;

    void eat(enum TokenType type);
    void advance();
    void showError(enum TokenType expected_type);

    bool is_identifier();
    bool is_constant();
    bool is_relop();
    bool is_addop();
    bool is_mulop();

    void  procProgram(TreeNode<std::string> *parent);
    void  procDeclList(TreeNode<std::string> *parent);
    void  procDecl(TreeNode<std::string> *parent);
    void  procIdentList(TreeNode<std::string> *parent);
    void  procType(TreeNode<std::string> *parent);
    void  procStmtList(TreeNode<std::string> *parent);
    void  procStmt(TreeNode<std::string> *parent);
    void  procAssignStmt(TreeNode<std::string> *parent);
    // void  procIfStmt(TreeNode<std::string> *parent);
    void  procIfStmtA(TreeNode<std::string> *parent);
    void  procIfStmtB(TreeNode<std::string> *parent);
    void  procCondition(TreeNode<std::string> *parent);
    void  procWhileStmt(TreeNode<std::string> *parent);
    void  procStmtSufix(TreeNode<std::string> *parent);
    void  procReadStmt(TreeNode<std::string> *parent);
    void  procWriteStmt(TreeNode<std::string> *parent);
    void  procWritable(TreeNode<std::string> *parent);
    // void  procExpression(TreeNode<std::string> *parent);
    void  procExpressionA(TreeNode<std::string> *parent);
    void  procExpressionB(TreeNode<std::string> *parent);
    // void  procSimpleExpr(TreeNode<std::string> *parent);
    void  procSimpleExprA(TreeNode<std::string> *parent);
    void  procSimpleExprB(TreeNode<std::string> *parent);
    // void  procTerm(TreeNode<std::string> *parent);
    void  procTermA(TreeNode<std::string> *parent);
    void  procTermB(TreeNode<std::string> *parent);
    void  procFactorA(TreeNode<std::string> *parent);
    void  procFactor(TreeNode<std::string> *parent);
    void  procRelOp(TreeNode<std::string> *parent);
    void  procAddOp(TreeNode<std::string> *parent);
    void  procMulOp(TreeNode<std::string> *parent);
    void  procConstant(TreeNode<std::string> *parent);
    void  procIntegerConst(TreeNode<std::string> *parent);
    void  procFloatConst(TreeNode<std::string> *parent);
    void  procLiteral(TreeNode<std::string> *parent);
    void  procIdentifier(TreeNode<std::string> *parent);
    void  procLetter(TreeNode<std::string> *parent);
    void  procDigit(TreeNode<std::string> *parent);
    void  procCharacter(TreeNode<std::string> *parent);
};

#endif