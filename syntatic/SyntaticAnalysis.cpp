#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SyntaticAnalysis.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex):
    m_lex(lex), m_current(lex.nextToken()), m_derivation(TreeNode<std::string>("<procProgram>")) {
       
}

SyntaticAnalysis::~SyntaticAnalysis(){delete &m_lex;}

void SyntaticAnalysis::start(){
    procProgram(&m_derivation);
    m_derivation.pretty_print();
}

void SyntaticAnalysis::advance(){
    m_current = m_lex.nextToken();
    printf("advance -> %s\n",tt2str(TokenType(m_current.type)).c_str());
}

void SyntaticAnalysis::showError(TokenType expected_type=TKN_NONE){ 
    printf("showError\n");
    printf("%02d: ", m_lex.m_line);
    switch(m_current.type){
        case TKN_INVALID_TOKEN: { 
            printf("Invalid Lexeme [%s]\n", m_current.token.c_str());
            break;
        }
        case TKN_UNEXPECTED_EOF:{}
        case TKN_END_OF_FILE:{
            printf("Unexpected EOF\n");
            break;
        }
        default:{
            printf("Unexpected Lexeme [%s]\n", m_current.token.c_str());
            break;
        }
    }
    printf("exec: DERIVATION TREE incomplete\n"); 
    m_derivation.pretty_print(); 
    if(expected_type!=TKN_NONE)
        printf("Expected Lexeme [%s]\n", tt2str(TokenType(expected_type)).c_str());
    if(!m_lex.showPrints) 
        exit(1);
    else{ // DEBug mode
        advance();
    }
}

void SyntaticAnalysis::eat(enum TokenType type){
    m_derivation.pretty_print();
    printf("eat +%s\n",tt2str(TokenType(type)).c_str());
    if(type == m_current.type)
        advance();
    else
        showError();
}

bool SyntaticAnalysis::is_identifier(){
    if(m_current.type == TKN_ID)
        return true;
    return false;
}
bool SyntaticAnalysis::is_constant(){
    if(m_current.type == TKN_INT
        ||m_current.type == TKN_FLOAT
        ||m_current.type == TKN_STRING
    )
        return true;
    return false;
}
bool SyntaticAnalysis::is_relop(){
    if(m_current.type == TKN_EQ
        ||m_current.type == TKN_GE
        ||m_current.type == TKN_GT
        ||m_current.type == TKN_LE
        ||m_current.type == TKN_LT
        ||m_current.type == TKN_NE
    )
        return true;
    return false;
}
bool SyntaticAnalysis::is_addop(){
    if(m_current.type == TKN_PLUS
        ||m_current.type == TKN_MINUS
    )
        return true;
    return false;
}
bool SyntaticAnalysis::is_mulop(){
    if(m_current.type == TKN_MUL
        ||m_current.type == TKN_DIV
        ||m_current.type == TKN_AND
    )
        return true;
    return false;
}

// program ::= start [decl-list] stmt-list exit
void SyntaticAnalysis::procProgram(TreeNode<std::string>* parent) {
    printf("procProgram\n");
    parent->addChild(tt2str(TokenType(TKN_START)));
    eat(TKN_START);

    TreeNode<std::string> child_procDeclList = TreeNode<std::string>("<procDeclList>");
    if(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING){
        parent->addChild(&child_procDeclList);
        procDeclList(&child_procDeclList);
    }

    TreeNode<std::string> child_procStmtList("<procStmtList>");
    parent->addChild(&child_procStmtList);
    procStmtList(&child_procStmtList);

    parent->addChild(tt2str(TokenType(TKN_EXIT)));
    eat(TKN_EXIT);

    printf("exec: DERIVATION TREE complete\n"); 
}

// decl-list ::= decl {decl}
void SyntaticAnalysis::procDeclList(TreeNode<std::string>* parent) {
    printf("procDeclList\n");

    TreeNode<std::string> child_procDecl("<procDecl>");
    parent->addChild(&child_procDecl);
    procDecl(&child_procDecl);

    while(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING
    ){
        TreeNode<std::string> new_child_procDecl("<procDecl>");
        parent->addChild(&new_child_procDecl);
        procDecl(&new_child_procDecl);
    }
}

