#include "tokenizer.h"
#include "runtime.h"

// make TokenizerContext::tok be the next token
//
// returns true is TokenizerContext::tok exists otherwise,
// either eos or error, which can be checked with the flags
//
// postconditons:
//      
int Tokenizer_Next(TokenizerContext* ctx){

        switch(ctx->state){
        case TokenizerState_Vanilla:
                break;
        case TokenizerState_OneBeforeEos:
                ctx->tok = ctx->peak;
                ctx->eos = 1;
                ctx->state = TokenizerState_EOS;
                return 1;
        case TokenizerState_EOS:
                assert( ctx->eos && "invariant failed");
                return 0;
        case TokenizerState_Error:
                assert( ctx->error && "invariant failed");
                return 0;
        }

        if( ctx->eos || ctx->error )
                return 0;
        ctx->tok = ctx->peak;
        
        memset( &ctx->peak, 0, sizeof(ctx->peak));

        // eat whitespace
        for( ; ctx->iter != ctx->end &&
               isspace(*ctx->iter) ; ++ctx->iter);

        if( ctx->iter == ctx->end ){
                ctx->state = TokenizerState_OneBeforeEos;
                return 1;
        }

        switch( *ctx->iter ){
                case '+':
                case '-':
                case '*':
                case '/':
                        ctx->peak.type = TokenType_Operator;
                        ctx->peak.op   = *ctx->iter;
                        ++ctx->iter;
                        break;
                case '(':
                        ctx->peak.type = TokenType_LParam;
                        ++ctx->iter;
                        break;
                case ')':
                        ctx->peak.type = TokenType_RParam;
                        ++ctx->iter;
                        break;
                case ',':
                        ctx->peak.type = TokenType_Comma;
                        ++ctx->iter;
                        break;
                default:{
                        if( isdigit(*ctx->iter)){
                                // try to get value, otherwise fail the whole stream
                                // because the peak is invalid
                                const char* end = ctx->iter;
                                ctx->peak.value = 0;
                                for(; end != ctx->end; ++end){
                                        if( ! isdigit(*end) )
                                                break;
                                        ctx->peak.value *= 10;
                                        ctx->peak.value += *end - '0';
                                }
                                // if we have a floating point literal
                                //      
                                //              234.23
                                //              123.1
                                //
                                if( end != ctx->end && *end == '.' ){
                                        ++end;
                                        double rest = 0;
                                        double base = 1e-1;
                                        for(; end != ctx->end; ++end){
                                                if( ! isdigit(*end) ){
                                                        break;
                                                }
                                                rest += ( *end - '0' ) * base;
                                                base /= 10.0;
                                        }
                                        ctx->peak.value += rest;
                                }
                                ctx->peak.type  = TokenType_Literal;
                                ctx->iter = end;
                                return 1;
                        } else if( isalpha(*ctx->iter)){
                                const char* end = ctx->iter;
                                char* word_ptr = ctx->peak.name;
                                for(; end != ctx->end; ++end){
                                        if( ! ( isalnum(*end) || *end == '_' ) )
                                                break;
                                        *word_ptr = *end;
                                        ++word_ptr;
                                }
                                ctx->peak.type  = TokenType_Identifier;
                                ctx->iter = end;
                                return 1;
                        } else{
                                // fail here
                                ctx->state = TokenizerState_Error;
                                ctx->error = 1;
                                return 0;
                        }
                }
        }
        return 1;
}

void Tokenizer_Init(TokenizerContext* ctx, const char* source){
        memset( ctx, 0, sizeof(*ctx));

        ctx->str = source;
        ctx->iter = source;
        ctx->end = source + strlen(source);
        ctx->eos = 0;
        
        // tok = (null), peak = (null)

        // tok = (null), peak = <token_0>
        Tokenizer_Next(ctx);
        
        // tok = <token_0>, peak = <token_1>
        Tokenizer_Next(ctx);
}

void Tokenizer_DumpToken(Token* tok){
        switch(tok->type){
        case TokenType_invalid:
        default:
                printf("<invalid>\n");
                break;
        case TokenType_Literal:
                printf("literal(%d)\n", tok->value);
                break;
        case TokenType_Operator:
                printf("operator(%c)\n", tok->op);
                break;
        case TokenType_Identifier:
                printf("identifier(%s)\n", (const char*)tok->name);
                break;
        case TokenType_LParam:
                printf(" ( \n");
                break;
        case TokenType_RParam:
                printf(" ) \n");
                break;
        case TokenType_Comma:
                printf(" , \n");
                break;
        }
}
void Tokenizer_Dump(TokenizerContext* ctx){
      Tokenizer_DumpToken(&ctx->tok);  
}

