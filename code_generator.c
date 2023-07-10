#include <stdio.h>
#include <stdbool.h>
#include "parser.c"

//gcc assembly.s -o executable_name
//echo %errorlevel%

void generate_code(ast* ast){
    FILE *file;
    file = fopen("assembly.s", "w");
    fputs("    .globl ", file);
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
        fputs("\n", file);

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
        int negation_counter = 0;
        int logical_negation_counter = 0;
        int bitwise_complement_counter = 0;
        while (ast->token.type == NEGATION | ast->token.type == LOGICAL_NEGATION | ast->token.type == BITWISE_COMPLEMENT){
            if (ast->token.type == NEGATION){
                if (negation_counter == 0){
                    negation_counter++;
                }
                else{
                    negation_counter = 0;
                }
            }
            if (ast->token.type == LOGICAL_NEGATION){
                if (logical_negation_counter == 0){
                    logical_negation_counter++;
                }
                else{
                    logical_negation_counter = 0;
                }
            }
            if (ast->token.type == BITWISE_COMPLEMENT){
                if (bitwise_complement_counter == 0){
                    bitwise_complement_counter++;
                }
                else{
                    bitwise_complement_counter = 0;
                }
            }
            ast = ast->sibling;
            ast = ast->child;
        }
        if (ast->token.type == INT_LITERAL){
            str name = ast->token.name;
            while (name.pointer != NULL){
                fputc(name.character, file);
                name = *name.pointer;
            }
            fputc(name.character, file);
        }
        fputs(", \%eax\n", file);
        if (negation_counter>0){
            fputs("    neg    \%eax\n", file);
        }
        if (bitwise_complement_counter>0){
            fputs("    not    \%eax\n", file);
        }
        if (logical_negation_counter>0){
            fputs("    cmpl $0, \%eax\n    movl $0, \%eax\n    sete    \%al\n", file);
        }
        fputs("    ret\n", file);
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
}