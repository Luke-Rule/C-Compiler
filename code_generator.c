#include <stdio.h>
#include <stdbool.h>
#include "parser.c"

/*
    .globl _foo
_foo:
    movl    $3, %eax
    ret 
*/
//gcc assembly.s -o executable_name

void generate_code(ast* ast){
    FILE *file;
    file = fopen("assembly.s", "w");
    fputs("    .globl _", file);
    while (ast->child!=NULL){
        ast = ast->child;
    }
    ast = ast->sibling;
    if (ast->token.type == IDENTIFIER){
        str name = ast->token.name;
        str temp = name;
        while (name.pointer != NULL){
            fputc(name.character, file);
            name = *name.pointer;
        }
        fputc(name.character, file);
        fputs("\n_", file);

        name = temp;
        while (name.pointer != NULL){
            fputc(name.character, file);
            name = *name.pointer;
        }
        fputc(name.character, file);
        fputs(":\n    movl    $", file);
        ast = ast->sibling;
        ast = ast->sibling;
        ast = ast->sibling;
        ast = ast->sibling;
        ast = ast->child;
        ast = ast->sibling;
        ast = ast->child;
        if (ast->token.type == INT_LITERAL){
            str name = ast->token.name;
            while (name.pointer != NULL){
                fputc(name.character, file);
                name = *name.pointer;
            }
            fputc(name.character, file);
        }
        fputs(", \%eax\n    ret\n", file);
        fclose(file);
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
    if (return_value.valid){
        printf("%c", 't');
        generate_code(return_value.root);
        
    }
    else{
        printf("%c", 'f');
    }
}