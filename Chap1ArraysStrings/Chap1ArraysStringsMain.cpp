/*
 * Chap1ArraysStringsMain.cpp
 *
 *  Created on: 2019-09-22
 *      Author: ckirsch
 */
#include "Chap1ArraysStringsMain.h"

#include <iostream>
#include <cassert>
#include <cstdint>
#include <stdlib.h>
#include <unordered_set>
#include <algorithm>
using namespace std;

const int NUM_ASCII_CHAR = 256;


/* Implement an algorithm to determine if a string has all unique characters. What if you
 * cannot use additional data structures?
 *
 * Time: O(character set size) as never iterate more than 256
 * Space: O(character set size) as will always be 256 hash
 *
 * If can reduce character set to 64 or 32 (variable size) then can use a bit hash to reduce memory
 * index = (1 << (int)(c - 'a')), hash & index == 1 => return false
 *
 * If not extra space then do brute force method O(n^2) by comparing each character to all others
 * Or could sort input string O(nlogn) and the iterate through O(n), however some sort methods take
 * extra space (recursive or arrays in divide and conquer */
bool isUnique(string str)
{
	// Check if more than number of unique characters, hence has to have duplicates
	if (str.size() > NUM_ASCII_CHAR)
	{
		return false;
	}

	bool hash[NUM_ASCII_CHAR] = {false};
	for (char c : str)
	{
		int index = (int)c;
		if (hash[index])
		{
			return false;
		}

		hash[index] = true;
	}

	return true;
}


