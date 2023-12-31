#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct str{
    char character;
    struct str *pointer;
} str;
char number_string[10000];
FILE *file;

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
    NEGATION,
    BITWISE_COMPLEMENT,
    COMMA,
    LOGICAL_NEGATION,
    ADDITION,
    DIVISION,
    SUBTRACTION,
    MULTIPLICATION,
    AND,
    OR,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    ASSIGNMENT,
    IF_KEYWORD,
    ELSE_KEYWORD,
    COLON,
    QUESTION_MARK,
    FOR_KEYWORD,
    WHILE_KEYWORD,
    DO_KEYWORD,
    BREAK_KEYWORD,
    CONTINUE_KEYWORD,
    NULL_KEYWORD,

    INVALID,

    PROGRAM,
    FUNCTION,
    BLOCK_ITEM,
    DECLARATION,
    STATEMENT,
    EXPRESSION_OPTION,
    EXPRESSION,
    CONDITIONAL_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    EQUALITY_EXPRESSION,
    RELATIONAL_EXPRESSION,
    ADDITIVE_EXPRESSION,
    FUNCTION_CALL,
    TERM,
    FACTOR,
    UNARY_OP
} tkn_type;

typedef struct tkn{
    str name;
    enum token_type type;
    int line_index;
    int character_index;
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

bool is_valid_char(char character){
    char valid_chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_','1','2','3','4','5','6','7','8','9','0'};
    for (int i = 0; i<26*2+11; i++){
        if (character==valid_chars[i]){
            return true;
        }
    }
    return false;
}

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
    char ending_tokens[] = {'(', ')', '{', '}', ';', '-', '~', '!', '+', '/', '*', '&', '|', '=', '<', '>', ','};
    for (int i = 0; i<17; i++){
        if (character==ending_tokens[i]){
            return true;
        }
    }
    return false;
}

bool is_ending_token_exc_comparators(char character){
    char ending_tokens[] = {'(', ')', '{', '}', ';', '~', ',', '-', '+', '*', '/'};
    for (int i = 0; i<11; i++){
        if (character==ending_tokens[i]){
            return true;
        }
    }
    return false;
}

