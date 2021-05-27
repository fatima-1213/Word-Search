#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//this function makes a grid of the given dimensions
char** MakeGrid(const int &rows, const int & cols)
{
	char empty = '-';
	char **grid = new char *[rows];

	for (int i = 0; i < rows; i++)
	{
		grid[i] = new char [cols + 1]; 

		for (int j = 0; j < cols; j++)
		{
			grid [i][j] = empty; //this symbol is stored everywhere
		}
	}	
	return grid;
}

//finds and returns the length of a string sent to it
int StringLength(char* myStr) 
{
	int ctr = 0;

	for (char *temp = myStr; *temp != '\0'; temp++)
	{
		ctr++; //controller adds when a character is found in the c-string
	}

	return ctr;
}

//if a word has gaps in it, this function removes it
void RemoveGaps(char *word)
{
	int nextLetter; 
	for (int i = 0; word[i] != '\0'; i++)
	{
		if (word[i] == ' ') //if there is a gap, it is replaces by the char after it.
		{
			for (int j = i; word[j] != '\0'; j++)
			{
				nextLetter = j + 1;
				word[j] = word[nextLetter];
			}
		}
	}	
}

//this function loads the input file 
void LoadInputFile(string &inputFile)
{
	cout << "Enter the name of input file \nFile name:\t";
	cin >> inputFile;

	//checks if the extension is correct
	int fileNameLength = inputFile.length(); 

	while (inputFile[fileNameLength - 4] != '.' 
		|| inputFile[fileNameLength - 3] != 't'
		|| inputFile[fileNameLength - 2] != 'x' 
		|| inputFile[fileNameLength - 1] != 't')
    {
        cout<<"\n\nLoad unsuccessful" <<endl<<"File extension not correct"<<endl
			<<"Enter correct file name \nFile name:\t";
		cin >> inputFile;
		fileNameLength = inputFile.length();
    }

	ifstream fin(inputFile.c_str());

	//checks if the file can be opened or not
	if (!fin)
    {
        cout<<"Load unsuccessful" <<endl<<"Unable to open a file for reading"<<endl
			<<"Enter correct file name \nFile name:\t";
		cin >> inputFile;
		ifstream fin(inputFile.c_str());
    }

	fin.close();
}

//reads word from file and returns it stored in a double pointer char array
char** ReadWordsFromFile(const int &wordCount, string &inputFile)
{
	//opens file, stores words in a temporary buffer, saves according to their size in 2d double pointer char array and returns it
	
	ifstream fin(inputFile.c_str());

	char** wordsList = new char *[wordCount];

	char tempBuffer[80]; //stores the name to get character count
	int length;
	
	for (int i = 0; i < wordCount; i++)
	{
		fin.getline(tempBuffer, 80);
		RemoveGaps(tempBuffer);
		char *word = wordsList[i];
		
		length = StringLength (tempBuffer); //counts characters

		if (length > 0)
		{
			*(wordsList+i) = new char [length + 1];  //last place for null character
			char *tempDest = *(wordsList + i);

			for (char * tempSrc = tempBuffer; *tempSrc != '\0'; tempSrc++, tempDest++)
			{
				*tempDest = *tempSrc; //stores the name into the list
			}

			*tempDest = '\0';
		}
	}

	fin.close();

	return wordsList;
}

//this function iputs word count, number of rows and number of cols
void InputValuesForCreatingGrid(int &wordCount, int &rowCount, int &colCount)
{
	cout << "\n\nEnter the number of words in the file \nWord Count:\t";
	cin >> wordCount;

	cout << "\nEnter the number of rows in the file \nRows Count:\t";
	cin >> rowCount;

	cout << "\nEnter the number of cols in the file \nCols Count:\t";
	cin >> colCount;

	cout << "\n\n\n";

}

//this function saves the output grid in file output.txt
void OutputGrid(char **grid, int const &rows, int const &cols)
{
	string outputFile;
	cout << "\n\nEnter the name of output file \nFile name:\t";
	cin >> outputFile;

	//checks if the extension is correct
	int fileNameLength = outputFile.length();
	while (outputFile[fileNameLength - 4] != '.' 
		|| outputFile[fileNameLength - 3] != 't'
		|| outputFile[fileNameLength - 2] != 'x' 
		|| outputFile[fileNameLength - 1] != 't' )
    {
        cout<<"\n\nLoad unsuccessful" <<endl<<"File extension not correct"<<endl
			<<"Enter correct file name \nFile name:\t";
		cin >> outputFile;
		fileNameLength = outputFile.length();
    }

	ofstream fout (outputFile.c_str());

	for (int i = 0; i < rows; i++)
	{	
		for (int j = 0; j < cols; j++)
		{
			fout << grid[i][j] << " ";
		}

		fout << endl;
	}	

	fout.close();

}

