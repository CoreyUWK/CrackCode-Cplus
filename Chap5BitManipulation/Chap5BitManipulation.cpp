/*
 * Chap5BitManipulation.cpp
 *
 *  Created on: May 20, 2020
 *      Author: ckirsch
 */
#include "Chap5BitManipulation.h"
#include <cinttypes>
#include <bitset>
#include <stdio.h>
#include <iostream>
#include <cassert>

using namespace std;


bool getBit(int num, int i)
{
    return (num & (1 << i)) != 0;
}

int setBit(int num, int i)
{
    return num | (1 << i);
}

int clearBit(int num, int i)
{
    int mask = ~(1 << i);
    return num & mask;
}

int clearBitMSBthroughI(int num, int i)
{
    int mask = (1 << i) - 1;
    return num & mask;
}

int clearBitsIthrough0(int num, int i)
{
    int mask = -1 << (i + 1);
    return num & mask;
}

int updateBit(int num, int i, bool bitIs1)
{
    int val = (bitIs1) ? 1 : 0;
    int mask = ~(1 << i);
    return (num & mask) | (val << i);
}

void insert(uint32_t &n, uint32_t m, int i, int j)
{
    // create mask containing 1s at j
    uint32_t onesAtJ = (1 << j);
    // create mask containing 1s up to (including) j
    uint32_t onesUpToIncludingJ = onesAtJ | (onesAtJ - 1);

    // Create mask containing 1s before i
    uint32_t onesBeforeI = (1 << i) - 1;

    // inverting onesUpToIncludingJ mask to make 0s before j and ANDing to make 0s between i-j
    uint32_t mask = ~onesUpToIncludingJ | onesBeforeI;

    // 0s between i and j
    n &= mask;

    // Shift m and merge m and n
    n |= (m << i); // merge m into n at i-j
}

/* You are given two 32-bit numbers, N and M, and two bit positions, i and j. Write a method to
 * insert M into N such that M starts at bit j and ends at bit i. You can assume that the bits j
 * through i have enough space to fit all of M. That is, if M = 10011, you can assume that there
 * are at least 5 bits between j and i. You would not, for example, have j = 3 and i = 2, because
 * M could not fully fit between bit 3 and bit 2.
EXAMPLE
Input: N=10000000000, M=10011, i=2, j=6
Output: N=10001001100 */
void insertTest()
{
    uint32_t n = 0b10000000000;
    uint32_t m = 0b10011;
    insert(n, m, 2, 6);

    bitset<32> N(n);
    cout << "Insert: " << N << endl;
    assert(n == 0b10001001100);

    cout << "insertTest: Passed" << endl;
}

/* Note: ? = 0.101 = 1*(1/2^1) + 0*(1/2^2) + 1*(1/2^3)
 * = 1*(0.5) + 0*(0.25) + 1*(0.125) = 0.625 */
string decimalToBinaryPrint1(double num)
{
    string binaryNum = "0.";
    double fraction = 0.5;
    while (num > 0)
    {
        if (num >= fraction)
        {
            binaryNum += "1";
            num -= fraction;
        }
        else
        {
            binaryNum += "0";
        }

        fraction /= 2;
    }

    return binaryNum;
}
string decimalToBinaryPrint2(double num)
{
    string binaryNum = "0.";
    while (num > 0)
    {
        num *= 2;
        if (num >= 1)
        {
            binaryNum += "1";
            num -= 1;
        }
        else
        {
            binaryNum += "0";
        }
    }

    return binaryNum;
}

/* Given a real number between 0 and 1 (e.g., 0.72) that is passed in as a double, print
the binary representation. If the number cannot be represented accurately in binary with at most 32
characters, print "ERROR:' */
void decimalToBinaryPrintTest()
{
    string binaryNum = decimalToBinaryPrint1(0.625);
    cout << "decimalToBinary1: " << binaryNum << endl;
    assert(0 == binaryNum.compare("0.101"));

    binaryNum = decimalToBinaryPrint2(0.625);
    cout << "decimalToBinary2: " << binaryNum << endl;
    assert(0 == binaryNum.compare("0.101"));
}


