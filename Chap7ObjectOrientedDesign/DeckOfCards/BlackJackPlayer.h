/*
 * BlackJackPlayer.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKPLAYER_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKPLAYER_H_

#include "BlackJackHand.h"
#include <stdio.h>
using namespace std;


class BlackJackPlayer
{
    string name;
    int turnIdx;
public:
    BlackJackHand hand;
    BlackJackPlayer(string name, int turnIdx) : name(name), turnIdx(turnIdx) {}

    string getName() {return name;}
};


#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKPLAYER_H_ */
