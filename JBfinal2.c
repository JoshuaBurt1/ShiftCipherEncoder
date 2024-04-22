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
        else if (choice == DECODE) {
           decode(inputFile);
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
    printf("4 Encode the text\n");
    printf("5 Decode the text\n\n");
    printf("0 Quit this program.\n\n");
    while((scanf(" %d", &option) != 1) || (option < 0) || (option > 5)) { //while there is an invalid choice
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

// Function to check if a word matches a list of target words
int wordMatchesList(char* word) {
    char* targetWords[] = {"and", "or", "the", "my", "This"}; // List of target words
    int numTargetWords = sizeof(targetWords) / sizeof(targetWords[0]); // Number of target words
	int i;
    for (i = 0; i < numTargetWords; ++i) {
        if (strcmp(word, targetWords[i]) == 0) {
            return 1; // Match found
        }
    }
    return 0; // No match found
}

// Function to decode text with all possible shift values and check for target words
void decode(char* inputFile) {
    FILE* inputFilePointer; // Input file pointer
    unsigned char ch;
    char word[MAX_WORD_LENGTH]; // Buffer to store a word
    char possibleDecodes[MAX_POSSIBLE_DECODES][100]; // Array to store possible decodes
    int decodeShifts[MAX_POSSIBLE_DECODES]; // Array to store corresponding shift values
    int decodeCount = 0; // Counter for number of possible decodes
    char decoded[100]; // Buffer for decoded string
    int q = 0; // Index for decoded string

    inputFilePointer = fopen(inputFile, "r");
    if (inputFilePointer == NULL) {
        printf("Error: Unable to open input file.\n");
        return;
    }

    // Loop over all words in the input file
    while (fscanf(inputFilePointer, "%s", word) == 1) {
        // Loop over all possible shift values
        int shiftNum;
        for (shiftNum = 0; shiftNum < 26; ++shiftNum) {
            int i = 0; // Index for decoded string

            // Decode the current word
            int j;
            for (j = 0; word[j] != '\0'; ++j) {
                ch = word[j];
                if (isalpha(ch)) {
                    if (islower(ch)) {
                        int lower = 'a' + (ch - 'a' - shiftNum + 26) % 26; // Reverse the shift
                        decoded[i++] = lower;
                    } else if (isupper(ch)) {
                        int upper = 'A' + (ch - 'A' - shiftNum + 26) % 26; // Reverse the shift
                        decoded[i++] = upper;
                    }
                } else {
                    decoded[i++] = ch; // Non-alphabetic characters are left unchanged
                }
            }
            decoded[i] = '\0'; // Null-terminate the decoded string

            // Check if the decoded word matches the target list
            if (wordMatchesList(decoded)) {
                // Add the decoded string and corresponding shift value to the arrays
                strcpy(possibleDecodes[decodeCount], decoded);
                decodeShifts[decodeCount] = shiftNum;
                decodeCount++; // Increment the counter
                break; // Stop further processing for this word
            }
        }
    }

    fclose(inputFilePointer);

    // Print possible decodes to console
    if (decodeCount == 0) {
        printf("No possible decodes found.\n");
    } else {
        printf("Possible decodes:\n");
        int k;
        for (k = 0; k < decodeCount; ++k) {
            printf("%s : %d\n", possibleDecodes[k], decodeShifts[k]);
        }
        
        // Decode and print the entire input text according to the unique shift values
        printf("Decoded text:\n");
        inputFilePointer = fopen(inputFile, "r");
        if (inputFilePointer == NULL) {
            printf("Error: Unable to open input file for decoding.\n");
            return;
        }
        
		while ((fscanf(inputFilePointer,"%c", &ch)) != EOF){ //if fp exists and until end of file (EOF)
            if (isalpha(ch)) {
				if (islower(ch)) {
					int lower  = 'a' + (ch - 'a' - decodeShifts[0]) % 26; //Example: 97+(ch-97+shiftNum) % 26  --> 97+(97-97+1)%26 = 98 = b; (98-97
		    		if(lower<'a'){ //97
		    			lower = 'z' + (ch - 'z' - decodeShifts[0]) % 26; //122+(98-122+3)%26
		    			ch = lower;
					}
					else{
						ch = lower;
					}
		    	}
				else if (isupper(ch)) {
					int upper = 'A' + (ch - 'A' - decodeShifts[0]) % 26; //Example: 65+(ch-65+shiftNum) % 26 --> 65+(65-65+1)%26 = 66 = B
		        	if(upper<'A'){
		    			upper = 'Z' + (ch - 'Z' - decodeShifts[0]) % 26;
		    			ch = upper;
					}
					else{
		        		ch = upper;
					}
		    	}
			}
            if (isprint(ch)) {
        		printf("%c", ch); // Print the decoded character if it's printable
    		}
        }
        fclose(inputFilePointer);
        printf("\n");
        return;
    }
}
