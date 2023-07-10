#include <stdio.h>
#include <stdbool.h>
#include "parser.c"

//gcc assembly.s -o executable_name
//echo %errorlevel%

void generate_code(ast* root, FILE *file){
    if (root->token.type == INT_LITERAL){
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
    }
    else if (root->token.type == NEGATION){
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
    }
    else if (root->token.type == BITWISE_COMPLEMENT){
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
    }
    else if (root->token.type == NEGATION){
        root->visited = true;
        fputs("    pop \%rax\n", file);
        fputs("    cmp $0, \%rax\n    movl $0, \%rax\n    sete \%al\n", file);
        fputs("    pushq \%rax\n", file);
        if (root->past_sibling != NULL){
            generate_code(root->sibling, file);
        }
        else{
            generate_code(root->root, file);
        }
    }
    else if (root->token.type == FACTOR & root->visited & root->past_sibling == NULL){
        root->visited = true;
        if (root->sibling != NULL){
            generate_code(root->sibling->sibling, file);
        }
        else{
            generate_code(root->root, file);
        }
    }
    else if (root->token.type == FACTOR & !root->visited){
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
    else if (root->token.type == FACTOR){
        root->visited = true;
        generate_code(root->past_sibling, file);
    }
    else if (root->token.type == MULTIPLICATION){
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
    }
    else if (root->token.type == DIVISION){
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
    }


    else if (root->token.type == TERM & root->visited & root->past_sibling == NULL){
        root->visited = true;
        if (root->sibling != NULL){
            generate_code(root->sibling->sibling, file);
        }
        else{
            generate_code(root->root, file);
        }
    }
    else if (root->token.type == TERM & !root->visited){
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
    else if (root->token.type == TERM){
        root->visited = true;
        generate_code(root->past_sibling, file);
    }
    else if (root->token.type == ADDITION){
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
    }
    else if (root->token.type == SUBTRACTION){
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
    }
    else if (root->token.type == EXPRESSION & root->root->token.type == FACTOR){
        generate_code(root->root, file);
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
        printf("%c", 't');
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