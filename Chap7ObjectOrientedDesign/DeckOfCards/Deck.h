/*
 * Deck.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_DECK_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_DECK_H_

#include "Card.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <type_traits>
using namespace std;


template <class C>
class Deck
{
    static_assert(std::is_base_of<Card, C>::value, "C must derive from base class Card");

protected:

    vector<C> cards; // all cards, dealt or not

    int dealtIndex = 0; // marks first undealt card

public:
    void setDeckOfCards(const vector<C> &deckOfCards)
    {
        cards = deckOfCards;
    }

    void shuffle()
    {
        /* This goes through all the cards and picks a random card/index ahead of current index and
         * swaps them
         * example: choose random card from deck and place at top
         * for i = 0 .. n => j = i + rand() % (n - i); swap (i, j) */
        std::random_shuffle(cards.begin(), cards.end());
    }

    int remainingCards() {return cards.size() - dealtIndex;}

    C* dealCard()
    {
        if (remainingCards() == 0)
        {
            return nullptr;
        }

        C *newCard = &cards[dealtIndex++]; // dealIndex [0-(N-1)] -> refers to [1-N] cards dealt after increase
        newCard->markUnavailable();
        return newCard;
    }

    vector<C*> dealHand(int number)
    {
        vector<C*> hand;
        if (remainingCards() == 0)
        {
            return hand;
        }

        for (int i = 0; i < number; ++i)
        {
            hand.push_back(dealCard());
        }

        return hand;
    }
};

#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_DECK_H_ */
