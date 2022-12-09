#include <iostream>

#include "lexical/LexicalAnalysis.h"
#include "lexical/SymbolTable.h"
#include "syntatic/SyntaticAnalysis.h"

int main(int argc, char* argv[]) {
    bool DEBug=false, NOError=false;
    if (argc < 2) {
        printf("Usage: %s (source file) [DEBug] [NOError]\n", argv[0]);
        return 1;
    } // argc == 2 runs normally
    if(argc == 3 || argc == 4){
        if(atoi(argv[2]) == 1){
            printf("DEBug Mode Enabled: (args3)\n");
            DEBug = true;
        }
        if(argc == 4 && atoi(argv[3])==1){
            printf("NOError Mode Enabled: (argv4)\n");
            NOError = true;
        }
    }
    if(argc > 4){
        printf("Unexpected argc: %d\n", argc);
    }
    try {
        SymbolTable st;
        LexicalAnalysis lexcl(argv[1], DEBug, &st);

        if(DEBug) {
            LexicalAnalysis lexcl_DEBug(argv[1], 1, &st);
            struct Lexeme lex;
            do {
                lex = lexcl_DEBug.nextToken();
                printf("%02d: <\"%s\", %s>\n", 
                    lexcl_DEBug.line(),
                    lex.token.c_str(), 
                    tt2str(TokenType(lex.type)).c_str()
                );
            } while (
                lex.type != TKN_END_OF_FILE 
                && lex.type != TKN_UNEXPECTED_EOF
                && (NOError == true
                    || lex.type != TKN_INVALID_TOKEN)
            );
        }

        SemanticAnalysis seman(&st);
        SyntaticAnalysis syntc(&lexcl, &seman);
        syntc.start();
        
        // O código a seguir é dado para testar o interpretador.
        // TODO: descomentar depois que o analisador léxico estiver OK.
        /*  SyntaticAnalysis s(l);
            Command* c = s.start();
            c->execute();
            delete c;
        */
    } catch (std::string msg) {
        fprintf(stderr, "Internal error: %s\n", msg.c_str());
        return 2;
    }

    return 0;
}
