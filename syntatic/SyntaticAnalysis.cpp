#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SyntaticAnalysis.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex):
    m_lex(lex), m_current(lex.nextToken()) {
}

SyntaticAnalysis::~SyntaticAnalysis(){}

void SyntaticAnalysis::start(){
    printf("start\n");
    procProgram();
}

void SyntaticAnalysis::advance(){
    m_current = m_lex.nextToken();
    printf("advance ->%s\n",tt2str(TokenType(m_current.type)).c_str());
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
    if(expected_type!=TKN_NONE)
        printf("Expected Lexeme [%s]\n", tt2str(TokenType(expected_type)).c_str());
    if(!m_lex.showPrints) 
        exit(1);
    else{ // DEBug mode
        advance();
    }
}

void SyntaticAnalysis::eat(enum TokenType type){
    printf("eat +%s\n",tt2str(TokenType(type)).c_str());
    if(type == m_current.type) advance();
    else showError();
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
void SyntaticAnalysis::procProgram() {
    printf("procProgram\n");
    eat(TKN_START);
    if(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING){
        procDeclList();
    }
    procStmtList();
    eat(TKN_EXIT);
}

// decl-list ::= decl {decl}
void SyntaticAnalysis::procDeclList() {
    printf("procDeclList\n");
    procDecl();
    while(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING
    ){
        procDecl();
    }
}

// decl ::= type ident-list ";"
void SyntaticAnalysis::procDecl() {
    printf("procDecl\n");
    procType();
    procIdentList();
    eat(TKN_SEMICOLON);
}

// ident-list ::= identifier {"," identifier}
void SyntaticAnalysis::procIdentList() {
    printf("procIdentList\n");
    eat(TKN_ID);
    while(m_current.type == TKN_COMMA){
        eat(TKN_COMMA);
        eat(TKN_ID);
    }
}

// type ::= int | float | string
void SyntaticAnalysis::procType() {
    printf("procType\n");
    if(m_current.type == TKN_TYPE_INT){
        eat(TKN_TYPE_INT);
    }
    else if(m_current.type == TKN_TYPE_FLOAT){
        eat(TKN_TYPE_FLOAT);
    }
    else if(m_current.type == TKN_TYPE_STRING){
        eat(TKN_TYPE_STRING);
    }
    else { showError();}

}

// stmt-list ::= stmt {stmt}
void SyntaticAnalysis::procStmtList() {
    printf("procStmtList\n");
    while(m_current.type == TKN_ID 
        || m_current.type == TKN_IF
        || m_current.type == TKN_DO
        || m_current.type == TKN_SCAN
        || m_current.type == TKN_PRINT
    )
        procStmt();
}

// stmt ::= assign-stmt ";" | if-stmt | while-stmt
//  | read-stmt ";" | write-stmt ";"
void SyntaticAnalysis::procStmt() {
    printf("procStmt\n");
    switch(m_current.type){
        case TKN_ID:{
            procAssignStmt();
            eat(TKN_SEMICOLON);
            break;
        }
        case TKN_IF:{
            procIfStmt();
            break;
        }
        case TKN_DO:{
            procWhileStmt();
            break;
        }
        case TKN_SCAN:{
            procReadStmt();
            eat(TKN_SEMICOLON);
            break;
        }
        case TKN_PRINT:{
            procWriteStmt();
            eat(TKN_SEMICOLON);
            break;
        }
        default:
            showError();
    }
}

// assign-stmt ::= identifier "=" simple_expr
void SyntaticAnalysis::procAssignStmt() {
    printf("procAssignStmt\n");
    procIdentifier();
    eat(TKN_ASSIGN);
    procSimpleExpr();
}

// if-stmt ::= if condition then stmt-list end
//  | if condition then stmt-list else stmt-list end
void SyntaticAnalysis::procIfStmt() {
    printf("procIfStmt\n");
    eat(TKN_IF);
    procCondition();
    eat(TKN_THEN);
    procStmtList();
    eat(TKN_END);
}

// condition ::= expression
void SyntaticAnalysis::procCondition() {
    printf("procCondition\n");
    procExpression();
}

// while-stmt ::= do stmt-list stmt-sufix
void SyntaticAnalysis::procWhileStmt() {
    printf("procWhileStmt\n");
    eat(TKN_DO);
    procStmtList();
    procStmtSufix();
}

// stmt-sufix ::= while condition end
void SyntaticAnalysis::procStmtSufix() {
    printf("procStmtSufix\n");
    eat(TKN_WHILE);
    procCondition();
    eat(TKN_END);
}

// read-stmt ::= scan "(" identifier ")"
void SyntaticAnalysis::procReadStmt() {
    printf("procReadStmt\n");
    eat(TKN_SCAN);
    eat(TKN_OPEN_PAR);
    procIdentifier();
    eat(TKN_CLOSE_PAR);
}

// write-stmt ::= print "(" writable ")"
void SyntaticAnalysis::procWriteStmt() {
    printf("procWriteStmt\n");
    eat(TKN_PRINT);
    eat(TKN_OPEN_PAR);
    procWritable();
    eat(TKN_CLOSE_PAR);
}


// writable ::= simple-expr | literal
void SyntaticAnalysis::procWritable() {
    printf("procWritable\n");
    if(m_current.type == TKN_OPEN_CUR){
        procLiteral();
        if( m_current.type == TKN_NOT
            || m_current.type == TKN_MINUS
            || m_current.type == TKN_OPEN_PAR
            || is_relop()
            || is_mulop()
        ){
            procSimpleExpr();
        }
    }
    else if(
        m_current.type == TKN_NOT
        || m_current.type == TKN_MINUS
        || m_current.type == TKN_OPEN_PAR
        || is_constant()
        || is_identifier()
    ){
        procSimpleExpr();
    }
    else { showError(); }
}

// expression ::= simple-expr | simple-expr relop simple-expr
void SyntaticAnalysis::procExpression() {
    printf("procExpression\n");
    while (
        m_current.type == TKN_NOT
        || m_current.type == TKN_MINUS
        || m_current.type == TKN_OPEN_PAR
        || is_identifier()
        || is_constant()
    ){ procSimpleExpr(); }

    if(is_relop()){
        procRelOp();
        procSimpleExpr();
    }
}

// ATE AQUI TA SAFE

// simple-expr ::= term | simple-expr addop term
void SyntaticAnalysis::procSimpleExpr() {
    printf("procSimpleExpr\n");
    while(m_current.type == TKN_NOT
        || m_current.type == TKN_MINUS
        || m_current.type == TKN_OPEN_PAR
        || is_identifier()
        || is_constant()
    ){
        procTerm(); // ESSA PARTE TA ESTRANHO
        if(m_current.type == TKN_NOT
            || m_current.type == TKN_MINUS
            || m_current.type == TKN_OPEN_PAR
            || is_identifier()
            || is_constant()){
            procSimpleExpr();
        }
    }

    if(is_addop()){
        procAddOp();
        procTerm();
    }
}

// term ::= factor-a | term mulop factor-a
void SyntaticAnalysis::procTerm() {
    printf("procTerm\n");
    while(m_current.type==TKN_OPEN_PAR
        ||m_current.type==TKN_NOT
        ||m_current.type == TKN_MINUS
        ||is_constant()
        ||is_identifier()
    ){
        procFactorA();
    }
    if(is_mulop()){
        procMulOp();
        procFactorA();
    }
}

// fator-a ::= factor | "!" factor | "-" factor
void SyntaticAnalysis::procFactorA() {
    printf("procFactorA\n");
    if(m_current.type == TKN_OPEN_PAR
        || is_identifier()
        || is_constant()){
        procFactor();
    }
    else if(m_current.type == TKN_NOT){
        eat(TKN_NOT);
        procFactor();
    }
    else if(m_current.type == TKN_MINUS){
        eat(TKN_MINUS);
        procFactor();
    }
    else{showError();}
}

// factor ::= identifier | constant | "(" expression ")"
void SyntaticAnalysis::procFactor() {
    printf("procFactor\n");
    if(m_current.type==TKN_OPEN_PAR){
        eat(TKN_OPEN_PAR);
        procExpression();
        eat(TKN_CLOSE_PAR);
    } else if(is_identifier()) {
        procIdentifier();
    } else if(is_constant()) {
        procConstant();
    } else { showError(); }
}

// relop ::= "==" | ">" | ">=" | "<" | "<=" | "<>"
void SyntaticAnalysis::procRelOp() {
    printf("procRelOp\n");
    if(m_current.type == TKN_EQ){
        eat(TKN_EQ);
    } else if (m_current.type == TKN_GT){
        eat(TKN_GT);
    } else if (m_current.type == TKN_GE){
        eat(TKN_GE);
    } else if (m_current.type == TKN_LT){
        eat(TKN_LT);
    } else if (m_current.type == TKN_LE){
        eat(TKN_LE);
    } else if (m_current.type == TKN_NE){
        eat(TKN_NE);
    } else { showError(); }
}
// addop ::= "+" | "-" | "||"
void SyntaticAnalysis::procAddOp() {
    printf("procAddOp\n");
    if(m_current.type == TKN_PLUS){
        eat(TKN_PLUS);
    }else if (m_current.type == TKN_MINUS){
        eat(TKN_MINUS);
    }else if (m_current.type == TKN_OR){
        eat(TKN_OR);
    }else { showError(); }
}

// mulop ::= "*" | "/" | "&&"
void SyntaticAnalysis::procMulOp() {
    printf("procMulOp\n");
    if(m_current.type == TKN_MUL){
        eat(TKN_MUL);
    }else if (m_current.type == TKN_DIV){
        eat(TKN_DIV);
    }else if (m_current.type == TKN_AND){
        eat(TKN_AND);
    }else { showError(); }
}

// constant ::= integer_const | float_const | literal
void SyntaticAnalysis::procConstant() {
    printf("procConstant\n");
    if(m_current.type == TKN_INT){
        procIntegerConst();
    }else if(m_current.type == TKN_INT){
        procFloatConst();
    }else if(m_current.type ==TKN_OPEN_CUR){
        procLiteral();
    }else{showError();}
}

// integer_const ::= digit+
void SyntaticAnalysis::procIntegerConst() {
    printf("procIntegerConst\n");
    eat(TKN_INT);
}

// float_const ::= digit+
// “.”digit+
void SyntaticAnalysis::procFloatConst() {
    printf("procFloatConst\n");
    eat(TKN_FLOAT);
}

// literal ::= " { " {caractere} " } "
void SyntaticAnalysis::procLiteral() {
    printf("procLiteral\n");
    eat(TKN_OPEN_CUR);
    if(m_current.type==TKN_STRING)
        procCharacter();
    eat(TKN_CLOSE_CUR);

}

// identifier ::= (letter | _ ) (letter | digit )*
void SyntaticAnalysis::procIdentifier() {printf("procIdentifier\n"); eat(TKN_ID);}

// caractere ::= um dos caracteres ASCII, exceto quebra de linha
void SyntaticAnalysis::procCharacter() {printf("procCharacter\n"); eat(TKN_STRING);}