// decl ::= type ident-list ";"
void SyntaticAnalysis::procDecl(TreeNode<std::string>* parent) {
    printf("procDecl\n");

    TreeNode<std::string> child_procType("<procType>");
    parent->addChild(&child_procType);
    procType(&child_procType);

    TreeNode<std::string> child_procIdentList("<procIdentList>");
    parent->addChild(&child_procIdentList);
    procIdentList(&child_procIdentList);

    parent->addChild(tt2str(TokenType(TKN_SEMICOLON))); 
    eat(TKN_SEMICOLON);
}

// ident-list ::= identifier {"," identifier}
void SyntaticAnalysis::procIdentList(TreeNode<std::string>* parent) {
    printf("procIdentList\n");

    TreeNode<std::string> child_procIdentifer("<procIdentifier>");
    parent->addChild(&child_procIdentifer);
    procIdentifier(&child_procIdentifer);

    while(m_current.type == TKN_COMMA){
        parent->addChild(tt2str(TokenType(TKN_COMMA)));
        eat(TKN_COMMA);

        TreeNode<std::string> new_child_procIdentifer("<procIdentifier>");
        parent->addChild(&new_child_procIdentifer);
        procIdentifier(&new_child_procIdentifer);
    }
}

// type ::= int | float | string
void SyntaticAnalysis::procType(TreeNode<std::string>* parent) {
    printf("procType\n");
    if(m_current.type == TKN_TYPE_INT){
        parent->addChild(tt2str(TokenType(TKN_TYPE_INT)));
        eat(TKN_TYPE_INT);

    }
    else if(m_current.type == TKN_TYPE_FLOAT){
        parent->addChild(tt2str(TokenType(TKN_TYPE_FLOAT)));
        eat(TKN_TYPE_FLOAT);

    }
    else if(m_current.type == TKN_TYPE_STRING){
        parent->addChild(tt2str(TokenType(TKN_TYPE_STRING)));
        eat(TKN_TYPE_STRING);
    }
    else { showError();}

}

// stmt-list ::= stmt {stmt}
void SyntaticAnalysis::procStmtList(TreeNode<std::string>* parent) {
    printf("procStmtList\n");
    while(m_current.type == TKN_ID 
        || m_current.type == TKN_IF
        || m_current.type == TKN_DO
        || m_current.type == TKN_SCAN
        || m_current.type == TKN_PRINT
    ){
        TreeNode<std::string> child_procStmt("<procStmt>");
        parent->addChild(&child_procStmt);
        procStmt(&child_procStmt);
    }
}

// stmt ::= assign-stmt ";" | if-stmt | while-stmt
//  | read-stmt ";" | write-stmt ";"
void SyntaticAnalysis::procStmt(TreeNode<std::string>* parent) {
    printf("procStmt\n");
    switch(m_current.type){
        case TKN_ID:{
            TreeNode<std::string> child_procAssignStmt("<procAssignStmt>");
            parent->addChild(&child_procAssignStmt);
            procAssignStmt(&child_procAssignStmt);

            parent->addChild(tt2str(TokenType(TKN_SEMICOLON)));
            eat(TKN_SEMICOLON);
            break;
        }
        case TKN_IF:{
            TreeNode<std::string> child_procIfStmt("<procIfStmt>");
            parent->addChild(&child_procIfStmt);
            procIfStmtA(&child_procIfStmt);
            break;
        }
        case TKN_DO:{
            TreeNode<std::string> child_procWhileStmt("<procWhileStmt>");
            parent->addChild(&child_procWhileStmt);
            procWhileStmt(&child_procWhileStmt);
            break;
        }
        case TKN_SCAN:{
            TreeNode<std::string> child_procReadStmt("<procReadStmt>");
            parent->addChild(&child_procReadStmt);
            procReadStmt(&child_procReadStmt);
            
            parent->addChild(tt2str(TokenType(TKN_SEMICOLON)));
            eat(TKN_SEMICOLON);

            break;
        }
        case TKN_PRINT:{
            TreeNode<std::string> child_procWriteStmt("<procWriteStmt>");
            parent->addChild(&child_procWriteStmt);
            procWriteStmt(&child_procWriteStmt);

            parent->addChild(tt2str(TokenType(TKN_SEMICOLON)));
            eat(TKN_SEMICOLON);
            break;
        }
        default:
            showError();
    }
}

