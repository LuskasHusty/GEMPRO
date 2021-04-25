#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //For time calculations

#define CHUNKSIZE 10        //Chunks to read the input buffer with
#define ONEHUNDRED 100      //Value used to do the hashing operation with 3 digit numbers
#define NUMBERSIZE 20000    //The max lenght of the number of operations we can do

//The prototypes for each used functions, we discuss more about then on each of their definitions
int simpleHash(char * number);
int decode(char * line, int tam);
int Exercicio();

time_t calculationStart;

/*
    Problem 02 of the 2000 
    ACM South Amerian Regional
    Collegiate Programming Contest 
    [18/11/2000]

    Some considerations: This is not a good answer because: 
    It's overly complicated for being written in C, and not making use of libs that it should be using
    It doesn't follow the requirements, and prints the answer in between inputs
    I only know it works for sure with the following examples:
    HELL1235670WO1234591561580                                                      --Decoding time:0.001
    PROGRAMMING037124670C123567123567156                                            --Decoding time:0.001
    AND MORE037124903735790278134573712467045612356735792781245612467278            --Decoding time:0.001
    And that is because the hashing algorithm is bad, and it can hit a valid number while searching for a non valid number to validate the zero as a space or a code
    It also means that it should work fine for all valid inputs as long as no zero is used for spacing purposes
*/


int LETTERS[100]; //Our bad 'HASHMAP' That is iniatilized with zeroes for being a global variable

int main(void) //Entry point
{
    clock_t BegunTime = clock();

    //We set our hashmap keys with the result of the hashing operation
    #pragma region "HASHMAP"
    LETTERS[91] = 'A';
    LETTERS[56] = 'B';
    LETTERS[13] = 'C';
    LETTERS[93] = 'D';
    LETTERS[19] = 'E';
    LETTERS[39] = 'F';
    LETTERS[71] = 'G';
    LETTERS[87] = 'H';
    LETTERS[37] = 'I';
    LETTERS[34] = 'J';
    LETTERS[75] = 'K';
    LETTERS[98] = 'L';
    LETTERS[77] = 'M';
    LETTERS[86] = 'N';
    LETTERS[40] = 'O';
    LETTERS[ 9] = 'P';
    LETTERS[59] = 'Q';
    LETTERS[79] = 'R';
    LETTERS[16] = 'S';
    LETTERS[94] = 'T';
    LETTERS[36] = 'U';
    LETTERS[56] = 'V';
    LETTERS[89] = 'W';
    LETTERS[18] = 'X';
    LETTERS[55] = 'Y';
    LETTERS[25] = 'Z';
    #pragma endregion
    //We run the program itself
    Exercicio();

    double calculationTime = (clock() - calculationStart)/(double)CLOCKS_PER_SEC;
    double TotalProgramTime = (clock() - BegunTime)/(double)CLOCKS_PER_SEC;
    printf("\n\nCalculation done in: %g\nProgram executed in: %g", calculationTime, TotalProgramTime);
    return 1;
}

int Exercicio()
{
    int nPhrases = 0;               //Number of phrases(lines) to be decoded
    char temp[NUMBERSIZE];          //Temp char to store the number of nPhrases momentarily
    char *line;                     //The line to be decoded
    char buffer[CHUNKSIZE];         //The read Buffer
    fgets(temp, NUMBERSIZE, stdin); //Gets the number of lines[...]
    nPhrases = atoi(temp);          //Decodes to int
    //Input
    for(int nLines = 0; nLines < nPhrases; nLines++) //A loop for every line
    {
        int tTam = 0; //Temp size of the line
        int tam = 0;  //Size of the line
        line = NULL;  //Clear the garbage that may be in the pointer
        //Gets the line input in a dynamic manner
        //It reads a CHUNKSIZE of the input buffer into the readbuffer then appends it into the line array
        //If the readbuffer doesn't have a character terminator we repeat the loop until it does.
        do
        {
            fgets(buffer, CHUNKSIZE, stdin);
            tTam = strlen(buffer);
            line = realloc(line, tam + tTam);
            strcpy(line + tam, buffer);
            tam += tTam;
        }
        while (tTam == CHUNKSIZE - 1 && (buffer[CHUNKSIZE - 2] != '\n' ));
        calculationStart = clock();
        //Call decode with the line array we just created
        decode(line, tam);

        //After it's done, we free the line array, so no memory leaks occurs
        free(line);
    }
}

#pragma region "1st Hashing results"
/* 1st "Hashing" is done by interpreting the input number as the individual bits that have a high value
e.g. A -> 123457 so the number that represents this input number in binary is 0010111110 that in decimal is 190.
This first result is unique for every combination, and can represent 1024 distinct values.
A - 123457		190
B - 1234567		254
C - 456			112
D - 1580		291
E - 12456		118
F - 1249		534
G - 12459		566
H - 13457		186
I - 37			136
J - 3567		232
K - 13459		570
L - 156			098
M - 12357		174
N - 3579		680
O - 123567		238
P - 1458		306
Q - 12347		158
R - 123459		574
S - 12467		214
T - 278			388
U - 13567		234
V - 1379		650
W - 135790		683
X - 90			513
Y - 1347		154
Z - 23456		124
*/
#pragma endregion
int simpleHash(char * number)
{
    //Does the first "hashing" pass.
    long int preResult = 0;
    for(int i = 0; number[i] != '\0'; i++)
    {
        char n[2];
        n[0] = number[i];
        n[1] = '\0';
        preResult ^= (1 << atoi(n));
    }
    // preResult now contains a unique number for a letter between 0 and 1023 (inclusive)
    // We only need 26 unique keys, so we try to get the number even smaller while trying to not to duplicate keys (Two letters with the same hash/key)

    //My hashing algorithm is really bad, so I already now I'm gonna have a collision with with two pairs of numbers, so I manually fix it
    short fixer = 0;
    if(preResult == 174 || preResult == 388) //if the first hash result is one of these two numbers, each from a different pair that collides
    {
        fixer = 3; // I set this value to 3 so that it will be added to the final hash at the end of the function
    }

    short tDig = preResult/ONEHUNDRED; 
    tDig = tDig%10;
    //I get the third digit of the number, ie 234 so tDig = 2

    preResult = preResult -tDig*ONEHUNDRED; //So now we get only the 2 reaming digits so, as with the last example, we now have 34
    preResult += tDig; //Finally we add to the reamining number, the numerical value of the third digit, so now we have 36
    if(preResult < 0 || preResult > 99)
    {
        // As I'm working with C, and I know this hash is gonna be used to access a array with 100 elements, I have to make sure it doesn't go out of bounds.
        return 0;
    }
    return preResult + fixer; //We add the fixer to the final result

    //In the end this function can return a unique key for every VALID number while staying in a smaller interval ~(0, 99)
}