//these functions check if place is empty for the copying thw word into the givven directions
bool CheckLeftToRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] == empty  || grid[i][j] == word[k])
				{
					while (k < wordLength && checked == false)
					{
						if (j + k < cols && (grid[i][j + k] == empty || grid[i][j+k] == word[k]))
						{
							k++;
						}
						else
						{
							k = 0;
							checked = true;
						}
					}
					if (k == wordLength)
					{
						startPosition_i = i; 
						startPosition_j = j;
						endPosition_j = wordLength + startPosition_j - 1;
						endPosition_i = i;
						return true;	
					}
					checked = false;
				}
			}
		}
		return false;
}
bool CheckRightToLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if (j - k >= 0 && (grid[i][j - k] == empty || grid[i][j - k] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j - wordLength + 1;
					endPosition_i = i;
					return true;	
				}
				checked = false;
			}	
		}
	}
	return false;
}
bool CheckTopToBottom(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if (i + k < rows && (grid[i + k][j] == empty || grid[i + k][j] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j;
					endPosition_i = i + wordLength - 1;
					return true;
				}
				checked = false;
			}
			
		}
	}
	return false;
}
bool CheckBottomToTop(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if (i - k >= 0 && (grid[i - k][j] == empty || grid[i - k][j] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j;
					endPosition_i = i - wordLength + 1;
					return true;
				}
				checked = false;
			}
		}
	}
	return false;
}
bool CheckTopLeftToBottomRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if ((i == j || j - i == (j+1) - (i+1) ) && (i + k < rows && j + k < cols) && (grid[i + k][j + k] == empty || grid[i + k][j + k] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0; 
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i;
					startPosition_j = j;
					endPosition_j = j + wordLength - 1;
					endPosition_i = i + wordLength - 1;
					return true;
				}
				checked = false;
			}
		}
	}
	return false;
}
bool CheckTopRightToBottomLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if ( (j + i == (j-1) + (i+1) ) && (i + k < rows && j - k >= 0) && (grid[i + k][j - k] == empty || grid[i + k][j - k] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j - wordLength + 1;
					endPosition_i = i + wordLength - 1;
					return true;
				}
				checked = false;
			}
		}
	}
	return false;
}
bool CheckBottomLeftToTopRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if ( (j + i == (j-1) + (i+1) ) && (i - k >= 0 && j + k < cols ) && (grid[i - k][j + k] == empty || grid[i - k][j + k] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{
					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j + wordLength - 1;
					endPosition_i = i - wordLength + 1;
					return true;
				}
				checked = false;
			}
		}
	}
	return false;
}
bool CheckBottomRightToTopLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, int &startPosition_i, int &startPosition_j, int &endPosition_i, int &endPosition_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places
	bool checked = false; //this boolean becomes true if a specific place is checked and word copying is not possible over there


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty  || grid[i][j] == word[k])
			{
				while (k < wordLength && checked == false)
				{
					if ((i == j || j - i == (j+1) - (i+1) ) && (i - k >= 0 && j - k >= 0) && (grid[i - k][j - k] == empty || grid[i - k][j - k] == word[k]))
					{
						k++;
					}
					else
					{
						k = 0;
						checked = true;
					}
				}
				if (k == wordLength)
				{

					startPosition_i = i; 
					startPosition_j = j;
					endPosition_j = j - wordLength + 1;
					endPosition_i = i - wordLength + 1;
					return true;
				}
				checked = false;
			}
			
		}
	}
	return false;
}

//these functions copy at the directions if the place is empty for them to copy
void CopyLeftToRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{

					if (grid[i][j + k] == empty || grid[i][j+k] == word[k])
					{
						grid[i][j+k] = word[k];
						k++;
					}
				}
			}
		}
	}
}
void CopyRightToLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i][j - k] == empty || grid[i][j - k] == word[k])
					{
						grid[i][j - k] = word[k];
						k++;
					}
				}
			}
		}
	}
	
}
void CopyTopToBottom(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i + k][j] == empty || grid[i + k][j] == word[k])
					{
						grid[i + k][j] = word[k];
						k++;
					}
				}
			}
		}
	}
}
void CopyBottomToTop(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i - k][j] == empty || grid[i - k][j] == word[k])
					{
						grid[i - k][j] = word[k];
						k++;
					}
				}
			}
		}
	}
}
void CopyTopLeftToBottomRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i  && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i + k][j + k] == empty || grid[i + k][j+k] == word[k])
					{
						grid[i + k][j+k] = word[k];
						k++;
					}
				}
			}
		}
	}
	

}
void CopyTopRightToBottomLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i + k][j - k] == empty || grid[i + k][j - k] == word[k])
					{
						grid[i + k][j - k] = word[k];
						k++;
					}
				}
			}
		}
	}
}
void CopyBottomLeftToTopRight(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
				
					if (grid[i - k][j + k] == empty || grid[i - k][j + k] == word[k])
					{
						grid[i - k][j + k] = word[k];
						k++;
					}
				}
			}
		}
	}
}
void CopyBottomRightToTopLeft(char **grid, const char *word, const int &rows, const int &cols, const int &wordLength, const int & position_i, const int &position_j)
{
	int k = 0; //this is an iterator to traverse through the kth letter of the word
	char empty = '-'; //this is the char placed at empty places

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == position_i && j == position_j)
			{
				while (k < wordLength)
				{
					if (grid[i - k][j - k] == empty || grid[i - k][j - k] == word[k])
					{
						grid[i - k][j - k] = word[k];
						k++;
					}
				}
			}
		}
	}
}