// assign-stmt ::= identifier "=" simple_expr
void SyntaticAnalysis::procAssignStmt(TreeNode<std::string>* parent) {
    printf("procAssignStmt\n");

    TreeNode<std::string> child_procIdentifier("<procIdentifier>");
    parent->addChild(&child_procIdentifier);
    procIdentifier(&child_procIdentifier);

    parent->addChild(tt2str(TokenType(TKN_ASSIGN)));
    eat(TKN_ASSIGN);

    TreeNode<std::string> child_procSimpleExpr("<procSimpleExprA>");
    parent->addChild(&child_procSimpleExpr);
    procSimpleExprA(&child_procSimpleExpr);
}

// if-stmtA ::= if condition then stmt-list if-stmtB
void SyntaticAnalysis::procIfStmtA(TreeNode<std::string>* parent) {
    printf("procIfStmtA\n");
    parent->addChild(tt2str(TokenType(TKN_IF)));
    eat(TKN_IF);

    TreeNode<std::string> child_procCondition("<procCondition>");
    parent->addChild(&child_procCondition);
    procCondition(&child_procCondition);

    parent->addChild(tt2str(TokenType(TKN_THEN)));
    eat(TKN_THEN);

    TreeNode<std::string> child_procStmtList("<procStmtList>");
    parent->addChild(&child_procStmtList);
    procStmtList(&child_procStmtList);
    
    TreeNode<std::string> child_procIfStmtB("<procIfStmtB>");
    parent->addChild(&child_procIfStmtB);
    procIfStmtB(&child_procIfStmtB);
}
// if-stmtB ::= end | else stmt-list end
void SyntaticAnalysis::procIfStmtB(TreeNode<std::string>* parent) {
    printf("procIfStmtB\n");
    if(m_current.type == TKN_ELSE){
        parent->addChild(tt2str(TokenType(TKN_ELSE)));
        eat(TKN_ELSE);

        TreeNode<std::string> child_procStmtList("<procStmtList>");
        parent->addChild(&child_procStmtList);
        procStmtList(&child_procStmtList);
    }
    parent->addChild(tt2str(TokenType(TKN_END)));
    eat(TKN_END);

}

// condition ::= expression
void SyntaticAnalysis::procCondition(TreeNode<std::string>* parent) {
    printf("procCondition\n");

    TreeNode<std::string> child_procExpression("<procExpression>");
    parent->addChild(&child_procExpression);
    procExpressionA(&child_procExpression);
}

// while-stmt ::= do stmt-list stmt-sufix
void SyntaticAnalysis::procWhileStmt(TreeNode<std::string>* parent) {
    printf("procWhileStmt\n");

    parent->addChild(tt2str(TokenType(TKN_DO)));
    eat(TKN_DO);

    TreeNode<std::string> child_procStmtList("<procStmtList>");
    parent->addChild(&child_procStmtList);
    procStmtList(&child_procStmtList);
    
    TreeNode<std::string> child_procStmtSufix("<procStmtSufix>");
    parent->addChild(&child_procStmtSufix);
    procStmtSufix(&child_procStmtSufix);
}

// stmt-sufix ::= while condition end
void SyntaticAnalysis::procStmtSufix(TreeNode<std::string>* parent) {
    printf("procStmtSufix\n");

    parent->addChild(tt2str(TokenType(TKN_WHILE)));
    eat(TKN_WHILE);

    TreeNode<std::string> child_procCondition("<procCondition>");
    parent->addChild(&child_procCondition);
    procCondition(&child_procCondition);

    parent->addChild(tt2str(TokenType(TKN_END)));
    eat(TKN_END);

}