int flipBitToWinBruteForce(int num)
{
    const int byteToBit = 8;
    const int sizeOfNumBit = sizeof(num) * byteToBit;

    if (~num == 0) return sizeOfNumBit;

    bitset<sizeOfNumBit> b(num);

    int maxCount = 1;
    for (uint i = 0; i < b.size(); ++i)
    {
        int count = 0;
        bool countHasZero = false;
        for (uint j = i; j < b.size(); ++j)
        {
            if (0 == b[j])
            {
                // if already added a zero stop current count check
                if (countHasZero)
                {
                    break;
                }

                // else add current zero to count
                countHasZero = true;
            }

            ++count;
        }

        if (maxCount < count)
            maxCount = count;
    }

    return maxCount;
}

int flipBitToWin(int num)
{
    const int byteToBit = 8;
    const int sizeOfNumBit = sizeof(num) * byteToBit;

    if (~num == 0) return sizeOfNumBit;

    bitset<sizeOfNumBit> b(num);

    int maxCount = 1; // can always flip zero to 1
    int count = 0;
    int prevZeroCount = 0;
    bool countHasZero = false;
    for (uint i = 0; i < b.size(); ++i)
    {
        if (b[i])
        {
            ++count;
        }
        else
        {
            if (countHasZero)
            {
                count -= prevZeroCount + 1; // +1 for current zero

                // reset as hitting back to back zeros
                if (i > 0 && 0 == b[i-1])
                {
                    count = 1;
                }
            }
            else
                ++count;

            prevZeroCount = count;
            countHasZero = true;
        }

        if (maxCount < count)
        {
            maxCount = count;
        }
    }

    return maxCount;
}

/* You have an integer and you can flip exactly one bit from a 13 to a 1. Write code to find the
 * length of the longest sequence of 1s you could create.
EXAMPLE
Input: 1775 (or: 1113111131111)
Output: 8 */
void flipBitToWinTest()
{
    cout << "flipBitToWinBruteForce: " << flipBitToWinBruteForce(1775) << endl;
    cout << "flipBitToWin: " << flipBitToWin(1775) << endl;

    cout << "flipBitToWinBruteForce: " << flipBitToWinBruteForce(87863) << endl;
    cout << "flipBitToWin: " << flipBitToWin(87863) << endl;

}


uint nextNumberBruteForce(uint num, bool up)
{
    auto countOnes = [](uint num) -> uint
    {
        int countOnes = 0;
        while (num > 0)
        {
            if (0x1 == (num & 0x1))
            {
                ++countOnes;
            }
            num >>= 1;
        }
        return countOnes;
    };

    // check if all ones so can't increase, or contains no ones so can never find another greater
    // number with same number of ones
    if (num == (uint)~0 || num == 0)
    {
        throw "no next plus number";
    }

    // count number of ones
    uint numOnes = countOnes(num);
    int movement = (up) ? 1 : -1;

    num += movement;
    while (numOnes != countOnes(num)) {num += movement;}

    return num;
}

uint nextNumberBitManipulation(uint num)
{
    if (num == 0 || num == (uint)~0)
    {
        throw "No valid next number with same number of bits";
    }

    int countZeros = 0;
    int countOnes = 0;

    uint numTmp = num;
    while (numTmp > 0 && 0x0 == (numTmp & 0x1))
    {
        ++countZeros;
        numTmp >>= 1;
    }

    while (0x1 == (numTmp & 0x1))
    {
        ++countOnes;
        numTmp >>= 1;
    }

    uint plusPosition = countZeros + countOnes;

    uint plusMask = 1 << plusPosition; // 0.010.0
    uint zeroMask = (~0) << plusPosition; // 11..1100..00

    num |= plusMask; // add new bit position for plus
    num &= zeroMask; // zero out all before new bit position for plus, zero out so can add new values

    uint shiftedOnes = (1 << (countOnes - 1)) - 1; // move front trailing ones so start at bit 0, minus one as one of the 1s bit was set at plus bit
    num |= shiftedOnes; // add front ones

    return num;
}
uint prevNumberBitManipulation(uint num)
{
    if (num == 0 || num == (uint)~0)
    {
        throw "No valid next number with same number of bits";
    }

    int countOnes = 0;
    int countZeros = 0;

    uint numTmp = num;
    while (0x1 == (numTmp & 0x1))
    {
        ++countOnes;
        numTmp >>= 1;
    }

    while (numTmp > 0 && 0x0 == (numTmp & 0x1))
    {
        ++countZeros;
        numTmp >>= 1;
    }

    uint minusCount = countOnes + countZeros;
    uint minusMask = (~0) << (minusCount + 1); // 11..1100..00

    // Flip 1 to zero and leading zero to add new values
    num &= minusMask; // X..X0..0

    // make sequence of ones of countOnes + 1 (contains flipped 1 and front 1s)
    uint frontOnes = (1 << (countOnes + 1)) - 1;

    // Add shifted front ones plus the flipped 1
    num |= frontOnes << (countZeros - 1);

    return num;
}