int decode(char * line, int tam)
{
    char * decodedString = (char *) malloc(sizeof(char)* tam); //The size of the decoded string can not be bigger than the original string, so we can take this shortcut
    int lastChar = 0;           //Were on the decoded string we currently are
    short isNumber = 0;         //A boolean like variable so we can keep track if we were already treating a number or not
    short nSize = 0;            //The size of the number we are treating
    char encodedNumber[12] = "\0\0\0\0\0\0\0\0\0\0\0\0"; //The number we are treating can have 10 chars maximun, so we can ballpark it, 11 should be fine.
    //we iniatialize it to terminators, but we can probably go without it

    for(int i = 0; i < tam + 1; i++)
    {
        if(line[i] > 47 && line[i] < 58) // if the ascii value falls in this range, then we have a number.
        {
            //If this is the start of the number decoding process
            if(!isNumber)
            {
                //We can only start with a zero if it's supposed to be a space
                if(line[i] == '0')
                {
                    decodedString[lastChar] = ' ';
                    lastChar++;
                    continue; //So we can add the space to the decoded string and continue the loop from there
                }
                //If it's actually a letter decoding, we add the first number, and setup the variables ie isNumber = 1 and its size is 0 (nSize)
                nSize = 0;
                encodedNumber[nSize] = line[i];
                isNumber = 1; 
                
            }
            else if(line[i] <= encodedNumber[nSize]) //Current numerical code has ended because we assume the numerical code is always in ascending order or we have a zero.
            {
                nSize++;
                if(line[i]!= '0') //if we do not have a zero
                {                    
                    encodedNumber[nSize] = '\0'; //Terminate the number array
                    int hash = simpleHash(encodedNumber); //pass it to the hashing function
                    char letter = LETTERS[hash];          //lookup the letter it represents in the 'hashmap'
                    decodedString[lastChar] = letter;     //adds the letter it got in the decoded string array
                    lastChar++;                           //increments the char count
                    i--;                                  //the current char has not been processed, so we will redo it
                    isNumber = 0;                         //But this number encoded letter has already been found
                    continue;                             //We can then end this loop instance and go to the next index (that will end up being the same index)
                }
                encodedNumber[nSize] = '\0';              //Before processing the 0 character, we make sure the number we made before adding it is not valid
                int hash = simpleHash(encodedNumber);     //Doing the same proccess to get the letter as described above
                char letter = LETTERS[hash];
                if(letter) //If the letter is not a terminator character (ie int 0 or char '\0') then we got a valid letter, and the zero is a space           
                {
                    decodedString[lastChar] = letter; //So we assign the letter we found to the decodedstring array
                    lastChar++;                       //Increment its counter
                    i--;                              //we are gonna reprocess the zero
                    isNumber = 0;                     //but outside of this number sequence
                    continue;                         //continue the loop from the next index value
                }
                //if the letter is a terminator we know we got it wrong and the zero must be part of the number to be decoded
                encodedNumber[nSize] = line[i];       //so we add the zero to the current numerical code
                encodedNumber[nSize + 1] = '\0';      //terminate it
                hash = simpleHash(encodedNumber);     //hash it
                letter = LETTERS[hash];               //and retrieve the letter
                decodedString[lastChar] = letter;     //add the letter to the decoded string
                lastChar++;                           //increment its counter
                isNumber = 0;                         //end this numerical sequence
            }
            else
            {
                //So this is a number in accending order to the precious, so it must be part of the same numerical sequence
                nSize++;
                encodedNumber[nSize] = line[i];
            }
        }
        else
        {
            if(isNumber) //Current Numerical code has ended, and it doesn't have a zero in it
            {
                encodedNumber[nSize + 1] = '\0';        //we terminate it
                int hash = simpleHash(encodedNumber);   //hash it
                char letter = LETTERS[hash];            //retrieve the letter
                decodedString[lastChar] = letter;       //add it to the decoded string array
                lastChar++;                             //increment its counter
                decodedString[lastChar] = line[i];      //we can process the current character as well, it's not a number after all
                lastChar++;                             //increment the counter
                isNumber = 0;                           //end the numerical sequence
            }
            else
            {
                //this is not a number, so we just add the characters to the decoded string array as is.
                decodedString[lastChar] = line[i];
                lastChar++; //increment its counter
            }
        }
    }
    printf("%s\n", decodedString); //we print it
    free(decodedString);           //we free the memory of the decodedString array, so we don't have memory leaks
}