// read-stmt ::= scan "(" identifier ")"
void SyntaticAnalysis::procReadStmt(TreeNode<std::string>* parent) {
    printf("procReadStmt\n");
    parent->addChild(tt2str(TokenType(TKN_SCAN)));
    eat(TKN_SCAN);
    
    parent->addChild(tt2str(TokenType(TKN_OPEN_PAR)));
    eat(TKN_OPEN_PAR);

    TreeNode<std::string> child_procIdentifier("<procIdentifier>");
    parent->addChild(&child_procIdentifier);
    procIdentifier(&child_procIdentifier);

    parent->addChild(tt2str(TokenType(TKN_CLOSE_PAR)));
    eat(TKN_CLOSE_PAR);
}

// write-stmt ::= print "(" writable ")"
void SyntaticAnalysis::procWriteStmt(TreeNode<std::string>* parent) {
    printf("procWriteStmt\n");
    parent->addChild(tt2str(TokenType(TKN_PRINT)));
    eat(TKN_PRINT);
    parent->addChild(tt2str(TokenType(TKN_OPEN_PAR)));
    eat(TKN_OPEN_PAR);

    TreeNode<std::string> child_procWritable("<procWritable>");
    parent->addChild(&child_procWritable);
    procWritable(&child_procWritable);

    parent->addChild(tt2str(TokenType(TKN_CLOSE_PAR)));
    eat(TKN_CLOSE_PAR);
}


// writable ::= simple-expr | literal
void SyntaticAnalysis::procWritable(TreeNode<std::string>* parent) {
    printf("procWritable\n");
    if(m_current.type == TKN_OPEN_CUR){
        TreeNode<std::string> child_procLiteral("<procLiteral>");
        parent->addChild(&child_procLiteral);
        procLiteral(&child_procLiteral);

        if( m_current.type == TKN_NOT
            || m_current.type == TKN_MINUS
            || m_current.type == TKN_OPEN_PAR
            || is_relop()
            || is_mulop()
        ){
            TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
            parent->addChild(&child_procSimpleExpr);
            procSimpleExprA(&child_procSimpleExpr);
        }
    }
    else if(
        m_current.type == TKN_NOT
        || m_current.type == TKN_MINUS
        || m_current.type == TKN_OPEN_PAR
        || is_constant()
        || is_identifier()
    ){
        TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
        parent->addChild(&child_procSimpleExpr);
        procSimpleExprA(&child_procSimpleExpr);
    }
    else { showError(); }
}

// expression ::= simple-expr | simple-expr relop simple-expr
// void SyntaticAnalysis::procExpression(TreeNode<std::string>* parent) {
//     printf("procExpression\n");
//     while (
//         m_current.type == TKN_NOT
//         || m_current.type == TKN_MINUS
//         || m_current.type == TKN_OPEN_PAR
//         || is_identifier()
//         || is_constant()
//     ){ TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
//             parent->addChild(&child_procSimpleExpr);
//             procSimpleExpr(&child_procSimpleExpr); }

//     if(is_relop()){
//         TreeNode<std::string> child_procRelOp("<procRelOp>");
//             parent->addChild(&child_procRelOp);
//             procRelOp(&child_procRelOp);
            
//         TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
//             parent->addChild(&child_procSimpleExpr);
//             procSimpleExpr(&child_procSimpleExpr);
//     }
// }

// expressionA ::= simple-expr expressionB
void SyntaticAnalysis::procExpressionA(TreeNode<std::string>* parent) {
    printf("procExpressionA\n");

    TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
    parent->addChild(&child_procSimpleExpr);
    procSimpleExprA(&child_procSimpleExpr);

    TreeNode<std::string> child_procExpressionB("<procExpressionB>");
    parent->addChild(&child_procExpressionB);
    procExpressionB(&child_procExpressionB);
}

