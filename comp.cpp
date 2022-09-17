#include <iostream>

#include "lexical/LexicalAnalysis.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [source file]\n", argv[0]);
        return 1;
    }

    try {
        LexicalAnalysis l(argv[1]);

/*
        // O código a seguir é dado para testar o interpretador.
        // TODO: descomentar depois que o analisador léxico estiver OK.
        SyntaticAnalysis s(l);
        Command* c = s.start();
        c->execute();
        delete c;
*/

        // O código a seguir é usado apenas para testar o analisador léxico.
        // TODO: depois de pronto, comentar o código abaixo.
        struct Lexeme lex;
        do {
            lex = l.nextToken();
            printf("%02d: <\"%s\", %s>\n", l.line(),
                lex.token.c_str(), tt2str(TokenType(lex.type)).c_str());
        } while (lex.type != TKN_END_OF_FILE &&
                    lex.type != TKN_INVALID_TOKEN &&
                    lex.type != TKN_UNEXPECTED_EOF);
    } catch (std::string msg) {
        fprintf(stderr, "Internal error: %s\n", msg.c_str());
        return 2;
    }

    return 0;
}