uint nextNumberArithmetic(uint num)
{
    if (num == 0 || num == (uint)~0)
    {
        throw "No valid next number with same number of bits";
    }

    int countOnes = 0;
    int countZeros = 0;

    uint numTmp = num;
    while (numTmp > 0 && 0x0 == (numTmp & 0x1))
    {
        ++countZeros;
        numTmp >>= 1;
    }

    while (0x1 == (numTmp & 0x1))
    {
        ++countOnes;
        numTmp >>= 1;
    }

    //int plusBit = countZeros + countOnes;

    // toggle all leading zeros to 1, and then add 1 to set plusBit followed by zeros
    //num += ((1 << countZeros) - 1); // X..X01..1
    //num += 1; // X..X10..0

    // Add back leadings 1s but shifted down so plus number is at it minimum
    //num += (1 << (countOnes - 1)) - 1; // count - 1 as plusBit took one of the 1s, works cause all front bits from above is zeros

    /* equivalent to:
     * num + ((2^zeros) - 1) + 1 + (2^(ones - 1) - 1)
     * = num + 2^zeros + 2^(ones - 1) - 1
     * = num + 1 << zeros + 1 << (ones - 1) - 1 */
    return num + (1 << countZeros) + (1 << (countOnes - 1)) - 1;
}

uint prevNumberArithmetic(uint num)
{
    if (num == 0 || num == (uint)~0)
    {
        throw "No valid next number with same number of bits";
    }

    int countOnes = 0;
    int countZeros = 0;

    uint numTmp = num;
    while (0x1 == (numTmp & 0x1))
    {
        ++countOnes;
        numTmp >>= 1;
    }

    while (numTmp > 0 && 0x0 == (numTmp & 0x1))
    {
        ++countZeros;
        numTmp >>= 1;
    }

    //uint minusBit = countOnes + countZeros;

    // to set minusBit to zero will clear all front ones and then - 1
    //num -= (1 << countOnes) - 1;
    //num -= 1;

    // need to remove extra ones at front, that is shift the orignal ones
    //num -= (1 << (countZeros - 1)) - 1;

    /* is equivalent to:
     * num - (2^ones - 1) - 1 - (2^(zeros - 1) - 1)
     * = num - 2^ones + 1 - 1 - 2^(zeros - 1) + 1
     * = num  - 2^ones - 2^(zeros - 1) + 1 */
    return num - (1 << countOnes) - (1 << (countZeros - 1)) + 1;
}

/* Given a positive integer, print the next smallest and the next largest number that
have the same number of 1 bits in their binary representation. */
void nextNumberTest()
{
    // next 5
    uint nextNum = nextNumberBruteForce(5, true);
    cout << "nextNumberBruteForcePlus: 5->" << nextNum << endl;
    assert(nextNum == 6);

    nextNum = nextNumberBitManipulation(5);
    cout << "nextNumberBitManipulation: 5->" << nextNum << endl;
    assert(nextNum == 6);

    nextNum = nextNumberArithmetic(5);
    cout << "nextNumberArithmetic: 5->" << nextNum << endl;
    assert(nextNum == 6);

    // prev 5
    nextNum = nextNumberBruteForce(5, false);
    cout << "nextNumberBruteForceMinus: 5<-" << nextNum << endl;
    assert(nextNum == 3);

    nextNum = prevNumberBitManipulation(5);
    cout << "prevNumberBitManipulation: 5<-" << nextNum << endl;
    assert(nextNum == 3);

    nextNum = prevNumberArithmetic(5);
    cout << "prevNumberArithmetic: 5<-" << nextNum << endl;
    assert(nextNum == 3);

    // next 13948
    nextNum = nextNumberBruteForce(13948, true);
    cout << "nextNumberBruteForcePlus: 13948->" << nextNum << endl;
    assert(nextNum == 13967);

    nextNum = nextNumberBitManipulation(13948);
    cout << "nextNumberBitManipulation: 13948->" << nextNum << endl;
    assert(nextNum == 13967);

    nextNum = nextNumberArithmetic(13948);
    cout << "nextNumberArithmetic: 13948->" << nextNum << endl;
    assert(nextNum == 13967);

    // prev 13948
    nextNum = nextNumberBruteForce(13948, false);
    cout << "nextNumberBruteForceMinus: 13948<-" << nextNum << endl;
    assert(nextNum == 13946);

    nextNum = prevNumberBitManipulation(13948);
    cout << "prevNumberBitManipulation: 13948<-" << nextNum << endl;
    assert(nextNum == 13946);

    nextNum = prevNumberArithmetic(13948);
    cout << "prevNumberArithmetic: 13948<-" << nextNum << endl;
    assert(nextNum == 13946);
}

