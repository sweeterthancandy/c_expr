#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "memory_pool.h"

typedef struct Node Node;

typedef struct ParserContext{
        TokenizerContext tokenizer;
        MemoryPool       pool;
        void*            stack_mem[128];
        void**           stack_ptr;
        int              fail; // fail flag
}ParserContext;

double ParserContext_Eval(ParserContext* ctx, Node* node);
int parse_expr(ParserContext* ctx);

#endif /* PARSER_H */