//this function assigns the words of the file to the grid
void AssignWords(char **grid, char **wordList, const int &rows, const int &cols, int wordCount)
{
	//this stores the assign words in the grid from
	//L to R
	//R to L
	//T to B
	//B to T
	//TR to BL
	//BL to TR
	//TL to BR
	//BR to TL

	int wordLength = 0, position_i, position_j, extra;
	wordCount -=1;

	while (wordCount >= 0)
	{
		

		//TL to BR
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);

			if (CheckTopLeftToBottomRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyTopLeftToBottomRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;

			}
		}
	
		//TR to BL	
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);

			if (CheckTopRightToBottomLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyTopRightToBottomLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}
		}

	
		//L to R
		if (wordCount >= 0)
		{
		
			wordLength = StringLength (wordList[wordCount]);

			if (CheckLeftToRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyLeftToRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}
		
		}

		//R to L
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);

			if (CheckRightToLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyRightToLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}
		
		}


		//T to B
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);

			if (CheckTopToBottom(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyTopToBottom(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}
		}

		//B to T	
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);

			if (CheckBottomToTop(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyBottomToTop(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}
		
		}

		//BL to TR	
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);
			if (CheckBottomLeftToTopRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyBottomLeftToTopRight(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}	
		}

		//BR to TL	
		if (wordCount >= 0)
		{
			wordLength = StringLength (wordList[wordCount]);
			if (CheckBottomRightToTopLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j, extra, extra))
			{
				CopyBottomRightToTopLeft(grid, wordList[wordCount], rows, cols, wordLength, position_i, position_j);
				wordCount--;
			}	
		}
	}
}

//this function assigns random letters where no words are placed
void AssignRandomLetters(char **grid, int const &rows, int const &cols)
{
	char empty = '-';

	for (int i = 0; i < rows ; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == empty)
			{
				char randLetter = 'A' + rand()%26;
				grid[i][j] = randLetter;
			}
		}
	}
}

//this function checks if words can fit, and returns true if they can
bool CanWordsFit(char **wordList, int const &wordsCount, int const &rowsCount, int const &colsCount)
{
	int length = 0, maxLength = StringLength(wordList[wordsCount-1]);
	
	if (maxLength > rowsCount || maxLength > colsCount)
	{
		return false;
	}

	return true;
}

//this function inputs words from user into a 2d char array
char** InputWords(string fileName, int &rowsCount, int &colsCount, int &testCases)
{
	ifstream fin(fileName.c_str());

	int dimensions [2];
	for (int i = 0; i < 2; i++)
	{
		fin >> dimensions[i]; //so they are copies from the same line
	}

	//copied to return these vals
	rowsCount = dimensions[0]; 
	colsCount = dimensions[1];

	fin >> testCases;

	//copies the wordlist
	char** wordsList = new char *[testCases];

	char tempBuffer[80]; //stores the name to get character count
	int length;

	fin.ignore();
	for (int i = 0; i < testCases; i++)
	{
		fin.getline(tempBuffer, 80);
		char *word = wordsList[i];
		
		length = StringLength (tempBuffer); //counts characters

		if (length > 0)
		{
			*(wordsList+i) = new char [length + 1];  //last place for null character
			char *tempDest = *(wordsList + i);

			for (char * tempSrc = tempBuffer; *tempSrc != '\0'; tempSrc++, tempDest++)
			{
				*tempDest = *tempSrc; //stores the name into the list
			}

			*tempDest = '\0';
		}
	}
	fin.close();

	return wordsList;
}

//this function reads the grid from input.txt
char** ReadGrid(string fileName, const int rows, const int cols)
{
 
	ifstream fin(fileName.c_str());

	char **grid = new char *[rows]; //memory allocation rows
	for (int i = 0; i < rows; i++)
	{
		grid[i] = new char [cols]; //memory allocation cols
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fin >> grid[i][j]; //input
		}
	}
	fin.close();

	return grid;
}