int conversionBruteForce(int a, int b)
{
    int count = 0;
    for (int aXorB = a ^ b; aXorB > 0; aXorB >>= 1)
    {
        if (0x1 == (aXorB & 0x1))
            ++count;
    }

    return count;
}
int conversionOptinal(int a, int b)
{
    int count = 0;
    for (int aXorB = a ^ b; aXorB > 0; aXorB &= (aXorB - 1))
    {
        ++count;
    }

    return count;
}

/* Write a function to determine the number of bits you would need to flip to convert integer A to integer B.
EXAMPLE
Input: 11101, 01111
Output: 2 */
void conversionTest()
{
    assert(2 == conversionBruteForce(0b11101, 0b01111));
    assert(2 == conversionOptinal(0b11101, 0b01111));

    cout << "conversionTest: passed" << endl;
}

int pairWiseSwap(int num)
{
    int firstPair = 0x55555555;
    int secondPair = 0xAAAAAAAA;

    return (((uint)(num & secondPair)) >> 1) | ((num & firstPair) << 1);
}

/* Pairwise Swap: Write a program to swap odd and even bits in an integer with as few instructions as
possible (e.g., bit 13 and bit 1 are swapped, bit 2 and bit 3 are swapped, and so on). */
void pairWiseSwapTest()
{
    assert((int)0b10000110011010101111011000110100 == pairWiseSwap(0b01001001100101011111100100111000));
    cout << "pairWiseSwapTest: passed" << endl;
}


/* width, x, y => in bits
 * y = [0 to height]
 * x = [0 to width*8] */
void drawLine(char screen[], int width, int x1, int x2, int y)
{
    const int bitsInBytes = 8;
    const int bytesInWidth = width / bitsInBytes;
    int yPos = bytesInWidth * y;
    int x1BytePos = x1 / bitsInBytes;
    int x1BitPos = x1 % bitsInBytes;

    int x2BytePos = x2 / bitsInBytes;
    int x2BitPos = x2 % bitsInBytes;

    // Set first byte if not starting from first bit in byte
    if (0 != x1BitPos)
    {
        screen[yPos + x1BytePos++] |= 0xFF << x1BitPos;
    }

    // Set all bytes until (not including) last byte
    for (;x1BytePos < x2BytePos; ++x1BytePos)
    {
        screen[yPos + x1BytePos] = 0xFF;
    }

    // Set last byte
    if ((bitsInBytes - 1) == x2BitPos)
    {
        screen[yPos + x2BytePos] = 0xFF;
    }
    else
    {
        screen[yPos + x2BytePos] |= ((1 << (x2BitPos + 1)) - 1);
    }
}


void drawLineTest()
{
    const int bitsInBytes = 8;
    const int width = 8;
    const int heigth = 8;
    char screen[width * heigth] = {};

    int x1 = 11;
    int x2 = 44;
    int y = 3;

    drawLine(screen, width * bitsInBytes, x1, x2, y);

    for (int i = 0; i < width; ++i)
    {
        bitset<bitsInBytes>byte(screen[(width * y) + i]);
        cout << i << ":";
        for (int j = 0; j < bitsInBytes; ++j)
        {
            cout << byte[j];
        }
        cout << " ";
    }
    cout << endl;
}

void Chap5BitManipulationMain()
{
    insertTest();
    decimalToBinaryPrintTest();
    flipBitToWinTest();
    nextNumberTest();
    conversionTest();
    pairWiseSwapTest();
    drawLineTest();
}

