// Name: Tyler Gauntlett & William Young
// Course: Systems Software - COP3402
// Date: 7/16/2016

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRING_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define NUM_OF_SPECIAL_SYMBOLS 16
#define NUM_OF_RESERVED_WORDS 14
#define NUM_OF_TOKENS 33
#define NUM_OF_INVALID_SYMBOLS 6

void getCleanInput();
void getLexemeList();
int tokenizeAssigner(char *inputArray);
int isReservedWord(char *word);
int isIdentifierWord(char *word);
int isValidNumber(char *inputWord);
int isSpecialSymbol(char *word);
int reservedWordConverter(int index);
int specialSymbolConverter(int index);
void getTokenInputArray();
char* getNextToken();
char* statement(char* token);
void ERROR(int msg);
char* condition(char * token);
char* expression(char* token);
char* term(char* token);
char* factor(char* token);
void printTokenArray();
int tableLookup(char* token);
int tableInsert(int kind,char* name, int val, int level, int addr);
void block(char* token, int level);
void getComplileRequest(int request);
int relation(int digtoken);

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct symbol{
	int kind;
	char name[12];
	int val;
	int level;
	int addr;
}symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

// Declare constants used below.
const char *tokens[NUM_OF_TOKENS] = { "nulsym", "identsym", "numbersym", "plussym", "minussym",
"multsym", "slashsym", "oddsym", "eqsym", "neqsym", "lessym", "leqsym",
"gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", "semicolonsym",
"periodsym", "becomessym", "beginsym", "endsym", "ifsym", "thensym",
"whilesym", "dosym", "fsym", "constsym", "varsym", "procsym", "writesym",
"readsym", "elsesym" };
const char *reservedWords[NUM_OF_RESERVED_WORDS] = { "const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"};
const char *specialSymbols[NUM_OF_SPECIAL_SYMBOLS] = { "<>", "-", "*", "/", "(", ")", ":=", "=", ",", ".", "<=", ">=", "<", ">", ";", "+" };
const char *invalidSymbols[NUM_OF_INVALID_SYMBOLS] = { "$", "#", "&", "!", "~", "?" };
const int   relationSymbols[6] = {9,10,11,12,13,14};
//Global Variables
char tokenArray[10000][100];
int tokenIndex = 0;
int tokenArraySize = 0;
int currentTableIndex = 0;
int errorOccurred = 0;

// LexemeTable and LexemeList are reliant on CleanInput's
// output file. They must go in this order.
int main(int argc, char *argv[]) {

    if( argc == 2 ) {
        //Generates Clean Input File from Input File
        getCleanInput();
        //Generates Lexeme List File from Clean Input File
        getLexemeList();
        //Fills TokenArray from Lexeme List
        getTokenInputArray();
        //Checks Syntax of Lexeme List
        getComplileRequest(atoi(argv[1]));
    }
    else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
    }
    else {
        printf("One argument expected.\n");
    }

	return 0;
}

// Show you what's inside the global array.
void printTokenArray(){
	// Example of printing the array using the getNextToken() function.
	int i = 0;
	// TODO: Using -1 to signify the end of the array. Wont let me using \0. Fix this later.
	while (strcmp(tokenArray[i], "-1")){
		char* token = getNextToken();

		printf("%s ", token);
		i++;
	}
	printf("\n");
}

// Assign tokens based on what category the input registers to.
// Start with reserved words and symbols. Variables names must go last.
int tokenizeAssigner(char *input){

	if (isReservedWord(input)){
		return reservedWordConverter(isReservedWord(input));
	}

	// Pass the invalid symbol code back to the function.
	if (isSpecialSymbol(input) == -2){
		return -2;
	}

	// Return the symbol token based on what symbol is used.
	if (isSpecialSymbol(input)){
		return specialSymbolConverter(isSpecialSymbol(input));
	}

	// Pass the invalid number code back to the function.
	if (isValidNumber(input) == -2){
		return -2;
	}

	// If it is a valid number, return 3 for valid numbers.
	if (isValidNumber(input)){
		return 3;
	}

	// Pass the invalid variable code back to the function.
	if (isIdentifierWord(input) == -2){
		return -2;
	}

	// If it is not a reserved word and is a identifier, return
	// 2 for identifiers.
	if (!isReservedWord(input) && isIdentifierWord(input)){
		return 2;
	}
	return -1;
}

