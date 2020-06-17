/*
 * BlackJackCard.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKCARD_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKCARD_H_

#include "Card.h"
#include <stdio.h>
#include <iostream>
using namespace std;


/* Black Jack */
class BlackJackCard : public Card
{

public:
    static const int LOWEST_CARD_VALUE = 1;
    static const int HIGHEST_CARD_VALUE = 13;

    BlackJackCard(Card::Suit suit, int value) : Card(suit, value) {}
    int getValue() const
    {
        if (isAce()) return 1;
        else if (isFaceCard()) return 10;
        else return faceValue;
    }

    bool isAce() const {return faceValue == 1;}

    int minVal() const
    {
        if (isAce()) return 1;
        return getValue();
    }

    int maxVal() const
    {
        if (isAce()) return 11;
        return getValue();
    }

    bool isFaceCard() const
    {
        return faceValue >= 11 && faceValue <= 13;
    }
};



#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKCARD_H_ */
