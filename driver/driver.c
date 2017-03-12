
/*
   accepts string of the grammar

      digit  -> 0123456789

      digits -> digit+

      term   -> digits 
             |  ( expr )

      factor -> term
             |  factor * term
             |  factor / term

      expr   -> factor
             |  expr   + factor
             |  expr   - factor

    +---------+     +------+     +---------------+
    |Tokenizer| --> |Parser|  -> |Expression Tree|
    +---------+     +------+     +---------------+

*/


/*
        Tokenizer
        =========

        Main thing with the tokenizer is the peaking
        mechinism, ie 3 cases for getting tokens

                Initlization
                ============
                <null>  -> init to eos
                1       -> init to 

*/

#include "tokenizer.h"
#include "memory_pool.h"
#include "expr.h"
#include "parser.h"
#include "runtime.h"

double eval(const char* str){
        ParserContext ctx = {0};
        Tokenizer_Init(&ctx.tokenizer, str);
        MemoryPool_Init(&ctx.pool);
        ctx.stack_ptr = ctx.stack_mem;

        parse_expr(&ctx);

        if( ! (ctx.stack_ptr - ctx.stack_mem == 1  )){
                printf("bad stack, stack_size = %d\n", ctx.stack_ptr - ctx.stack_mem );
                return -1;
        }

        double result = Node_Eval( ctx.stack_mem[0] );
        
        #if 1
        Node_Dump( ctx.stack_mem[0] );
        printf("\n");
        Node_DumpPretty( ctx.stack_mem[0] );
        printf("\n");
        printf("eval -> %f\n", result  );
        printf("\n");
        #endif
        return result;

}


int self_test(){
        #if 0
        #define EVAL_TEST( EXPR ) do{\
                int result = eval(#EXPR);\
                printf(#EXPR " = %d\n", result); \
                if( result != (EXPR) ){\
                        printf("eval failed on "#EXPR"\n");\
                }\
        }while(0)

        EVAL_TEST(11);
        EVAL_TEST(11+22);
        EVAL_TEST(11- 444);
        EVAL_TEST(11- 444 + 222);
        EVAL_TEST(11- 444 + 222 + 999);

        EVAL_TEST(1+2+3+4+5);
        EVAL_TEST(1+2+3+4*5);
        EVAL_TEST(1+2+3*4+5);
        EVAL_TEST(1+2*3+4+5);
        EVAL_TEST(1*2+3+4+5);
        
        EVAL_TEST(1+2*(3+4)+5);
        EVAL_TEST(1*(2+3)+4+5);

        EVAL_TEST( 234 + 8788 / 2334); 
        EVAL_TEST( 234 * 8788 / 2334); 

        eval("hello");
        eval("hello+world");

        Tokenizer_dotest("f(1)");
        Tokenizer_dotest("f(1+2)");
        Tokenizer_dotest("f(1,2)");
        #endif
        
        
        #if 0
        eval("f(1)");
        eval("f(1+2)");
        eval("f(1,2)");
        eval("f()");
        #endif
        
        #if 0
        eval("cos(2)");
        eval("2*sin(23)");
        #endif
        eval("0.34");
        
        #if 0
        eval("");
        eval("1");
        eval("12");
        eval("  1");
        eval("1   ");
        eval("1+3");
        eval("1   *334   /2");
        eval("35454");
        eval("1+3sb");
        #endif
}

typedef enum RuntimeFlag{
        RuntimeFlag_debug = 1
}RuntimeFlag;

typedef struct DriverContext{
        int flags;
}DriverContext;

int main(int argc, char** argv){
        if( argc == 3 && strcmp(argv[1],"-c") == 0 ){
                printf("%f\n", eval(argv[2]));
        }
}
