#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "parser.c"

int and_jumper_count = 0;
int or_jumper_count = 0;
int and_jumpee_count = 0;
int or_jumpee_count = 0;
int conditional_count = 0;
char number_string[10000];

typedef struct local_variable{
    str token_name;
    int index;
    bool declared_in_block;
} local_variable;

bool function_returned = false;

int current_local_variable_count = 0;
int current_local_variable_byte_count = 0;
local_variable local_variable_maps[1000][1000];
int map_counter = 0;

void add_to_local_variable_map(str name, local_variable local_variable_map[1000]){
    local_variable variable;
    variable.token_name = name;
    variable.declared_in_block = true;
    current_local_variable_byte_count = current_local_variable_byte_count+8;
    variable.index = current_local_variable_byte_count;
    local_variable_map[current_local_variable_count] = variable;
    current_local_variable_count++;
}

void set_declared_in_local_variable_map(str name, local_variable local_variable_map[1000]){
    for (int i = 0; i < current_local_variable_count; i++){
        if (is_name_equal(name, local_variable_map[i].token_name)){
            local_variable_map[i].declared_in_block = true;
        }
    }
    
}

bool is_in_local_variable_map(str name, local_variable local_variable_map[1000]){
    for (int i = 0; i < current_local_variable_count; i++){
        if (is_name_equal(name, local_variable_map[i].token_name)){
            return true;
        }
    }
    return false;
}

bool is_declared_in_local_variable_map(str name, local_variable local_variable_map[1000]){
    for (int i = 0; i < current_local_variable_count; i++){
        if (is_name_equal(name, local_variable_map[i].token_name)){
            return local_variable_map[i].declared_in_block;
        }
    }
    return false;
}

bool is_declared_in_block(str name, local_variable local_variable_map[1000]){
    for (int i = 0; i < current_local_variable_count; i++){
        if (is_name_equal(name, local_variable_map[i].token_name)){
            return local_variable_map[i].declared_in_block;
        }
    }
    return false;
}

int get_local_variable_index(str name, local_variable local_variable_map[1000]){
    for (int i = 0; i < current_local_variable_count; i++){
        if (is_name_equal(name, local_variable_map[i].token_name)){
            return -local_variable_map[i].index;
        }
    }
}

