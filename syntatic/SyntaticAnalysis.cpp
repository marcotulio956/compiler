#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SyntaticAnalysis.h"

#define c_tt2str(tt) tt2str(TokenType(tt)).c_str()

#define ttprint(tt) printf("X %s\n", c_tt2str(tt));

#define printct ttprint(m_current.type)

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis* syntatical, SemanticAnalysis* semantic):
    lexicalAnalysis(syntatical), m_current(syntatical->nextToken()), m_derivation(TreeNode<std::string>("<procProgram>")), semanticAnalysis(semantic){
}

SyntaticAnalysis::~SyntaticAnalysis(){}

void SyntaticAnalysis::start(){
    procProgram("");
}

void SyntaticAnalysis::advance(){
    m_current = lexicalAnalysis->nextToken();
    //printf("advance -> %s\n",tt2str(TokenType(m_current.type)).c_str());
}

void SyntaticAnalysis::showError(TokenType expected_type=TKN_NONE){ 
    printf("showError\n");
    printf("%02d: ", lexicalAnalysis->m_line);
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
    if(expected_type!=TKN_NONE)
        printf("Expected Lexeme [%s]\n", tt2str(TokenType(expected_type)).c_str());
    if(!lexicalAnalysis->showPrints) // in case of debug mode, advance anyway(may not halt)
        exit(1);
    else{ // DEBug mode
        //advance();
        exit(1);
    }
}

