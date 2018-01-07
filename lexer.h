
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    TYPE, VAR, REAL, INT,//4
    BOOLEAN, STRING, LONG,//7
    WHILE, DO, SWITCH, CASE,//11
    COMMA, COLON, SEMICOLON,//14
    LBRACE, RBRACE, LPAREN, RPAREN,//18
    EQUAL, PLUS, MULT, DIV,//22
    GREATER, GTEQ, LESS, LTEQ, NOTEQUAL,//27
    ID, NUM, REALNUM, ERROR//31
} TokenType;

class Token {
  public:
    void Print();
    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};

#endif  //__LEXER__H__
