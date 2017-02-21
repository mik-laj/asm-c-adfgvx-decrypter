#include <stdio.h>
#include <string.h>     /* strlen */
#include <math.h>       /* ceil */

char keysquare[] = "PH0QG64MEA1YL2NOFDXKR3CVS5ZW7BJ9UTI8";

// char cipher[500] = "GFFADGDXGAAVDDFDFAFVXXFXDXFAGXDAFFGG";
// char check_keyword[] = "GRAM";
char cipher[200] = "GFVGVGDXXAVVGFVFFXXXVGFFFGFDFGXVAVGDVXDDDG";
char check_keyword[] = "TECZKA";

int cipher_length = 0;
int column_height;
char step_12[4000];
// char step_3_indexes[6] = {0, 0, 0, 0, 0, 0};
// char step_3_indexes[6] = {2, 1, 5, 3, 0, 4}; // GERMAN
char step_3_indexes[6] = {4, 0, 5, 1, 3, 2}; // MARCHE
char step_3_matrix[4000];
char step_4_plaintext[500];
int step_4_plaintext_length = 0;

void step1_2() {
    for(int i = 0; i < cipher_length; i++){
        int row_index = i % column_height;
        int column_index = i / column_height;
        // printf("%d %d\n", row_index, column_index);
        int cell_index = row_index * 200 + column_index;
        step_12[cell_index] = cipher[i];
        // printf("%d %d %d = %c\n", row_index, column_index, cell_index, cipher[i]);
    }
}

void step1_2_dump() {
    for(int j = 0; j < column_height; j++){
        for(int i = 0; i < 6; i++) {
            int cell_index = j * 200 + i;
            printf("%c", step_12[cell_index]);
        }
        printf("\n");
    }
}

void step3() {
    for(int j = 0; j < column_height; j++){
        for(int i = 0; i < 6; i++) {
            int column_index = step_3_indexes[i];
            // printf("column_index: %d\n", column_index);
            int cell_index_target = j * 200 + i;
            int cell_index_source = j * 200 + column_index;
            // printf("%d %d %d\n", column_index, cell_index_target, cell_index_source);
            step_3_matrix[cell_index_target] = step_12[cell_index_source];
        }
    }
}

void step3_dump() {
    for(int j = 0; j < column_height; j++){
        for(int i = 0; i < 6; i++) {
            int cell_index = j * 200 + i;
            printf("%c", step_3_matrix[cell_index]);
        }
        printf("\n");
    }
}

int letter_to_index(int letter){
    if(letter == 'A') return 0;
    if(letter == 'D') return 1;
    if(letter == 'F') return 2;
    if(letter == 'G') return 3;
    if(letter == 'V') return 4;
    if(letter == 'X') return 5;
    return 0xff;
}
char index_to_letter(int index){
    if(index == 0) return 'A';
    if(index == 1) return 'B';
    if(index == 2) return 'C';
    if(index == 3) return 'D';
    if(index == 4) return 'E';
    if(index == 5) return 'F';
    return '#';
}


int decode_letter(int letter_a, int letter_b){
    int row_index = letter_to_index(letter_b);
    int column_index = letter_to_index(letter_a);
    int keysquare_index = row_index * 6 + column_index;
    int r = keysquare[keysquare_index];
    return r;
}

void step4_clear_plaintext() {
    for(int i = 0 ; i < step_4_plaintext_length; i++) {
        step_4_plaintext[i] = '\0';
    }
    step_4_plaintext_length = 0;
}

void step4_decode() {
    for(int i = 0; i < column_height; i++) {
        int row_index = i * 200;
        for(int j = 0; j < 3; j++) {
            char r = decode_letter(
                step_3_matrix[row_index++], 
                step_3_matrix[row_index++]
            );
            step_4_plaintext[step_4_plaintext_length] = r;
            step_4_plaintext_length++;
        }
    }
}

void step4_dump() {
    printf("Plaintext: %s\n", step_4_plaintext);
}

void step4_preview() {
    printf("┌───┬───┬───┐\n");
    for(int i = 0; i < column_height; i++) {
        printf("│%c %c│%c %c│%c %c│\n", 
            step_3_matrix[i * 200],
            step_3_matrix[i * 200 + 1],
            step_3_matrix[i * 200 + 2],
            step_3_matrix[i * 200 + 3],
            step_3_matrix[i * 200 + 4],
            step_3_matrix[i * 200 + 5]
        );
        if(column_height != i + 1) {
            printf("├───┼───┼───┤\n");
        }
    }
    printf("└───┴───┴───┘\n");
}

void step4() {
    step4_clear_plaintext();
    step4_decode();
}

bool isValidPlaintext() {
    // return // strstr(step_4_plaintext, check_keyword);
    int check_keyword_len = strlen(check_keyword);
    int step_4_plaintext_len = strlen(step_4_plaintext);
    if(check_keyword_len > step_4_plaintext_len){
        return false;
    }


    for (int i = 0; i <= step_4_plaintext_len - check_keyword_len; i++) {
        int j = 0;
        for (; (j < check_keyword_len) && (step_4_plaintext[i + j] == check_keyword[j]); j++)
            ;
        if (j == check_keyword_len) {
            return true;
        }
    }
    return false;
}

int calcArithmeticalRatio()
{
    return (step_3_indexes[0] + 1) * 
        (step_3_indexes[1] + 1) * 
        (step_3_indexes[2] + 1) * 
        (step_3_indexes[3] + 1) * 
        (step_3_indexes[4] + 1) * 
        (step_3_indexes[5] + 1); 
}

int calcBinaryRatio()
{
    return step_3_indexes[0] ^ 
        step_3_indexes[1] ^ 
        step_3_indexes[2] ^ 
        step_3_indexes[3] ^ 
        step_3_indexes[4] ^ 
        step_3_indexes[5];
}

bool isstep_3_indexesContainUniqueElements() {
    if(calcBinaryRatio() != 1) {
        return false;
    }
    if(calcArithmeticalRatio() != 720){
        return false;
    }
    return true;
}

void print_result() {
    printf("Found permutation [%d, %d, %d, %d, %d, %d] = %s\n", 
        step_3_indexes[0],
        step_3_indexes[1],
        step_3_indexes[2],
        step_3_indexes[3],
        step_3_indexes[4],
        step_3_indexes[5],
        step_4_plaintext
    );
}
int try_crack() {
    for(int i = 0; i < 6 * 6 * 6 * 6 * 6 * 6; i++){
        int tmp = i;
        for(int j = 0; j < 6; j++){
            step_3_indexes[j] = tmp % 6; tmp /= 6;
        }
        if(isstep_3_indexesContainUniqueElements()) {
            step3();
            // step3_dump();
            // step4_preview();
            step4();
            if(isValidPlaintext()){
                print_result();
            }
        }
    }
}
int main() {
    cipher_length = strlen(cipher);
    column_height = cipher_length / 6;
    // column_height = ceil(((double)cipher_length) / 6);
    printf("cipher_length: %d\n", cipher_length);
    printf("column_height: %d\n", column_height);
    step1_2();
    // step1_2_dump();
    // printf("-------\n");
    // step3();
    // step3_dump();
    // step4();
    // step4_dump();
    // step4_preview();
    try_crack();

    // strcpy(step_4_plaintext, "CIASTECZKA");
    // printf("%s @ %s = %d\n", step_4_plaintext, check_keyword, isValidPlaintext());
}