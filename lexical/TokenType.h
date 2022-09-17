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
    TKN_OPEN_CUR,      // {
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
    TKN_AND,           // &&
    TKN_OR,            // ||
    TKN_GT,            // >
    TKN_LT,            // <
    TKN_GE,            // >=
    TKN_LE,            // <=

    // KEYWORDS
    TKN_START,         // start
    TKN_EXIT,          // exit
    TKN_IF,            // if
    TKN_THEN,          // then
    TKN_ELSE,          // else
    TKN_END,           // end
    TKN_DO,            // do
    TKN_WHILE,         // while
    TKN_SCAN,          // scan
    TKN_PRINT,         // print

    // DATA TYPES
    TKN_INT,           // int
    TKN_FLOAT,         // float
    TKN_STRING,        // string

    // OTHERS
    TKN_ID,            // identifier
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
        case TKN_SEMICOLON:
            return "SEMICOLON";
        case TKN_OPEN_PAR:
            return "OPEN_PAR";
        case TKN_CLOSE_PAR:
            return "CLOSE_PAR";
        case TKN_OPEN_CUR:
            return "OPEN_CUR";
        case TKN_CLOSE_CUR:
            return "CLOSE_CUR";
        case TKN_DOT:
            return "DOT";
        case TKN_COMMA:
            return "COMMA";

        // OPERATORS
        case TKN_ASSIGN:        // =
            return "ASSIGN";
        case TKN_EQ:            // ==
            return "EQ";
        case TKN_NE:            // <>
            return "NE";
        case TKN_NOT:           // ! negate logical
            return "NOT";
        case TKN_PLUS:          // +
            return "PLUS";
        case TKN_MINUS:         // -
            return "MINUS";
        case TKN_MUL:           // *
            return "MUL";
        case TKN_DIV:           // /
            return "DIV";
        case TKN_AND:           // &&
            return "AND";
        case TKN_OR:            // ||
            return "OR";
        case TKN_GT:            // >
            return "GT";
        case TKN_LT:            // <
            return "LT";
        case TKN_GE:            // >=
            return "GE";
        case TKN_LE:            // <=
            return "LE";

        // KEYWORDS
        case TKN_START:         // start
            return "START";
        case TKN_EXIT:          // exit
            return "EXIT";
        case TKN_IF:            // if
            return "IF";
        case TKN_THEN:          // then
            return "THEN";
        case TKN_ELSE:          // else
            return "ELSE";
        case TKN_END:           // end
            return "END";
        case TKN_DO:            // do
            return "DO";
        case TKN_WHILE:         // while
            return "WHILE";
        case TKN_SCAN:          // scan
            return "SCAN";
        case TKN_PRINT:         // print
            return "PRINT";

        // DATA TYPES
        case TKN_INT:           // int
            return "INT";
        case TKN_FLOAT:         // float
            return "FLOAT";
        case TKN_STRING:        // string
            return "STRING";

        // OTHERS
        case TKN_ID:            // identifier
            return "ID";

        default:
            throw std::string("invalid token type");
    }
}

#endif
