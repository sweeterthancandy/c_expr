extern "C"{
#include "tokenizer.h"
}

#include <gtest/gtest.h>

namespace{
        struct tokenizer_aux{
                tokenizer_aux(std::string const& expr){
                        TokenizerContext ctx = {0};
                        Tokenizer_Init(&ctx, expr.c_str());
                        for( ; ! ( ctx.eos || ctx.error ) ;
                             Tokenizer_Next(&ctx) )
                        {
                                tokens_.push_back( ctx.tok);
                        }
                }
                decltype(auto) operator[](size_t i)const{
                        return tokens_.at(i);
                }
                decltype(auto) size()const{ return tokens_.size(); }
        private:
                std::vector<Token> tokens_;
        };
}

struct tokenizer : testing::Test{
        void SetUp()final{
        }
};

TEST_F( tokenizer, null ){
        for( auto s : {"", "    ", "\t   ", "\t\n\t  \n"} ){
                tokenizer_aux aux(s);
                EXPECT_EQ( 0 , aux.size() );
        }
}

TEST_F( tokenizer, single_value ){
        for( auto s : {"123", "    123", "    123", "   123   "} ){
                tokenizer_aux aux(s);
                EXPECT_EQ( 1 , aux.size() );
                EXPECT_EQ( TokenType_Literal, aux[0].type );
                EXPECT_FLOAT_EQ( 123, aux[0].value );
        }
}

TEST_F( tokenizer, real_value ){
        tokenizer_aux aux("  12.23   78.55555");
        EXPECT_EQ( 2 , aux.size() );

        EXPECT_EQ( TokenType_Literal, aux[0].type );
        EXPECT_FLOAT_EQ( 12.23 , aux[0].value );
        
        EXPECT_EQ( TokenType_Literal, aux[1].type );
        EXPECT_FLOAT_EQ( 78.55555 , aux[1].value );
}

TEST_F( tokenizer, lp ){
        tokenizer_aux aux("  (");
        EXPECT_EQ( 1 , aux.size() );
        EXPECT_EQ( TokenType_LParam, aux[0].type );
}

TEST_F( tokenizer, rp ){
        tokenizer_aux aux(")  ");
        EXPECT_EQ( 1 , aux.size() );
        EXPECT_EQ( TokenType_RParam, aux[0].type );
}

TEST_F( tokenizer, comma ){
        tokenizer_aux aux("  ,  ");
        EXPECT_EQ( 1 , aux.size() );
        EXPECT_EQ( TokenType_Comma, aux[0].type );
}

TEST_F( tokenizer, operators ){
        tokenizer_aux aux("+-*/");
        EXPECT_EQ( 4 , aux.size() );
        EXPECT_EQ( TokenType_Operator, aux[0].type );
        EXPECT_EQ( TokenType_Operator, aux[1].type );
        EXPECT_EQ( TokenType_Operator, aux[2].type );
        EXPECT_EQ( TokenType_Operator, aux[3].type );
        
        EXPECT_EQ( '+' , aux[0].op);
        EXPECT_EQ( '-' , aux[1].op);
        EXPECT_EQ( '*' , aux[2].op);
        EXPECT_EQ( '/' , aux[3].op);

}

TEST_F( tokenizer, ident ){
        tokenizer_aux aux(" hello world");
        EXPECT_EQ( 2 , aux.size() );
        EXPECT_EQ( TokenType_Identifier, aux[0].type );
        EXPECT_EQ( TokenType_Identifier, aux[1].type );
        EXPECT_EQ( "hello", std::string(aux[0].name));
        EXPECT_EQ( "world", std::string(aux[1].name));
}

TEST_F( tokenizer, composite_0 ){
        tokenizer_aux aux(" a + 12.34 / c *f(e,3)");
        
        //                               a + 12.34 / c *f(e,3)
        std::vector<TokenType> seq = {
                TokenType_Identifier, // a
                TokenType_Operator,   //   +
                TokenType_Literal,    //     12.34
                TokenType_Operator,   //           /
                TokenType_Identifier, //             c
                TokenType_Operator,   //               *
                TokenType_Identifier, //                f
                TokenType_LParam,     //                 (
                TokenType_Identifier, //                  e
                TokenType_Comma,      //                   ,
                TokenType_Literal,    //                    3
                TokenType_RParam      //                     )
        };

        EXPECT_EQ( seq.size() , aux.size() );
        for(size_t i =0; i !=seq.size();++i){
                EXPECT_EQ( seq[i], aux[i].type );
        }
}