//this function finds the words
bool FindWord(char **grid, char* word, int const rows, int const cols, int &iStart, int &iEnd, int &jStart, int &jEnd)
{

	//this function looks for the word 8 ways
	//L to R
	//R to L
	//T to B
	//B to T
	//TL to BR
	//BR to TL
	//TR to BL
	//BL to TR

	//if those words are found, it immediately changes the values which have to be output and returns true

	int length, k = 0;
	length = StringLength (word);
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			iStart = i;
			jStart = j;

			if (grid[i][j] == word[k])
			{
				//check L to R
				if (CheckLeftToRight(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//check R to L
				if (CheckRightToLeft(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//check T to B
				if (CheckTopToBottom(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//check B to T
				if (CheckBottomToTop(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//TL to BR
				if (CheckTopLeftToBottomRight(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//BR to TL
				if (CheckBottomRightToTopLeft(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//TR to BL
				if (CheckTopRightToBottomLeft(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;

				//BL to TR
				if (CheckBottomLeftToTopRight(grid, word, rows, cols, length, iStart, jStart, iEnd, jEnd))
					return true;
			}
		}
	}
	
	//if word not found it returns false
	return false;
}

//this function outputs the values into output.txt
void Output(ofstream &fout, int iStart, int iEnd, int jStart, int jEnd)
{
	fout << "{" << iStart << "," << jStart << "}" << "{" << iEnd << "," << jEnd << "}" << endl;
}

void main()
{
	char option, **wordList, **grid;
	int wordCount, rowCount, colCount;

	string fileName1, fileName2;

	do
	{
		cout << "Welcome!\n\n"
			 << "Enter C: To Create a Grid\n"
			 << "Enter S: To Search Words from Grid\n"
			 << "Enter Q: To Quit\n\n\n"
			 << "Option: ";

		cin >> option;

		system ("CLS");

		while (option != 'C' && option != 'c' && option != 'S' && option != 's' && option != 'Q' && option != 'q')
		{
			cout << "\n\nError! \nEnter the correct option: ";
			cin >> option;
		}
		
		if (option == 'C' || option == 'c')
		{
			cout << "Creating Grid Function Called\n";

			//this function inputs word, rows and cols count
			InputValuesForCreatingGrid(wordCount, rowCount, colCount);

			// this function input the name of the input file and tries to load it
			LoadInputFile(fileName1); 

			//copie sthe words from the input file into this char**
			wordList = ReadWordsFromFile(wordCount, fileName1);

			//checks if the biggest word can fit or not
			if (CanWordsFit(wordList, wordCount, rowCount, colCount))
			{
				//makes the grid and copies it into char**
				grid = MakeGrid(rowCount, colCount);

				//assigns the words from the wordlist to the grid
				AssignWords(grid, wordList, rowCount, colCount, wordCount);

				//assigns random letters to the grid in empty places
				AssignRandomLetters(grid, rowCount, colCount);

				//outputs the grid into the output file
				OutputGrid(grid, rowCount, colCount);

				cout << "\n\nFile has been Created Successfully";

			}
			else
			{
				cout << "\nGrid can not be formed";
			}

			cout <<"\n\nPress any key to return to home screen\n";
			cin.ignore();
			getchar();
			system ("CLS");
		}


		if (option == 's' || option == 'S')
		{
			cout << "Searching Words from Grid Function Called\n\n";

			//checks if the name of file is correct
			cout << "For the grid, ";
			LoadInputFile(fileName1);

			//checks if the name of file is correct
			cout << "\n\nFor the dimensions, number of words and wordlist, ";
			LoadInputFile(fileName2); 

			//reads the word list
			wordList = InputWords(fileName2, rowCount, colCount, wordCount);

			//reads the grid
			grid = ReadGrid(fileName1, rowCount, colCount);

			//the positions
			int iStart, iEnd, jStart, jEnd;

			//inputs the output file name
			cout << "\n\nEnter the name of the Output File\n";
			cin >> fileName1;
			ofstream fout(fileName1.c_str()); //creating the output file

			for (int i = 0; i < wordCount; i++)
			{
				//looks for the word
				if (FindWord(grid, wordList[i], rowCount, colCount, iStart, iEnd, jStart, jEnd) == true)
				{
					Output(fout, iStart, iEnd, jStart, jEnd); //if the word is found, it send the values for output
				}
				else 
				{
					fout << "Not Found" << endl; //otherwise sends this statement
				}
			}
			cout << "\n\nFile has been created Successfully\n"
			     <<"\n\nPress any key to return to home screen\n";

				cin.ignore();
				getchar();
				system ("CLS");
		}
	}
	while (option != 'q' && option != 'Q');
	
}