bool is_ending_token_for_comparators(char character){
    char valid_chars[] = {' ', ',', 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_','1','2','3','4','5','6','7','8','9','0', '(', ')', '{', '}', ';', '-', '~', '+', '/', '*'};
    for (int i = 0; i<26*2+11+12; i++){
        if (character==valid_chars[i]){
            return true;
        }
    }
    return false;
}

bool is_comparator(char character){
    char comparator_tokens[] = {'!', '&', '|', '=', '<', '>'};
    for (int i = 0; i<6; i++){
        if (character==comparator_tokens[i]){
            return true;
        }
    }
    return false;
}

int line_number = 1;
int character_number = 1;

tkn_return get_next_token(FILE *file_pointer){
    char character;
    char last_character;
    bool end = false;
    bool post = false;
    str *token_string;
    token_string = (str *)malloc(sizeof(str));
    token_string->pointer = NULL;
    character=fgetc(file_pointer);
    while (character == ' ' | character == '\n'){
        if (character == '\n'){
            line_number++;
            character_number = 0;
        }
        else{
            character_number++;
        }
        character=fgetc(file_pointer);
    }
    if (is_valid_char(last_character) && (character == '+' || character == '-' || character == '/' || character == '*')){
        char temp = last_character;
        last_character = character;
        character=fgetc(file_pointer);
        character_number++;
        if ((last_character == '+' && character == '+') || (last_character == '-' && character == '-') || character == '='){
            end = true;
            append_string(token_string, last_character);
            append_string(token_string, character);
            post = true;
        }
        else{
            character_number--;
            character = last_character;
            last_character = temp;
            fseek(file_pointer, -1, SEEK_CUR);
        }
    }
    if (!post && is_ending_token_exc_comparators(character)){
        end = true;
        append_string(token_string, character);
    }
    while (character != EOF & character != ' ' & character != '\n' & !end & !post){
        append_string(token_string, character);
        last_character = character;
        character_number++;
        character=fgetc(file_pointer);
        if (!is_comparator(last_character) & is_ending_token(character)){
            character_number--;
            end = true;
            fseek(file_pointer, -1, SEEK_CUR);
        }
        if (is_comparator(last_character) & (is_ending_token_for_comparators(character) | character == '!')){
            character_number--;
            end = true;
            fseek(file_pointer, -1, SEEK_CUR);
        }
        else if (is_comparator(last_character)){
            append_string(token_string, character);
            end = true;
        }
    }
    tkn token;
    token.line_index = line_number;
    token.character_index = character_number;
    if (token_string->pointer != NULL){
        token.name = *(token_string->pointer);
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
    file = fopen("assembly.s", "w");
    tkn_list *token_list;
    token_list = (tkn_list *)malloc(sizeof(tkn_list));
    token_list->pointer = NULL;
    tkn last_token;
    tkn new_token;
    last_token.name.character = '?';
    tkn_return token_return = get_next_token(file_pointer);
    while (!token_return.end_of_file){
        if (last_token.name.character != '?' && token_return.token.name.character == '+' && token_return.token.name.pointer != NULL && token_return.token.name.pointer->character == '+'){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
            new_token.name.character = '1';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
        }
        else if (last_token.name.character != '?' && token_return.token.name.character == '-' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '-'){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
            new_token.name.character = '1';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
        }
        else if (last_token.name.character != '?' && token_return.token.name.character == '+' &&token_return.token.name.pointer != NULL && token_return.token.name.pointer->character == '='){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
        }
        else if (last_token.name.character != '?' && token_return.token.name.character == '-' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '='){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
        }
        else if (last_token.name.character != '?' && token_return.token.name.character == '*' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '='){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
        }
        else if (last_token.name.character != '?' && token_return.token.name.character == '/' &&token_return.token.name.pointer != NULL && token_return.token.name.pointer->character == '='){
            new_token.name.character = '=';
            new_token.name.pointer = NULL;
            append_token(token_list, new_token);
            append_token(token_list, last_token);
            append_token(token_list, token_return.token);
        }
        else{
            append_token(token_list, token_return.token);
        }
        last_token = token_return.token;
        file_pointer = token_return.file_pointer;
        token_return = get_next_token(file_pointer);
    }
    if (last_token.name.character != '?' && token_return.token.name.character == '+' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '+'){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
        new_token.name.character = '1';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
    }
    else if (last_token.name.character != '?' && token_return.token.name.character == '-' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '-'){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
        new_token.name.character = '1';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
    }
    else if (last_token.name.character != '?' && token_return.token.name.character == '+' &&token_return.token.name.pointer != NULL && token_return.token.name.pointer->character == '='){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
    }
    else if (last_token.name.character != '?' && token_return.token.name.character == '-' &&token_return.token.name.pointer != NULL && token_return.token.name.pointer->character == '='){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
    }
    else if (last_token.name.character != '?' && token_return.token.name.character == '*' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '='){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
    }
    else if (last_token.name.character != '?' && token_return.token.name.character == '/' && token_return.token.name.pointer != NULL &&token_return.token.name.pointer->character == '='){
        new_token.name.character = '=';
        new_token.name.pointer = NULL;
        append_token(token_list, new_token);
        append_token(token_list, last_token);
        append_token(token_list, token_return.token);
    }
    else{
        append_token(token_list, token_return.token);
    }
    fclose(file_pointer);
    return token_list->pointer;
}

tkn_type typify_token(tkn *token, tkn *previous){
    str name = token->name;
    bool end = false;
    bool possible_return_keyword = false;
    bool possible_int_keyword = false;
    bool possible_if_keyword = false;
    bool possible_else_keyword = false;
    bool possible_for_keyword = false;
    bool possible_while_keyword = false;
    bool possible_do_keyword = false;
    bool possible_continue_keyword = false;
    bool possible_break_keyword = false;
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

        case ':':
            return COLON;
            break;

        case '?':
            return QUESTION_MARK;
            break;

        case '-':
            if (previous->type == INT_LITERAL | previous->type == IDENTIFIER | previous->type == CLOSED_PARENTHESES){
                return SUBTRACTION;
            }
            return NEGATION;
            break;

        case '~':
            return BITWISE_COMPLEMENT;
            break;
        
        case ',':
            return COMMA;
            break;
        
        case '!':
            if (name.pointer != NULL && name.pointer->character == '=' && name.pointer->pointer == NULL){
                return NOT_EQUAL;
            }
            if (name.pointer == NULL){
                return LOGICAL_NEGATION;
            }
            
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case '+':
            return ADDITION;
            break;

        case '/':
            return DIVISION;
            break;
        
        case '*':
            return MULTIPLICATION;
            break;
        
        case '&':
            if (name.pointer != NULL && name.pointer->character == '&' && name.pointer->pointer == NULL){
                return AND;
            }
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case '|':
            if (name.pointer != NULL && name.pointer->character == '|' && name.pointer->pointer == NULL){
                return OR;
            }
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case '=':
            if (name.pointer != NULL && name.pointer->character == '=' && name.pointer->pointer == NULL){
                return EQUAL;
            }
            if (name.pointer == NULL){
                return ASSIGNMENT;
            }
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case '<':
            if (name.pointer != NULL && name.pointer->character == '=' && name.pointer->pointer == NULL){
                return LESS_THAN_OR_EQUAL;
            }
            if (name.pointer == NULL){
                return LESS_THAN;
            }
            
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case '>':
            if (name.pointer != NULL && name.pointer->character == '=' && name.pointer->pointer == NULL){
                return GREATER_THAN_OR_EQUAL;
            }
            if (name.pointer == NULL){
                return GREATER_THAN;
            }
            
            fputc( '\'', file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' invalid token [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
            break;

        case 'r':
            possible_return_keyword = true;
            break;
        
        case 'i':
            if (name.pointer!=NULL){
                if (name.pointer->character == 'f'){
                    possible_if_keyword = true;
                }
                else{
                    possible_int_keyword = true;
                }
            }
            break;
        
        case 'e':
            possible_else_keyword = true;
            break;
        
        case 'f':
            possible_for_keyword = true;
            break;
        
        case 'w':
            possible_while_keyword = true;
            break;
        
        case 'd':
            possible_do_keyword = true;
            break;
        
        case 'b':
            possible_break_keyword = true;
            break;
        
        case 'c':
            possible_continue_keyword = true;
            break;
        
        default:
            break;
    }

    str *temp = &name;
    if (possible_if_keyword){
        if (name.pointer != NULL){
            name = *name.pointer;
            if (name.character != 'f'){
                possible_if_keyword = false;
            }
        }
        else{
            possible_if_keyword = false;
        }
        if (possible_if_keyword & name.pointer!=NULL){
            possible_if_keyword = false;
        }
        if (possible_if_keyword){
            return IF_KEYWORD;
        }
    }
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
    else if (possible_else_keyword){
        char return_keyword[] = {'l','s','e'};
        for (int i = 0; i<3; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != return_keyword[i]){
                    possible_else_keyword = false;
                }
            }
            else{
                possible_else_keyword = false;
            }
        }
        if (possible_else_keyword & name.pointer!=NULL){
            possible_else_keyword = false;
        }
        else if (possible_else_keyword){
            return ELSE_KEYWORD;
        }
    }
    else if (possible_for_keyword){
        char for_keyword[] = {'o','r'};
        for (int i = 0; i<2; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != for_keyword[i]){
                    possible_for_keyword = false;
                }
            }
            else{
                possible_for_keyword = false;
            }
        }
        if (possible_for_keyword & name.pointer!=NULL){
            possible_for_keyword = false;
        }
        else if (possible_for_keyword){
            return FOR_KEYWORD;
        }
    }
    else if (possible_while_keyword){
        char while_keyword[] = {'h','i','l','e'};
        for (int i = 0; i<4; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != while_keyword[i]){
                    possible_while_keyword = false;
                }
            }
            else{
                possible_while_keyword = false;
            }
        }
        if (possible_while_keyword & name.pointer!=NULL){
            possible_while_keyword = false;
        }
        else if (possible_while_keyword){
            return WHILE_KEYWORD;
        }
    }
    else if (possible_do_keyword){
        char do_keyword[] = {'o'};
        for (int i = 0; i<1; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != do_keyword[i]){
                    possible_do_keyword = false;
                }
            }
            else{
                possible_do_keyword = false;
            }
        }
        if (possible_do_keyword & name.pointer!=NULL){
            possible_do_keyword = false;
        }
        else if (possible_do_keyword){
            return DO_KEYWORD;
        }
    }
    else if (possible_break_keyword){
        char break_keyword[] = {'r','e','a','k'};
        for (int i = 0; i<4; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != break_keyword[i]){
                    possible_break_keyword = false;
                }
            }
            else{
                possible_break_keyword = false;
            }
        }
        if (possible_break_keyword & name.pointer!=NULL){
            possible_break_keyword = false;
        }
        else if (possible_break_keyword){
            return BREAK_KEYWORD;
        }
    }
    else if (possible_continue_keyword){
        char continue_keyword[] = {'o','n','t','i','n','u','e'};
        for (int i = 0; i<7; i++){
            if (name.pointer != NULL){
                name = *name.pointer;
                if (name.character != continue_keyword[i]){
                    possible_continue_keyword = false;
                }
            }
            else{
                possible_continue_keyword = false;
            }
        }
        if (possible_continue_keyword & name.pointer!=NULL){
            possible_continue_keyword = false;
        }
        else if (possible_continue_keyword){
            return CONTINUE_KEYWORD;
        }
    }
    name = *temp;
    if (isdigit(name.character)){
        while (name.pointer != NULL){
            if (!isdigit(name.character)){
            
                fputs( "Invalid suffix \'", file);
                while (name.pointer != NULL){
                    fputc( name.character, file);
                    name = *name.pointer;
                }
                fputc( name.character, file);
                fputs( "\' on integer constant", file);
                fputs( " [Ln ", file);
                sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
                fputs( ", Col ", file);
                sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
                fputs( "]\n", file);
                return INVALID;
            }
            if (!is_valid_char(name.character)){
            
                fputc( '\'', file);
                fputc( name.character, file);
                fputs( "\' invalid character", file);
                fputs( " [Ln ", file);
                sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
                fputs( ", Col ", file);
                sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
                fputs( "]\n", file);
                return INVALID;
            }
            name = *name.pointer;
        }
        if (!isdigit(name.character)){
            
            fputs( "Invalid suffix \'", file);
            while (name.pointer != NULL){
                fputc( name.character, file);
                name = *name.pointer;
            }
            fputc( name.character, file);
            fputs( "\' on integer constant", file);
            fputs( " [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
        }
        if (!is_valid_char(name.character)){
            
            fputc( '\'', file);
            fputc( name.character, file);
            fputs( "\' invalid character", file);
            fputs( " [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
        }
        return INT_LITERAL;
    }
    else{
        while (name.pointer != NULL){
            
            if (!is_valid_char(name.character)){
                fputc( '\'', file);
                fputc( name.character, file);
                fputs( "\' invalid character", file);
                fputs( " [Ln ", file);
                sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
                fputs( ", Col ", file);
                sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
                fputs( "]\n", file);
                return INVALID;
            }
            name = *name.pointer;
        }
        if (!is_valid_char(name.character)){
            
            fputc( '\'', file);
            fputc( name.character, file);
            fputs( "\' invalid character", file);
            fputs( " [Ln ", file);
            sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
            fputs( ", Col ", file);
            sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
            fputs( "]\n", file);
            return INVALID;
        }
        return IDENTIFIER;
    }
            
    fputc( '\'', file);
    while (name.pointer != NULL){
        fputc( name.character, file);
        name = *name.pointer;
    }
    fputc( name.character, file);
    fputs( "\' invalid token", file);
    fputs( " [Ln ", file);
    sprintf(number_string, "%d",token->line_index);
    fputs(number_string, file);
    fputs( ", Col ", file);
    sprintf(number_string, "%d",token->character_index);
    fputs(number_string, file);
    fputs( "]\n", file);
    return INVALID;
}

tkn_list* typify_tokens(tkn_list *token_list){
    tkn_list *temp = token_list;
    tkn *token = &token_list->token;
    int open = 0;
    bool found = false;
    while (token_list->pointer != NULL){
        tkn *previous = token;
        tkn_list *temp_list;
        token = &token_list->token;
        token->type = typify_token(token, previous);
        token = &token_list->token;
        token_list = token_list->pointer;
    }
    return temp;
}