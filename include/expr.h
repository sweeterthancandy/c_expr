#ifndef EXPR_H
#define EXPR_H

typedef enum Exprkind{
        ExprKind_Value,
        ExprKind_Indentifier,
        ExprKind_Call,
        ExprKind_BinaryOperator
}kind;

typedef enum Operator{
        Operator_Add,
        Operator_Sub,
        Operator_Div,
        Operator_Mul
}Operator;
        
typedef struct Node{
        int kind;
        double value;
        char name[128];
        Operator op;
        struct Node* args[32];
        int arity;
}Node;

typedef struct InternalFunctionMapping{
        const char* name;
        double(*fun)();
}InternalFunctionMapping;

double Node_Eval(Node* node);
void Node_Dump(Node* node);
void Node_DumpPretty(Node* node);
void Node_InitValue(Node* ptr, double value);
void Node_InitIdentifer(Node* ptr, const char* name);
void Node_InitCall(Node* ptr, const char* name, Node** args);
void Node_InitOperator(Node* ptr, Operator op, Node* left, Node* right);

#endif /* EXPR_H */
