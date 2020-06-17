/*
 * BlackJackDeck.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKDECK_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKDECK_H_

#include "Deck.h"
#include "BlackJackCard.h"

class BlackJackDeck : public Deck<BlackJackCard> {};


#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKDECK_H_ */
