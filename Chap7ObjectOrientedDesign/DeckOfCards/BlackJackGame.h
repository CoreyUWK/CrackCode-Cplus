/*
 * BlackJackGame.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKGAME_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKGAME_H_

#include "BlackJackPlayer.h"
#include "BlackJackDeck.h"
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

class BlackJackGame
{
    BlackJackDeck deck;
    vector<BlackJackPlayer> players;
    static const int HIT_UNTIL = 16;

public:
    void initializeDeck()
    {
        vector<BlackJackCard> cards;

        for (int i = BlackJackCard::LOWEST_CARD_VALUE;
                i <= BlackJackCard::HIGHEST_CARD_VALUE; ++i)
        {
            for (int j = 0; j < static_cast<int>(Card::Suit::Size); ++j)
            {
                BlackJackCard card(static_cast<Card::Suit>(j), i);
                cards.push_back(card);
            }
        }

        deck.setDeckOfCards(cards);
        deck.shuffle();
    }

    bool initialDeal(int numPlayers)
    {
        if (deck.remainingCards() < numPlayers * 2)
        {
            throw "Deck size cannot support number of players";
            return false;
        }

        for (int i = 0; i < numPlayers; ++i)
        {
            string playerName;
            if (i == numPlayers - 1)
            {
                playerName = "Dealer";
            }
            else
            {
                playerName = to_string(i);
            }
            BlackJackPlayer player(playerName, i);

            auto card1 = deck.dealCard();
            auto card2 = deck.dealCard();

            if (card1 == nullptr || card2 == nullptr)
                return false;

            player.hand.addCard(card1);
            player.hand.addCard(card2);
            players.push_back(player);
        }

        return true;
    }

    vector<BlackJackPlayer*> getBlackJacks()
    {
        vector<BlackJackPlayer*> winners;

        for (auto &player : players)
        {
            if (player.hand.isBlackJack())
            {
                winners.push_back(&player);
            }
        }

        return winners;
    }

    bool playAllHands()
    {
        for (auto &player : players)
        {
            while (player.hand.score() < HIT_UNTIL)
            {
                auto card = deck.dealCard();
                if (nullptr == card)
                {
                    return false;
                }

                player.hand.addCard(card);
            }
        }

        return true;
    }

    vector<BlackJackPlayer*> getWinners()
    {
        vector<BlackJackPlayer*> winners;

        int winningScore = 0;
        for (auto &player : players)
        {
            if (!player.hand.busted())
            {
                int playerScore = player.hand.score();
                if (playerScore > winningScore)
                {
                    winningScore = playerScore;
                    winners.clear();
                    winners.push_back(&player);
                }
                else if (playerScore == winningScore)
                {
                    winners.push_back(&player);
                }
            }
        }

        return winners;
    }

    void printHandsAndScores()
    {
        for (auto player : players)
        {
            cout << "Player: " << player.getName() << " " << player.hand.toString() <<
                    "Score: " << player.hand.score() << endl;
        }
    }
};




#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKGAME_H_ */