// expressionB ::= relop simple-expr
void SyntaticAnalysis::procExpressionB(TreeNode<std::string>* parent) {
    printf("procExpressionB\n");

    TreeNode<std::string> child_procRelOp("<procRelOp>");
    parent->addChild(&child_procRelOp);
    procRelOp(&child_procRelOp);

    TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
    parent->addChild(&child_procSimpleExpr);
    procSimpleExprA(&child_procSimpleExpr);
}

// // simple-expr ::= term | simple-expr addop term
// void SyntaticAnalysis::procSimpleExpr(TreeNode<std::string>* parent) {
//     printf("procSimpleExpr\n");
//     while(m_current.type == TKN_NOT
//         || m_current.type == TKN_MINUS
//         || m_current.type == TKN_OPEN_PAR
//         || is_identifier()
//         || is_constant()
//     ){
//         TreeNode<std::string> child_procTerm("<procTerm>");
//             parent->addChild(&child_procTerm);
//             procTerm(&child_procTerm); // ESSA PARTE TA ESTRANHO

//         if(m_current.type == TKN_NOT
//             || m_current.type == TKN_MINUS
//             || m_current.type == TKN_OPEN_PAR
//             || is_identifier()
//             || is_constant()){
//             TreeNode<std::string> child_procSimpleExpr("<procSimpleExpr>");
//             parent->addChild(&child_procSimpleExpr);
//             procSimpleExpr(&child_procSimpleExpr);
//         }
//     }

//     if(is_addop()){
//         TreeNode<std::string> child_procAddOp("<procAddOp>");
//             parent->addChild(&child_procAddOp);
//             procAddOp(&child_procAddOp);
//         TreeNode<std::string> child_procTerm("<procTerm>");
//             parent->addChild(&child_procTerm);
//             procTerm(&child_procTerm);
//     }
// }

// simple-exprA ::= term simple-exprB
void SyntaticAnalysis::procSimpleExprA(TreeNode<std::string>* parent) {
    printf("procSimpleExprA\n");

    TreeNode<std::string> child_procTerm("<procTermA>");
    parent->addChild(&child_procTerm);
    procTermA(&child_procTerm);

    TreeNode<std::string> child_procSimpleExprB("<procSimpleExprB>");
    parent->addChild(&child_procSimpleExprB);
    procSimpleExprB(&child_procSimpleExprB);
}

// simple-exprB ::= addop term simple-exprB
void SyntaticAnalysis::procSimpleExprB(TreeNode<std::string>* parent) {
    printf("procSimpleExprB\n");

    TreeNode<std::string> child_procAddOp("<procAddOp>");
    parent->addChild(&child_procAddOp);
    procAddOp(&child_procAddOp);

    TreeNode<std::string> child_procTerm("<procTerm>");
    parent->addChild(&child_procTerm);
    procTermA(&child_procTerm);
    
    TreeNode<std::string> child_procSimpleExprB("<procSimpleExprB>");
    parent->addChild(&child_procSimpleExprB);
    procSimpleExprB(&child_procSimpleExprB);
}

// term ::= factor-a | term mulop factor-a
// void SyntaticAnalysis::procTerm(TreeNode<std::string>* parent) {
//     printf("procTerm\n");
//     while(m_current.type==TKN_OPEN_PAR
//         ||m_current.type==TKN_NOT
//         ||m_current.type == TKN_MINUS
//         ||is_constant()
//         ||is_identifier()
//     ){
//         TreeNode<std::string> child_procFactorA("<procFactorA>");
//             parent->addChild(&child_procFactorA);
//             procFactorA(&child_procFactorA);
//     }
//     if(is_mulop()){
//         TreeNode<std::string> child_procMulOp("<procMulOp>");
//             parent->addChild(&child_procMulOp);
//             procFactorA(&child_procMulOp);

//         TreeNode<std::string> child_procFactorA("<procFactorA>");
//             parent->addChild(&child_procFactorA);
//             procFactorA(&child_procFactorA);
//     }
// }

