#include <cstdio>
#include <cctype>
#include <cassert>

#include <iostream>

#include "LexicalAnalysis.h"

LexicalAnalysis::LexicalAnalysis(const char* filename) : m_line(1) {
    m_file = fopen(filename, "r");
    if (!m_file)
        throw std::string("Unable to open file");
}

LexicalAnalysis::~LexicalAnalysis() {
    fclose(m_file);
}

int LexicalAnalysis::line() const {
    return m_line;
}

struct Lexeme LexicalAnalysis::nextToken() {
    struct Lexeme lex;
    const int sb_find = -1;
    const int tkn_defined = -2;
    int state = 1;
    while (state != sb_find && state != tkn_defined) {
        int c = getc(m_file);
        printf("\tstate %02d, (char) '%c', %d ascii\n", state, (char) c, c);

        switch (state) {
            case 1:{
                if(c == '\t' || c == '\r' || c == '\n' || c == ' '){
                    state = 1;
                }else if (isdigit(c)){
                    lex.token += (char) c;
                    state = 2;
                }else if (c == '"'){
                    lex.token += (char) c;
                    state = 4;
                }else if(c == ';'
                    || c == '('
                    || c == ')'
                    || c == '{'
                    || c == '}'
                    || c == '.'
                    || c == ','
                    || c == '!'
                    || c == '+'
                    || c == '-'
                    || c == '*'
                    || c == '/'
                ){
                    lex.token += (char) c;
                    state = sb_find;
                }else if(c == '='){
                    lex.token += (char) c;
                    state = 5;
                }else if(c == '>'){
                    lex.token += (char) c;
                    state = 6;
                }else if(c == '<'){
                    lex.token += (char) c;
                    state = 7;
                }else if(c == '&'){
                    lex.token += (char) c;
                    state = 8;
                }else if(c == '|'){
                    lex.token += (char) c;
                    state = 9;
                }else if(isalpha(c)){
                    lex.token += (char) c;
                    state = 10;
                }else if(c == -1){
                    ungetc(c,m_file);
                    lex.type = TKN_END_OF_FILE;
                    state = tkn_defined;
                }
                break;
            }
            case 2:{ // get digit from until '.', else ret int
                if (isdigit(c)){ 
                    lex.token += (char) c;
                    state = 2;
                } else if (isalpha(c)){
                    ungetc(c,m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                } else if (c == '.'){
                    lex.token += (char) c;
                    state = 3;
                } else {
                    ungetc(c, m_file);
                    lex.type = TKN_INT;
                    state = tkn_defined;
                }
                break;
            }
            case 3:{// after '.' get digit until other then return
                if (isdigit(c)){
                    lex.token += (char) c;
                    state = 3;
                } else {
                    ungetc(c, m_file);
                    lex.type = TKN_FLOAT;
                    state = tkn_defined;
                }
                break;
            }
            case 4:{
                if(c >= 32 && c<= 254 && c != 34){
                    lex.token += (char) c;
                    state = 4;
                } else if (c == '"'){
                    lex.type = TKN_STRING;
                    state = tkn_defined;
                } else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                } else {

                }
                break;
            }
            case 5:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_EQ;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_ASSIGN;
                    state = tkn_defined;
                }
                break;
            }
            case 6:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_GE;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_GT;
                    state = tkn_defined;
                }
                break;
            }
            case 7:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_LE;
                    state = tkn_defined;
                }else if (c == '>'){
                    lex.token += (char) c;
                    lex.type = TKN_NE;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_LT;
                    state = tkn_defined;
                }
                break;
            }
            case 8:{
                if(c == '&'){
                    lex.token += (char) c;
                    lex.type = TKN_AND;
                    state = tkn_defined;
                }else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                }
                break;
            }
            case 9:{
                if(c == '|'){
                    lex.token += (char) c;
                    lex.type = TKN_OR;
                    state = tkn_defined;
                }else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                }
                break;
            }
            case 10:{
                if(isalnum(c) || c=='_'){
                    lex.token += (char) c;
                    state = 10;
                }
                else{
                    ungetc(c,m_file);
                    state = sb_find;
                }
                break;
            }
            default: {
                assert(false);
            }
        }
    }
    if(state==sb_find){
        lex.type = m_st.find(lex.token);
    }
    std::cout << "\t\tm_st[" << lex.token << "]: " << lex.type << " _ " << tt2str(TokenType(lex.type)) << std::endl;
    return lex;
}
