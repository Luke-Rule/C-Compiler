#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct str{
    char character;
    struct str *pointer;
} str;

typedef enum token_type{
    OPEN_BRACE,
    CLOSED_BRACE,
    OPEN_PARENTHESES,
    CLOSED_PARENTHESES,
    SEMICOLON,
    INT_KEYWORD,
    RETURN_KEYWORD,
    IDENTIFIER,
    INT_LITERAL,
    INVALID,
    PROGRAM,
    FUNCTION,
    STATEMENT,
    EXPRESSION,
    NEGATION,
    BITWISE_COMPLEMENT,
    LOGICAL_NEGATION
} tkn_type;

typedef struct tkn{
    str name;
    int value;
    enum token_type type;
} tkn;

typedef struct tkn_list{
    tkn token;
    struct tkn_list *pointer;
} tkn_list;

typedef struct tkn_return{
    tkn token;
    bool end_of_file;
    FILE *file_pointer;
} tkn_return;

void append_string(str *string, char character){
    while (string->pointer != NULL){
        string = string->pointer;
    }
    string->pointer = (str *)malloc(sizeof(str));
    string->pointer->character = character;
    string->pointer->pointer = NULL;
}

void append_token(tkn_list *token_list, tkn token){
    while (token_list->pointer != NULL){
        token_list = token_list->pointer;
    }
    token_list->pointer = (tkn_list *)malloc(sizeof(tkn_list));
    token_list->pointer->token = token;
    token_list->pointer->pointer = NULL;
}

int length(str string, int count){
    while (string.pointer != NULL){
        string = *string.pointer;
        count++;
    }
    return count;
}

int get_int_value(str *string, int count){
    while (string->pointer != NULL){
        count+=(int)string->character;
        string = string->pointer;
    }
    return count;
}

bool is_ending_token(char character){
    char ending_tokens[] = {'(', ')', '{', '}', ';', '-', '~', '!'};
    for (int i = 0; i<8; i++){
        if (character==ending_tokens[i]){
            return true;
        }
    }
    return false;
}

tkn_return get_next_token(FILE *file_pointer){
    char character;
    bool first = false;
    str *token_string;
    token_string = (str *)malloc(sizeof(str));
    token_string->pointer = NULL;
    character=fgetc(file_pointer);
    while (character == ' ' | character == '\n'){
        character=fgetc(file_pointer);
    }
    if (is_ending_token(character)){
        append_string(token_string, character);
    }
    while (character != EOF & character != ' ' & character != '\n' & !is_ending_token(character)){
        append_string(token_string, character);
        if (!first & isdigit(character)){
            first = true;
        }
        character=fgetc(file_pointer);
        if (is_ending_token(character)){
            fseek(file_pointer, -1, SEEK_CUR);
        }
    }
    tkn token;
    if (token_string->pointer != NULL){
        token.name = *(token_string->pointer);
    }
    if (first){
        token.value = get_int_value(token_string, 0);
    }
    tkn_return token_return;
    token_return.token = token;
    if (character==EOF){
        token_return.end_of_file = 1;
    }
    else{
        token_return.end_of_file = 0;
    }
    token_return.file_pointer = file_pointer;
    return token_return;
}

tkn_list* get_tokens(FILE *file_pointer){
    tkn_list *token_list;
    token_list = (tkn_list *)malloc(sizeof(tkn_list));
    token_list->pointer = NULL;
    tkn_return token_return = get_next_token(file_pointer);
    while (!token_return.end_of_file){
        append_token(token_list, token_return.token);
        file_pointer = token_return.file_pointer;
        token_return = get_next_token(file_pointer);
    }
    append_token(token_list, token_return.token);
    fclose(file_pointer);
    return token_list->pointer;
}

void print_token(tkn token){
    str *token_string = &token.name;
    while (token_string != NULL){
        printf("%c", token_string->character);
        token_string = token_string->pointer;
    }
    printf("%c", ' ');
}

void print_tokens(tkn_list *token_list){
    while (token_list != NULL){
        print_token(token_list->token);
        token_list = token_list->pointer;
    }
}

bool is_valid_char(char character){
    char valid_chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_','1','2','3','4','5','6','7','8','9','0'};
    for (int i = 0; i<26*2+11; i++){
        if (character==valid_chars[i]){
            return true;
        }
    }
    return false;
}

tkn_type typify_token(tkn *token){
    str name = token->name;
    bool end = false;
    bool possible_return_keyword = false;
    bool possible_int_keyword = false;
    tkn_type type = INVALID;
    switch (name.character)
        {
        case '{':
            return OPEN_BRACE;
            break;

        case '}':
            return CLOSED_BRACE;
            break;

        case '(':
            return OPEN_PARENTHESES;
            break;

        case ')':
            return CLOSED_PARENTHESES;
            break;
        
        case ';':
            return SEMICOLON;
            break;

        case '-':
            return NEGATION;
            break;

        case '~':
            return BITWISE_COMPLEMENT;
            break;
        
        case '!':
            return LOGICAL_NEGATION;
            break;

        case 'r':
            possible_return_keyword = true;
            break;
        
        case 'i':
            possible_int_keyword = true;
            break;
        
        default:
            break;
    }

    str *temp = &name;

    if (possible_int_keyword){
        char int_keyword[] = {'n','t'};
        for (int i = 0; i<2; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != int_keyword[i]){
                    possible_int_keyword = false;
                }   
            }
            else{
                possible_int_keyword = false;
            }
        }
        if (possible_int_keyword & name.pointer!=NULL){
            possible_int_keyword = false;
        }
        else if (possible_int_keyword){
            return INT_KEYWORD;
        }
    }

    else if (possible_return_keyword){
        char return_keyword[] = {'e','t','u','r','n'};
        for (int i = 0; i<5; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != return_keyword[i]){
                    possible_return_keyword = false;
                }
            }
            else{
                possible_return_keyword = false;
            }
        }
        if (possible_return_keyword & name.pointer!=NULL){
            possible_return_keyword = false;
        }
        else if (possible_return_keyword){
            return RETURN_KEYWORD;
        }
    }

    name = *temp;
    if (isdigit(name.character)){
        while (name.pointer != NULL){
            if (!isdigit(name.character) | !is_valid_char(name.character)){
                printf("%s", "Invalid token");
                return INVALID;
            }
            name = *name.pointer;
        }
        if (!isdigit(name.character)){
            printf("%s", "Invalid token");
            return INVALID;
        }
        return INT_LITERAL;
    }
    else{
        while (name.pointer != NULL){
            if (!is_valid_char(name.character)){
                return INVALID;
            }
            name = *name.pointer;
        }
        if (!is_valid_char(name.character)){
            printf("%s", "Invalid token");
            return INVALID;
        }
        return IDENTIFIER;
    }
    printf("%s", "Invalid token");
    return INVALID;
}

tkn_list* typify_tokens(tkn_list *token_list){
    tkn_list *temp = token_list;
    tkn *token = &token_list->token;
    while (token_list->pointer != NULL){
        token = &token_list->token;
        token->type = typify_token(token);
        token_list = token_list->pointer;
    }
    return temp;
}