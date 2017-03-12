#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum TokenizerState{
        TokenizerState_Vanilla = 0,
        TokenizerState_OneBeforeEos,
        TokenizerState_EOS,
        // gets in the error state getting the peak,
        // go parsing 
        //
        //       123 aaa  
        //       ^
        // will fail because the peak will fail.
        TokenizerState_Error,
}TokenizerState;

typedef enum TokenType{
        TokenType_invalid = 0, // so memset gives an invalid token
        TokenType_Literal,
        TokenType_Identifier,
        TokenType_Operator,
        TokenType_LParam,
        TokenType_RParam,
        TokenType_Comma
}TokenType;

typedef struct Token{
        TokenType type;
        double value;
        char name[128];
        char op;
}Token;

typedef struct TokenizerContext{
        const char* str;
        // (iter,end) corresping to peak
        const char* iter;
        const char* end; 
        int eos;           // end of stream flag
        int error;         // error flag, once this set
                           // theres no going back
        Token tok;
        Token peak;        // restrict outselves to grammars
                           // where one token ahead is enough
        TokenizerState state;
}TokenizerContext;

int Tokenizer_Next(TokenizerContext* ctx);
void Tokenizer_Init(TokenizerContext* ctx, const char* source);
void Tokenizer_DumpToken(Token* tok);
void Tokenizer_Dump(TokenizerContext* ctx);

#endif /* TOKENIZER_H */

