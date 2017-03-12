#include "expr.h"
#include "runtime.h"


extern double cos(double);
extern double sin(double);

static InternalFunctionMapping internalFunctions[] = {
        {"cos", &cos},
        {"sin", &sin},
        {0,0}
};

typedef struct InternalConstantMapping{
        const char* name;
        double value;
}InternalConstantMapping;

static InternalConstantMapping internalConstantMappings[] = {
        {"pi", 3.14159265359},
        {0,0}
};


double Node_Eval(Node* node){


        if( node == 0 ){
                return -1;
        }
        switch(node->kind){
                case ExprKind_Value:
                        return node->value;
                case ExprKind_Indentifier:
                        for(InternalConstantMapping* iter = internalConstantMappings; iter->name !=0;++iter){
                                if( strcmp( iter->name, node->name ) == 0 ){
                                        return iter->value;
                                }
                        }
                        printf("bad internal mapping %s\n", node->name);
                        // TODO
                        return 1;
                case ExprKind_Call:
                        for(InternalFunctionMapping* iter = internalFunctions; iter->name != 0;++iter){
                                if( strcmp( iter->name, node->name ) == 0 ){
                                        double evalArgs[32] = {0};
                                        double* evalPtr = evalArgs;
                                        double ret;

                                        for(Node** iter = node->args; *iter != 0; ++iter){
                                                *evalPtr = Node_Eval( *iter);
                                                ++evalPtr;
                                        }
                                        switch(node->arity){
                                        case 2: ret = iter->fun(evalArgs[0], evalArgs[1]); break;
                                        case 1: ret = iter->fun(evalArgs[0]);              break;
                                        case 0: ret = iter->fun();                         break;
                                        default:
                                                // TODO not implemented
                                                printf("not implenated\n");
                                                return 0;
                                        }
                                        return ret;
                                }
                        }
                        printf("bad internal mapping %s\n", node->name);
                        // TODO
                        return 1;
                case ExprKind_BinaryOperator: {
                        double lp = Node_Eval(node->args[0]);
                        double rp = Node_Eval(node->args[1]);
                        double ret = -1;
                        switch(node->op){
                        case Operator_Add: ret = lp + rp; break;
                        case Operator_Sub: ret = lp - rp; break;
                        case Operator_Mul: ret = lp * rp; break;
                        case Operator_Div: ret = lp / rp; break;
                        }
                        return ret;
                }
                default:
                        return -1;
        }
}

void Node_Dump(Node* node){
        if( node == 0 ){
                printf("<null>");
                return;
        }
        switch(node->kind){
        case ExprKind_Value:
                printf("value(%f)", node->value);
                break;
        case ExprKind_Indentifier:
                printf("identifier(%s)", (const char*)node->name);
                break;
        case ExprKind_Call:
                printf("call(%s,", node->name );
                for(Node** iter = node->args; *iter != 0; ++iter){
                        Node_Dump(*iter);
                }
                printf(")");
                break;
        case ExprKind_BinaryOperator:
                printf("operator("); 
                switch(node->op){
                case Operator_Add: printf("+"); break;
                case Operator_Sub: printf("-"); break;
                case Operator_Div: printf("/"); break;
                case Operator_Mul: printf("*"); break;
                }
                printf(" ,");
                Node_Dump(node->args[0]);
                printf(" ,");
                Node_Dump(node->args[1]);
                printf(")");
                break;
        defualt:
                printf("<invalid>");
                break;
        }
}

void Node_DumpPretty(Node* node){
        if( node == 0 ){
                printf("<null>");
                return;
        }
        switch(node->kind){
        case ExprKind_Value:
                printf("%f", node->value);
                break;
        case ExprKind_Indentifier:
                printf("%s", node->name );
                break;
        case ExprKind_Call:
                printf("%s(", node->name );
                for(Node** iter = node->args; *iter != 0; ++iter){
                        Node_DumpPretty(*iter);
                }
                printf(")");
                break;
        case ExprKind_BinaryOperator:
                printf("(");
                Node_DumpPretty(node->args[0]);
                switch(node->op){
                case Operator_Add: printf("+"); break;
                case Operator_Sub: printf("-"); break;
                case Operator_Div: printf("/"); break;
                case Operator_Mul: printf("*"); break;
                }
                Node_DumpPretty(node->args[1]);
                printf(")");
                break;
        defualt:
                printf("<invalid>");
                break;
        }
}

void Node_InitValue(Node* ptr, double value){
        memset(ptr, 0, sizeof(*ptr));
        ptr->kind  = ExprKind_Value;
        ptr->value = value;
}
void Node_InitIdentifer(Node* ptr, const char* name){
        memset(ptr, 0, sizeof(*ptr));
        ptr->kind  = ExprKind_Indentifier;
        strcpy(ptr->name, name);
}
// args is null-terminated
void Node_InitCall(Node* ptr, const char* name, Node** args){
        memset(ptr, 0, sizeof(*ptr));
        ptr->kind  = ExprKind_Call;
        strcpy(ptr->name, name);
        for(Node** iter = args, **dest = ptr->args; *iter != 0; ++iter){
                *dest = *iter;
                ++ptr->arity;
        }
}
void Node_InitOperator(Node* ptr, Operator op, Node* left, Node* right){
        memset(ptr, 0, sizeof(*ptr));
        ptr->kind   = ExprKind_BinaryOperator;
        ptr->op     = op;
        ptr->args[0]= left;
        ptr->args[1]= right;
        ptr->arity = 2;
}
