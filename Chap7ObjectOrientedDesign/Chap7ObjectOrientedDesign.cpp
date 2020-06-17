/*
 * Chap7ObjectOrientedDesign.cpp
 *
 *  Created on: May 23, 2020
 *      Author: ckirsch
 */
#include "Chap7ObjectOrientedDesign.h"

#include "DeckOfCards/Card.h"
#include "DeckOfCards/Hand.h"
#include "DeckOfCards/Deck.h"
#include "DeckOfCards/BlackJackCard.h"
#include "DeckOfCards/BlackJackHand.h"
#include "DeckOfCards/BlackJackPlayer.h"
#include "DeckOfCards/BlackJackDeck.h"
#include "DeckOfCards/BlackJackGame.h"

#include <stdio.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <type_traits>
#include <numeric>
using namespace std;


void blackJackGame()
{
    const int numPlayers = 5;
    BlackJackGame game;

    // Setup Game
    game.initializeDeck();

    bool deal = game.initialDeal(numPlayers);
    if (!deal)
    {
        cout << "Initial deal Failed" << endl;
        return;
    }

    // Check initial scores
    game.printHandsAndScores();
    auto blackJacks = game.getBlackJacks();
    if (blackJacks.size() > 0)
    {
        cout << "BlackJacks: ";
        for (auto player : blackJacks)
        {
            cout << "Player: " << player->getName() << " ";
        }
        cout << endl;
    }
    else
    {
        // Play
        bool deal = game.playAllHands();
        if (!deal)
        {
            cout << "Deal Failed" << endl;
            return;
        }
        else
        {
            game.printHandsAndScores();
            auto winners = game.getWinners();
            if (winners.size() > 0)
            {
                cout << "Winners: " << endl;
                for (auto winner : winners)
                {
                    cout << "Player: " << winner->getName() << endl;
                }
            }
            else
            {
               cout << "Draw. All players have busted." << endl;
            }
        }
    }
}


void Chap7ObjectOrientedDesignMain()
{
    blackJackGame();
}