void generate_code(ast* root, FILE *file, local_variable local_variable_map[1000]){
    switch (root->token.type){
        case INT_LITERAL:
            root->visited = true;
            fputs("    mov $", file);
            str name = root->token.name;
            while (name.pointer != NULL){
                fputc(name.character, file);
                name = *name.pointer;
            }
            fputc(name.character, file);
            fputs(", \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            root = root->root;
            if (root->past_sibling != NULL && (root->past_sibling->token.type == NEGATION | root->past_sibling->token.type == LOGICAL_NEGATION | root->past_sibling->token.type == BITWISE_COMPLEMENT)){
                generate_code(root->past_sibling, file, local_variable_map);
            }
            else{
                generate_code(root, file, local_variable_map);
            }
            break;
        
        case IDENTIFIER:
            name = root->token.name;
            if (is_in_local_variable_map(name, local_variable_map)){
                root->visited = true;
                fputs("    mov ", file);
                sprintf(number_string, "%d", get_local_variable_index(name, local_variable_map));
                fputs(number_string, file);
                fputs("(\%rbp), \%rax\n", file);
                fputs("    pushq \%rax\n", file);
                
                root = root->root;
                if (root->past_sibling != NULL && (root->past_sibling->token.type == NEGATION | root->past_sibling->token.type == LOGICAL_NEGATION | root->past_sibling->token.type == BITWISE_COMPLEMENT)){
                    generate_code(root->past_sibling, file, local_variable_map);
                }
                else{
                    generate_code(root, file, local_variable_map);
                }
            }
            else if (root->past_sibling != NULL && root->past_sibling->token.type == INT_KEYWORD){
                generate_code(root->root, file, local_variable_map);
            }
            else{
                printf("%s", "Variable not declared");
            }
            break;
        
        case NEGATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    neg \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case BITWISE_COMPLEMENT:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    not \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case LOGICAL_NEGATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    cmp $0, \%rax\n    mov $0, \%rax\n    sete \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case FACTOR:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;
        
        case MULTIPLICATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    pop \%rcx\n", file);
            fputs("    imul \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case DIVISION:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    mov \%eax, \%eax\n", file);
            fputs("    cdq\n", file);
            fputs("    idiv \%ecx\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case TERM:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;
        
        case ADDITION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    pop \%rcx\n", file);
            fputs("    add \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case SUBTRACTION:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    sub \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case ADDITIVE_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;

        
        
        case LESS_THAN:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setl \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case LESS_THAN_OR_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setle \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;

        case GREATER_THAN:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setg \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;

        case GREATER_THAN_OR_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setge \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case RELATIONAL_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;

        case NOT_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rax, \%rcx\n    mov $0, \%rax\n    setne \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rax, \%rcx\n    mov $0, \%rax\n    sete \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;

        case EQUALITY_EXPRESSION:
            if (root->visited & (root->past_sibling == NULL || root->past_sibling->visited)){
                root->visited = true;
                if (root->sibling != NULL){
                    fputs("    pop \%rax\n", file);
                    fputs("    pushq \%rax\n", file);
                    fputs("    cmp $0, \%rax\n", file);
                    fputs("    je _skip_and_", file);
                    and_jumper_count++;
                    sprintf(number_string, "%d", and_jumper_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;

        case AND:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    pushq \%rcx\n", file);
            
            fputs("_skip_and_", file);
            and_jumpee_count++;
            sprintf(number_string, "%d", and_jumpee_count);
            fputs(number_string, file);
            fputs(":\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;
        
        case LOGICAL_AND_EXPRESSION:
            if (root->visited & (root->past_sibling == NULL || root->past_sibling->visited)){
                root->visited = true;
                if (root->sibling != NULL){
                    fputs("    pop \%rax\n", file);
                    fputs("    pushq \%rax\n", file);
                    fputs("    cmp $1, \%rax\n", file);
                    fputs("    je _skip_or_", file);
                    or_jumper_count++;
                    sprintf(number_string, "%d", or_jumper_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;

        case OR:
            root->visited = true;

            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    pushq \%rcx\n", file);

            fputs("_skip_or_", file);
            or_jumpee_count++;
            sprintf(number_string, "%d", or_jumpee_count);
            fputs(number_string, file);
            fputs(":\n", file);
            if (root->sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            else{
                generate_code(root->root, file, local_variable_map);
            }
            break;

        case LOGICAL_OR_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file, local_variable_map);
            }
            break;
        
        case QUESTION_MARK:
            fputs("    pop \%rax\n", file);
            fputs("    cmp $0, \%rax\n", file);
            fputs("    je _e3_", file);
            conditional_count++;
            sprintf(number_string, "%d", conditional_count);
            fputs(number_string, file);
            fputs("\n", file);
            generate_code(root->sibling, file, local_variable_map);
            break;
        
        case CONDITIONAL_EXPRESSION:
            if (root->visited & (root->past_sibling != NULL && root->past_sibling->token.type == COLON)){
                fputs("_post_conditional_", file);
                sprintf(number_string, "%d", conditional_count);
                fputs(number_string, file);
                fputs(":\n", file);
                generate_code(root->root, file, local_variable_map);
                break;
            }
            else if (root->visited){
                generate_code(root->root, file, local_variable_map);
            }
            else{
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            break;

        case EXPRESSION_OPTION:
            root->visited = true;
            if (root->child->token.type == NULL_KEYWORD){
                if (root->root->child->token.type == FOR_KEYWORD){
                    root->child->visited = true;
                    generate_code(root->child, file, local_variable_map);
                }
                else if (root->sibling->sibling == NULL){
                    generate_code(root->root->sibling, file, local_variable_map);
                }
                else{
                    generate_code(root->sibling, file, local_variable_map);
                }
            }
            else{
                generate_code(root->child, file, local_variable_map);
            }
            break;

        case EXPRESSION:
            if (root->root->token.type == EXPRESSION_OPTION && !root->root->visited && root->root->sibling != NULL && root->root->sibling->token.type != COLON){
                root->visited = true;
                root = root->child;
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }            
            else if (!root->visited && ((root->sibling != NULL && root->sibling->token.type != COLON) || root->root->root->child->token.type == FOR_KEYWORD)){
                root->visited = true;
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            else if (root->root->root->child->token.type == FOR_KEYWORD){
                if (root->root->sibling->token.type == SEMICOLON){
                    fputs("    pop \%rax\n", file);
                    fputs("    cmp $0, \%rax\n", file);
                    fputs("    je _for_end_", file);  
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root->root, file, local_variable_map);
                }
                else if (root->root->sibling->token.type == CLOSED_PARENTHESES){
                    fputs("    jmp _for_condition_", file);  
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    fputs("    _for_break_", file);  
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    generate_code(root->root->root, file, local_variable_map);
                }
                else if (root->root->token.type == DECLARATION){
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else if (root->root->child->token.type == WHILE_KEYWORD){
                fputs("    pop \%rax\n", file);
                fputs("    cmp $0, \%rax\n", file);
                fputs("    je _while_end_", file); 
                sprintf(number_string, "%d", while_count);
                fputs(number_string, file);
                fputs("\n", file);
                generate_code(root->root, file, local_variable_map);
            }
            else if (root->root->child->token.type == DO_KEYWORD){
                fputs("    pop \%rax\n", file);
                fputs("    cmp $0, \%rax\n", file);
                fputs("    je _do_while_end_", file); 
                sprintf(number_string, "%d", do_while_count);
                fputs(number_string, file);
                fputs("\n", file);
                generate_code(root->root, file, local_variable_map);
            }
            else if (root->root->token.type == EXPRESSION_OPTION){
                if (root->root->sibling != NULL && root->root->sibling->token.type == COLON){
                    fputs("    jmp _post_conditional_", file);
                    sprintf(number_string, "%d", conditional_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    fputs("_e3_", file);
                    sprintf(number_string, "%d", conditional_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->root->sibling->sibling, file, local_variable_map);
                }
                else if (root->root->root->token.type == FACTOR | root->root->past_sibling == NULL | (root->root->past_sibling != NULL && root->root->past_sibling->past_sibling == NULL)){
                    generate_code(root->root->root, file, local_variable_map);
                }
                else{
                    name = root->root->past_sibling->past_sibling->token.name;
                    if (is_in_local_variable_map(name, local_variable_map)){
                        fputs("    pop \%rax\n", file);
                        if (root->root->past_sibling->past_sibling->past_sibling != NULL){
                            fputs("    pushq \%rax\n", file);
                        }
                        fputs("    mov \%rax, ", file);
                        sprintf(number_string, "%d", get_local_variable_index(name, local_variable_map));
                        fputs(number_string, file);
                        fputs("(\%rbp)\n", file);
                        for (int i = map_counter; i > 0; i--){
                            if (!is_declared_in_block(name, local_variable_maps[i]) && root->root->past_sibling->past_sibling->past_sibling == NULL){
                                fputs("    mov \%rax, ", file);
                                sprintf(number_string, "%d", (local_variable_byte_count*(map_counter-i+1)+get_local_variable_index(name, local_variable_maps[i])));
                                fputs(number_string, file);
                                fputs("(\%rbp)\n", file);
                            }
                            else{
                                break;
                            }
                        }
                        generate_code(root->root->root, file, local_variable_map);
                    }
                    else if (root->root->past_sibling != NULL && root->root->past_sibling->past_sibling != NULL && (root->root->past_sibling->past_sibling->token.type == IF_KEYWORD | (root->root->past_sibling->past_sibling->past_sibling != NULL && root->root->past_sibling->past_sibling->past_sibling->token.type == INT_KEYWORD))){
                        generate_code(root->root->root, file, local_variable_map);
                    }
                    else{
                        printf("%s", "Variable not declared");
                    }
                }
            }
            else{
                if (root->sibling != NULL && root->sibling->token.type == COLON){
                    fputs("    jmp _post_conditional_", file);
                    sprintf(number_string, "%d", conditional_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    fputs("_e3_", file);
                    sprintf(number_string, "%d", conditional_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->sibling->sibling, file, local_variable_map);
                }
                else if (root->root->token.type == FACTOR | root->past_sibling == NULL | (root->past_sibling != NULL && root->past_sibling->past_sibling == NULL)){
                    generate_code(root->root, file, local_variable_map);
                }
                else{
                    name = root->past_sibling->past_sibling->token.name;
                    if (is_in_local_variable_map(name, local_variable_map)){
                        fputs("    pop \%rax\n", file);
                        if (root->past_sibling->past_sibling->past_sibling != NULL){
                            fputs("    pushq \%rax\n", file);
                        }
                        fputs("    mov \%rax, ", file);
                        sprintf(number_string, "%d", get_local_variable_index(name, local_variable_map));
                        fputs(number_string, file);
                        fputs("(\%rbp)\n", file);
                        for (int i = map_counter; i > 0; i--){
                            if (!is_declared_in_block(name, local_variable_maps[i]) && root->past_sibling->past_sibling->past_sibling == NULL){
                                fputs("    mov \%rax, ", file);
                                sprintf(number_string, "%d", (local_variable_byte_count*(map_counter-i+1)+get_local_variable_index(name, local_variable_maps[i])));
                                fputs(number_string, file);
                                fputs("(\%rbp)\n", file);
                            }
                            else{
                                break;
                            }
                        }
                        generate_code(root->root, file, local_variable_map);
                    }
                    else if (root->past_sibling != NULL && root->past_sibling->past_sibling != NULL && (root->past_sibling->past_sibling->token.type == IF_KEYWORD || (root->past_sibling->past_sibling->past_sibling != NULL && root->past_sibling->past_sibling->past_sibling->token.type == INT_KEYWORD))){
                        generate_code(root->root, file, local_variable_map);
                    }
                    else{
                        printf("%s", "Variable not declared");
                    }
                }
            }
            break; 
        
        case DECLARATION:
            if (root->visited){
                if (root->child->token.type == INT_KEYWORD){
                    str variable_name = root->child->sibling->token.name;
                    if (!is_declared_in_local_variable_map(variable_name, local_variable_map)){
                        if (!is_in_local_variable_map(variable_name, local_variable_map)){
                            add_to_local_variable_map(variable_name, local_variable_map);
                        }
                        else{
                            set_declared_in_local_variable_map(variable_name, local_variable_map);
                        }
                        if (root->child->sibling->sibling->token.type == ASSIGNMENT){
                            fputs("    pop \%rax\n", file);
                            fputs("    mov \%rax, ", file);
                            sprintf(number_string, "%d", get_local_variable_index(variable_name, local_variable_map));
                            fputs(number_string, file);
                            fputs("(\%rbp)\n", file);
                        }
                        else{
                            fputs("    mov $0, \%rax\n", file);
                            fputs("    mov \%rax, ", file);
                            sprintf(number_string, "%d", get_local_variable_index(variable_name, local_variable_map));
                            fputs(number_string, file);
                            fputs("(\%rbp)\n", file);
                        }
                    }
                    else{
                        printf("%s", "Variable already declared");
                        break;
                    }
                }
                if (root->root->child->token.type == FOR_KEYWORD){
                    generate_code(root->root, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else{
                while (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            break;
        
        case STATEMENT:
            if (root->child->token.type == FOR_KEYWORD){
                if ((root->child->sibling->sibling->token.type == DECLARATION && root->child->sibling->sibling->sibling->sibling->sibling->visited)||(root->child->sibling->sibling->token.type != DECLARATION && root->child->sibling->sibling->sibling->sibling->sibling->sibling->visited)){
                    fputs("_for_end_", file);
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if ((root->child->sibling->sibling->token.type == DECLARATION && root->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->visited)||(root->child->sibling->sibling->token.type != DECLARATION && root->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->visited)){
                    fputs("jmp _for_skip_end_", file);
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs("\n", file);

                    fputs("_for_skip_", file);
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    fputs("_for_skip_end_", file);
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    if (root->child->sibling->sibling->token.type == DECLARATION){
                        generate_code(root->child->sibling->sibling->sibling->sibling->sibling, file, local_variable_map);
                    }
                    else{
                        generate_code(root->child->sibling->sibling->sibling->sibling->sibling->sibling, file, local_variable_map);
                    }
                }

                else if ((root->child->sibling->sibling->token.type == DECLARATION && root->child->sibling->sibling->sibling->visited)||(root->child->sibling->sibling->token.type != DECLARATION && root->child->sibling->sibling->sibling->sibling->visited)){
                    if (root->child->sibling->sibling->token.type == DECLARATION){
                        generate_code(root->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling, file, local_variable_map);
                    }
                    else{
                        generate_code(root->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling, file, local_variable_map);
                    }
                }
                else if (root->child->sibling->sibling->visited){
                    fputs("_for_condition_", file);
                    for_count+=10;
                    sprintf(number_string, "%d", for_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    if (root->child->sibling->sibling->token.type == DECLARATION){
                        generate_code(root->child->sibling->sibling->sibling, file, local_variable_map);
                    }
                    else{
                        generate_code(root->child->sibling->sibling->sibling->sibling, file, local_variable_map);
                    }
                }
                else{
                    generate_code(root->child->sibling->sibling, file, local_variable_map);
                }
            }
            else if (root->child->token.type == WHILE_KEYWORD){
                if (root->child->sibling->sibling->sibling->sibling->visited){
                    fputs("    jmp _while_condition_", file);
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs("\n", file);

                    fputs("    _while_skip_", file);
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    fputs("jmp _while_condition_", file);
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs("\n", file);

                    fputs("    _while_break_", file);  
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    
                    fputs("_while_end_", file);
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->child->sibling->sibling->visited){
                    generate_code(root->child->sibling->sibling->sibling->sibling, file, local_variable_map);
                }
                else{
                    fputs("_while_condition_", file);
                    while_count+=1;
                    sprintf(number_string, "%d", while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->child->sibling->sibling, file, local_variable_map);
                }   
            }
            else if (root->child->token.type == DO_KEYWORD){
                if (root->child->sibling->sibling->sibling->sibling->visited){
                    fputs("jmp _do_while_condition_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    
                    fputs("_do_while_break_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    fputs("_do_while_end_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->child->sibling->visited){
                    fputs("jmp _do_while_skip_end_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    fputs("    _do_while_skip_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    add $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    fputs("_do_while_skip_end_", file);
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->child->sibling->sibling->sibling->sibling, file, local_variable_map);
                }
                else{
                    fputs("_do_while_condition_", file);
                    do_while_count+=1;
                    sprintf(number_string, "%d", do_while_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->child->sibling, file, local_variable_map);
                }
            }
            else if (root->child->token.type == BREAK_KEYWORD){
                if (root->root->root->root->root->child->token.type == WHILE_KEYWORD){
                    fputs("jmp _while_break_", file);
                    sprintf(number_string, "%d", while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->root->child->token.type == FOR_KEYWORD){
                    fputs("jmp _for_break_", file);
                    sprintf(number_string, "%d", for_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->root->child->token.type == DO_KEYWORD){
                    fputs("jmp _do_while_break_", file);
                    sprintf(number_string, "%d", do_while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                if (root->root->root->root->child->token.type == WHILE_KEYWORD){
                    fputs("jmp _while_break_", file);
                    sprintf(number_string, "%d", while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->child->token.type == FOR_KEYWORD){
                    fputs("jmp _for_break_", file);
                    sprintf(number_string, "%d", for_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->child->token.type == DO_KEYWORD){
                    fputs("jmp _do_while_break_", file);
                    sprintf(number_string, "%d", do_while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else{
                    printf("%s", "Cannot have break outside a loop");
                }
            }
            else if (root->child->token.type == CONTINUE_KEYWORD){
                if (root->root->root->root->root->child->token.type == WHILE_KEYWORD){
                    fputs("jmp _while_skip_", file);
                    sprintf(number_string, "%d", while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->root->child->token.type == FOR_KEYWORD){
                    fputs("jmp __for_skip_", file);
                    sprintf(number_string, "%d", for_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->root->child->token.type == DO_KEYWORD){
                    fputs("jmp _do_while_skip_", file);
                    sprintf(number_string, "%d", do_while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                if (root->root->root->root->child->token.type == WHILE_KEYWORD){
                    fputs("jmp _while_skip_", file);
                    sprintf(number_string, "%d", while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->child->token.type == FOR_KEYWORD){
                    fputs("jmp _for_skip_", file);
                    sprintf(number_string, "%d", for_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->root->root->root->child->token.type == DO_KEYWORD){
                    fputs("jmp _do_while_skip_", file);
                    sprintf(number_string, "%d", do_while_count/10);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else{
                    printf("%s", "Cannot have continue outside a loop");
                }
            }
            else if (root->visited){
                if (root->child->token.type == RETURN_KEYWORD){
                    function_returned = true;
                    fputs("    pop \%rax\n", file);
                    
                    for (int i = 0; i<map_counter; i++){
                        fputs("    add $", file);
                        sprintf(number_string, "%d", local_variable_byte_count);
                        fputs(number_string, file);
                        fputs(", \%rsp\n", file);
                        fputs("    add $", file);
                        sprintf(number_string, "%d", local_variable_byte_count);
                        fputs(number_string, file);
                        fputs(", \%rbp\n", file);
                    }

                    
                    fputs("    mov \%rbp, \%rsp\n", file);
                    fputs("    pop \%rbp\n", file);
                    fputs("    ret\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else if (root->child->token.type == IF_KEYWORD && !root->child->sibling->sibling->sibling->sibling->visited){
                    fputs("    pop \%rax\n", file);
                    fputs("    cmp $0, \%rax\n", file);
                    fputs("    je _else_", file);
                    else_count++;
                    sprintf(number_string, "%d", else_count);
                    fputs(number_string, file);
                    fputs("\n", file);
                    generate_code(root->child->sibling->sibling->sibling->sibling, file, local_variable_map);
                }
                else if (root->past_sibling != NULL && root->past_sibling->token.type != DO_KEYWORD && root->past_sibling->past_sibling->past_sibling->past_sibling->token.type == IF_KEYWORD){
                    if (root->sibling != NULL && root->sibling->token.type == ELSE_KEYWORD){
                        fputs("    jmp _if_end_", file);
                        sprintf(number_string, "%d", else_count);
                        fputs(number_string, file);
                        fputs("\n", file);
                        fputs("_else_", file);
                        sprintf(number_string, "%d", else_count);
                        fputs(number_string, file);
                        fputs(":\n", file);
                        generate_code(root->sibling->sibling, file, local_variable_map);
                    }
                    else{
                        fputs("_else_", file);
                        sprintf(number_string, "%d", else_count);
                        fputs(number_string, file);
                        fputs(":\n", file);
                        generate_code(root->root, file, local_variable_map);
                    }
                }
                else if (root->past_sibling != NULL && root->past_sibling->token.type == ELSE_KEYWORD){
                    fputs("_if_end_", file);
                    sprintf(number_string, "%d", else_count);
                    fputs(number_string, file);
                    fputs(":\n", file);
                    generate_code(root->root, file, local_variable_map);
                }
                else{
                    generate_code(root->root, file, local_variable_map);
                }
            }
            else{
                while (root->token.type!=BLOCK_ITEM & (root->token.type != INT_LITERAL & (root->token.type != IDENTIFIER | (root->sibling != NULL && root->sibling->token.type != SEMICOLON)))){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file, local_variable_map);
            }
            break;
        
        case BLOCK_ITEM:
            if (root->visited){
                if (root->sibling->token.type != CLOSED_BRACE){
                    generate_code(root->sibling, file, local_variable_map);
                }
                else{
                    if (root->child->child->token.type != RETURN_KEYWORD){
                        local_variable local;
                        local.index = 0;
                        local.declared_in_block = false;
                        for (int i = 0; i<local_variable_count; i++){
                            local_variable_maps[map_counter][i] = local;
                        }
                        current_local_variable_byte_count = 0;
                        current_local_variable_count = 0;
                        while (local_variable_maps[map_counter-1][current_local_variable_count].index != 0){
                            current_local_variable_count++;
                            current_local_variable_byte_count+=8;
                        }
                        map_counter--;
                        else_count/=10;
                        for_count/=10;
                        while_count/=10;
                        do_while_count/=10;
                        fputs("    add $", file);
                        sprintf(number_string, "%d", local_variable_byte_count);
                        fputs(number_string, file);
                        fputs(", \%rsp\n", file);
                        fputs("    add $", file);
                        sprintf(number_string, "%d", local_variable_byte_count);
                        fputs(number_string, file);
                        fputs(", \%rbp\n", file);
                    }
                    else{
                        else_count/=10;
                        for_count/=10;
                        while_count/=10;
                        do_while_count/=10;
                        map_counter--;
                    }
                    generate_code(root->root, file, local_variable_maps[map_counter]);
                }
            }
            else{
                if (root->past_sibling->token.type == OPEN_BRACE){
                    map_counter++;
                    for (int i = 0; i<local_variable_count; i++){
                        local_variable_maps[map_counter][i] = local_variable_maps[map_counter-1][i];
                        local_variable_maps[map_counter][i].declared_in_block = false;
                    }
                    else_count*=10;
                    for_count*=10;
                    while_count*=10;
                    do_while_count*=10;
                    fputs("    sub $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rsp\n", file);
                    fputs("    sub $", file);
                    sprintf(number_string, "%d", local_variable_byte_count);
                    fputs(number_string, file);
                    fputs(", \%rbp\n", file);
                    for (int i = 0; i < local_variable_count; i++){
                        if (local_variable_map[i].index != 0){
                            fputs("    mov ", file);
                            sprintf(number_string, "%d", -(local_variable_map[i].index-local_variable_byte_count));
                            fputs(number_string, file);
                            fputs("(\%rbp), ", file);
                            fputs("\%rax\n", file);
                            fputs("    mov \%rax, ", file);
                            sprintf(number_string, "%d", -(local_variable_map[i].index));
                            fputs(number_string, file);
                            fputs("(\%rbp)\n", file);
                        }
                    }
                }
                root->visited = true;
                generate_code(root->child, file, local_variable_maps[map_counter]);
            }
            break;
        
        case FUNCTION:
            if (!root->visited){
                root->visited = true;
                function_returned = false;
                fputs("    .globl ", file);
                root = root->child->sibling;
                str name = root->token.name;
                str temp = name;
                while (name.pointer != NULL){
                    fputc(name.character, file);
                    name = *name.pointer;
                }
                fputc(name.character, file);
                fputs("\n", file);

                name = temp;
                while (name.pointer != NULL){
                    fputc(name.character, file);
                    name = *name.pointer;
                }
                fputc(name.character, file);
                fputs(":\n", file);
                fputs("    pushq \%rbp\n", file);
                fputs("    mov \%rsp, \%rbp\n", file);
                fputs("    sub $", file);
                sprintf(number_string, "%d", local_variable_byte_count);
                fputs(number_string, file);
                fputs(", \%rsp\n", file);
                root = root->sibling;
                root = root->sibling;
                root = root->sibling;
                root = root->sibling;
                generate_code(root, file, local_variable_map);
                break;
            }
            else if (!function_returned){
                fputs("    mov \%rbp, \%rsp\n", file);
                fputs("    pop \%rbp\n", file);
                fputs("    mov $0, \%rax\n", file);
                fputs("    ret\n", file);
            }
            if (root->sibling != NULL){
                generate_code(root->sibling, file, local_variable_map);
            }
            break;
    }
}


int main(){
    FILE *file_pointer;
    file_pointer = fopen("file.txt", "r");
    tkn_list *token_list = typify_tokens(get_tokens(file_pointer));
    ast* root;
    tkn program;
    program.type = PROGRAM;
    root = (ast *)malloc(sizeof(ast));
    root->token = program;
    root->child = (ast *)malloc(sizeof(ast));
    root->child->root = NULL;
    root->child->child = NULL;
    root->child->sibling = NULL;
    parse_return return_value = parse(token_list, PROGRAM_SYMBOL, root);
    root = return_value.root;
    if (return_value.valid){
        printf("%s", "Syntax valid\n");
        FILE *file;
        file = fopen("assembly.s", "w");
        root = root->child;
        generate_code(root, file, local_variable_maps[0]);
        fclose(file);
    }
}