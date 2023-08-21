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
FILE *file;

char number_string[10000];
typedef enum non_terminal{
    PROGRAM_SYMBOL,
    FUNCTION_SYMBOL, 
    STATEMENT_SYMBOL,
    DECLARATION_SYMBOL,
    BLOCK_ITEM_SYMBOL,
    EXPRESSION_SYMBOL,
    EXPRESSION_OPTION_SYMBOL,
    CONDITIONAL_EXPRESSION_SYMBOL,
    LOGICAL_OR_EXPRESSION_SYMBOL,
    LOGICAL_AND_EXPRESSION_SYMBOL,
    EQUALITY_EXPRESSION_SYMBOL,
    RELATIONAL_EXPRESSION_SYMBOL,
    ADDITIVE_EXPRESSION_SYMBOL,
    TERM_SYMBOL,
    FACTOR_SYMBOL,
    FUNCTION_CALL_SYMBOL
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

str declared_variables[10000];

int local_variable_byte_count = 0;
int else_count = 0;
int for_count = 0;
int while_count = 0;
int do_while_count = 0;
int local_variable_count = 0;
int and_total_count = 0;
int or_total_count = 0;

bool is_name_equal(str name1, str name2){
    bool equal = true;
    if (name1.character != name2.character){
        return false;
    }
    while (name1.pointer != NULL & name2.pointer != NULL & equal){
        if (name1.character==name2.character){
            name1 = *name1.pointer;
            name2 = *name2.pointer;
        }
        else{
            equal = false;
        }
    }
    if (equal){
        if (name1.character == name2.character){
            if (name1.pointer != NULL & name2.pointer == NULL){
                equal = false;
            }
            
            if (name1.pointer == NULL & name2.pointer != NULL){
                equal = false;
            }
        }
        else{
            return false;
        }
    }
    return equal;
}

bool is_declared(str name){
    for (int i = 0; i<local_variable_count;i++){
        if (is_name_equal(name, declared_variables[i])){
            return true;
        }
    }
    return false;
}


parse_return parse(tkn_list *token_list, non_terminal symbol, ast *root){
    ast *node;
    node = (ast *)malloc(sizeof(ast));
    tkn function;
    function.type = FUNCTION;
    tkn expression;
    expression.type = EXPRESSION;
    tkn expression_option;
    expression_option.type = EXPRESSION_OPTION;
    tkn declaration;
    declaration.type = DECLARATION;
    tkn block_item;
    block_item.type = BLOCK_ITEM;
    tkn logical_or_expression;
    logical_or_expression.type = LOGICAL_OR_EXPRESSION;
    tkn conditional_expression;
    conditional_expression.type = CONDITIONAL_EXPRESSION;
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
    tkn function_call;
    function_call.type = FUNCTION_CALL;
    tkn term;
    term.type = TERM;
    parse_return return_value;
    switch (symbol)
    {
        case PROGRAM_SYMBOL:
            file = fopen("assembly.s", "w");
            if (token_list!=NULL && token_list->pointer == NULL && token_list->token.type!=SEMICOLON){
                
                fputs("Invalid token order [Ln ", file);
                sprintf(number_string, "%d",   token_list->token.line_index);
                fputs(number_string, file);
                fputs(", Col ", file);
                sprintf(number_string, "%d",   token_list->token.character_index);
                fputs(number_string, file);
                fputs("]\n", file);
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            if (token_list->pointer != NULL && token_list->pointer->pointer != NULL && token_list->pointer->pointer->token.type == OPEN_PARENTHESES){
                node = initialise_child(root, function);
                return_value = parse(token_list, FUNCTION_SYMBOL, node);
                if (!return_value.valid){
                
                    fputs("Invalid token order [Ln ", file);
                    sprintf(number_string, "%d",   return_value.token_list->token.line_index);
                fputs(number_string, file);
                    fputs(", Col ", file);
                    sprintf(number_string, "%d",   return_value.token_list->token.character_index);
                fputs(number_string, file);
                    fputs("]\n", file);
                    return_value.token_list = token_list;
                    return_value.valid = false;
                    return return_value;
                    break;
                }
                token_list = return_value.token_list;
            }
            else if (token_list->pointer != NULL){
                node = initialise_child(root, declaration);
                return_value = parse(token_list, DECLARATION_SYMBOL, node);
                if (!return_value.valid){
                
                    fputs("Invalid token order [Ln ", file);
                    sprintf(number_string, "%d",   return_value.token_list->token.line_index);
                fputs(number_string, file);
                    fputs(", Col ", file);
                    sprintf(number_string, "%d",   return_value.token_list->token.character_index);
                fputs(number_string, file);
                    fputs("]\n", file);
                    return_value.token_list = token_list;
                    return_value.valid = false;
                    return return_value;
                    break;
                }
                token_list = return_value.token_list;
            }
            while (token_list->pointer != NULL){ 
                if (token_list->pointer->pointer != NULL && token_list->pointer->pointer->token.type == OPEN_PARENTHESES){
                    node = initialise_sibling(node, function);
                    return_value = parse(token_list, FUNCTION_SYMBOL, node);
                    if (!return_value.valid){
                
                        fputs("Invalid token order [Ln ", file);
                        sprintf(number_string, "%d",   return_value.token_list->token.line_index);
                fputs(number_string, file);
                        fputs(", Col ", file);
                        sprintf(number_string, "%d",   return_value.token_list->token.character_index);
                fputs(number_string, file);
                        fputs("]\n", file);
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
                    token_list = return_value.token_list;
                }
                else{
                    node = initialise_sibling(node, declaration);
                    return_value = parse(token_list, DECLARATION_SYMBOL, node);
                    if (!return_value.valid){
                
                        fputs("Invalid token order [Ln ", file);
                        sprintf(number_string, "%d",   return_value.token_list->token.line_index);
                fputs(number_string, file);
                        fputs(", Col ", file);
                        sprintf(number_string, "%d",   return_value.token_list->token.character_index);
                fputs(number_string, file);
                        fputs("]\n", file);
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
                    token_list = return_value.token_list;
                }
            }
            return_value.token_list = return_value.token_list;
            return_value.valid = true;
            return_value.root = root;
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
                        if (token_list->token.type == INT_KEYWORD){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            if (token_list->token.type == IDENTIFIER){
                                node = initialise_sibling(node, token_list->token);
                                if (!is_declared(token_list->token.name)){
                                    local_variable_byte_count += 8;
                                    declared_variables[local_variable_count] = token_list->token.name;
                                    local_variable_count += 1;
                                }
                                token_list = token_list->pointer;
                                while (token_list->token.type == COMMA){
                                    node = initialise_sibling(node, token_list->token);
                                    token_list = token_list->pointer;
                                    if (token_list->token.type == INT_KEYWORD){
                                        node = initialise_sibling(node, token_list->token);
                                        token_list = token_list->pointer;
                                        if (token_list->token.type == IDENTIFIER){
                                            node = initialise_sibling(node, token_list->token);
                                            if (!is_declared(token_list->token.name)){
                                                local_variable_byte_count += 8;
                                                declared_variables[local_variable_count] = token_list->token.name;
                                                local_variable_count += 1;
                                            }
                                            token_list = token_list->pointer;
                                        }
                                        else{
                                            token_list = token_list;
                                            return_value.token_list = token_list;
                                            return_value.valid = false;
                                            return return_value;
                                            break;
                                        }
                                    }
                                    else{
                                        token_list = token_list;
                                        return_value.token_list = token_list;
                                        return_value.valid = false;
                                        return return_value;
                                        break;
                                    }
                                }
                            }
                            else{
                                token_list = token_list;
                                return_value.token_list = token_list;
                                return_value.valid = false;
                                return return_value;
                                break;
                            }
                        }
                        if (token_list->token.type == CLOSED_PARENTHESES){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            if (token_list->token.type == OPEN_BRACE){
                                node = initialise_sibling(node, token_list->token);
                                token_list = token_list->pointer;
                                 while (token_list->token.type != CLOSED_BRACE) {
                                    node = initialise_sibling(node, block_item);
                                    return_value = parse(token_list, BLOCK_ITEM_SYMBOL, node);
                                    if (return_value.valid){
                                        token_list = return_value.token_list;
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
                            else if (token_list->token.type == SEMICOLON){
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
        
        case BLOCK_ITEM_SYMBOL:
            if (token_list->token.type == INT_KEYWORD){
                node = initialise_child(root, declaration);
                return_value = parse(token_list, DECLARATION_SYMBOL, node);
                if (return_value.valid){
                    return_value.token_list = return_value.token_list;
                    return_value.valid = true;
                    return return_value;
                    break;
                }
            }
            else{
                node = initialise_child(root, statement);
                return_value = parse(token_list, STATEMENT_SYMBOL, node);
                if (return_value.valid){
                    return_value.token_list = return_value.token_list;
                    return_value.valid = true;
                    return return_value;
                    break;
                }
            }
            return_value.token_list = return_value.token_list;
            return_value.valid = false;
            return return_value;
            break;


        case DECLARATION_SYMBOL:
            if (token_list->token.type == INT_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == IDENTIFIER){
                    if (!is_declared(token_list->token.name)){
                        local_variable_byte_count += 8;
                        declared_variables[local_variable_count] = token_list->token.name;
                        local_variable_count += 1;
                    }
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    if (token_list->token.type == ASSIGNMENT){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        if (token_list->pointer->token.type != SEMICOLON && root->root->token.type == PROGRAM){
                            str variable_name = node->past_sibling->token.name;
                            
                            fputs("Global variable \'", file);
                            while (variable_name.pointer != NULL){
                                fputc(variable_name.character, file);
                                variable_name = *variable_name.pointer;
                            }
                            fputc(variable_name.character, file);
                            fputs("\' initialised with expression, must be integer only", file);
                            fputs(" [Ln ", file);
                            sprintf(number_string, "%d",   root->token.line_index);
                fputs(number_string, file);
                            fputs(", Col ", file);
                            sprintf(number_string, "%d",   root->token.character_index);
                fputs(number_string, file);
                            fputs("]\n", file);
                            fclose(file);
                            return_value.token_list = token_list;
                            return_value.valid = false;
                            return return_value;
                            break;
                        }
                        node = initialise_sibling(node, expression);
                        return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                        if (return_value.valid){
                            token_list = return_value.token_list;
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
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;

        case STATEMENT_SYMBOL:
            if (token_list->token.type == RETURN_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, expression);
                return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
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
            else if (token_list->token.type == IF_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == OPEN_PARENTHESES){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, expression);
                    return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                        if (token_list->token.type == CLOSED_PARENTHESES){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            node = initialise_sibling(node, statement);
                            ast *temp_node = node;
                
                            if (token_list->token.type != OPEN_BRACE){
                                temp_node = node;
                                tkn new_token;
                                str new_name;
                                new_name.character = '{';
                                new_name.pointer = NULL;
                                new_token.name = new_name;
                                new_token.type = OPEN_BRACE;
                                node = initialise_child(node, new_token);
                                node = initialise_sibling(node, block_item);
                                new_name.character = '}';
                                new_token.name = new_name;
                                new_token.type = CLOSED_BRACE;
                                initialise_sibling(node, new_token);
                                node = initialise_child(node, statement);
                            }

                            return_value = parse(token_list, STATEMENT_SYMBOL, node);
                            if (return_value.valid){
                                token_list = return_value.token_list;
                                if (token_list->token.type == ELSE_KEYWORD){
                                    node = temp_node;
                                    node = initialise_sibling(node, token_list->token);
                                    token_list = token_list->pointer;

                                    node = initialise_sibling(node, statement);
                                    if (token_list->token.type != OPEN_BRACE){
                                        tkn new_token;
                                        str new_name;
                                        new_name.character = '{';
                                        new_name.pointer = NULL;
                                        new_token.name = new_name;
                                        new_token.type = OPEN_BRACE;
                                        node = initialise_child(node, new_token);
                                        node = initialise_sibling(node, block_item);
                                        new_name.character = '}';
                                        new_token.name = new_name;
                                        new_token.type = CLOSED_BRACE;
                                        initialise_sibling(node, new_token);
                                        node = initialise_child(node, statement);
                                    }

                                    return_value = parse(token_list, STATEMENT_SYMBOL, node);
                                    if (return_value.valid){
                                        return_value.token_list = return_value.token_list;
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
                        }
                    }
                }
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            else if (token_list->token.type == OPEN_BRACE){
                if (token_list->pointer->token.type != CLOSED_BRACE){
                    node = initialise_child(root, token_list->token);
                    token_list = token_list->pointer;
                    while (token_list->token.type != CLOSED_BRACE){
                        node = initialise_sibling(node, block_item);
                        return_value = parse(token_list, BLOCK_ITEM_SYMBOL, node);
                        if (return_value.valid){
                            token_list = return_value.token_list;
                        }
                        else{
                            return_value.token_list = token_list;
                            return_value.valid = false;
                            return return_value;
                            break;
                        }
                    }
                    initialise_sibling(node, token_list->token);
                }
                else{
                    token_list = token_list->pointer;
                }
                token_list = token_list->pointer;
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            else if (token_list->token.type == FOR_KEYWORD && token_list->pointer != NULL && token_list->pointer->pointer->token.type != INT_KEYWORD){
                tkn new_token;
                str new_name;
                new_name.character = '{';
                new_name.pointer = NULL;
                new_token.name = new_name;
                new_token.type = OPEN_BRACE;
                node = initialise_child(root, new_token);
                node = initialise_sibling(node, block_item);
                new_name.character = '}';
                new_token.name = new_name;
                new_token.type = CLOSED_BRACE;
                initialise_sibling(node, new_token);
                node = initialise_child(node, statement);
                node = initialise_child(node, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == OPEN_PARENTHESES){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, expression_option);
                    return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                        if (token_list->token.type == SEMICOLON){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            node = initialise_sibling(node, expression_option);
                            return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                            if (return_value.valid){
                                token_list = return_value.token_list;
                                if (token_list->token.type == SEMICOLON){
                                    node = initialise_sibling(node, token_list->token);
                                    token_list = token_list->pointer;
                                    node = initialise_sibling(node, expression_option);
                                    return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                                    if (return_value.valid){
                                        token_list = return_value.token_list;
                                        if (token_list->token.type == CLOSED_PARENTHESES){
                                            node = initialise_sibling(node, token_list->token);
                                            token_list = token_list->pointer;
                                            node = initialise_sibling(node, statement);
                                            if (token_list->token.type != OPEN_BRACE){
                                                tkn new_token;
                                                str new_name;
                                                new_name.character = '{';
                                                new_name.pointer = NULL;
                                                new_token.name = new_name;
                                                new_token.type = OPEN_BRACE;
                                                node = initialise_child(node, new_token);
                                                node = initialise_sibling(node, block_item);
                                                new_name.character = '}';
                                                new_token.name = new_name;
                                                new_token.type = CLOSED_BRACE;
                                                initialise_sibling(node, new_token);
                                                node = initialise_child(node, statement);
                                            }
                                            return_value = parse(token_list, STATEMENT_SYMBOL, node);
                                            if (return_value.valid){
                                                token_list = return_value.token_list;
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
                }
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            else if (token_list->token.type == FOR_KEYWORD && token_list->pointer != NULL && token_list->pointer->pointer->token.type == INT_KEYWORD){
                tkn new_token;
                str new_name;
                new_name.character = '{';
                new_name.pointer = NULL;
                new_token.name = new_name;
                new_token.type = OPEN_BRACE;
                node = initialise_child(root, new_token);
                node = initialise_sibling(node, block_item);
                new_name.character = '}';
                new_token.name = new_name;
                new_token.type = CLOSED_BRACE;
                initialise_sibling(node, new_token);
                node = initialise_child(node, statement);
                node = initialise_child(node, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == OPEN_PARENTHESES){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, declaration);
                    return_value = parse(token_list, DECLARATION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                        node = initialise_sibling(node, expression_option);
                        return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                        if (return_value.valid){
                            token_list = return_value.token_list;
                            if (token_list->token.type == SEMICOLON){
                                node = initialise_sibling(node, token_list->token);
                                token_list = token_list->pointer;
                                node = initialise_sibling(node, expression_option);
                                return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                                if (return_value.valid){
                                    token_list = return_value.token_list;
                                    if (token_list->token.type == CLOSED_PARENTHESES){
                                        node = initialise_sibling(node, token_list->token);
                                        token_list = token_list->pointer;
                                        node = initialise_sibling(node, statement);
                                        if (token_list->token.type != OPEN_BRACE){
                                            tkn new_token;
                                            str new_name;
                                            new_name.character = '{';
                                            new_name.pointer = NULL;
                                            new_token.name = new_name;
                                            new_token.type = OPEN_BRACE;
                                            node = initialise_child(node, new_token);
                                            node = initialise_sibling(node, block_item);
                                            new_name.character = '}';
                                            new_token.name = new_name;
                                            new_token.type = CLOSED_BRACE;
                                            initialise_sibling(node, new_token);
                                            node = initialise_child(node, statement);
                                        }
                                        return_value = parse(token_list, STATEMENT_SYMBOL, node);
                                        if (return_value.valid){
                                            token_list = return_value.token_list;
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
            }
            else if (token_list->token.type == WHILE_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == OPEN_PARENTHESES){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, expression);
                    return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                        if (token_list->token.type == CLOSED_PARENTHESES){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            node = initialise_sibling(node, statement);
                            return_value = parse(token_list, STATEMENT_SYMBOL, node);
                            if (return_value.valid){
                                token_list = return_value.token_list;
                                return_value.token_list = token_list;
                                return_value.valid = true;
                                return return_value;
                                break;
                            }
                        }
                    }
                }
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            else if (token_list->token.type == DO_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                node = initialise_sibling(node, statement);
                return_value = parse(token_list, STATEMENT_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
                    if (token_list->token.type == WHILE_KEYWORD){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        if (token_list->token.type == OPEN_PARENTHESES){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            node = initialise_sibling(node, expression);
                            return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                            if (return_value.valid){
                                token_list = return_value.token_list;
                                if (token_list->token.type == CLOSED_PARENTHESES){
                                    node = initialise_sibling(node, token_list->token);
                                    token_list = token_list->pointer;
                                    if (token_list->token.type == SEMICOLON){
                                        node = initialise_sibling(node, token_list->token);
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
                return_value.token_list = token_list;
                return_value.valid = false;
                return return_value;
                break;
            }
            else if (token_list->token.type == BREAK_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == SEMICOLON){
                    node = initialise_sibling(node, token_list->token);
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
            }
            else if (token_list->token.type == CONTINUE_KEYWORD){
                node = initialise_child(root, token_list->token);
                token_list = token_list->pointer;
                if (token_list->token.type == SEMICOLON){
                    node = initialise_sibling(node, token_list->token);
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
            }
            else{
                node = initialise_child(root, expression_option);
                return_value = parse(token_list, EXPRESSION_OPTION_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
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
        
        case EXPRESSION_OPTION_SYMBOL:
            if (token_list->token.type == SEMICOLON && root->past_sibling!=NULL && root->past_sibling->token.type == SEMICOLON && root->root->child->token.type == FOR_KEYWORD){
                tkn one_token;
                str one_str;
                one_str.character = '1';
                one_str.pointer = NULL;
                one_token.name = one_str;
                one_token.type = INT_LITERAL;
                tkn semicolon_token;
                str semicolon_str;
                semicolon_str.character = ';';
                semicolon_str.pointer = NULL;
                semicolon_token.name = semicolon_str;
                semicolon_token.type = SEMICOLON;
                tkn_list* new_node = (tkn_list*)malloc(sizeof(tkn_list));
                tkn_list* new_node_2 = (tkn_list*)malloc(sizeof(tkn_list));
                new_node->token = one_token;
                new_node->pointer = new_node_2;
                new_node_2->token = semicolon_token;
                new_node_2->pointer = token_list->pointer;
                token_list->pointer = new_node;
                token_list = token_list->pointer;
                node = initialise_child(root, expression);
                return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
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
            else if (token_list->token.type == SEMICOLON){
                tkn null_token;
                null_token.type = NULL_KEYWORD;
                initialise_child(root, null_token);
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            else if (token_list->token.type == CLOSED_PARENTHESES && root->past_sibling->token.type == SEMICOLON && root->root->child->token.type == FOR_KEYWORD){
                tkn null_token;
                null_token.type = NULL_KEYWORD;
                initialise_child(root, null_token);
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            else{
                node = initialise_child(root, expression);
                return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
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

        case EXPRESSION_SYMBOL:
            if (token_list->pointer != NULL && token_list->pointer->token.type == ASSIGNMENT){
                if (token_list->token.type == IDENTIFIER){
                    node = initialise_child(root, token_list->token);
                    token_list = token_list->pointer;
                    if (token_list->token.type == ASSIGNMENT){
                        node = initialise_sibling(node, token_list->token);
                        token_list = token_list->pointer;
                        node = initialise_sibling(node, expression);
                        return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                        if (return_value.valid){
                            token_list = return_value.token_list;
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
            node = initialise_child(root, conditional_expression);
            return_value = parse(token_list, CONDITIONAL_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                return_value.token_list = token_list;
                return_value.valid = true;
                return return_value;
                break;
            }
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;      
        
        case CONDITIONAL_EXPRESSION_SYMBOL:
            node = initialise_child(root, logical_or_expression);
            return_value = parse(token_list, LOGICAL_OR_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                if (token_list->token.type == QUESTION_MARK){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, expression);
                    return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                        if (token_list->token.type == COLON){
                            node = initialise_sibling(node, token_list->token);
                            token_list = token_list->pointer;
                            node = initialise_sibling(node, conditional_expression);
                            return_value = parse(token_list, CONDITIONAL_EXPRESSION_SYMBOL, node);
                            if (return_value.valid){
                                token_list = return_value.token_list;
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
            return_value = parse(token_list, LOGICAL_AND_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == OR) {
                    or_total_count++;
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, logical_and_expression);
                    return_value = parse(token_list, LOGICAL_AND_EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
            return_value = parse(token_list, EQUALITY_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == AND) {
                    and_total_count++;
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, equality_expression);
                    return_value = parse(token_list, EQUALITY_EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
            return_value = parse(token_list, RELATIONAL_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == NOT_EQUAL | token_list->token.type == EQUAL) {
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, relational_expression);
                    return_value = parse(token_list, RELATIONAL_EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
            return_value = parse(token_list, ADDITIVE_EXPRESSION_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == LESS_THAN | token_list->token.type == LESS_THAN_OR_EQUAL | token_list->token.type == GREATER_THAN | token_list->token.type == GREATER_THAN_OR_EQUAL) {
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, additive_expression);
                    return_value = parse(token_list, ADDITIVE_EXPRESSION_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
            return_value = parse(token_list, TERM_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == ADDITION | token_list->token.type == SUBTRACTION) {
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, term);
                    return_value = parse(token_list, TERM_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
            return_value = parse(token_list, FACTOR_SYMBOL, node);
            if (return_value.valid){
                token_list = return_value.token_list;
                while (token_list->token.type == MULTIPLICATION | token_list->token.type == DIVISION) {
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, factor);
                    return_value = parse(token_list, FACTOR_SYMBOL, node);
                    if (return_value.valid){
                        token_list = return_value.token_list;
                    }
                    else{
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
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
                return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                if (return_value.valid){
                    token_list = return_value.token_list;
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
                if (token_list->pointer->token.type == OPEN_PARENTHESES){
                    node = initialise_child(root, function_call);
                    return parse(token_list, FUNCTION_CALL_SYMBOL, node);
                    break;
                }
                else{
                    node = initialise_child(root, token_list->token);
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
        
        case FUNCTION_CALL_SYMBOL:
            node = initialise_child(root, token_list->token);
            token_list = token_list->pointer;
            node = initialise_sibling(node, token_list->token);
            token_list = token_list->pointer;
            if (token_list->token.type != CLOSED_PARENTHESES){
                node = initialise_sibling(node, expression);
                return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                if (!return_value.valid){
                    return_value.token_list = token_list;
                    return_value.valid = false;
                    return return_value;
                    break;
                }
                token_list = return_value.token_list;
                while (token_list->token.type == COMMA){
                    node = initialise_sibling(node, token_list->token);
                    token_list = token_list->pointer;
                    node = initialise_sibling(node, expression);
                    return_value = parse(token_list, EXPRESSION_SYMBOL, node);
                    if (!return_value.valid){
                        return_value.token_list = token_list;
                        return_value.valid = false;
                        return return_value;
                        break;
                    }
                    token_list = return_value.token_list;
                }
            }
            node = initialise_sibling(node, token_list->token);
            token_list = token_list->pointer;
            return_value.token_list = token_list;
            return_value.valid = true;
            return return_value;
            break;

        default:
            return_value.token_list = token_list;
            return_value.valid = false;
            return return_value;
            break;
    }
}
