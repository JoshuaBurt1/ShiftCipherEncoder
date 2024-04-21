/* final.h -- constants and declarations for final.c */

//menu options
#define QUIT 0
#define INPUT 1
#define OUTPUT 2
#define SHIFT 3
#define ENCODE 4
#define STARS "***********************************************************************"

//menu display of valid options
int menu(char* inputFile, char* outputFile, int shiftNum);

//prompts user to select .txt file to be read
void input(char* inputFile);

//prompts user to select .txt file to be written
void output(char* outputFile);

//asks user to input an integer and returns an integer value to shift the input text by
int shift(int number);

//shifts inputFile text by shiftNum and outputs shifted text to outputFile
void encode(char* inputFile, char* outputFile, int shiftNum);

