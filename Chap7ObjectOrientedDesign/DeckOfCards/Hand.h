/*
 * Hand.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_HAND_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_HAND_H_

#include "Card.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;

template <class C>
class Hand
{
    static_assert(std::is_base_of<Card, C>::value, "C must derive from base Card");

protected:
    vector<C*> cards;

public:
    virtual ~Hand() = 0;

    virtual int score()
    {
        int score = 0;
        for (auto card : cards)
        {
            score += card->getValue();
        }

        return score;
    }

    virtual void addCard(C *card)
    {
        cards.push_back(card);
    }

    string toString()
    {
        string hand = "";
        for (auto card : cards)
        {
            hand += card->toString();
            hand += " ";
        }
        return hand;
    }
    void printHand()
    {
        for (auto card : cards)
        {
            cout << *card << " ";
        }
        cout << endl;
    }
};


#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_HAND_H_ */
