#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

char lines[10000][1000];
FILE *file_write;
FILE *file;
void optimise(){
    file_write = fopen("assembly.s", "w");
    file = fopen("assembly_unoptimised.s", "r");
    int file_counter = 0;
    while (fgets(lines[file_counter], 1000, file)){
        file_counter++;
    }
    for (int i = 0; i < file_counter; i++){
        int length = 0;
        while (lines[i][length] != '\n'){
            length++;
        }
        bool skip = false;
        if (lines[i][length-3] == 'r' && lines[i][length-2] == 'a' && lines[i][length-1] == 'x'){
            if (lines[i+1][0] == 'p' && lines[i+1][1] == 'u' && lines[i+1][2] == 's' && lines[i+1][3] == 'h' && lines[i+1][4] == 'q' && lines[i+1][5] == ' ' && lines[i+1][6] == '%' && lines[i+1][7] == 'r' && lines[i+1][8] == 'a' && lines[i+1][9] == 'x'){
                if (lines[i+2][0] == 'p' && lines[i+2][1] == 'o' && lines[i+2][2] == 'p' && lines[i+2][3] == ' ' && lines[i+2][4] == '%' && lines[i+2][5] == 'r' && lines[i+2][6] == 'a' && lines[i+2][7] == 'x'){
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    for (int j = 0; j < length; j++){
                        fputc(lines[i][j], file_write);
                    }
                    fputc('\n', file_write);
                    i+=2;
                    skip = true;
                }
            }
        }
        if (lines[i][0] == 'p' && lines[i][1] == 'u' && lines[i][2] == 's' && lines[i][3] == 'h' && lines[i][4] == 'q' && lines[i][5] == ' ' && lines[i][6] == '%' && lines[i][7] == 'r' && lines[i][8] == 'a' && lines[i][9] == 'x'){
            if (lines[i+1][0] == 'p' && lines[i+1][1] == 'o' && lines[i+1][2] == 'p' && lines[i+1][3] == ' ' && lines[i+1][4] == '%' && lines[i+1][5] == 'r' && lines[i+1][6] == 'a' && lines[i+1][7] == 'x'){
                if (lines[i+2][0] == 'p' && lines[i+2][1] == 'u' && lines[i+2][2] == 's' && lines[i+2][3] == 'h' && lines[i+2][4] == 'q' && lines[i+2][5] == ' ' && lines[i+2][6] == '%' && lines[i+2][7] == 'r' && lines[i+2][8] == 'a' && lines[i+2][9] == 'x'){
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    fputc(' ', file_write);
                    for (int j = 0; j < length; j++){
                        fputc(lines[i][j], file_write);
                    }
                    fputc('\n', file_write);
                    i+=2;
                    skip = true;
                }
            }
        }
        if (lines[i][0] == 'p' && lines[i][1] == 'u' && lines[i][2] == 's' && lines[i][3] == 'h' && lines[i][4] == 'q' && lines[i][5] == ' ' && lines[i][6] == '%' && lines[i][7] == 'r' && lines[i][8] == 'a' && lines[i][9] == 'x'){
            if (lines[i+1][0] == 'p' && lines[i+1][1] == 'o' && lines[i+1][2] == 'p' && lines[i+1][3] == ' ' && lines[i+1][4] == '%' && lines[i+1][5] == 'r' && lines[i+1][6] == 'a' && lines[i+1][7] == 'x'){
                i+=1;
                skip = true;
            }
        }
        if (lines[i][0] == 'm' && lines[i][1] == 'o' && lines[i][2] == 'v' && lines[i][3] == ' ' && lines[i][4] == '%' && lines[i][5] == 'r' && lines[i][6] == 'a' && lines[i][7] == 'x'){
            if (lines[i+1][0] == 'm' && lines[i+1][1] == 'o' && lines[i+1][2] == 'v' && lines[i+1][3] == ' ' && lines[i+1][length-3] == 'r' && lines[i+1][length-1] == 'a' && lines[i+1][length-2] == 'x' && lines[i][9] == lines[i+1][4] && lines[i][10] == lines[i+1][5]){
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
                for (int j = 0; j < length; j++){
                    fputc(lines[i][j], file_write);
                }
                fputc('\n', file_write);
                i+=1;
                skip = true;
            }
        }
        if (lines[i][0] == 'a' && lines[i][1] == 'd' && lines[i][2] == 'd' && lines[i][3] == ' ' && lines[i][4] == '$' && lines[i][5] == '0' && lines[i][6] == ','){
            skip = true;
        }
        if (lines[i][0] == 's' && lines[i][1] == 'u' && lines[i][2] == 'b' && lines[i][3] == ' ' && lines[i][4] == '$' && lines[i][5] == '0' && lines[i][6] == ','){
            skip = true;
        }
        if (!skip){
            if (lines[i][0] != '_'){
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
            }
            if (lines[i][0] == '.' && (lines[i][1] == 'l' || lines[i][1] == 'z')){
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
                fputc(' ', file_write);
            }
            for (int j = 0; j < length; j++){
                fputc(lines[i][j], file_write);
            }
            fputc('\n', file_write);
        }
    }
    fclose(file_write);
}