TokenType SyntaticAnalysis::eat(enum TokenType type, std::string tab){
    printf("%seat +%s | curr =%s",tab.c_str(),tt2str(TokenType(type)).c_str(),tt2str(TokenType(m_current.type)).c_str());
    if(type == TKN_ID || type == TKN_INT|| type == TKN_STRING || type == TKN_FLOAT )
        printf(" : '%s'", m_current.token.c_str());
    else{
        printf(" - '%s'", m_current.token.c_str());
    }
    printf("\n");
    if(type == m_current.type)
        advance();
    else
        showError(type);
    return m_current.type;
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
void SyntaticAnalysis::procProgram(std::string tab) {
    printf("%s<procProgram>\n",tab.c_str());
    eat(TKN_START, tab+"\t");

    if(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING){
            
        procDeclList(tab+"\t");
    }

    procStmtList(tab+"\t");

    eat(TKN_EXIT, tab+"\t");

    printf("exec: DERIVATION TREE complete\n"); 
}

// decl-list ::= decl {decl}
void SyntaticAnalysis::procDeclList(std::string tab) {
    printf("%s<procDeclList>\n",tab.c_str());

    procDecl(tab+"\t");

    while(m_current.type == TKN_TYPE_INT 
        || m_current.type == TKN_TYPE_FLOAT 
        || m_current.type == TKN_TYPE_STRING
    ){
        procDecl(tab+"\t");
    }
}

// decl ::= type ident-list ";"
void SyntaticAnalysis::procDecl(std::string tab) {
    printf("%s<procDecl>\n",tab.c_str());

    enum TokenType dtype = procType(tab+"\t");

    procIdentList(dtype, tab+"\t");

    eat(TKN_SEMICOLON, tab+"\t");
}

// ident-list ::= identifier {"," identifier}
void SyntaticAnalysis::procIdentList(enum TokenType dtype, std::string tab) {
    printf("%s<procIdentList>\n",tab.c_str());

    semanticAnalysis->checkLexeme(m_current, dtype);
    procIdentifier(tab+"\t");

    while(m_current.type == TKN_COMMA){
        eat(TKN_COMMA, tab+"\t");
        
        semanticAnalysis->checkLexeme(m_current, dtype);
        procIdentifier(tab+"\t");
    }
}

// type ::= int | float | string
enum TokenType SyntaticAnalysis::procType(std::string tab) {
    printf("%s<procType>\n",tab.c_str());
    if(m_current.type == TKN_TYPE_INT){
        eat(TKN_TYPE_INT, tab+"\t");
        return TKN_TYPE_INT;
    }
    else if(m_current.type == TKN_TYPE_FLOAT){
        eat(TKN_TYPE_FLOAT, tab+"\t");
        return TKN_TYPE_FLOAT;

    }
    else if(m_current.type == TKN_TYPE_STRING){
        eat(TKN_TYPE_STRING, tab+"\t");
        return TKN_TYPE_STRING;
    }
    else { showError(); return TKN_NONE;}

}

// stmt-list ::= stmt {stmt}
void SyntaticAnalysis::procStmtList(std::string tab) {
    printf("%s<procStmtList>\n",tab.c_str());
    while(m_current.type == TKN_ID 
        || m_current.type == TKN_IF
        || m_current.type == TKN_DO
        || m_current.type == TKN_SCAN
        || m_current.type == TKN_PRINT
    ){
        procStmt(tab+"\t");
    }
}

// stmt ::= assign-stmt ";" | if-stmt | while-stmt
//  | read-stmt ";" | write-stmt ";"
void SyntaticAnalysis::procStmt(std::string tab) {
    printf("%s<procStmt>\n",tab.c_str());
    switch(m_current.type){
        case TKN_ID:{
            procAssignStmt(tab+"\t");

            eat(TKN_SEMICOLON, tab+"\t");
            break;
        }
        case TKN_IF:{
            procIfStmtA(tab+"\t");
            break;
        }
        case TKN_DO:{
            procWhileStmt(tab+"\t");
            break;
        }
        case TKN_SCAN:{
            procReadStmt(tab+"\t");
            
            eat(TKN_SEMICOLON, tab+"\t");

            break;
        }
        case TKN_PRINT:{
            procWriteStmt(tab+"\t");

            eat(TKN_SEMICOLON, tab+"\t");
            break;
        }
        default:
            showError();
    }
}

// assign-stmt ::= identifier "=" simple_expr
void SyntaticAnalysis::procAssignStmt(std::string tab) {
    printf("%s<procAssignStmt>\n",tab.c_str());

    TokenType tt1 = semanticAnalysis->getTokenType(m_current.token);
    procIdentifier(tab+"\t");

    eat(TKN_ASSIGN, tab+"\t");

    TokenType tt2 = procSimpleExprA(tab+"\t");

    if(!semanticAnalysis->checkDataType(tt1, tt2)){
        printf("smntc: dtype error line %d: <procIdentifier>(%s) = <procSimpleExprA>(%s)\n",
            lexicalAnalysis->line(),
            tt2str(TokenType(tt1)).c_str(),
            tt2str(TokenType(tt2)).c_str()
        );
        
        exit(1);
    }
}

// if-stmtA ::= if condition then stmt-list if-stmtB
void SyntaticAnalysis::procIfStmtA(std::string tab) {
    printf("%s<procIfStmtA>\n",tab.c_str());
    eat(TKN_IF, tab+"\t");

    procCondition(tab+"\t");

    eat(TKN_THEN, tab+"\t");

    procStmtList(tab+"\t");
    
    procIfStmtB(tab+"\t");
}
// if-stmtB ::= end | else stmt-list end
void SyntaticAnalysis::procIfStmtB(std::string tab) {
    printf("%s<procIfStmtB>\n",tab.c_str());
    if(m_current.type == TKN_ELSE){
        eat(TKN_ELSE, tab+"\t");

        procStmtList(tab+"\t");
    }
    eat(TKN_END, tab+"\t");

}

// condition ::= expression
void SyntaticAnalysis::procCondition(std::string tab) {
    printf("%s<procCondition>\n",tab.c_str());

    procExpressionA(tab+"\t");
}

// while-stmt ::= do stmt-list stmt-sufix
void SyntaticAnalysis::procWhileStmt(std::string tab) {
    printf("%s<procWhileStmt>\n",tab.c_str());

    eat(TKN_DO, tab+"\t");

    procStmtList(tab+"\t");
    
    procStmtSufix(tab+"\t");
}

// stmt-sufix ::= while condition end
void SyntaticAnalysis::procStmtSufix(std::string tab) {
    printf("%s<procStmtSufix>\n",tab.c_str());

    eat(TKN_WHILE, tab+"\t");

    procCondition(tab+"\t");

    eat(TKN_END, tab+"\t");

}

// read-stmt ::= scan "(" identifier ")"
void SyntaticAnalysis::procReadStmt(std::string tab) {
    printf("%s<procReadStmt>\n",tab.c_str());
    eat(TKN_SCAN, tab+"\t");
    
    eat(TKN_OPEN_PAR, tab+"\t");

    procIdentifier(tab+"\t");

    eat(TKN_CLOSE_PAR, tab+"\t");
}

// write-stmt ::= print "(" writable ")"
void SyntaticAnalysis::procWriteStmt(std::string tab) {
    printf("%s<procWriteStmt>\n",tab.c_str());
    eat(TKN_PRINT, tab+"\t");
    eat(TKN_OPEN_PAR, tab+"\t");

    procWritable(tab+"\t");

    eat(TKN_CLOSE_PAR, tab+"\t");
}


// writable ::= simple-expr | literal
void SyntaticAnalysis::procWritable(std::string tab) {
    printf("%s<procWritable>\n",tab.c_str());
    if(m_current.type == TKN_OPEN_CUR){
        procLiteral(tab+"\t");
    }
    else if(
        m_current.type == TKN_NOT
        || m_current.type == TKN_MINUS
        || m_current.type == TKN_OPEN_PAR
        || is_constant()
        || is_identifier()
    ){
        procSimpleExprA(tab+"\t");
    }
    else { showError(); }
}

// expression ::= simple-expr | simple-expr relop simple-expr
// void SyntaticAnalysis::procExpression(std::string tab) {
//     printf("%s<procExpression>\n",tab.c_str());
//     while (
//         m_current.type == TKN_NOT
//         || m_current.type == TKN_MINUS
//         || m_current.type == TKN_OPEN_PAR
//         || is_identifier()
//         || is_constant()
//             procSimpleExpr(tab+"\t"); }

//     if(is_relop()){
//             procRelOp(tab+"\t");
            
//             procSimpleExpr(tab+"\t");
//     }
// }

// expressionA ::= simple-expr expressionB
TokenType SyntaticAnalysis::procExpressionA(std::string tab) {
    printf("%s<procExpressionA>\n",tab.c_str());
    TokenType tt1, tt2;
    tt1 = procSimpleExprA(tab+"\t");

    tt2 = procExpressionB(tab+"\t");

    semanticAnalysis->checkDataType(tt1,tt2);

    return tt1;
}

// expressionB ::= relop simple-exprA | lambda
TokenType SyntaticAnalysis::procExpressionB(std::string tab) {
    printf("%s<procExpressionB>\n",tab.c_str());
    TokenType tt1=TKN_NONE;
    if(is_relop()){
        procRelOp(tab+"\t");
        tt1 = procSimpleExprA(tab+"\t");
    }
    return tt1;
}

// simple-exprA ::= termA simple-exprB
TokenType SyntaticAnalysis::procSimpleExprA(std::string tab) {
    printf("%s<procSimpleExprA>\n",tab.c_str());

    TokenType tt1 = procTermA(tab+"\t");

    TokenType tt2 = procSimpleExprB(tab+"\t");

    semanticAnalysis->checkDataType(tt1, tt2);

    return tt1;
}

// simple-exprB ::= addop term simple-exprB | lambda
TokenType SyntaticAnalysis::procSimpleExprB(std::string tab) {
    printf("%s<procSimpleExprB>\n",tab.c_str());

    TokenType tt1=TKN_NONE, tt2=TKN_NONE;
    if(is_addop()){
        procAddOp(tab+"\t");

        tt1 = procTermA(tab+"\t");
    
        tt2 = procSimpleExprB(tab+"\t");
    }else if(is_mulop()){
        tt1 = procTermB(tab+"\t");
    
        tt2 = procSimpleExprB(tab+"\t");
    }
    semanticAnalysis->checkDataType(tt1, tt2);
    return tt1;
}

// termA ::= factor-a termB
TokenType SyntaticAnalysis::procTermA(std::string tab) {
    printf("%s<procTermA>\n",tab.c_str());

    TokenType tt1 = procFactorA(tab+"\t");
    TokenType tt2 = procTermB(tab+"\t");

    semanticAnalysis->checkDataType(tt1, tt2);

    return tt1;
}
// termB ::= mulop factor-a termB
TokenType SyntaticAnalysis::procTermB(std::string tab) {
    printf("%s<procTermB>\n",tab.c_str());
    TokenType tt1=TKN_NONE, tt2=TKN_NONE;
    if(is_mulop()){
        procMulOp(tab+"\t");
        tt1 = procFactorA(tab+"\t");
        tt2 = procTermB(tab+"\t");
    }
    semanticAnalysis->checkDataType(tt1, tt2);
    return tt1;
}

// fator-a ::= factor | "!" factor | "-" factor
TokenType SyntaticAnalysis::procFactorA(std::string tab) {
    printf("%s<procFactorA>\n",tab.c_str());
    TokenType tt1;
    if(m_current.type == TKN_OPEN_PAR
        || is_identifier()
        || is_constant()){
        
        tt1 = procFactor(tab+"\t");
    }
    else if(m_current.type == TKN_NOT){
        eat(TKN_NOT, tab+"\t");

        tt1 = procFactor(tab+"\t");
    }
    else if(m_current.type == TKN_MINUS){
        eat(TKN_MINUS, tab+"\t");

        tt1 = procFactor(tab+"\t");
    }
    else{showError();}
    return tt1;
}

// factor ::= identifier | constant | "(" expression ")"
TokenType SyntaticAnalysis::procFactor(std::string tab) {
    printf("%s<procFactor>\n",tab.c_str());
    TokenType tt1;
    if(m_current.type==TKN_OPEN_PAR){
        eat(TKN_OPEN_PAR, tab+"\t");

        tt1 = procExpressionA(tab+"\t");

        eat(TKN_CLOSE_PAR, tab+"\t");
    } else if(is_identifier()) {
        std::string token = m_current.token;
        procIdentifier(tab+"\t");
        tt1 = semanticAnalysis->getTokenType(token);
    } else if(is_constant()) {
        tt1 = procConstant(tab+"\t");
    } else { showError(); }

    return tt1;
}

// relop ::= "==" | ">" | ">=" | "<" | "<=" | "<>"
void SyntaticAnalysis::procRelOp(std::string tab) {
    printf("%s<procRelOp>\n",tab.c_str());
    if(m_current.type == TKN_EQ){
        eat(TKN_EQ, tab+"\t");
    } else if (m_current.type == TKN_GT){
        eat(TKN_GT, tab+"\t");
    } else if (m_current.type == TKN_GE){
        eat(TKN_GE, tab+"\t");
    } else if (m_current.type == TKN_LT){
        eat(TKN_LT, tab+"\t");
    } else if (m_current.type == TKN_LE){
        eat(TKN_LE, tab+"\t");
    } else if (m_current.type == TKN_NE){
        eat(TKN_NE, tab+"\t");
    } else { showError(); }
}

// addop ::= "+" | "-" | "||"
void SyntaticAnalysis::procAddOp(std::string tab) {
    printf("%s<procAddOp>\n",tab.c_str());
    if(m_current.type == TKN_PLUS){
        eat(TKN_PLUS, tab+"\t");
    }else if (m_current.type == TKN_MINUS){
        eat(TKN_MINUS, tab+"\t");
    }else if (m_current.type == TKN_OR){
        eat(TKN_OR, tab+"\t");
    }else { showError(); }
}

// mulop ::= "*" | "/" | "&&"
void SyntaticAnalysis::procMulOp(std::string tab) {
    printf("%s<procMulOp>\n",tab.c_str());
    if(m_current.type == TKN_MUL){
        eat(TKN_MUL, tab+"\t");
    }else if (m_current.type == TKN_DIV){
        eat(TKN_DIV, tab+"\t");
    }else if (m_current.type == TKN_AND){
        eat(TKN_AND, tab+"\t");
    }else { showError(); }
}

// constant ::= integer_const | float_const | literal
TokenType SyntaticAnalysis::procConstant(std::string tab) {
    printf("%s<procConstant>\n",tab.c_str());
    TokenType tt1;
    if(m_current.type == TKN_INT){
        procIntegerConst(tab+"\t");
        tt1 = TKN_INT;
    }else if(m_current.type == TKN_FLOAT){
        procFloatConst(tab+"\t");
        tt1 = TKN_FLOAT;
    }else if(m_current.type ==TKN_STRING){
        procLiteral(tab+"\t");
        tt1 = TKN_STRING;
    }else{showError();}

    return tt1;
}

// integer_const ::= digit+
void SyntaticAnalysis::procIntegerConst(std::string tab) {
    printf("%s<procIntegerConst>\n",tab.c_str());
    eat(TKN_INT, tab+"\t");
}

// float_const ::= digit+
// “.”digit+
void SyntaticAnalysis::procFloatConst(std::string tab) {
    printf("%s<procFloatConst>\n",tab.c_str());
    eat(TKN_FLOAT, tab+"\t");
}

// literal ::= " { " {caractere} " } "
void SyntaticAnalysis::procLiteral(std::string tab) {
    printf("%s<procLiteral>\n",tab.c_str());
    //eat(TKN_OPEN_CUR, tab+"\t");
    if(m_current.type==TKN_STRING){
        procCharacter(tab+"\t");
    }
    //eat(TKN_CLOSE_CUR, tab+"\t");
}

// identifier ::= (letter | _ ) (letter | digit )*
void SyntaticAnalysis::procIdentifier(std::string tab) {
    printf("%s<procIdentifier>\n",tab.c_str());

    eat(TKN_ID, tab+"\t");
}

// caractere ::= um dos caracteres ASCII, exceto quebra de linha
void SyntaticAnalysis::procCharacter(std::string tab) {
    printf("%s<procCharacter>\n",tab.c_str());
    eat(TKN_STRING, tab+"\t");
}