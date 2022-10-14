#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>
#include "SyntaticAnalysis.h"


using namespace std;

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex):
lex(lex), current(lex.nextToken(){
}

SyntaticAnalysis::~SyntaticAnalysis(){

}

void SyntaticAnalysis::start(){

}

void SyntaticAnalysis::advance(){
    current = lex.nextToken();
}


void SyntaticAnalysis::eat(enum TokenType type){
    if(type == current.type) advance();
    else showError();

}
void SyntaticAnalysis:: showError(){ 
    printf("%02d: ", lex.m_line);
    switch(lex){
        case TKN_INVALID_TOKEN: { 
            printf("Lexema inválido [%s]\n", current.token.c_str();
            break;
        }
        case TKN_UNEXPECTED_EOF:{}
        case TKN_END_OF_FILE:{
            printf("Fim de arquivo inesperado\n");
            break;
        }
        default:{
            printf("Lexema não esperado [%s]\n", current.token.c_str());
            break;
        }
    }
    exit(1);
}


// program ::= start [decl-list] stmt-list exit
void SyntaticAnalysis:: procProgram() {
    eat(TokenType.TKN_START);
    if(current.type == TokenType.TKN_TYPE_INT || TokenType.TKN_TYPE_FLOAT || TokenType.TKN_TYPE_STRING){
    advance();
    procDeclList();
    }
    procStmtList();
    eat(TokenType.EXIT);
}
// decl-list ::= decl {decl}
void SyntaticAnalysis:: procDeclList() {
    procDecl();
    if(current.type == TokenType.TKN_TYPE_INT || TokenType.TKN_TYPE_FLOAT || TokenType.TKN_TYPE_STRING){
        advance();
        procDecl();
    }
}
// decl ::= type ident-list ";"
void SyntaticAnalysis:: procDecl() {
    if(current.type == TokenType.TKN_TYPE_INT){
        eat(TokenType.TKN_TYPE_INT);
        procIdentList();
        eat(TokenType.TKN_SEMICOLON);
    }
    else if (current.type == TokenType.TKN_TYPE_FLOAT){
        eat( TokenType.TKN_TYPE_FLOAT);
        procIdentList();
        eat(TokenType.TKN_SEMICOLON);
    }
    else if (current.type == TokenType.TKN_TYPE_STRING){
        eat( TokenType.TKN_TYPE_STRING);
        procIdentList();
        eat(TokenType.TKN_SEMICOLON);
    }
    else{
        showError();
    }
}

// ident-list ::= identifier {"," identifier}
void SyntaticAnalysis:: procIdentList() {
    procIdentifier();
    if(current.type == TokenType.TKN_COMMA){
        advance();
        eat(TokenType.TKN_COMMA);
        procIdentifier();
    }
}

// type ::= int | float | string
void SyntaticAnalysis:: procType() {
    if(current.type == TokenType.TKN_TYPE_INT){
        eat(TokenType.TKN_TYPE_INT);
    }
    else if(current.type == TokenType.TKN_TYPE_FLOAT){
        eat(TokenType.TKN_TYPE_FLOAT);
    }
    else if(current.type == TokenType.TKN_TYPE_STRING){
        eat(TokenType.TKN_TYPE_STRING);
    }
    else { showError();}

}


// stmt-list ::= stmt {stmt}
void SyntaticAnalysis:: procStmtList() {
    procStmt();
    if(){
    procStmt();
    }
}



// stmt ::= assign-stmt ";" | if-stmt | while-stmt
//  | read-stmt ";" | write-stmt ";"
void SyntaticAnalysis:: procStmt() {}
// assign-stmt ::= identifier "=" simple_expr
void SyntaticAnalysis:: procAssignStmt() {
    procIdentifier();
    eat(TokenType.TKN_ASSIGN);
    procSimpleExpr();
}


// if-stmt ::= if condition then stmt-list end
//  | if condition then stmt-list else stmt-list end
// ESSE AQUI TO COM PROBLEMA
void SyntaticAnalysis:: procIfStmt() {
    eat(TokenType.TKN_IF);
    procCondition();
    eat(TokenType.TKN_THEN);
    procStmtList();
    eat(TokenType.TKN_END);
}


// condition ::= expression
void SyntaticAnalysis:: procCondition() {
    procExpression();
}
// while-stmt ::= do stmt-list stmt-sufix
void SyntaticAnalysis:: procWhileStmt() {
    eat(TokenType.TKN_DO);
    procStmtList();
    procStmtSufix();
}
// stmt-sufix ::= while condition end
void SyntaticAnalysis:: procStmtSufix() {
    eat(TokenType.TKN_WHILE);
    procCondition();
    eat(TokenType.TKN_END);
}
// read-stmt ::= scan "(" identifier ")"
void SyntaticAnalysis:: procReadStmt() {
    eat(TokenType.TKN_SCAN);
    eat(TokenType.TKN_OPEN_PAR);
    procIdentifier();
    eat(TokenType.TKN_CLOSE_PAR);
}
// write-stmt ::= print "(" writable ")"
void SyntaticAnalysis:: procWriteStmt() {
    eat(TokenType.TKN_PRINT);
    eat(TokenType.OPEN_PAR);
    procWritable();
    eat(TokenType.TKN_CLOSE_PAR);
}
// writable ::= simple-expr | literal
void SyntaticAnalysis:: procWritable() {
    if(){
        advance();
        procSimpleExpr();
    }
    else if(){
        advance();
        procLiteral();
    }
    else { showError();}
}

// expression ::= simple-expr | simple-expr relop simple-expr
void SyntaticAnalysis:: procExpression() {
    if(){
        advance();
        procSimpleExpr();
    }
    else if(){
        advance();
        procSimpleExpr();
        procRelOp();
        procSimpleExpr();
    }
    else(){
        showError();
    }
}
// simple-expr ::= term | simple-expr addop term
void SyntaticAnalysis:: procSimpleExpr() {
    if(){
        advance();
        procTerm();
    }
    else if(){
        advance();
        procSimpleExpr();
        procAddOp();
        procTerm();
    }
    else{showError();}
}
// term ::= factor-a | term mulop factor-a
void SyntaticAnalysis:: procTerm() {
    if(){
        advance();
        procFactorA();
    }
    else if(){
        advance();
        procTerm();
        procMulOp();
        procFactorA();
    }
    else {showError();}
}
// fator-a ::= factor | "!" factor | "-" factor
void SyntaticAnalysis:: procFactorA() {
    if(){
        advance();
        procFactor();
    }
    else if(current.type == TokenType.TKN_NOT){
        advance();
        eat(TokenType.TKN_NOT);
        procFactor();
    }
    else if(current.type == TokenType.TKN_MINUS){
        advance();
        eat(TokenType.TKN_MINUS);
        procFactor();
    }
    else{ showError();}
}
// factor ::= identifier | constant | "(" expression ")"
void SyntaticAnalysis:: procFactor() {}
// relop ::= "==" | ">" | ">=" | "<" | "<=" | "<>"
void SyntaticAnalysis:: procRelOp() {
    if(current.type == TokenType.EQ){
        eat(TokenType.EQ);
    }
    else if (current.type == TokenType.GT){
        eat(TokenType.GT);
    }
    else if (current.type == TokenType.GE){
        eat(TokenType.GE);
    } else if (current.type == TokenType.LT){
        eat(TokenType.LT);
    } else if (current.type == TokenType.LE){
        eat(TokenType.LE);
    }
    else{
        eat(TokenType.NE);
    }
}
// addop ::= "+" | "-" | "||"
void SyntaticAnalysis:: procAddOp() {
    if(current.type == TokenType.PLUS){
        eat(TokenType.PLUS);
    }
    else if (current.type == TokenType.MINUS){
        eat(TokenType.MINUS);
    }
    else{
        eat(TokenType.TKN_OR);
    }
}
// mulop ::= "*" | "/" | "&&"
void SyntaticAnalysis:: procMulOp() {
    if(current.type == TokenType.MUL){
        eat(TokenType.MUL);
    }
    else if (current.type == TokenType.DIV){
        eat(TokenType.DIV);
    }
    else{
        eat(TokenType.TKN_AND);
    }
}
// constant ::= integer_const | float_const | literal
void SyntaticAnalysis:: procConstant() {
    if(){
        advance();
        procIntegerConst();
    }
    else if(){
        advance();
        procFloatConst();
    }
    else if(){
        advance();
        procLiteral();
    }
    else{showError();}
}
// integer_const ::= digit+
void SyntaticAnalysis:: procIntegerConst() {
    procDigit();    
}
// float_const ::= digit+
// “.”digit+
void SyntaticAnalysis:: procFloatConst() {
    procDigit();
    eat(TokenType.TKN_DOT);
    procDigit();
}
// literal ::= " { " {caractere} " } "
void SyntaticAnalysis:: procLiteral() {
    eat(TokenType.OPEN_CUR);
    while(current.type == ){
        procCaractere();
    }
    eat(TokenType.CLOSE_CUR);

}
// identifier ::= (letter | _ ) (letter | digit )*
void SyntaticAnalysis:: procIdentifier() {}
// letter ::= [A-za-z]
void SyntaticAnalysis:: procLetter() {}
// digit ::= [0-9]
void SyntaticAnalysis:: procDigit() {}
// caractere ::= um dos caracteres ASCII, exceto quebra de linha
void SyntaticAnalysis:: procCaractere() {}

