#include <stdio.h>
#include <string.h>
#include "JBfinal2.h"

//main program loop taking user input to move through options
int main(void){
    int choice = -1;
    char inputFile[100] = ""; 
    char outputFile[100] = "";
    char encodeString[100] = "";
    int shiftNum = 0;
    while (choice != QUIT){ //while 0 is not selected
        choice = menu(inputFile, outputFile, shiftNum);
        printf("Choice: %d\n", choice);
        if (choice == INPUT) {
            input(inputFile);
            printf("Input: %s\n", inputFile);
        } else if (choice == OUTPUT) {
            output(outputFile);
            printf("Output: %s\n", outputFile);
        }
        else if (choice == SHIFT) {
            shiftNum = shift(shiftNum);
            printf("Shift amount: %d\n", shiftNum);
        }
        else if (choice == ENCODE) {
           encode(inputFile, outputFile, shiftNum);
        }
        printf("\n");
    }
	return 0;
}

//menu display of valid options
int menu(char* inputFile, char* outputFile, int shiftNum){
    int option;
    printf("Text Encoder Service\n\n");
    printf("1 Enter name of input file (Current file: \"%s\") \n", inputFile);
    printf("2 Enter name of output file (Current file: \"%s\") \n", outputFile);
    printf("3 Enter number of characters data should be shifted (Shift amount: %d) \n", shiftNum);
    printf("4 Encode the text\n\n");
    printf("0 Quit this program.\n\n");
    while((scanf(" %d", &option) != 1) || (option < 0) || (option > 4)) { //while there is an invalid choice
        fflush(stdin);
        printf("That selection isn't valid. Please try again.\n");
        printf("Your choice? ");
    }
    return option;
}

//prompts user to select a .txt file to be read
void input(char* inputFile){
    char fileType[5];
    printf("What file do you wish to read? ");
    scanf("%s", inputFile); 
    int validFileType = 0;
    while (validFileType == 0) {
        int length = strlen(inputFile);
        if (strcmp(inputFile + length - 4, ".txt") == 0) { //if filetype is identical to .txt
            FILE *inputFilePointer = fopen(inputFile, "r");
            if (inputFilePointer != NULL) { //if a file exists in the system
            	validFileType=1;
                fclose(inputFilePointer);
                break;
            } else {
                printf("Unable to find %s. Please enter a valid file name: ", inputFile);
                scanf("%s", inputFile);
            }
        } else {
            printf("Invalid file type. Please enter a file with '.txt' extension: ");
            scanf("%s", inputFile);
        }
    }
}

//prompts user to select a file to be written
void output(char* outputFile){
    printf("Name the encoded text file: ");
    scanf("%s", outputFile); 
    FILE *outputFilePointer = fopen(outputFile, "w");
    fclose(outputFilePointer);
}

//asks user to input an integer and returns an integer value to shift the input text by
int shift(int shiftNum) {
    int isThisADigit = 0;
    while (isThisADigit == 0) { //while isThisADigit = 0 (false)
        printf("Enter the shift amount: ");
        isThisADigit = scanf("%d", &shiftNum); //if isThisADigit = 1 (true), return shiftNum
        if (isThisADigit == 0) {
            printf("Invalid shift amount. Please enter an integer.\n");
            while (getchar() != '\n'); //prevent infinite loop 
        }
    }
    return shiftNum;
}

//shifts inputFile text by shiftNum and outputs shifted text to outputFile
void encode(char* inputFile, char* outputFile, int shiftNum){
	FILE *inputFilePointer; //input pointer
	FILE *outputFilePointer; //output pointer
	unsigned char ch; 
	unsigned char encoded[100];
	int i;
	inputFilePointer=fopen(inputFile, "r");
	if (outputFile[0] != '\0') { //encodes text if there is an output file
		while ((fscanf(inputFilePointer,"%c", &ch)) != EOF){ //if fp exists and until end of file (EOF)
			printf("%c", ch); //prints input file contents to stdout
		    if (isalpha(ch)) {
				if (islower(ch)) {
					int lower = 'a' + (ch - 'a' + shiftNum) % 26; //Example: 97+(ch-97+shiftNum) % 26  --> 97+(97-97+1)%26 = 98 = b
		    		if(lower<'a'){
		    			lower = 'z' + (ch - 'z' + shiftNum) % 26;
		    			encoded[i] = lower;
					}
					else{
						encoded[i] = lower; 
					}
		    	}
				else if (isupper(ch)) {
					int upper = 'A' + (ch - 'A' + shiftNum) % 26; //Example: 65+(ch-65+shiftNum) % 26 --> 65+(65-65+1)%26 = 66 = B
		        	if(upper<'A'){
		    			upper = 'Z' + (ch - 'Z' + shiftNum) % 26;
		    			encoded[i] = upper;
					}
					else{
		        		encoded[i] = upper;
					}
		    	}
			}
			else{
				encoded[i] = ch;
			}
		i++;
		}
	}
    fclose(inputFilePointer);
	if (inputFilePointer == NULL) {
		printf("You must select an input file. Ensure option 1 is filled out.\n");
	}
	if(outputFile[0] == '\0'){
		printf("You must select an output file. Ensure option 2 is filled out.\n");
	}
	outputFilePointer = fopen(outputFile, "w");
	if (inputFilePointer != NULL && outputFile[0] != '\0'){ 
		printf("\n");
		for (i = 0; i < strlen(encoded); i++) { //prints encoded text to console
			printf("%c", encoded[i]);
		}
		printf("\n");
		fprintf(outputFilePointer, "%s", encoded); //prints encoded text to output file
		printf("%s encoded to %s \n\n%s\n\n", inputFile, outputFile, STARS);
		fclose(outputFilePointer);
	}	
}

