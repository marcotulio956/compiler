#ifndef TOKENTYPE_H
#define TOKENTYPE_H

enum TokenType {
    // SPECIALS
    TKN_UNEXPECTED_EOF = -2,
    TKN_INVALID_TOKEN = -1,
    TKN_END_OF_FILE = 0,

    // SYMBOLS
    TKN_SEMICOLON,     // ;
    TKN_OPEN_PAR,      // (
    TKN_CLOSE_PAR,     // )
    TKN_OPEN_CUR,     // {
    TKN_CLOSE_CUR,     // }
    TKN_DOT,           // .
    TKN_COMMA,         // ,
    
    // OPERATORS
    TKN_ASSIGN,        // =
    TKN_EQ,            // ==
    TKN_NE,            // <>
    TKN_NOT,           // ! negate logical 
    TKN_PLUS,          // +
    TKN_MINUS,         // -
    TKN_MUL,           // *
    TKN_DIV,           // /
    TKN_AND,            // &&
    TKN_OR,            // ||
    TKN_GT,            // > 
    TKN_LT,            // <
    TKN_GE,            // >=
    TKN_LE,            // <=

    // KEYWORDS

    // OTHERS
    TKN_ID,          // identifier
    TKN_STRING       // string

};

inline std::string tt2str(enum TokenType type) {
    switch (type) {
        // Specials
        case TKN_UNEXPECTED_EOF:
            return "UNEXPECTED_EOF";
        case TKN_INVALID_TOKEN:
            return "INVALID_TOKEN";
        case TKN_END_OF_FILE:
            return "END_OF_FILE";

        // Symbols
        // OPERATORS
        // KEYWORDS
        // OTHERS
        case TKN_ID:
            return "ID";            // identifier
        case TKN_STRING:
            return "STR";        // string

        default:
            throw std::string("invalid token type");
    }
}

#endif
