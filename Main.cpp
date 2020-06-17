/*
 * Main.cpp
 *
 *  Created on: 2019-09-22
 *      Author: ckirsch
 */
#include "Chap1ArraysStrings/Chap1ArraysStringsMain.h"
#include "Chap2LinkedList/Chap2LinkedListMain.h"
#include "Chap3StacksQueues/Chap3StacksQueues.h"
#include "Chap4TreesGraphs/Chap4TreesGraphs.h"
#include "Chap5BitManipulation/Chap5BitManipulation.h"
#include "Chap7ObjectOrientedDesign/Chap7ObjectOrientedDesign.h"
#include "Chap8Recursion/Chap8Recursion.h"
#include "Chap10SortingAndSearching/Chap10SortingAndSearchingMain.h"
#include "SmartPointers/SmartPointers.h"
#include <iostream>
using namespace std;



/* Need to finish Chap6 and 7 */
int main()
{
    // Study: Trees, Recursion, OOD

	cout << "Chap1ArraysStringsMain: " << endl;
	Chap1ArraysStringsMain();
	cout << endl;

	cout << "Chap2LinkedListMain: " << endl;
	Chap2LinkedListMain();
	cout << endl;

	cout << "Chap3StacksQueuesMain: " << endl;
	Chap3StacksQueuesMain();
	cout << endl;

    cout << "Chap4TreesGraphsMain: " << endl;
    Chap4TreesGraphsMain();
    cout << endl;

    cout << "Chap5BitManipulationMain: " << endl;
    Chap5BitManipulationMain();
    cout << endl;

    cout << "Chap7ObjectOrientedDesignMain: " << endl;
    Chap7ObjectOrientedDesignMain();
    cout << endl;

    cout << "chap8RecusionMain: " << endl;
    chap8RecusionMain();
    cout << endl;

    cout << "Chap10SortingAndSearchingMain: " << endl;
    Chap10SortingAndSearchingMain();
    cout << endl;

    cout << "SmartPointersMain: " << endl;
    SmartPointersMain();
    cout << endl;

	return 0;
}

