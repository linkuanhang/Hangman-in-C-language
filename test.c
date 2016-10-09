//-----------------------------------------------------------------------------
//                                                                   2015/10/08
//                                  Hangman
//                                                      Lin, Kuan-Han
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* global variable declaration */
const unsigned int MAX_WRONG = 7;

/* global function declaration */
unsigned long long int count_lines(FILE*);
unsigned long long int rand_num(unsigned long long int);
char* get_line(unsigned long long int , FILE*);
char** get_word(const char*, const char*);
char* lower_word(const char*);
char* inser_word(const char*, const char*);
char* mask_word(const char*, char);


int main(int argc, char** argv)
{
    char* filePath;
    char* line, * spLine;
    char** wordStr, * word, * wordDes, * lowWord, * inserWord, * maskWord;
    char wrongChar[MAX_WRONG + 1];
    unsigned long long int lineNum, lineIdx;
    FILE* wordDb;
    
    srand(time(NULL));
    
    filePath = "test.txt";
    wordDb = fopen(filePath, "r");
    lineNum = count_lines(wordDb);
    printf("Total lines: |%llu|\n", lineNum);
    
    lineIdx = rand_num(lineNum);
    printf("Line index: |%llu|\n", lineIdx);
    
    lineIdx = 0;
    line = get_line(lineIdx, wordDb);
    printf("Line: |%s|\n", line);
    
    wordStr = get_word(line, "\t");
    word = wordStr[0];
    wordDes = wordStr[1];
    printf("Word: |%s|. Descriptions: |%s|\n", word, wordDes);
    
    lowWord = lower_word(word);
    printf("Word: |%s|\n", lowWord);
    
    inserWord = inser_word(lowWord, " ");
    printf("Word: |%s|\n", inserWord);
    
    maskWord = mask_word(inserWord,'_');
    printf("Word: |%s|\n", maskWord);
    
    free(line);
    free(word);
    //free(wordDes);
    free(wordStr);
    free(lowWord);
    free(inserWord);
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

char* inser_word(const char* word, const char* inser)
{
    unsigned long long int i, lenWord, lenInsert, len;
    char* newWord;
    
    lenWord = strlen(word);
    lenInsert = strlen(inser) + 1;
    len = (lenWord - 1) * lenInsert + 1;
    newWord =  (char*)malloc((len + 1) * sizeof(char));
    
    for (i = 0; i < len; i++)
    {
        if (i % lenInsert == 0)
        {
            newWord[i] = word[i / lenInsert];
        }
        else
        {
            newWord[i] = inser[(i % lenInsert) - 1];
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










