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
    LOGICAL_OR_EXPRESSION_SYMBOL,
    LOGICAL_AND_EXPRESSION_SYMBOL,
    EQUALITY_EXPRESSION_SYMBOL,
    RELATIONAL_EXPRESSION_SYMBOL,
    ADDITIVE_EXPRESSION_SYMBOL,
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

int local_variable_byte_count = 0;
int local_variable_count = 0;

parse_return parse(tkn_list *token_list, non_terminal symbol, ast *root){
    ast *node;
    node = (ast *)malloc(sizeof(ast));
    tkn function;
    function.type = FUNCTION;
    tkn expression;
    expression.type = EXPRESSION;
    tkn logical_or_expression;
    logical_or_expression.type = LOGICAL_OR_EXPRESSION;
    tkn logical_and_expression;
    logical_and_expression.type = LOGICAL_AND_EXPRESSION;
    tkn equality_expression;
    equality_expression.type = EQUALITY_EXPRESSION;
    tkn relational_expression;
    relational_expression.type = RELATIONAL_EXPRESSION;
    tkn additive_expression;
    additive_expression.type = ADDITIVE_EXPRESSION;
    tkn statement;
    statement.type = STATEMENT;
    tkn factor;
    factor.type = FACTOR;
    tkn term;
    term.type = TERM;
    parse_return return_value;
    parse_return statement_return;
    parse_return expression_return;
    parse_return logical_or_expression_return;
    parse_return logical_and_expression_return;
    parse_return equality_expression_return;
    parse_return relational_expression_return;
    parse_return additive_expression_return;
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
                                 while (token_list->token.type != CLOSED_BRACE) {
                                    node = initialise_sibling(node, statement);
                                    statement_return = parse(token_list, STATEMENT_SYMBOL, node);
                                    if (statement_return.valid){
                                        token_list = statement_return.token_list;
                                    }
                                    else{
                                        token_list = token_list;
                                        return_value.token_list = token_list;
                                        return_value.valid = false;
                                        return return_value;
                                        break;
                                    }
                                }
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
            else if (token_list->token.type == INT_KEYWORD){
                local_variable_byte_count += 8;
                local_variable_count += 1;
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == IDENTIFIER){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    if (token_list->token.type == ASSIGNMENT){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        node = initialise_sibling(node, expression);
                        expression_return = parse(token_list, EXPRESSION_SYMBOL, node);
                        if (expression_return.valid){
                            token_list = expression_return.token_list;
                        }
                        else{
                            return_value.token_list = token_list;
                            return_value.valid = false;
                            return return_value;
                            break;
                        }
                    }
                    if (token_list->token.type == SEMICOLON){
                        initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        return_value.token_list = token_list;
                        return_value.valid = true;
                        return return_value;
                        break;
                    }
                }
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            else{
                node = initialise_child(root, expression);
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
        
            if (token_list->pointer != NULL && token_list->pointer->token.type == ASSIGNMENT){
                if (token_list->token.type == IDENTIFIER){
                    node = initialise_child(root, token_list->token);
                    token_list = token_list->pointer;
                    if (token_list->token.type == ASSIGNMENT){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        node = initialise_sibling(node, expression);
                        expression_return = parse(token_list, EXPRESSION_SYMBOL, node);
                        if (expression_return.valid){
                            token_list = expression_return.token_list;
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
            }
            node = initialise_child(root, logical_or_expression);
            logical_or_expression_return = parse(token_list, LOGICAL_OR_EXPRESSION_SYMBOL, node);
            if (logical_or_expression_return.valid){
                token_list = logical_or_expression_return.token_list;
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;        

        case LOGICAL_OR_EXPRESSION_SYMBOL:
            node = initialise_child(root, logical_and_expression);
            logical_and_expression_return = parse(token_list, LOGICAL_AND_EXPRESSION_SYMBOL, node);
            if (logical_and_expression_return.valid){
                token_list = logical_and_expression_return.token_list;
                if (token_list->token.type == OR){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, logical_and_expression);
                    logical_and_expression_return = parse(token_list, LOGICAL_AND_EXPRESSION_SYMBOL, node);
                    if (logical_and_expression_return.valid){
                        token_list = logical_and_expression_return.token_list;  
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

        case LOGICAL_AND_EXPRESSION_SYMBOL:
            node = initialise_child(root, equality_expression);
            equality_expression_return = parse(token_list, EQUALITY_EXPRESSION_SYMBOL, node);
            if (equality_expression_return.valid){
                token_list = equality_expression_return.token_list;
                if (token_list->token.type == AND){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, equality_expression);
                    equality_expression_return = parse(token_list, EQUALITY_EXPRESSION_SYMBOL, node);
                    if (equality_expression_return.valid){
                        token_list = equality_expression_return.token_list;  
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

        case EQUALITY_EXPRESSION_SYMBOL:
            node = initialise_child(root, relational_expression);
            relational_expression_return = parse(token_list, RELATIONAL_EXPRESSION_SYMBOL, node);
            if (relational_expression_return.valid){
                token_list = relational_expression_return.token_list;
                if (token_list->token.type == EQUAL | token_list->token.type == NOT_EQUAL){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, relational_expression);
                    relational_expression_return = parse(token_list, RELATIONAL_EXPRESSION_SYMBOL, node);
                    if (relational_expression_return.valid){
                        token_list = relational_expression_return.token_list;  
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

        case RELATIONAL_EXPRESSION_SYMBOL:
            node = initialise_child(root, additive_expression);
            additive_expression_return = parse(token_list, ADDITIVE_EXPRESSION_SYMBOL, node);
            if (additive_expression_return.valid){
                token_list = additive_expression_return.token_list;
                if (token_list->token.type == LESS_THAN | token_list->token.type == LESS_THAN_OR_EQUAL | token_list->token.type == GREATER_THAN | token_list->token.type == GREATER_THAN_OR_EQUAL){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, additive_expression);
                    additive_expression_return = parse(token_list, ADDITIVE_EXPRESSION_SYMBOL, node);
                    if (additive_expression_return.valid){
                        token_list = additive_expression_return.token_list;  
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
        
        case ADDITIVE_EXPRESSION_SYMBOL:
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
            else if (token_list->token.type == NEGATION | token_list->token.type == LOGICAL_NEGATION | token_list->token.type == BITWISE_COMPLEMENT){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, factor);
                return parse(token_list, FACTOR_SYMBOL, node);
            }
            else if (token_list->token.type == INT_LITERAL){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            else if (token_list->token.type == IDENTIFIER){
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
