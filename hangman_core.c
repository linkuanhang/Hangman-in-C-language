//-----------------------------------------------------------------------------
//                                                                   2015/10/08
//                                  Hangman
//                                                      Lin, Kuan-Han
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

/* global variables declaration */
const unsigned short int MAX_WRONG = 7;
const char MASK_CHAR = '_';
const char* WRONG_SEP = ", ";

/* global functions declaration */
/** word preprocessing functions **/
unsigned long long int count_lines(FILE*);
unsigned long long int rand_num(unsigned long long int);
char* get_line(unsigned long long int , FILE*);
char** get_word(const char*, const char*);
char* lower_word(const char*);
char* insert_word(const char*, const char*);
char* mask_word(const char*, char);
/** guess functions **/
bool check_input(char);
char get_input(char);
bool check_guess(const char*, const char*, char);
void make_guess(const char*, char*, char*, char);
bool check_win(const char*, const char*);
bool check_lose(const char*);

int main_test(int argc, char** argv)
{
    char* filePath;
    char* line, * spLine;
    char** wordStr, * word, * wordDes, * lowWord, * insertWord, * maskWord;
    char wrongChar[MAX_WRONG + 1], input;
    unsigned long long int lineNum, lineIdx, i;
    FILE* wordDb;
    bool check;
    
    srand(time(NULL));
    
    filePath = "test.txt";
    wordDb = fopen(filePath, "r");
    lineNum = count_lines(wordDb);
    printf("Total lines: |%llu|\n", lineNum);
    
    lineIdx = rand_num(lineNum);
    printf("Line index: |%llu|\n", lineIdx);
    
    line = get_line(lineIdx, wordDb);
    printf("Line: |%s|\n", line);
    
    wordStr = get_word(line, "\t");
    word = wordStr[0];
    wordDes = wordStr[1];
    printf("Word: |%s|. Descriptions: |%s|\n", word, wordDes);
    
    lowWord = lower_word(word);
    printf("Word: |%s|\n", lowWord);
    
    insertWord = insert_word(lowWord, " ");
    printf("Word: |%s|\n", insertWord);
    
    maskWord = mask_word(insertWord,MASK_CHAR);
    printf("Word: |%s|\n", maskWord);
    
    wrongChar[0] = '\0';
    
    while (input != '\\' && !check_win(insertWord, maskWord) && \
            !check_lose(wrongChar))
    {
        scanf("\n%c", &input);
        check = check_input(input);
        if (check == true)
        {
            input = get_input(input);
            check = check_guess(wrongChar, maskWord, input);
            if (check == false)
            {
                make_guess(insertWord, maskWord, wrongChar, input);
            }
        }

        printf("==========\n");
        printf("Input: |%c|\n", input);
        printf("Word: |%s|, |%s|\n", maskWord, lowWord);
        printf("Wrong: |%s|\n", insert_word(wrongChar, WRONG_SEP));
        printf("==========\n");
    }
    
    if(check_win(insertWord, maskWord) == true)
    {
        printf("You win!!!\n");
    }
    else
    {
        printf("You lose!!!\n");
    }
    
    printf("End.\n");
    
    free(line);
    free(wordStr[0]);
    free(wordStr);
    free(lowWord);
    free(insertWord);
    free(maskWord);
    fclose(wordDb);
    return 0;
}

unsigned long long int count_lines(FILE* file)
{
    char nowChar;
    unsigned long long int count;
    
    count = 0;
    nowChar = fgetc(file);
    while (nowChar != EOF)
    {
       if (nowChar == '\n')
        {
            count++;
        }
        
        nowChar = fgetc(file);
    }
    
    rewind(file);
    
    return count;
}

unsigned long long int rand_num(unsigned long long int upper)
{
    unsigned long long int num, quotient, remain, base;
    
    num = 0;
    base = 1;    
    quotient = upper;
    remain = 0;
    
    while (quotient > 0)
    {
        remain = quotient % RAND_MAX;
        num = num + (rand() % (remain + 1)) * base;
        base = base * RAND_MAX;
        quotient = quotient / RAND_MAX;
    }
    
    return num;
}

