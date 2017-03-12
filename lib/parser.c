#include "parser.h"
#include "expr.h"

double ParserContext_Eval(ParserContext* ctx, Node* node){
        if( ctx->fail ||
            ctx->stack_ptr - ctx->stack_mem != 1 )
                return 0;

        double result = Node_Eval( ctx->stack_mem[0] );
        return result;
}

int parse_expr(ParserContext* ctx);

int parse_term(ParserContext* ctx){
        switch(ctx->tokenizer.tok.type){
        case TokenType_Literal: {
                // we got a literal
                Node* ptr = MemoryPool_Alloc(&ctx->pool, sizeof(Node));
                Node_InitValue(ptr, ctx->tokenizer.tok.value);
                *ctx->stack_ptr = ptr;
                ++ctx->stack_ptr;
                // eat it
                Tokenizer_Next(&ctx->tokenizer);
                return 1;
        }
        case TokenType_Identifier: {
                // this can be a function call

                Token idTok = ctx->tokenizer.tok;
                // eat it
                Tokenizer_Next(&ctx->tokenizer);

                // it's a function call when 
                //      <identifier> ( ...
                if( ctx->tokenizer.tok.type == TokenType_LParam ){
                        // eat the (
                        Tokenizer_Next(&ctx->tokenizer);


                        Node* args[32] = {0};
                        Node** arg_ptr = args;

                        // check if not a nullary function call
                        if( ctx->tokenizer.tok.type != TokenType_RParam ){
                                for(;;){
                                        if( ! parse_expr(ctx) ){
                                                ctx->fail = 1;
                                                return 0;
                                        }

                                        // put it on the arg stack
                                        --ctx->stack_ptr;
                                        *arg_ptr = *ctx->stack_ptr;

                                        ++arg_ptr;

                                        switch( ctx->tokenizer.tok.type ){
                                        case TokenType_Comma:
                                                // eat it
                                                Tokenizer_Next(&ctx->tokenizer);
                                                continue;
                                        }

                                        // XXX
                                        break;
                                }
                        }
                        if( ctx->tokenizer.tok.type != TokenType_RParam ){
                                ctx->fail = 1;
                                return 0;
                        }
                        // eat it
                        Tokenizer_Next(&ctx->tokenizer);
                
                        // we got a function call
                        Node* ptr = MemoryPool_Alloc(&ctx->pool, sizeof(Node));
                        Node_InitCall(ptr, idTok.name, args);
                        *ctx->stack_ptr = ptr;
                        ++ctx->stack_ptr;
                        return 1;
                } else {



                        // we got a idenfier
                        Node* ptr = MemoryPool_Alloc(&ctx->pool, sizeof(Node));
                        Node_InitIdentifer(ptr, idTok.name);
                        *ctx->stack_ptr = ptr;
                        ++ctx->stack_ptr;
                        return 1;
                }
        }

        case TokenType_LParam:
                // eat it
                Tokenizer_Next(&ctx->tokenizer);
                parse_expr(ctx);
                if(ctx->tokenizer.tok.type != TokenType_RParam){

                        ctx->fail = 1;
                        return 0;
                }
                // eat it
                Tokenizer_Next(&ctx->tokenizer);
                return 1;
        default:
                Tokenizer_Dump(&ctx->tokenizer);
                return 0;
        }
}

int parse_factor(ParserContext* ctx){
        // this should populate the stack with a term
        if ( ! parse_term(ctx) ){
                return 0;
        }

        for(; ctx->tokenizer.tok.type == TokenType_Operator ;){
                Token tokOp = ctx->tokenizer.tok;
                Operator mappedOp;
                switch(tokOp.op){
                case '*' : mappedOp = Operator_Mul; break;
                case '/' : mappedOp = Operator_Div; break;
                default:
                           return 1;
                }

                // eat the operator
                Tokenizer_Next(&ctx->tokenizer);
                if( ! parse_term(ctx) ){
                        ctx->fail = 1;
                        return 0;
                }
                --ctx->stack_ptr;
                Node* rp = *ctx->stack_ptr;
                --ctx->stack_ptr;
                Node* lp = *ctx->stack_ptr;

                Node* result = MemoryPool_Alloc(&ctx->pool, sizeof(Node));
                *ctx->stack_ptr = result;
                ++ctx->stack_ptr;


                Node_InitOperator( result, mappedOp, lp, rp);
        }
        return 1;
}

int parse_expr(ParserContext* ctx){
        // this should populate the stack with a term
        if ( ! parse_factor(ctx) ){
                return 0;
        }

        for(; ctx->tokenizer.tok.type == TokenType_Operator ;){
                Token tokOp = ctx->tokenizer.tok;
                Operator mappedOp;
                switch(tokOp.op){
                case '+' : mappedOp = Operator_Add; break;
                case '-' : mappedOp = Operator_Sub; break;
                default:
                           return 1;
                }

                // eat the operator
                Tokenizer_Next(&ctx->tokenizer);
                if( ! parse_factor(ctx) ){
                        ctx->fail = 1;
                        return 0;
                }
                --ctx->stack_ptr;
                Node* rp = *ctx->stack_ptr;
                --ctx->stack_ptr;
                Node* lp = *ctx->stack_ptr;

                Node* result = MemoryPool_Alloc(&ctx->pool, sizeof(Node));
                *ctx->stack_ptr = result;
                ++ctx->stack_ptr;


                Node_InitOperator( result, mappedOp, lp, rp);
        }
        return 1;
}
