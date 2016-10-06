// Team Name: Compiler Group 13
// Alexander deCurnou
// Peter Hoang
// Shuaicheng Ma
// Stephen Rigdon

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 256
#define MAX_CHAR_PER_LINE 256


typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

typedef struct node
{
    char *word;
    struct node *next;
} node;

const char *keyword[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
"odd", "procedure", "read", "then", "var", "while", "write"};

const int keyword_type[] = {nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym,
ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

// arithmetic operators
// comparison operators (note that = is also used to assign the value to a constant)
// assignment and some special symbols
const char *symbols[] = {"+","-","*","/","=","<>","<=","<",">",">=",":=",
",",";",".","(",")"};


// read input file
// put everything which includes newline, space to 1D char array
// build 1D char array, so we can use build-in string function
char* readInput(const char* fileName)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    pFile = fopen ( fileName , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */
    // terminate
    fclose (pFile);

	return buffer;
}

// replace comments with space from source code
// output raw source code
char* removeComment(char* sourceCode)
{
    // make a copy of source code
    char *raw = (char*)malloc(strlen(sourceCode)+1);
    strncpy(raw,sourceCode,strlen(sourceCode)+1);

    if(strstr(raw,"*/") && !strstr(raw,"/*"))
    {
        fprintf(stderr,"missing open comment symbol /*\n");
        exit(1);
    }
    //search occurrence of open comment symbol /*
    char *open = strstr(raw,"/*");
    char *close = strstr(raw,"*/");
    while(open)
    {

        // if close comment symbol exist,
        // replce comment with space
        // else throw error message
        if(close)
        {
            // if close comment symnol */ occurrs before open comment symbol /*
            // throw error message
            if(open > close)
            {
                fprintf(stderr,"close comment symnol */ occurrs before open comment symbol /*\n");
                exit(1);
            }
            // memset(open,' ',strlen(open)-strlen(close)+2);
            // replace all character in comment with space character
            // except new line character
            int size = strlen(open)-strlen(close)+2;
            int i;
            for(i=0;i<size;i++)
            {
                if(open[i] != '\n')
                    open[i] = ' ';
            }
        }
        else
        {
            fprintf(stderr,"missing close comment symbol */\n");
            exit(1);
        }
        close = strstr(open+2,"*/");
        open = strstr(open+2,"/*");
        if(close && !open)
        {
            fprintf(stderr,"missing open comment symbol /*\n");
            exit(1);
        }
    }
    return raw;
}

node *createNode();
node *letter(char letter, node *end, FILE *input);
node *number(char ldigit, node *end, FILE *input);
node *other(char firstSymbol, node *end, FILE *input);
void printTable(char *text);




int main(int argc, char *argv[])
{
    int clean = 0;
    int source = 0;
    if(argc < 2) 
    {
        fprintf(stderr, "usage: ./lexer filename\n"); 
        exit(1);
    }
    
    int i = 0;
    for(i = 1; i < argc - 1; i++)
    {
        if(strcmp(argv[i],"--source") == 0)
            source = 1;
        else if(strcmp(argv[i],"--clean") == 0)
            clean = 1;
        else
        {
            fprintf(stderr,"unknown argument\n");
            exit(1);
        }
        
    }
    
    char *sourceCode = readInput(argv[argc-1]);
    char *raw = removeComment(sourceCode);
    
    if(source)
        printf("source code:\n------------\n%s\n",sourceCode);
    if(clean)
        printf("source code without comments:\n-----------------------------\n%s\n",raw);


    //Variables
    int c;
    char character;

    node *front, *end;
    front = end= createNode();

    FILE *input = fopen(argv[1], "r");

    //Get very first character in input file
    c = fgetc(input);

    //Depending on what the character c is depends on what case should be implemented
    while(c != EOF)
    {
        if(isalpha(c))
        {
            character = c;
            end = letter(character, end, input);
            c = fgetc(input);
        }

        else if(isdigit(c))
        {
            end = number(c, end, input);
            c = fgetc(input);
        }
        else if(ispunct(c))
        {
            character = c;
            end = other(character, end, input);
            c = fgetc(input);
        }
        else
        {
            c = fgetc(input);
        }



    }



    //Spacing
    printf("\n");

    //Heading for token table
    printf("tokens:\n");
    printf("-------\n");

    //Print out the token table
    for(; front->next != NULL; front = front->next)
    {
        printTable(front->word);
    }



	return 0;
}


//Function to create a new node
node *createNode()
{
    node *newNode = malloc(sizeof(node));

    newNode ->next = NULL;

    return newNode;
}

//Case where first character is a letter
node *letter(char letter, node *end, FILE *input)
{
    //Variables
    int length = 12;
    int nextLetter;
    int position = 1;

    //Create memory for new word variable
    char *word = calloc(length+1, sizeof(char));

    //Copy first letter
    strcpy(word, "");
    word[0] = letter;

    //Read the next character of the input file
    nextLetter = fgetc(input);

    //While next character is a letter or number, we add it to the word
    while(isalpha(nextLetter) || isdigit(nextLetter))
    {
        //In case more space is needed for the word
        if (position >= length)
        {
            length *= 2;
            word = realloc(word, length+1);
        }

        word[position] = nextLetter;
        position++;

        nextLetter = fgetc(input);
    }

    //Copy word to link list
    end->word = malloc(strlen(word) + 1);
    strcpy(end->word, word);
    end->next = createNode();

    //Free the space
    free(word);

    //If we reach a whitespace, go back so file can be read again
    if(nextLetter != EOF)
        fseek(input, -1, SEEK_CUR);


    //return the linked list
    return end->next;

}

//Case where first character starts with a number
node *number(char digit, node *end, FILE *input)
{
    //Variables
    int maxDigits = 5;
    int position = 1;
    int nextDigit;

    //Reserve space for the number
    char *word = calloc(maxDigits+1, sizeof(char));
    strcpy(word, "");
    word[0] = digit;

    //Get next digit of the number or possibly a whitespace
    nextDigit = fgetc(input);

    //Continue to look and add digits to the number if possible
    while( isdigit(nextDigit))
    {
        //If needed for extra memory, add extra space
        if(position >= maxDigits)
        {
            maxDigits *= 2;
            word = realloc(word, maxDigits+1);

        }

        word[position] = nextDigit;
        position++;

        //Get next digit or whitespace
        nextDigit = fgetc(input);
    }

    //Error if a letter follows the number
    if (isalpha(nextDigit))
    {
        printf("Error: Letter follows a number.\n");
        exit(1);
    }

    //Copy number to word variable
    end-> word = malloc(strlen(word) + 1);
    strcpy(end->word, word);
    end->next = createNode();


    //Go back on the cursor if needed
    if(nextDigit != EOF)
        fseek(input, -1, SEEK_CUR);

    //Free word variable
    free(word);

    //Return end of the list
    return end->next;

}

//Case where first character is not a letter or a number
node *other(char firstSymbol, node *end, FILE *input)
{
    int maxSym = 2;

    //Create space for symbol string
    char *symbol = calloc(maxSym +1, sizeof(char));
    strcpy(symbol, "");
    symbol[0] = firstSymbol;

    //Switch statement to determine if any cases need to be applied
    switch (firstSymbol)
    {
        //Could be a comment case, we ignore everything until end of comment
        case '/':
        {
                    char nextChar;
                    nextChar = fgetc(input);

                    //Check if it is the start of a comment, if true, we ignore everything inside the comment symbols
                    if(nextChar == '*')
                    {
                        nextChar = fgetc(input);

                        do
                        {
                            while(nextChar != '*')
                            {
                                if(nextChar == EOF)
                                {
                                    printf("No closing comments.\n");
                                    exit(1);
                                }
                                nextChar = fgetc(input);
                            }
                            nextChar = fgetc(input);
                        } while (nextChar != '/');


                    return end;
                    }
                    else
                    {
                        if (nextChar != EOF)
                          fseek(input, -1, SEEK_CUR);
                    }
                    break;



        }

        case '<':
        {
            char nextChar;
            int position = 1;

            nextChar = fgetc(input);

            //Only case is either <> or <=
            if(nextChar == '=' || nextChar == '>')
            {
                symbol[position] = nextChar;
            }
            else
            {
                if(nextChar != EOF)
                    fseek(input, -1, SEEK_CUR);
            }
            break;

        }

        case '>':
        {
            char nextChar;
            int position = 1;

            nextChar = fgetc(input);

            //Only case is either > or >=
            if(nextChar == '=')
            {
                symbol[position] = nextChar;
            }
            else
            {
                if(nextChar != EOF)
                    fseek(input, -1, SEEK_CUR);
            }

            break;
        }

        case ':':
        {
            char nextChar;
            int position = 1;
            nextChar = fgetc(input);

            //Only other case is :=
            if(nextChar == '=')
            {
                symbol[position] = nextChar;
            }
            else
            {
                printf("Invalid symbol.\n");
                exit(1);
            }
        }

        //All of the following symbols dont need much
        case '+':
        case '-':
        case '*':
        case '(':
        case ')':
        case '=':
        case ',':
        case '.':
        case ';':
            break;

        //Any other symbols aren't acceptable so there will be an error
        default :
            printf("Error: Invalid symbol,\n");
            exit(1);

    }

    //Copy symbol to list
    end->word = malloc(strlen(symbol) + 1);
    strcpy(end->word, symbol);
    end->next = createNode();


    //Free space
    free(symbol);

    //Return the list
    return end->next;
}

void printTable(char *text)
{

}