// termA ::= factor-a termB
void SyntaticAnalysis::procTermA(TreeNode<std::string> *parent){
    TreeNode<std::string> child_procFactorA("<procFactorA>");
    parent->addChild(&child_procFactorA);
    procFactorA(&child_procFactorA);

    TreeNode<std::string> child_procTermB("<procTermB>");
    parent->addChild(&child_procTermB);
    procTermB(&child_procTermB);
}
// termB ::= mulop termB
void SyntaticAnalysis::procTermB(TreeNode<std::string> *parent){
    TreeNode<std::string> child_procMulOp("<procMulOp>");
    parent->addChild(&child_procMulOp);
    procMulOp(&child_procMulOp);

    TreeNode<std::string> child_procTermB("<procTermB>");
    parent->addChild(&child_procTermB);
    procTermB(&child_procTermB);
}

// fator-a ::= factor | "!" factor | "-" factor
void SyntaticAnalysis::procFactorA(TreeNode<std::string>* parent) {
    printf("procFactorA\n");
    if(m_current.type == TKN_OPEN_PAR
        || is_identifier()
        || is_constant()){
        
        TreeNode<std::string> child_procFactor("<procFactor>");
        parent->addChild(&child_procFactor);
        procFactor(&child_procFactor);
    }
    else if(m_current.type == TKN_NOT){
        parent->addChild(tt2str(TokenType(TKN_NOT)));
        eat(TKN_NOT);

        TreeNode<std::string> child_procFactor("<procFactor>");
        parent->addChild(&child_procFactor);
        procFactor(&child_procFactor);
    }
    else if(m_current.type == TKN_MINUS){
        parent->addChild(tt2str(TokenType(TKN_MINUS)));
        eat(TKN_MINUS);

        TreeNode<std::string> child_procFactor("<procFactor>");
        parent->addChild(&child_procFactor);
        procFactor(&child_procFactor);
    }
    else{showError();}
}

// factor ::= identifier | constant | "(" expression ")"
void SyntaticAnalysis::procFactor(TreeNode<std::string>* parent) {
    printf("procFactor\n");
    if(m_current.type==TKN_OPEN_PAR){
        parent->addChild(tt2str(TokenType(TKN_OPEN_PAR)));
        eat(TKN_OPEN_PAR);

        TreeNode<std::string> child_procExpression("<procExpression>");
        parent->addChild(&child_procExpression);
        procExpressionA(&child_procExpression);

        parent->addChild(tt2str(TokenType(TKN_OPEN_PAR)));
        eat(TKN_CLOSE_PAR);
    } else if(is_identifier()) {
        TreeNode<std::string> child_procIdentifier("<procIdentifier>");
            parent->addChild(&child_procIdentifier);
            procIdentifier(&child_procIdentifier);
    } else if(is_constant()) {
        TreeNode<std::string> child_procConstant("<procConstant>");
            parent->addChild(&child_procConstant);
            procConstant(&child_procConstant);
    } else { showError(); }
}

// relop ::= "==" | ">" | ">=" | "<" | "<=" | "<>"
void SyntaticAnalysis::procRelOp(TreeNode<std::string>* parent) {
    printf("procRelOp\n");
    if(m_current.type == TKN_EQ){
        parent->addChild(tt2str(TokenType(TKN_EQ)));
        eat(TKN_EQ);
    } else if (m_current.type == TKN_GT){
        parent->addChild(tt2str(TokenType(TKN_GT)));
        eat(TKN_GT);
    } else if (m_current.type == TKN_GE){
        parent->addChild(tt2str(TokenType(TKN_GE)));
        eat(TKN_GE);
    } else if (m_current.type == TKN_LT){
        parent->addChild(tt2str(TokenType(TKN_LT)));
        eat(TKN_LT);
    } else if (m_current.type == TKN_LE){
        parent->addChild(tt2str(TokenType(TKN_LE)));
        eat(TKN_LE);
    } else if (m_current.type == TKN_NE){
        parent->addChild(tt2str(TokenType(TKN_NE)));
        eat(TKN_NE);
    } else { showError(); }
}