char* get_line(unsigned long long int index, FILE* file)
{
    int count, len, i;
    char* line;
    char nowChar;
    
    count = 0;
    
    while (count < index)
    {
        nowChar = fgetc(file);
        if (nowChar == '\n')
        {
            count++;
        }
    }
    
    len = 0;
    nowChar = fgetc(file);
    while (nowChar != '\n')
    {
        len++;
        nowChar = fgetc(file);
    }
    
    line = (char*)malloc((len + 1) * sizeof(char));
    fseek(file, -1-len , SEEK_CUR);
    fread(line, sizeof(char), len, file);
    line[len] = '\0';
    rewind(file);
    
    return line;
}

char** get_word(const char* line, const char* sep)
{
    unsigned long long lineLen, sepLen, count, i;
    char** splitLine, * tempSep, * tempLine;
    
    lineLen = strlen(line);
    sepLen = strlen(sep);
    tempSep = strdup(sep);
    count = 1;
    
    for(i = 0; i <= lineLen - sepLen; i++)
    {
        tempSep = strncpy(tempSep, line + i, sepLen);
        if (strcmp(tempSep, sep) == 0)
        {
            count++;
        }
    }
    
    splitLine = (char**)malloc(count * sizeof(char*));
    tempLine = strdup(line);
    splitLine[0] = strtok(tempLine, sep);
    for(i = 1; i < count; i++)
    {
        splitLine[i] = strtok(NULL, sep);
    }
    
    free(tempSep);
    
    return splitLine;
}

char* lower_word(const char* word)
{
    unsigned long long int i, len;
    char* newWord;
    
    len = strlen(word);
    newWord = strdup(word);
    for(i = 0; i < len ; i++){
        newWord[i] = tolower(word[i]);
    }
    
    return newWord;
}

char* insert_word(const char* word, const char* insert)
{
    unsigned long long int i, lenWord, lenInsert, len;
    char* newWord;
    
    lenWord = strlen(word);
    lenInsert = strlen(insert) + 1;
    len = (lenWord - 1) * lenInsert + 1;
    
    if (lenWord == 0)
    {
        return "";
    }
    
    newWord =  (char*)malloc((len + 1) * sizeof(char));
    
    for (i = 0; i < len; i++)
    {
        if (i % lenInsert == 0)
        {
            newWord[i] = word[i / lenInsert];
        }
        else
        {
            newWord[i] = insert[(i % lenInsert) - 1];
        }
    }
    newWord[len] = '\0';
    
    return newWord;
}

char* mask_word(const char* word, char mask)
{
    unsigned int i, len;
    char* newWord;
    char newChar;
    
    len = strlen(word);
    newWord = strdup(word);
    for(i = 0; i < len ; i++){
        newChar = word[i];
        if (isalpha(newChar))
        {
            newWord[i] = mask;
        }
        else
        {
            newWord[i] = newChar;
        }
    }
    
    return newWord;
}

bool check_input(char input)
{
    bool res;
    res = isalpha(tolower(input));
    return res;
}

char get_input(char input)
{
    char res;
    res = tolower(input);
    return res;
}

bool check_guess(const char* wrong, const char* mask, char guess)
{
    unsigned long long int len, i;
    
    len = strlen(wrong);
    
    for (i = 0; i < len; i++)
    {
        if (wrong[i] == guess)
        {
            return true;
        }
    }
    
    len = strlen(mask);
    
    for (i = 0; i < len; i++)
    {
        if (mask[i] == guess)
        {
            return true;
        }
    }
    
    return false;
}

void make_guess(const char* word, char* mask, char* wrong, char guess)
{
    unsigned long long int len, i;
    char guessStr[2];
    bool match;
    
    len = strlen(word);
    match = false;
    guessStr[0] = guess;
    guessStr[1] = '\0';
    
    for (i = 0; i <len; i++)
    {
        if (word[i] == guess)
        {
            mask[i] = word[i];
            match = true;
        }
    }
    
    if (match == false)
    {
        wrong = strcat(wrong, guessStr);
    }
}

bool check_win(const char* word, const char* mask)
{
    bool res;
    
    if (strcmp(word, mask) == 0)
    {
        res = true;
    }
    else
    {
        res = false;
    }
    
    return res;
}

bool check_lose(const char* wrong)
{
    bool res;
    
    if (strlen(wrong) == MAX_WRONG)
    {
        res = true;
    }
    else
    {
        res = false;
    }
    
    return res;
}
