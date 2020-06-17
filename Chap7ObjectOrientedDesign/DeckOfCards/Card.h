/*
 * Card.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_CARD_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_CARD_H_

#include <stdio.h>
#include <iostream>
#include <type_traits>
using namespace std;


class Card
{
public:
    enum class Suit
    {
        Heart = 0,
        Spades,
        Diamond,
        Club,
        Size
    };

protected:
    Suit suit;
    /* Number or face that's on card - a number 2 through 10, or 11 for Jack, 12 for Queen, 13 for
     * King, or 1 for Ace */
    int faceValue;

    bool avaliable;

    friend std::ostream & operator<<(std::ostream & Str, Card const & v) {
        // print something from v to str, e.g: Str << v.getX();
        return Str << v.toString();
    }

public:
    Card(Suit suit, int value) : suit(suit), faceValue(value), avaliable(true) {}
    virtual ~Card() = 0;

    virtual int getValue() const
    {
        return faceValue;
    }

    Suit getSuit() const {return suit;}

    /* Checks if the card is available to be given out to someone */
    bool isAvailable () const {return avaliable;}
    void markUnavailable () {avaliable = false;}
    void markAvailable() {avaliable = true;}

    string toString() const
    {
        string suit;

        switch (this->suit)
        {
        case Suit::Heart:
            suit = "Heart";
            break;
        case Suit::Spades:
            suit = "Suit";
            break;
        case Suit::Diamond:
            suit = "Diamond";
            break;
        case Suit::Club:
            suit = "Club";
            break;
        default:
            throw "Invalid Suit";
            break;
        }

        return suit + to_string(this->faceValue);
    }
};


#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_CARD_H_ */
