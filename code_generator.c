#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "parser.c"

//gcc assembly.s -o executable_name
//echo %errorlevel%
int and_jump_count = 0;
int or_jump_count = 0;
char number_string[1000];


void generate_code(ast* root, FILE *file){
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
                generate_code(root->past_sibling, file);
            }
            else{
                generate_code(root, file);
            }
            break;
        
        case NEGATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    neg \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case BITWISE_COMPLEMENT:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    not \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case LOGICAL_NEGATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    cmp $0, \%rax\n    mov $0, \%rax\n    sete \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->past_sibling != NULL){
                generate_code(root->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case FACTOR:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;
        
        case MULTIPLICATION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    pop \%rcx\n", file);
            fputs("    imul \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case DIVISION:
            root->visited = true;
            fputs("    pop \%rbx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    mov \%eax, \%eax\n", file);
            fputs("    cdq\n", file);
            fputs("    idiv \%ebx\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case TERM:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;
        
        case ADDITION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    pop \%rcx\n", file);
            fputs("    add \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case SUBTRACTION:
            root->visited = true;
            fputs("    pop \%rax\n", file);
            fputs("    pop \%rcx\n", file);
            fputs("    sub \%rcx, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case ADDITIVE_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;

        
        
        case LESS_THAN:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setl \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case LESS_THAN_OR_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setle \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;

        case GREATER_THAN:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setg \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;

        case GREATER_THAN_OR_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rcx, \%rax\n    mov $0, \%rax\n    setge \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case RELATIONAL_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;

        case NOT_EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rax, \%rcx\n    mov $0, \%rax\n    setne \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else if (root->root->sibling != NULL & root->root->past_sibling != NULL){
                fputs("    pop \%rax\n", file);
                fputs("    pushq \%rax\n", file);
                fputs("    cmp $0, \%rax\n", file);
                fputs("    je _skip_and_", file);
                sprintf(number_string, "%d", and_jump_count+1);
                fputs(number_string, file);
                fputs("\n", file);
                generate_code(root->sibling->sibling->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case EQUAL:
            root->visited = true;
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp \%rax, \%rcx\n    mov $0, \%rax\n    sete \%al\n", file);
            fputs("    pushq \%rax\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling->sibling, file);
            }
            else if (root->root->sibling != NULL & root->root->past_sibling != NULL){
                fputs("    pop \%rax\n", file);
                fputs("    pushq \%rax\n", file);
                fputs("    cmp $1, \%rax\n", file);
                fputs("    je _skip_and_", file);
                sprintf(number_string, "%d", and_jump_count+1);
                fputs(number_string, file);
                fputs("\n", file);
                generate_code(root->root, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;

        case EQUALITY_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;

        case AND:
            root->visited = true;
            and_jump_count++;
            sprintf(number_string, "%d", and_jump_count);
            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp $0, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            fputs("    je _skip_and_", file);
            fputs(number_string, file);
            fputs("\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    pushq \%rcx\n", file);
            
            fputs("_skip_and_", file);
            fputs(number_string, file);
            fputs(":\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling->sibling, file);
            }
            else if (root->root->sibling != NULL & root->root->past_sibling != NULL){
                fputs("    pop \%rax\n", file);
                fputs("    pushq \%rax\n", file);
                fputs("    cmp $0, \%rax\n", file);
                fputs("    je _skip_or_", file);
                sprintf(number_string, "%d", or_jump_count+1);
                fputs(number_string, file);
                fputs("\n", file);
                generate_code(root->root, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;
        
        case LOGICAL_AND_EXPRESSION:
            if (root->visited & root->past_sibling == NULL){
                root->visited = true;
                if (root->sibling != NULL){
                    generate_code(root->sibling->sibling, file);
                }
                else{
                    generate_code(root->root, file);
                }
            }
            else if (!root->visited){
                while (root->token.type != INT_LITERAL){
                    root->visited = true;
                    if (root->child != NULL){
                        root = root->child;
                    }
                    else{
                        root = root->sibling;
                    }
                }
                generate_code(root, file);
            }
            else{
                root->visited = true;
                generate_code(root->past_sibling, file);
            }
            break;

        case OR:
            root->visited = true;
            or_jump_count++;
            sprintf(number_string, "%d", or_jump_count);

            fputs("    pop \%rcx\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    cmp $1, \%rax\n", file);
            fputs("    pushq \%rax\n", file);
            fputs("    je _skip_or_", file);
            fputs(number_string, file);
            fputs("\n", file);
            fputs("    pop \%rax\n", file);
            fputs("    pushq \%rcx\n", file);
            fputs("_skip_or_", file);
            fputs(number_string, file);
            fputs(":\n", file);
            if (root->sibling->sibling != NULL){
                generate_code(root->sibling, file);
            }
            else{
                generate_code(root->root, file);
            }
            break;

        case EXPRESSION:
            if (root->root->token.type == FACTOR){
                generate_code(root->root, file);
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
        printf("%s", "Syntax valid");
        FILE *file;
        file = fopen("assembly.s", "w");
        fputs("    .globl ", file);
        while (root->child!=NULL){
            root = root->child;
        }
        root = root->sibling;
        if (root->token.type == IDENTIFIER){
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
            root = root->sibling;
            root = root->sibling;
            root = root->sibling;
            root = root->sibling;
            root = root->child;
            root = root->sibling;
            while (root->token.type != INT_LITERAL){
                root->visited = true;
                if (root->child != NULL){
                    root = root->child;
                }
                else{
                    root = root->sibling;
                }
            }
            generate_code(root, file);
            fclose(file);
            file = fopen("assembly.s", "a");
            fputs("    pop \%rax\n    ret\n", file);
            fclose(file);
        }
    }
}