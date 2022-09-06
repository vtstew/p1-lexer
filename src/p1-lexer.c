/**
 * @file p1-lexer.c
 * @brief Compiler phase 1: lexer
 * Vivian Stewart and Katie Brasacchio
 */
#include "p1-lexer.h"

TokenQueue* lex (char* text)
{
    TokenQueue* tokens = TokenQueue_new();
 
    /* compile regular expressions */
    Regex* whitespace = Regex_new("^[ \n]");
    Regex* letter = Regex_new("^[a-zA-Z][a-zA-Z]*([0-9]*[a-zA-Z]*_*)*");
    Regex* numbers = Regex_new("^0|^[1-9]+");
    Regex* symbols = Regex_new("\\+|\\(|\\)|\\*");

    int line_count = 1;
    
    /* read and handle input */
    char match[MAX_TOKEN_LEN];
    while (*text != '\0') {
 
        /* match regular expressions */
        if (Regex_match(whitespace, text, match)) {
            /* ignore whitespace */
        } else if (Regex_match(letter, text, match)) {
            TokenQueue_add(tokens, Token_new(ID, match, line_count)); 
            line_count++;
        } else if (Regex_match(numbers, text, match)) {
            TokenQueue_add(tokens, Token_new(DECLIT, match, line_count)); 
            line_count++;
        } else if (Regex_match(symbols, text, match)) {
            TokenQueue_add(tokens, Token_new(SYM, match, line_count)); 
            line_count++;
        } else {
            Error_throw_printf("Invalid token!\n");
        }
 
        /* skip matched text to look for next token */
        text += strlen(match);
    }
 
    /* clean up */
    Regex_free(whitespace);
    Regex_free(letter);
    Regex_free(numbers);
    Regex_free(symbols);
 
    return tokens;
}