bool isUniqueSet(string str)
{
	if (str.length() > NUM_ASCII_CHAR)
	{
		return false;
	}

	unordered_set<char> hash;
	for (char c : str)
	{
		if (hash.find(c) == hash.end())
		{
			hash.insert(c);
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool isUniqueSort(string str)
{
	if (str.length() > NUM_ASCII_CHAR)
	{
		return false;
	}

	// Can sort with std::sort-> O(nlogn)
	// Or use counting sort of O(n)

	std::sort(str.begin(), str.end());
	for (uint i = 1; i < str.length(); ++i)
	{
		if (str[i - 1] == str[i])
		{
			return false;
		}
	}
	return true;
}


bool isUniqueBit(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	unsigned int bitHash = 0;
	for (char c : str)
	{
		unsigned int bitOffset = 1 << (c - 'a');
		if (bitHash & bitOffset)
		{
			return false;
		}
		else
		{
			bitHash |= bitOffset;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------

/* Given two strings, write a method to decide if one is a permutation of the other.
 * 1) check character counts
 * Time: O(max(n,m))
 * Space: O(1)
 * 2) Sort and compare 2 of same characters
 * Time: O(nlogn)
 * Space: O(1) */
bool permutation(string str1, string str2)
{
	const int NUM_ASCII_CHAR = 256;

	if (str1.size() != str2.size())
	{
		return false;
	}

	int hash[NUM_ASCII_CHAR] = {0};
	for (char c : str1)
	{
		++hash[(int)c];
	}

	for (char c : str2)
	{
		--hash[(int)c];

		if (hash[(int)c] < 0)
		{
			return false;
		}
	}

	return true;
}


// Time: O(n), Space: O(1) - works for small strings
bool permutationCount(string str1, string str2)
{
	if (str1.length() != str2.length())
	{
		return false;
	}

	int count = 0;
	for (char c : str1)
	{
		count += (c - 'a'); // - 'a' to try and keep count value low to fit in count data type
	}

	for (char c : str2)
	{
		count -= (c - 'a');
	}

	return (count == 0);
}


//-----------------------------------------------------------------------------

/* Write a method to replace all spaces in a string with '%20: You may assume that the string
has sufficient space at the end to hold the additional characters, and that you are given the "true"
length of the string. (Note: If implementing in Java, please use a character array so that you can
perform this operation in place.)

EXAMPLE
Input: "Mr John Smith ", 13
Output: "Mr%20John%20Smith" */
string& URLify(string &url, int trueLength)
{
	// Find where new end of string should be
	unsigned int spaceCount = 0;
	for (int i = 0; i < trueLength; ++i)
	{
		if (isspace(url[i]))
		{
			++spaceCount;
		}
	}
	unsigned int newArraySize = trueLength + spaceCount * 2;

	// Check if enough space
	if (newArraySize > url.size())
	{
		return url;
	}

	int newArrayIndex = newArraySize - 1;
	int oldArrayIndex = trueLength - 1;

	while (newArrayIndex >= 0 && oldArrayIndex >= 0)
	{
		if (isspace(url[oldArrayIndex]))
		{
			url[newArrayIndex--] = '0';
			url[newArrayIndex--] = '2';
			url[newArrayIndex--] = '%';
		}
		else
		{
			url[newArrayIndex--] = url[oldArrayIndex];
		}

		--oldArrayIndex;
	}

	return url;
}


//-----------------------------------------------------------------------------


/* 1) Get all permutations and check if each is palindrome O(n*n!)
 * isPalidrom(string) => pick leftIdx--, rightIdx++ and check equal O(n)
 * 2) Want to see if any combinations of characters in string makes a palindrome
 * A none ordered palindrome/permutation of a palindrome can be checked with character counts
 * O(n) */
bool containsPalidromPermutation(string str)
{
	int hash[26] = {0};

	int spaceCount = 0;
	for (char c : str)
	{
		if (isspace(c))
		{
			++spaceCount;
			continue;
		}

		int index = tolower(c) - 'a';
		++hash[index];
	}

	int stringSize = str.size() - spaceCount;
	bool processedMiddle = false;
	for (char c : str)
	{
		if (isspace(c))
		{
			continue;
		}
		int index = tolower(c) - 'a';

		if (hash[index] != 2)
		{
			if (hash[index] == 1 && (stringSize % 2) != 0 && false == processedMiddle)
			{
				processedMiddle = true;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}


bool containsPalidromPermutationBit(string str)
{
    unsigned int bitVector = 0;

    // Make bitvector -> toggle
    for (char c : str)
    {
        if (c == ' ')
        {
            continue;
        }

        int mask = 1 << (c - 'a');
       /* if (bitVector & mask)
        {
        	// Turn off
        	bitVector &= ~mask;
        }
        else
        {
        	// Turn on
        	bitVector |= mask;
        }*/
        bitVector ^= mask;
    }

    // Check all matching or if odd only 1 different
    return (0 == bitVector) || 0 == (bitVector & (bitVector - 1));
}


//-----------------------------------------------------------------------------


/* Time: O(n)
 * Space: O(1) */
bool containsPalidromPermutationOptimized(string str)
{
	uint32_t hash = 0;

	// Setup hash
	int spaceCount = 0;
	for (char c : str)
	{
		if (isspace(c))
		{
			++spaceCount;
			continue;
		}

		int index = tolower(c) - 'a';
		hash ^= (1 << index);
	}

	// Check hash
	int stringSize = str.size() - spaceCount;
	if (stringSize % 2 == 0)
	{
		// Hash has a bit still set so not palidrome
		if (hash != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// Check if more than one bit set
	if (0 != ((hash - 1) & hash))
	{
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------

bool oneEditReplace(string str1, string str2)
{
	if (str1.size() != str2.size())
	{
		return false;
	}

	bool oneReplace = false;
	for (uint i = 0; i < str1.size(); ++i)
	{
		if (str1[i] != str2[i])
		{
			if (oneReplace)
			{
				return false;
			}
			else
			{
				oneReplace = true;
			}
		}
	}

	return true;
}


bool oneEditInsert(string strLarge, string strSmall)
{
	if (strLarge.size() < strSmall.size() ||
			strLarge.size() > (strSmall.size() + 1))
	{
		return false;
	}

	bool oneEdit = false;
	for (int strLargeIdx = 0, strSmallIdx = 0;
			strLargeIdx < static_cast<int>(strLarge.size()) &&
					strSmallIdx < static_cast<int>(strSmall.size());
			++strLargeIdx, ++strSmallIdx)
	{
		if (strLarge[strLargeIdx] != strSmall[strSmallIdx])
		{
			if (oneEdit)
			{
				return false;
			}
			else
			{
				oneEdit = true;
				--strSmallIdx; // To realign both index's
			}
		}
	}

	return true;
}


/* O(n) as if both strings differ in size => O(1), so only if both strings are close +1 O(n) */
bool isOneAway(string str1, string str2)
{
	if (str1.size() == str2.size())
	{
		// Cannot insert or remove, so check only 1 replace
		return oneEditReplace(str1, str2);
	}
	else if (str1.size() == (str2.size() + 1))
	{
		// Check only one insert to str2 will match str1
		return oneEditInsert(str1, str2);
	}
	else if (str2.size() == str1.size() + 1)
	{
		// Check only one insert to str1 will match str2
		return oneEditInsert(str2, str1);
	}

	return false;
}


bool isOneAwayCompressed(string str1, string str2)
{
	if (abs(static_cast<int>(str1.size() - str2.size())) > 1)
	{
		return false;
	}

	string *strLarge = nullptr;
	string *strSmall = nullptr;
	if (str1.size() > str2.size())
	{
		strLarge = &str1;
		strSmall = &str2;
	}
	else
	{
		strLarge = &str2;
		strSmall = &str1;
	}

	bool oneEdit = false;
	for (int strLargeIdx = 0, strSmallIdx = 0;
			strLargeIdx < static_cast<int>(strLarge->size()) &&
					strSmallIdx < static_cast<int>(strSmall->size());
			++strLargeIdx, ++strSmallIdx)
	{
		if ((*strLarge)[strLargeIdx] != (*strSmall)[strSmallIdx])
		{
			if (oneEdit)
			{
				return false;
			}
			else
			{
				if (str1.size() != str2.size())
				{
 					--strSmallIdx; // To realign both index's
				}
				oneEdit = true;
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------

/* Implement a method to perform basic string compression using the counts
 * of repeated characters. For example, the string aabcccccaaa would become a2b1c5a3. If the
 * "compressed" string would not become smaller than the original string, your method should return
 * the original string. You can assume the string has only uppercase and lowercase letters (a - z). */
string stringCompress(string str)
{
	if (str.size() <= 2)
	{
		return str;
	}

	string out = "";
	int charCount = 1;
	for (int i = 1; i <= static_cast<int>(str.size()); ++i)
	{
		if (i < static_cast<int>(str.size()) &&
				str[i - 1] == str[i])
		{
			++charCount;
		}
		else
		{
			out += str[i - 1];
			out += to_string(charCount);
			charCount = 1;
		}
	}

	return (out.size() < str.size()) ? out : str;
}


//-----------------------------------------------------------------------------
/* Given an image represented by an NxN matrix, where each pixel in the image is 4
 * bytes, write a method to rotate the image by 90 degrees. (an you do this in place? */
/* Rotate layer by layer
 * 1) copy top row, then rotate left->top, bottom->left, right->bottom, tmp/top->right
 * O(n) extra memory
 * 2) *perform a swap where layer by layer O(n^2) as it touches all elements in nxn matrix
 * idea is fill the already processed index's first */
template <int n>
bool rotateMatrix90ClockWise(int arr[n][n])
{
	if(n <= 1)
	{
		return false;
	}

	int layers = n / 2;
	for (int layer = 0; layer < layers; ++layer)
	{
		int first = layer;
		int last = (n - 1) - layer;

		// Iterate over all items in layer
		for (int i = first; i < last; ++i)
		{
			int offset = i - first;

			// Save top
			int tmp = arr[first][i];

			// left -> top
			arr[first][i] = arr[last - offset][first];

			// bottom -> left
			arr[last - offset][first] = arr[last][last - offset];

			// right -> bottom
			arr[last][last - offset] = arr[i][last];

			// top -> right
			arr[i][last] = tmp;
		}
	}
	return true;
}


//-----------------------------------------------------------------------------

/* Write an algorithm such that if an element in an MxN matrix is 0, its entire row and
 * column are set to O.
 * 1) preprocess matrix to find zeros => O(n^2) space, O(n^2*(n+n))=O(n^3) time (n + m = setting zeros & n*m = search in copy)
 * 2) preprocess matrix but since more than 1 zero on the same row/col will have the same zeroing
 * effect on that row/col just need to track per each row and col => O(2n) = O(n) space
 * time can be O(n^2)
 * 3) Based on solution 2 above, which is based on solution 1, use first row and col as preprocess
 * array instead to save memory  */
template <int n>
void zeroMatrix(bool matrix[n][n])
{
	bool clearCol[n] = {false};
	bool clearRow[n] = {true};

	// Preprocess and find zeros
	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			if (matrix[row][col] == false)
			{
				clearRow[row] = true;
				clearCol[col] = true;
			}
		}
	}

	// Zero matrix
	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			if (clearRow[row] || clearCol[col])
			{
				matrix[row][col] = false;
			}
		}
	}
}

template <int n>
void zeroMatrixReduceMemory(bool matrix[n][n])
{
	bool rowHasZero = false;
	bool colHasZero = false;

	// Check if zero in first row
	for (int col = 0; col < n; ++col)
	{
		if (matrix[0][col] == false)
		{
			rowHasZero = true;
			break;
		}
	}

	// Check if zero in first col
	for (int row = 0; row < n; ++row)
	{
		if (matrix[row][0] == false)
		{
			colHasZero = true;
			break;
		}
	}

	// Preprocess and find zeros
	for (int row = 1; row < n; ++row)
	{
		for (int col = 1; col < n; ++col)
		{
			if (matrix[row][col] == false)
			{
				matrix[row][0] = false;
				matrix[0][col] = false;
			}
		}
	}

	// Zero internal matrix
	for (int col = 1; col < n; ++col)
	{
		if (matrix[0][col] == false)
		{
			for (int row = 1; row < n; ++row)
			{
				matrix[row][col] = false;
			}
		}
	}

	for (int row = 1; row < n; ++row)
	{
		if (matrix[row][0] == false)
		{
			for (int col = 1; col < n; ++col)
			{
				matrix[row][col] = false;
			}
		}
	}

	// Handle first row and col of matrix
	if (rowHasZero)
	{
		for (int col = 0; col < n; ++col)
		{
			matrix[0][col] = false;
		}
	}

	if (colHasZero)
	{
		for (int row = 0; row < n; ++row)
		{
			matrix[row][0] = false;
		}
	}
}


//-----------------------------------------------------------------------------


/* Check if s2 is a rotation of s1 using only one isSubstring call
 * Shift string to left and rotate/wrap onto right
 * s1=AB s2=BA where A=first part of string, B=second part
 *
 * ex. s1=wat erbottle s2=erbottle wat
 *        A   B           B        A
 *
 *
 * s1=  ABAB +2s1
 * s2=   BA
 * Space=O(1)
 * Time=O(n) */
bool isRotation(string s1, string s2)
{
	if (s1.size() != s2.size())
	{
		return false;
	}

	s1 += s1; // duplicate

	return (string::npos != s1.find(s2));
}

//-----------------------------------------------------------------------------


void Chap1ArraysStringsMain()
{
	bool pass = true;

	// isUnique
	assert(pass &= (true == isUnique("abcde")));
	assert(pass &= (false == isUnique("hello world")));
	assert(pass &= (true == isUniqueSet("abcde")));
	assert(pass &= (false == isUniqueSet("hello world")));
	assert(pass &= (true == isUniqueSort("abcde")));
	assert(pass &= (false == isUniqueSort("hello world")));
	assert(pass &= (true == isUniqueBit("abcde")));
	assert(pass &= (false == isUniqueBit("hello world")));

	// permutation
	assert(pass &= (true == permutation("hello", "lohel")));
	assert(pass &= (true == permutationCount("hello", "lohel")));

	// URLify
	string url = "Mr John Smith    ";
	assert(pass &= ("Mr%20John%20Smith" == URLify(url, 13)));

	// Permutation of Palidrome
	assert(pass &= (true == containsPalidromPermutation("Tact Coa")));
	assert(pass &= (true == containsPalidromPermutationOptimized("Tact Coa")));
	assert(pass &= (true == containsPalidromPermutationBit("Tact Coa")));

	// Is One Edit Difference
	assert(pass &= (true == isOneAway("pale", "ple")));
	assert(pass &= (true == isOneAway("pales", "pale")));
	assert(pass &= (true == isOneAway("pale", "bale")));
	assert(pass &= (false == isOneAway("pale", "bae")));
	assert(pass &= (false == isOneAway("pale", "ples")));

	assert(pass &= (true == isOneAwayCompressed("pale", "ple")));
	assert(pass &= (true == isOneAwayCompressed("pales", "pale")));
	assert(pass &= (true == isOneAwayCompressed("pale", "bale")));
	assert(pass &= (false == isOneAwayCompressed("pale", "bae")));
	assert(pass &= (false == isOneAwayCompressed("pale", "ples")));

	// Compress String
	assert(pass &= ("a2b1c5a3" == stringCompress("aabcccccaaa")));

	// Rotate
	int arr[4][4] = {
			{1,2,3,4},
			{5,6,7,8},
			{9,10,11,12},
			{13,14,15,16}
	};
	rotateMatrix90ClockWise<4>(arr);
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			cout << arr[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;

	// Zero Matrix
	bool matrix[3][3] = {
			{true, true, false},
			{true, false, true},
			{true, true, true}
	};
	zeroMatrix<3>(matrix);
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			cout << matrix[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;

	bool matrix2[3][3] = {
			{true, true, false},
			{true, false, true},
			{true, true, true}
	};
	zeroMatrixReduceMemory<3>(matrix2);
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			cout << matrix2[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;

	// String rotation
	assert(pass &= (true == isRotation("waterbottle", "erbottlewat")));

	cout << "Pass: " << pass << endl;
}