// Check if the word starts with a alpha character and is
// followed by alpha numerical characters. Check for length > 11.
int isIdentifierWord(char *word){
	int i = 0, length = strlen(word);

	// If invalid  size, return error code -2.
	/*if (length > MAX_STRING_LENGTH){
		printf("%s",word);
		printf("OVER 12!!");
		return -2;
	}*/

	// Verify the string has some length.
	if (length > 0){
		// Check if the first letter is alpha and
		// all others are alphanumeric.
		if (isalpha(word[i])){
			for (; i < length; i++){
				if (isalnum(word[i])){
					continue;
				}
				else{
					return 0;
				}
			}
			return 1;
		}
		else{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

// Returns 0 for not a reserved word or the index + 1 of the reserved word.
// This is later subtracted out in the corresponding switch statement.
int isReservedWord(char *word){
	int i, found = 0;
	for (i = 0; i < NUM_OF_RESERVED_WORDS; i++){
		if (strcmp(word, reservedWords[i]) == 0){
			found = i + 1;
		}
	}
	return found;
}

// Loops up the reserved word base on the corresponding index
// of the reserved words array.
int reservedWordConverter(int index){
	switch (index-1)
	{
	// const
	case 0:
		return 28;
	// var
	case 1:
		return 29;
	// procedure
	case 2:
		return 30;
	// call
	case 3:
		return 27;
	// begin
	case 4:
		return 21;
	// end
	case 5:
		return 22;
	// if
	case 6:
		return 23;
	// then
	case 7:
		return 24;
	// else
	case 8:
		return 33;
	// while
	case 9:
		return 25;
	// do
	case 10:
		return 26;
	// read
	case 11:
		return 32;
	// write
	case 12:
		return 31;
	// odd
	case 13:
		return 8;
	default:
		return -1;
	}
}

// Check if it is a special symbol. Add one to the index to account for it
// being the first element in the list.
int isSpecialSymbol(char *word){
	int i, found = 0;
	for (i = 0; i < NUM_OF_SPECIAL_SYMBOLS; i++){
		if (strcmp(word, specialSymbols[i]) == 0){
			found = i + 1;
		}
	}
	for (i = 0; i < NUM_OF_INVALID_SYMBOLS; i++){
		if (strcmp(word, invalidSymbols[i]) == 0){
			found = -2;
		}
	}

	return found;
}

// Get the corresponding value associated with the special symbol.
// Subtract 1 to counteract the extra one assigned in the function above.
int specialSymbolConverter(int index){
	switch (index-1)
	{
	// <>
	case 0:
		return 10;
	// -
	case 1:
		return 5;
	// *
	case 2:
		return 6;
	// /
	case 3:
		return 7;
	// (
	case 4:
		return 15;
	// )
	case 5:
		return 16;
	// :=
	case 6:
		return 20;
	// =
	case 7:
		return 9;
	// ,
	case 8:
		return 17;
	// .
	case 9:
		return 19;
	// <=
	case 10:
		return 12;
	// >=
	case 11:
		return 14;
	// <
	case 12:
		return 11;
	// >
	case 13:
		return 13;
	// ;
	case 14:
		return 18;
	// +
	case 15:
		return 4;
	default:
		return -1;
	}
}

// Returns 1 if the number is of valid size (less than 5 digits).
int isValidNumber(char *inputWord){
	int isNumber = 1;
	int j;
	int length = strlen(inputWord);
	// Check if each number in the total number is an integer.
	for (j = 0; j < length; j++){
		if (isdigit(inputWord[j]))
		{
			continue;
		}
		else
		{
			isNumber = 0;
		}
	}
	// At this point, we can be sure it is a number.
	if (isNumber){
		int count = 0;
		int tempNumber = atoi(inputWord);
		while (tempNumber != 0)
		{
			tempNumber /= 10;
			++count;
		}
		if (count > MAX_NUMBER_LENGTH){
            ERROR(25);
			return -2;
		}
		else
			return isNumber;
	}
	else
		return isNumber;
}

// Removes all comments from the input file.
void getCleanInput() {
	FILE *inputFile, *outputFile;
	char outputFileName[] = "cleaninput.txt";
	char *mode = "r";

	inputFile = fopen("input.txt", mode);
	outputFile = fopen(outputFileName, "w");

	if (inputFile == NULL)
	{
		perror("Error");
		return;
	}
	else {
		int openComment = 0;
		int closeComment = 0;
		int endOfComment = 0;
		while (!feof(inputFile))
		{
			char *inputWord = (char*)malloc(sizeof(char*) * 100);
			int i;
			//loops through inputWord array and sets EOF char to '\0'
			//this allows for printing the last line of the input
			for(i = 0; i < 100; i++){
                if (inputWord[i] == feof(inputFile)){
                    inputWord[i] = '\0';
                }
			}
			char whitespace;
			fscanf(inputFile,"%s%c", inputWord, &whitespace);

			// Check if the word contains a close comment at the beginning or end.
			int length = strlen(inputWord);
			if ((inputWord[0] == '*' && inputWord[1] == '/') || (inputWord[length - 2] == '*' && inputWord[length - 1] == '/')){
				closeComment++;
			}

			// Remove the content inside the comment from the output file.
			while ((openComment > closeComment) && !feof(inputFile)){
				int length = strlen(inputWord);
				if ((inputWord[0] == '*' && inputWord[1] == '/') || (inputWord[length-2] == '*' && inputWord[length-1] == '/')){
					closeComment++;
					endOfComment = 1;
				}
				else{
					fscanf(inputFile, "%s%c", inputWord, &whitespace);
				}
			}

			// Check if the closing comment brace occurs at the end of a file (prevents printing random chars
			// because inputWord is uninitialized).
			if ((openComment > closeComment) && feof(inputFile)){
				int length = strlen(inputWord);
				if ((inputWord[0] == '*' && inputWord[1] == '/') || (inputWord[length - 2] == '*' && inputWord[length - 1] == '/')){
					closeComment++;
					endOfComment = 1;
				}
			}

			// Prints a new line in the event the closing comment brace is at the end of the line.
			if (endOfComment){
				endOfComment = 0;
				if (whitespace == '\n'){
					fprintf(outputFile, "\n");
				}
				continue;
			}

			// Check if the word begins with the open comment symbols.
			if (inputWord[0] == '/' && inputWord[1] == '*'){
				openComment++;
				free(inputWord);
				continue;
			}

			// Writes word to output file and checks if the line needs a new line char.
			fprintf(outputFile, "%s ", inputWord);
			if (whitespace == '\n'){
				fprintf(outputFile, "\n");
			}
			free(inputWord);
		}
	}
	fclose(inputFile);
	fclose(outputFile);
}

void getLexemeList() {
	FILE *inputFile, *outputFile;
	char outputFileName[] = "lexemelist.txt";
	char *mode = "r";

	inputFile = fopen("cleaninput.txt", mode);
	outputFile = fopen(outputFileName, "w");

	if (inputFile == NULL)
	{
		perror("Error");
	}
	else {

		while (!feof(inputFile))
		{
			char *inputWord = (char*)malloc(sizeof(char*) * 100);
			char whitespace;
			fscanf(inputFile, "%s%c", inputWord, &whitespace);

			// Break if at the end of the input.
			if (feof(inputFile)){
				break;
			}

			int tokenValue = tokenizeAssigner(inputWord);
			// If the token value is -2, than there is an invalid number
			// or variable. Exit program without printing.
			/*if (tokenValue == -2){
				continue;
			}*/
			// If it came back with a valid token value, print it.
			if (tokenValue != -1){
				if (tokenValue == 2 || tokenValue == 3){
					fprintf(outputFile, "%d %s ", tokenValue, inputWord);
				}
				else{
					fprintf(outputFile, "%d ", tokenValue);
				}
			}
			// If it was not valid, tear the word down and try substrings.
			else{
				// Begin making words starting with the first and last element.
				// If it does not exist, move the end counter down. If it does,
				// switch the beginning with the end and end to length of total size.
				int start = 0, length = strlen(inputWord), end = length;
				while (start < length) {
					char *temp = (char*)malloc(sizeof(char*) * 100);

					memcpy(temp, &inputWord[start], (sizeof(char)*(end - start)));
					temp[end - start] = '\0';

					int tokenValue = tokenizeAssigner(temp);

					if (tokenValue == -1){
						end--;
					}
					else{
						if (tokenValue == 2 || tokenValue == 3)
							fprintf(outputFile, "%d %s ", tokenValue, temp);
						else{
							fprintf(outputFile, "%d ", tokenValue);
						}
						start = end;
						end = length;
					}

					free(temp);
				}
			}

			free(inputWord);
		}
	}

	fclose(inputFile);
	fclose(outputFile);
}

void getTokenInputArray(){

	FILE *inputFile;
	char *mode = "r";

	inputFile = fopen("lexemelist.txt", mode);

	if (inputFile == NULL)
	{
		perror("Error");
	}
	else {

		int i = 0;
		while (!feof(inputFile))
		{
			char inputWord[100];
			fscanf(inputFile, "%s", inputWord);

			if (feof(inputFile)){
				strcpy(tokenArray[i], "-1");
				break;
			}
			strcpy(tokenArray[i], inputWord);
			tokenArraySize++;
			i++;
		}
	}

	fclose(inputFile);
}

char* getNextToken(){
	if (tokenIndex > tokenArraySize){
		return "error";
	}
	return tokenArray[tokenIndex++];
}

int getSymId(char* symbol){
	int i;
	for (i = 0; i < NUM_OF_TOKENS; i++){
		if (strcmp(symbol, tokens[i]) == 0){
			// Array starts at 0, symbol id's start at 1.
			return i+1;
		}
	}
	return 0;
}

//returns token, returns "-1" if errors
//the -1 return is for future modification, the program currently exits on error
//Converts token to digtoken by atoi and compares to SymIds
char* statement(char* token){
	int digtoken = atoi(token);
	//if token is an ident symbol then checks for template
	//ident symbol - valid ident name - becomes symbol - expression
	if (digtoken == getSymId("identsym")){
		//call getToken function and place into token
		token = getNextToken();
        int kind = tableLookup(token);
		if (!kind){
			ERROR(11);
			return "-1";
		}
        if(kind == 1 || kind == 3){
            ERROR(12);
            return "-1";
        }
		token = getNextToken();
		//converts token to int and stores in digtoken
		digtoken = atoi(token);
		if (digtoken != getSymId("becomessym")){
			//call error function
			ERROR(13);
			return "-1";
		}
		//call getToken function and place into token
		token = getNextToken();
		//call expression function
		token = expression(token);
		return token;
	}
	//if token is a call symbol then checks for template
	//call symbol - ident symbol - valid ident name - semicolon
	else if (digtoken == getSymId("fsym")){
		token = getNextToken();
		digtoken = atoi(token);
		if (digtoken != getSymId("identsym")){
			//call error function
			ERROR(14);
			return "-1";
		}
		token = getNextToken();
        
        int kind = tableLookup(token);
        if (!kind){
            ERROR(11);
            return "-1";
        }
        if(kind == 1 || kind == 2){
            ERROR(15);
            return "-1";
        }
        
		token = getNextToken();
		return token;
	}
	//if token is a begin symbol then checks for template
	//begin symbol - statement followed by semicolon symbol (repeat) - end symbol
	else if (digtoken == getSymId("beginsym")){
		token = getNextToken();
		//call statement, recursively
		token = statement(token);
		//Missing getNextToken Call?
		digtoken = atoi(token);
		while(digtoken == getSymId("semicolonsym")){
			token = getNextToken();
			digtoken = atoi(token);
			if(digtoken != getSymId("endsym")){
                token = statement(token);
                digtoken = atoi(token);
			}
            if(digtoken != getSymId("semicolonsym") && digtoken != getSymId("endsym")){
                ERROR(7);
            }
		}
		digtoken = atoi(token);
		if (digtoken != getSymId("endsym")){
			ERROR(19);
			return "-1";
		}
		token = getNextToken();
		return token;
	}
	//if token is an if symbol then checks for template
	//if symbol - condition - then symbol = statement
	else if (digtoken == getSymId("ifsym")){
		token = getNextToken();
		//Call Condition function
		token = condition(token);
		digtoken = atoi(token);
		if (digtoken != getSymId("thensym")){
			ERROR(16);
			return "-1";
		}
		token = getNextToken();
		token = statement(token);
		digtoken = atoi(token);
		if(digtoken == getSymId("elsesym")){
            token = getNextToken();
            token = statement(token);
		}
		return token;
	}
	//if token is a while symbol then checks for template
	//while symbol - condition - do symbol - statement
	else if (digtoken == getSymId("whilesym")){
		token = getNextToken();
		//Call Condition function
		token = condition(token);
		digtoken = atoi(token);
		if (digtoken != getSymId("dosym")){
			ERROR(18);
			return "-1";
		}
		token = getNextToken();
		token = statement(token);
		return token;
	}
	else if(digtoken == getSymId("writesym")){
        token = getNextToken();
        digtoken = atoi(token);
        if(digtoken != getSymId("identsym")){
            ERROR(27);
            return "-1";
        }
        token = getNextToken();
        if (!tableLookup(token)){
			ERROR(11);
			return "-1";
		}
		token = getNextToken();
		digtoken = atoi(token);
		if(digtoken != getSymId("semicolonsym")){
            ERROR(5);
            return "-1";
		}
        return token;
	}
	else if(digtoken == getSymId("readsym")){
        token = getNextToken();
        digtoken = atoi(token);
        if(digtoken != getSymId("identsym")){
            ERROR(27);
            return "-1";
        }
        token = getNextToken();
        if (!tableLookup(token)){
			ERROR(11);
			return "-1";
		}
		token = getNextToken();
		digtoken = atoi(token);
		if(digtoken != getSymId("semicolonsym")){
            ERROR(5);
            return "-1";
		}
        return token;
	}
	return token;
}

//ERROR FUNCTION
void ERROR(int msg){
	errorOccurred = 1;
	switch (msg){
	case 1:
		printf("Error number %d, Use = instead of := \n", msg);
		break;
	case 2:
		printf("Error number %d, = must be followed by a number.\n", msg);
		break;
	case 3:
		printf("Error number %d, Identifier must be followed by =\n", msg);
		break;
	case 4:
		printf("Error number %d, Const,var,procedure must be followed by identifier.\n", msg);
		break;
	case 5:
		printf("Error number %d, Semicolon or comma missing.\n", msg);
		break;
	case 6:
		printf("Error number %d, Incorrect symbol after procedure declaration.\n", msg);
		break;
	case 7:
		printf("Error number %d, Statement expected.\n", msg);
		break;
	case 8:
		printf("Error number %d, Incorrect symbol after statement part in block.\n", msg);
		break;
	case 9:
		printf("Error number %d, Period expected.\n", msg);
		break;
	case 10:
		printf("Error number %d, Semicolon between statement missing.\n", msg);
		break;
	case 11:
		printf("Error number %d, Undeclared identifier.\n", msg);
		break;
	case 12:
		printf("Error number %d, Assignment to constant or procedure is not allowed.\n", msg);
		break;
	case 13:
		printf("Error number %d, Assignment operator expected.\n", msg);
		break;
	case 14:
		printf("Error number %d, call must be followed by an identifier.\n", msg);
		break;
	case 15:
		printf("Error number %d, Call of a constant or variable is meaningless.\n", msg);
		break;
	case 16:
		printf("Error number %d, then expected.\n", msg);
		break;
	case 17:
		printf("Error number %d, Semicolon or } expected.\n", msg);
		break;
	case 18:
		printf("Error number %d, do expected.\n", msg);
		break;
	case 19:
		printf("Error number %d, Incorrect symbol following statement.\n", msg);
		break;
	case 20:
		printf("Error number %d, Relational operator expected.\n", msg);
		break;
	case 21:
		printf("Error number %d, Expression must not contain a procedure identifier.\n", msg);
		break;
	case 22:
		printf("Error number %d, Right parenthesis missing\n.", msg);
		break;
	case 23:
		printf("Error number %d, The preceding factor cannot being with this symbol.\n", msg);
		break;
	case 24:
		printf("Error number %d, An expression cannot begin with this symbol.\n", msg);
		break;
	case 25:
		printf("Error number %d, This number is too large.\n", msg);
		break;
	case 26:
		printf("Error number %d, Symbol Table Overflow.\n",msg);
		break;
    case 27:
        printf("Error number %d, Read/Write operation must be followed by identifier.\n",msg);
        break;
    case 28:
        printf("Error number %d, Identifier too long.\n",msg);
        break;
	default:
	    //Errors are hardcoded, default is implemented on rare case that everything  breaks
		printf("EVERYTHING IS BROKEN!");
	}
	//exits program if error occurred
	exit(-1);
}

char* condition(char * token){
	int digtoken = atoi(token);
	if (digtoken == getSymId("oddsym")){
		token = getNextToken();
		token = expression(token);
		return token;
	}
	//token was not an odd symbol therefore checks that it follows template
	//expression - relational symbol - expression
	else{
		token = expression(token);
		digtoken = atoi(token);

		//checks that the token is a relational token
		if (!relation(digtoken)){
			ERROR(20);
		}
		token = getNextToken();
		//relational token has been found, calls expression
		token = expression(token);
		//returns current token up the chain
		return token;
	}
}
//Checks for Template
//(+ | -) token {+|- token}
char* expression(char* token){
	int digtoken = atoi(token);
	if (digtoken == getSymId("plussym") || digtoken == getSymId("minussym")){
		token = getNextToken();
	}
	token = term(token);
	digtoken = atoi(token);
	while (digtoken == getSymId("plussym") || digtoken == getSymId("minussym")){
		token = getNextToken();
		token = term(token);
		digtoken = atoi(token);
	}
	return token;
}
//Checks for Template
//factor - {(*|/  factor)}
char* term(char* token){
	token = factor(token);
	int digtoken = atoi(token);
	token = getNextToken();
	digtoken = atoi(token);
	while (digtoken == getSymId("multsym") || digtoken == getSymId("slashsym")){
		token = getNextToken();
		token = factor(token);
		token = getNextToken();
		digtoken = atoi(token);
	}
	return token;
}
//Checks for Template
//identifier symbol - identifier name | number symbol |
//left parenthesis symbol - expression - right parenthesis symbol
char* factor(char* token){
	int digtoken = atoi(token);
	if (digtoken == getSymId("identsym")){
		token = getNextToken();
        int kind = tableLookup(token);
        if (!kind){
            ERROR(11);
            return "-1";
        }
        if(kind == 3){
            ERROR(21);
            return "-1";
        }
    }
	else if (digtoken == getSymId("numbersym")){
		token = getNextToken();
	}
	//need int value of left parenthesis
	else if (digtoken == getSymId("lparentsym")){
		token = getNextToken();
		token = expression(token);
		digtoken = atoi(token);
		if (digtoken != getSymId("rparentsym")){
			//right parenthesis expected
			ERROR(22);
			token = getNextToken();
		}
	}
	else{
		ERROR(23);
		return "-1";
	}
	return token;
}

//Searches for identifier in symbol table
//returns 1 if found, returns 0 if not found
int tableLookup(char* token){
	int i;
	for(i = 0; i < currentTableIndex; i++){
		if(strcmp(token, symbol_table[i].name) == 0){
			return symbol_table[i].kind;
		}
	}
	return 0;
}

//Inserts Constant,Variable,or Procedure name into Symbol Table
//returns 0 if Symbol Table is at Max Size, return 1 if insert is performed
int tableInsert(int kind,char* name, int val, int level, int addr){
	//checks current table size
	if(strlen(name)> 12){
        ERROR(28);
	}
	if(currentTableIndex < MAX_SYMBOL_TABLE_SIZE){
		int i;
		//stores kind of symbol (Constant, Variable, or Procedure)
		symbol_table[currentTableIndex].kind = kind;
		for(i = 0; i < 12; i++){
			symbol_table[currentTableIndex].name[i] = name[i];
		}
		//stores value if it is a constant
		if(kind == 1){
            symbol_table[currentTableIndex].val = val;
		}
		symbol_table[currentTableIndex].level = level;
		//stores address if it is not a constant (a procedure or variable)
		if(kind != 1){
            symbol_table[currentTableIndex].addr = addr;
		}
		currentTableIndex++;
		return 1;
	}
	else{
		return 0;
	}
}
//Checks if token is a Relational Symbol
//Returns 1 if token is a Relational Symbol, returns 0 otherwise
int relation(int digtoken){
	int i;
	for(i = 0; i < 6; i++){
		if(digtoken == relationSymbols[i]){
			return 1;
		}
	}
	return 0;
}

void block(char* token, int level){
	int digtoken = atoi(token);
	//declares and resets addr whenever a block is created
	//this keeps addr as the M address for each new ident to be stored
    int addr = 0;
    //if token is a const symbol then checks for template
    //const symbol - {ident symbol - insert valid ident name - number symbol - value- (comma between idents)} - semicolon
	if (digtoken == getSymId("constsym")){
		do{
			token = getNextToken();
			digtoken = atoi(token);
			if (digtoken != getSymId("identsym")){
				ERROR(4);
			}
			token = getNextToken();
			char * constToken = token;
			//insert const name into symbol table
			//get next token after const name
			token = getNextToken();
			digtoken = atoi(token);
			if (digtoken != getSymId("eqsym")){
                if(digtoken == getSymId("becomessym")){
                    ERROR(1);
                }
                else{
                    ERROR(3);
                }
				ERROR(1);
			}
			token = getNextToken();
			digtoken = atoi(token);
			if (digtoken != getSymId("numbersym")){
				ERROR(2);
			}
			digtoken = atoi(getNextToken());
			token = getNextToken();
			//ident is a constant and address is not stored
			//therefore, addr is not incremented
			if(!tableInsert(1, constToken, digtoken, level, addr)){
				ERROR(26);
			}
			digtoken = atoi(token);
		}while(digtoken == getSymId("commasym"));
		if (digtoken != getSymId("semicolonsym")){
			ERROR(5);
		}
		token = getNextToken();
		digtoken = atoi(token);
	}
    //if token is var symbol then checks for template
    //var symbol - {ident symbol - insert valid ident name - comma symbol} - semicolon
	if (digtoken == getSymId("varsym")){

        do{
            token = getNextToken();
			digtoken = atoi(token);
			if (digtoken != getSymId("identsym")){
				ERROR(4);
			}
			token = getNextToken();
			digtoken = atoi(token);
			//insert Variable name into Symbol Table
			if(!tableInsert(2, token, digtoken, level, addr)){
				ERROR(26);
			}
			else{
                addr++;
			}
			//grab next token after variable name
			token = getNextToken();
			digtoken = atoi(token);
		}while (digtoken == getSymId("commasym"));
		if (digtoken != getSymId("semicolonsym")){
			ERROR(5);
		}
		token = getNextToken();
		digtoken = atoi(token);
	}
    //if token is a procedure symbol then checks for the template
    //procedure symbol - ident symbol - insert valid ident name - semicolon - block - semicolon - statement
	while (digtoken == getSymId("procsym")){
		token = getNextToken();
		digtoken = atoi(token);
		if (digtoken != getSymId("identsym")){
			ERROR(4);
		}
		token = getNextToken();
        //insert procedure name into symbol table
        if(!tableInsert(3, token, digtoken, level, addr)){
				ERROR(26);
		}
		else{
            addr++;
		}
		token = getNextToken();
		digtoken = atoi(token);
		if (digtoken != getSymId("semicolonsym")){
			ERROR(5);
		}
		//grab token after procedure
		token = getNextToken();
		//Calls Block, increments the Lexicographical
		block(token, level+1);
		if (digtoken != getSymId("semicolonsym")){
			ERROR(5);
		}
		token = getNextToken();
		digtoken = atoi(token);
	}
	token = statement(token);
}

//routes request from args
void getComplileRequest(int request){
	switch (request)
	{

	case -1:
		printTokenArray();
		break;
	case -2:
		block(getNextToken(),0);
		if(atoi(tokenArray[tokenIndex-1]) != getSymId("periodsym")){
            ERROR(9);
		}
		if (!errorOccurred){
			printf("The program successfully compiled with no errors\n");
		}
		break;
	default:
		printf("Invalid command. Please try again.");
		break;
	}

}
