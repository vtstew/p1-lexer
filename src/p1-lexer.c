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
    Regex* keyword = Regex_new("^(def|if|else|while|return|break|continue|int|bool|void|true|false)");
    Regex* reserved = Regex_new("^(for|callout|class|interface|extends|implements|new|this|string|float|double|null)");
    Regex* whitespace = Regex_new("^[ \n|\t]");
    Regex* newline = Regex_new("^\n");
    Regex* letter = Regex_new("^[a-zA-Z]([0-9]|[a-zA-Z]|_)*");
    Regex* numbers = Regex_new("^(0|[1-9]+)");
    Regex* grouping = Regex_new("^(\\(|\\)|\\{|\\}|\\[|\\]|\\,|\\;)");
    Regex* symbols = Regex_new("^(\\+|\\*|\\=)");
    Regex* or_equal = Regex_new("^(<|>|=|!)=");
    Regex* strings = Regex_new("^\"([a-zA-Z]|[0-9])*\"");
    Regex* hex = Regex_new("^(0x)([0-9]|[a-f])*");
    Regex* comment = Regex_new("^(\\/\\/)");


    int line_count = 1;
    /* read and handle input */
    char match[MAX_TOKEN_LEN];
    
    while (*text != '\0') {
        // printf("%c", *text);
        /* match regular expressions */

        if (Regex_match(newline, text, match)) {
            line_count++;
        }


        if (Regex_match(whitespace, text, match)) {
            /* ignore whitespace */
        } else if (Regex_match(comment, text, match)) {
            while (*text != '\n'){
                text += 1;
            }
            text--;
        } else if (Regex_match(reserved, text, match)) {
            Error_throw_printf("Invalid token!\n");
        } else if (Regex_match(hex, text, match)) {
            TokenQueue_add(tokens, Token_new(HEXLIT, match, line_count)); 
        } else if (Regex_match(letter, text, match)) {
            // char* fullText = text;
            if (Regex_match(keyword, text, match)) {
                TokenQueue_add(tokens, Token_new(KEY, match, line_count)); 
            } else {
                TokenQueue_add(tokens, Token_new(ID, match, line_count)); 
                // text += strlen(match);
            }
        } else if (Regex_match(numbers, text, match)) {
            TokenQueue_add(tokens, Token_new(DECLIT, match, line_count)); 
        } else if (Regex_match(or_equal, text, match)) {
            TokenQueue_add(tokens, Token_new(SYM, match, line_count)); 
        } else if ( Regex_match(grouping, text, match)) {
            TokenQueue_add(tokens, Token_new(SYM, match, line_count)); 
        } else if ( Regex_match(symbols, text, match)) {
            TokenQueue_add(tokens, Token_new(SYM, match, line_count)); 
        } else if (Regex_match(strings, text, match)) {
            TokenQueue_add(tokens, Token_new(STRLIT, match, line_count)); 
        } else {
            Error_throw_printf("Invalid token!\n");
        }
 
        /* skip matched text to look for next token */
        text += strlen(match);
    }
 
    /* clean up */
    Regex_free(keyword);
    Regex_free(reserved);
    Regex_free(whitespace);
    Regex_free(letter);
    Regex_free(numbers);
    Regex_free(symbols);
    Regex_free(grouping);
    Regex_free(or_equal);
    Regex_free(strings);
    Regex_free(hex);
    Regex_free(comment);
    Regex_free(newline);
 
    return tokens;
}

