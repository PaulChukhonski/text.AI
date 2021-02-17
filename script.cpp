#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
using namespace std;
using namespace cgicc;

#define SHINGLE_WORDS_AMOUNT 3
#define MAX_PERCENT 100.0
#define SIZE 256
#define STR_END '\0'
#define SPACE ' '
#define N 1024

void showText(char str[]);
int strLength(char str[]);
int strLengthByWordIndex(char str[], int lastShingleWordIndex, int lastShingleWord);
bool isLetter(char c);
void removeMarks(char str[]);
void trimStr(char str[]);
void toLowerCase(char str[]);
void textCanonization(char str[]);
int getAmountOfWords(char str[]);
void getWordsIndex(char str[], int wordsIndex[]);
void removeExtraWords(char str[]);
bool strCompare(char s1[], char s2[]);
int countShingleAmount(char fragment[], char text[], int shingleMatches);
double antiPlagiarism(string textS, string fragmentS);
void getCharArrayFromString(char textArray[], string text);
string getDB();

int main()
{   
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";

    name = form("name");

    if (!name.empty())
    {
        cout << antiPlagiarism(getDB(), name) << "\n";
    }
    else
    {
        cout << "Text is not provided!\n";
    }
    
    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";
}

double antiPlagiarism(string textS, string fragmentS)
{
    char fragment[N];
    char text[N];
    
    getCharArrayFromString(fragment, fragmentS);
    getCharArrayFromString(text, textS);
    
    int shingleMatches = 0, shingleAmount = 0;
    double uniquenessPercent = 0;
    
    textCanonization(fragment);
    textCanonization(text);
    
    shingleAmount = getAmountOfWords(fragment) - (SHINGLE_WORDS_AMOUNT-1); 	
    shingleMatches = countShingleAmount(fragment, text, shingleMatches);
    uniquenessPercent = MAX_PERCENT - MAX_PERCENT / shingleAmount * shingleMatches;
    
    return uniquenessPercent;
}

string getDB()
{
    std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
        dbText += s1;
  
    in.close();
    
    return dbText;
}

void getCharArrayFromString(char textArray[], string text)
{
    int i = 0;
    
    for(i = 0; i < text.length(); i++)
        textArray[i] = text[i];
    textArray[i] = '\0';
}

void showText(char str[])
{
    for(int i = 0; str[i] != STR_END; i++)
        cout << str[i];
        
    cout << endl;
}

int strLength(char str[])
{
    int counter = 0;
    
    while (str[counter] != STR_END) 
        counter++; 
    
    return counter;
}

int strLengthByWordIndex(char str[], int lastShingleWordIndex, int lastShingleWord)
{
    int length = 0;

    if(lastShingleWord == getAmountOfWords(str)-1)
    {
        for(int i = lastShingleWordIndex; str[i] != STR_END; i++)
            length++;
    }
    else
    {
        for(int i = lastShingleWordIndex; str[i] != SPACE; i++)
            length++;
    }

    return length;
}


bool isLetter(char c)
{    
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    return false;
}

void removeMarks(char str[])
{
    for(int i = 0; str[i] != STR_END; i++)
    {
        if(!isLetter(str[i]))
            str[i] = SPACE;
    }
}

void trimStr(char str[])
{
    int lenghtStr = strLength(str);

    for(int i = 0; i < lenghtStr; i++)
    {
        if(str[i] == SPACE && str[i+1] == SPACE)
        {
            for(int j = i; j < lenghtStr-1; j++)
                str[j] = str[j+1];

            str[lenghtStr-1] = STR_END;
            i--;
        }
    }
}

void toLowerCase(char str[]) 
{
    for(int i = 0; str[i] != STR_END; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - ('Z' - 'z');
    }
}

void textCanonization(char str[])
{
    removeMarks(str);   
    toLowerCase(str); 
    trimStr(str);
    removeExtraWords(str);
}

int getAmountOfWords(char str[])
{
    int amount = 1;
    
    for(int i = 0; i < strLength(str); i++)
    {
        if(str[i] == SPACE && str[i+1] != STR_END)
            amount++;
    }
    
    return amount;
}

void getWordsIndex(char str[], int wordsIndex[])
{
    int index = 0;
    
    for(int i = 0; str[i] != STR_END; i++)
    {
        if(i == 0)
        {
            wordsIndex[index] = i;
            index++;
        }
        else if (str[i] == SPACE && str[i+1] != STR_END)
        {
            wordsIndex[index] = i + 1;
            index++;
        }
    }
}

void removeExtraWords(char str[])
{
    char word[SIZE], prevWord[SIZE];
    int iw = 0, pos = 0;
    
    for (int i = 0; str[i] != STR_END; i++) 
    {
        if (isLetter(str[i])) 
        {
            word[iw] = str[i];
            iw++;
            
            if (str[i+1] == SPACE || str[i+1] == STR_END) 
            {
                word[iw] = STR_END;
                
                if(!strCompare(prevWord, word))
                {
                    for(int j = 0; j < iw; j++)
                        prevWord[j] = word[j]; 
                    
                    prevWord[iw] = STR_END;
                }
                else
                {
                    pos = i-iw+1; 
                    
                    for(int k = 0; k <= iw; k++)
                    {
                        for(int j = pos; j < strLength(str)-1; j++)
                            str[j] = str[j+1];
                            
                        str[strLength(str)-1] = STR_END;
                        i--;
                    }
                }
                
                iw = 0;
            }
        }
    }   
}

bool strCompare(char s1[], char s2[])
{
    int i = 0;
    bool flag = true;
    
    while(s1[i] != STR_END)
    {
        if(s2[i] != s1[i])
            flag = false;
            
        i++; 
    }
    
    if(strLength(s1) != strLength(s2))
        flag = false;
    
    return flag;
}

int countShingleAmount(char fragment[], char text[], int shingleMatches)
{
    int lastIndex = 0, lastShingleWordLength = 0, lastShingleWord = 0, shingleLength = 0;
    int wordIndexesOfFragment[getAmountOfWords(fragment)], wordIndexesOfText[getAmountOfWords(text)];
    char textShingle[SIZE];
    bool flag = true; 
    
    getWordsIndex(fragment, wordIndexesOfFragment);    
    getWordsIndex(text, wordIndexesOfText); 
    
    for(int i = 0; i <= getAmountOfWords(fragment)-SHINGLE_WORDS_AMOUNT; i++)
    {
        lastShingleWord = i+SHINGLE_WORDS_AMOUNT-1;
        lastShingleWordLength = strLengthByWordIndex(fragment, wordIndexesOfFragment[lastShingleWord], lastShingleWord); 
        lastIndex = wordIndexesOfFragment[lastShingleWord] + lastShingleWordLength; 
        shingleLength = lastIndex-wordIndexesOfFragment[i];
        
        for(int j = wordIndexesOfFragment[i], k = 0; k < shingleLength; j++, k++)
        {
            textShingle[k] = fragment[j];
        }
        
        textShingle[shingleLength] = STR_END;    
            
        for(int l = 0; l <= getAmountOfWords(text)-SHINGLE_WORDS_AMOUNT; l++)
        {
            for(int j = wordIndexesOfText[l], k = 0; k < shingleLength; j++, k++)
            {
                if(textShingle[k] != text[j])
                    flag = false;
            }
            
            if(flag)
                shingleMatches++;
            
            flag = true;
        }
    }
    
    return shingleMatches;
}