// addop ::= "+" | "-" | "||"
void SyntaticAnalysis::procAddOp(TreeNode<std::string>* parent) {
    printf("procAddOp\n");
    if(m_current.type == TKN_PLUS){
        parent->addChild(tt2str(TokenType(TKN_PLUS)));
        eat(TKN_PLUS);
    }else if (m_current.type == TKN_MINUS){
        parent->addChild(tt2str(TokenType(TKN_MINUS)));
        eat(TKN_MINUS);
    }else if (m_current.type == TKN_OR){
        parent->addChild(tt2str(TokenType(TKN_OR)));
        eat(TKN_OR);
    }else { showError(); }
}

// mulop ::= "*" | "/" | "&&"
void SyntaticAnalysis::procMulOp(TreeNode<std::string>* parent) {
    printf("procMulOp\n");
    if(m_current.type == TKN_MUL){
        parent->addChild(tt2str(TokenType(TKN_MUL)));
        eat(TKN_MUL);
    }else if (m_current.type == TKN_DIV){
        parent->addChild(tt2str(TokenType(TKN_DIV)));
        eat(TKN_DIV);
    }else if (m_current.type == TKN_AND){
        parent->addChild(tt2str(TokenType(TKN_AND)));
        eat(TKN_AND);
    }else { showError(); }
}

// constant ::= integer_const | float_const | literal
void SyntaticAnalysis::procConstant(TreeNode<std::string>* parent) {
    printf("procConstant\n");
    if(m_current.type == TKN_INT){
        TreeNode<std::string> child_procIntegerConst("<procIntegerConst>");
            parent->addChild(&child_procIntegerConst);
            procIntegerConst(&child_procIntegerConst);

    }else if(m_current.type == TKN_FLOAT){
        TreeNode<std::string> child_procFloatConst("<procFloatConst>");
            parent->addChild(&child_procFloatConst);
            procFloatConst(&child_procFloatConst);

    }else if(m_current.type ==TKN_OPEN_CUR){
        TreeNode<std::string> child_procLiteral("<procLiteral>");
            parent->addChild(&child_procLiteral);
            procLiteral(&child_procLiteral);
    }else{showError();}
}

// integer_const ::= digit+
void SyntaticAnalysis::procIntegerConst(TreeNode<std::string>* parent) {
    printf("procIntegerConst\n");
    parent->addChild(tt2str(TokenType(TKN_INT)));
    eat(TKN_INT);
}

// float_const ::= digit+
// “.”digit+
void SyntaticAnalysis::procFloatConst(TreeNode<std::string>* parent) {
    printf("procFloatConst\n");
    parent->addChild(tt2str(TokenType(TKN_FLOAT)));
    eat(TKN_FLOAT);
}

// literal ::= " { " {caractere} " } "
void SyntaticAnalysis::procLiteral(TreeNode<std::string>* parent) {
    printf("procLiteral\n");
    parent->addChild(tt2str(TokenType(TKN_OPEN_CUR)));
    eat(TKN_OPEN_CUR);
    if(m_current.type==TKN_STRING){
        TreeNode<std::string> child_procCharacter("<procCharacter>");
            parent->addChild(&child_procCharacter);
            procCharacter(&child_procCharacter);
    }
    parent->addChild(tt2str(TokenType(TKN_CLOSE_CUR)));
    eat(TKN_CLOSE_CUR);

}

// identifier ::= (letter | _ ) (letter | digit )*
void SyntaticAnalysis::procIdentifier(TreeNode<std::string>* parent) {printf("procIdentifier\n"); parent->addChild(tt2str(TokenType(TKN_ID))); eat(TKN_ID); }

// caractere ::= um dos caracteres ASCII, exceto quebra de linha
void SyntaticAnalysis::procCharacter(TreeNode<std::string>* parent) {printf("procCharacter\n"); parent->addChild(tt2str(TokenType(TKN_STRING))); eat(TKN_STRING);}