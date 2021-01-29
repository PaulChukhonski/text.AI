#include<iostream>
using namespace std;

#define SHINGLE_WORDS_AMOUNT 3
#define MAX_PERCENT 100.0
#define SIZE 256
#define STR_END '\0'
#define SPACE ' '

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

int main()
{
	char fragment[] = "The Java programming language are practical guides for programmers."
						" The Java Tutorials are organized into 'trails'." 
						" Create applications include hundreds of complete." 
						" Groups of dozens of lessons.";
						
	char text[] = "The Java Tutorials are practical guides for programmers who want to use the Java programming language to create applications."
					" They include hundreds of complete, working examples, and dozens of lessons."
					" Groups of related lessons are organized into 'trails'.";
									
    int wordCount = 0, shingleMatches = 0, shingleAmount = 0;
    double uniquenessPercent = 0;
    
    textCanonization(fragment);
    textCanonization(text);
    
	shingleAmount = getAmountOfWords(fragment) - (SHINGLE_WORDS_AMOUNT-1);   	
	shingleMatches = countShingleAmount(fragment, text, shingleMatches);
	uniquenessPercent = MAX_PERCENT - MAX_PERCENT / shingleAmount * shingleMatches;
	
	cout << "Text uniqueness: " << uniquenessPercent << "%."; 
	
    return 0;
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
		{
			length++;
		}
	}
	else
	{
		for(int i = lastShingleWordIndex; str[i] != SPACE; i++)
		{
			length++;
		}		
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
            {
                str[j] = str[j+1];
            }

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
        {
            str[i] = str[i] - ('Z' - 'z');
        }
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
					{
						prevWord[j] = word[j]; 
					}	
					
					prevWord[iw] = STR_END;				
				}
				else
				{						
					pos = i-iw+1; 
					
					for(int k = 0; k <= iw; k++)
					{
						for(int j = pos; j < strLength(str)-1; j++)
						{
							str[j] = str[j+1];	
						}
					    		    
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
		{
			flag = false;
		}
		i++; 
	}	
	
	if(strLength(s1) != strLength(s2))
	{
		flag = false;
	}
	
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
				{
					flag = false;
				}		
			}
			
			if(flag)
			{
				shingleMatches++;
			}
			
			flag = true;
		}
    }
    
	return shingleMatches;
}
