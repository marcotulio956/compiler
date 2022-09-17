#include <cstdio>
#include <cctype>
#include <cassert>

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
    const int final_state = 0;
    int state = 1;
    while (state != final_state) {
        int c = getc(m_file);
        // printf("\tstate %02d, (char) '%c'\n", state, (char) c);

        switch (state) {
            case 1:{
                if(c == '\t' || c == '\r' || c == '\n'){
                    state = 1;
                }else if (isdigit(c)){
                    lex.token += (char) c;
                    state = 2;
                }else if (c == '"'){
                    lex.token += (char) c;
                    state = 4;
                }else if(c == -1){
                    lex.type = TKN_END_OF_FILE;
                    state = final_state;
                }
                break;
            }
            case 2:{ // get digit from until '.', else ret int
                if (isdigit(c)){ 
                    lex.token += (char) c;
                    state = 2;
                } else if (c == '.'){
                    lex.token += (char) c;
                    state = 3;
                } else {
                    ungetc(c, m_file);
                    lex.type = TKN_INT;
                    state = final_state;
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
                    state = final_state;
                }
                break;
            }
            case 4:{
                if(isalpha(c) || isdigit(c)){
                    lex.token += (char) c;
                    state = 4;
                } else if (c == '"'){
                    lex.type = TKN_STRING;
                    state = final_state;
                }
                break;
            }
            case final_state:{
                lex.type = m_st.find(lex.token);
            }
            default: {
                assert(false);
            }
        }
    }
    return lex;
}
