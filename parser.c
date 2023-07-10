#include <stdio.h>
#include <stdbool.h>
#include "lexer.c"

typedef struct ast{
    tkn token;
    struct ast *root;
    struct ast *child;
    struct ast *sibling;
    struct ast *past_sibling;
    bool visited;
} ast;

typedef enum non_terminal{
    PROGRAM_SYMBOL,
    FUNCTION_SYMBOL, 
    STATEMENT_SYMBOL,
    EXPRESSION_SYMBOL,
    TERM_SYMBOL,
    FACTOR_SYMBOL
} non_terminal;

typedef struct parse_return{
    bool valid;
    tkn_list *token_list;
    ast* root;
} parse_return;

ast* initialise_child(ast *root, tkn token){
    root->child = (ast *)malloc(sizeof(ast));
    root->child->root = root;
    root->child->child = NULL;
    root->child->sibling = NULL;
    root->child->visited = false;
    root->child->past_sibling = NULL;
    root->child->token = token;
    return root->child;
}

ast* initialise_sibling(ast *node, tkn token){
    node->sibling = (ast *)malloc(sizeof(ast));
    node->sibling->root = node->root;
    node->sibling->child = NULL;
    node->sibling->sibling = NULL;
    node->sibling->visited = false;
    node->sibling->past_sibling = node;
    node->sibling->token = token;
    return node->sibling;
}

parse_return parse(tkn_list *token_list, non_terminal symbol, ast *root){
    ast *node;
    ast* test_root;
    node = (ast *)malloc(sizeof(ast));
    test_root = (ast *)malloc(sizeof(ast));
    tkn function;
    function.type = FUNCTION;
    tkn expression;
    expression.type = EXPRESSION;
    tkn statement;
    statement.type = STATEMENT;
    tkn factor;
    factor.type = FACTOR;
    tkn term;
    term.type = TERM;
    parse_return return_value;
    parse_return expression_return;
    parse_return term_return;
    parse_return factor_return;
    switch (symbol)
    {
        case PROGRAM_SYMBOL:
            node = initialise_child(root, function);
            return_value = parse(token_list, FUNCTION_SYMBOL, node);
            if (return_value.valid){
                return_value.token_list = return_value.token_list->pointer;
                return_value.valid = true;
                return_value.root = root;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            printf("%s", "invalid token order");
            return return_value;
            break;

        case FUNCTION_SYMBOL:
            if (token_list->token.type == INT_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == IDENTIFIER){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    if (token_list->token.type == OPEN_PARENTHESES){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        if (token_list->token.type == CLOSED_PARENTHESES){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            if (token_list->token.type == OPEN_BRACE){
                                node = initialise_sibling(node, token_list->token);
                                token_list = token_list->pointer;
                                node = initialise_sibling(node, statement);
                                return_value = parse(token_list, STATEMENT_SYMBOL, node);
                                if (return_value.valid){
                                    token_list = return_value.token_list;
                                    if (token_list->token.type == CLOSED_BRACE){
                                        initialise_sibling(node, token_list->token);
                                        token_list = token_list->pointer;
                                        return_value.token_list = token_list;
                                        return_value.valid = true;
                                        return return_value;
                                        break;
                                    }
                                }
                            }
                        }
                    }   
                }
            }   
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
        
        case STATEMENT_SYMBOL:
            if (token_list->token.type == RETURN_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, expression);
                expression_return = parse(token_list, EXPRESSION_SYMBOL, node);
                if (expression_return.valid){
                    token_list = expression_return.token_list;
                    if (token_list->token.type == SEMICOLON){
                        initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        return_value.token_list = token_list;
                        return_value.valid = true;
                        return return_value;
                        break;
                    }
                }
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
                
        case EXPRESSION_SYMBOL:
            node = initialise_child(root, term);
            term_return = parse(token_list, TERM_SYMBOL, node);
            if (term_return.valid){
                token_list = term_return.token_list;
                if (token_list->token.type == ADDITION | token_list->token.type == SUBTRACTION){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, term);
                    term_return = parse(token_list, TERM_SYMBOL, node);
                    if (term_return.valid){
                        token_list = term_return.token_list;  
                        return_value.token_list = token_list;
                        return_value.valid = true;
                        return return_value;
                        break;
                    }
                    return_value.token_list = token_list;
                    return_value.valid = false;
                    return return_value;
                    break;
                }
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
        
        case TERM_SYMBOL:
            node = initialise_child(root, factor);
            factor_return = parse(token_list, FACTOR_SYMBOL, node);
            if (factor_return.valid){
                token_list = factor_return.token_list;
                if (token_list->token.type == MULTIPLICATION | token_list->token.type == DIVISION){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, factor);
                    factor_return = parse(token_list, FACTOR_SYMBOL, node);
                    if (factor_return.valid){
                        token_list = factor_return.token_list;
                        return_value.token_list = token_list;  
                        return_value.valid = true;
                        return return_value;
                        break;
                    }
                    return_value.token_list = token_list;
                    return_value.valid = false;
                    return return_value;
                    break;
                }
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
        
        case FACTOR_SYMBOL:
            if (token_list->token.type == OPEN_PARENTHESES){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, expression);
                expression_return = parse(token_list, EXPRESSION_SYMBOL, node);
                if (expression_return.valid){
                    token_list = expression_return.token_list;
                    if (token_list->token.type == CLOSED_PARENTHESES){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        return_value.token_list = token_list;
                        return_value.valid = true;
                        return return_value;
                        break;
                    }
                }
            }
            if (token_list->token.type == NEGATION | token_list->token.type == LOGICAL_NEGATION | token_list->token.type == BITWISE_COMPLEMENT){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, factor);
                return parse(token_list, FACTOR_SYMBOL, node);
            }
            if (token_list->token.type == INT_LITERAL){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;

        default:
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
    }
}
