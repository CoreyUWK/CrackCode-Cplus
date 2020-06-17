/*
 * BlackJackHand.h
 *
 *  Created on: May 25, 2020
 *      Author: ckirsch
 */

#ifndef CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKHAND_H_
#define CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKHAND_H_

#include "Hand.h"
#include "BlackJackCard.h"
#include "Hand.cpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class BlackJackHand : public Hand<BlackJackCard>
{
    const int blackJack = 21;

    vector<int> scores;

    void addCardToScoreList(const BlackJackCard &card, vector<int> &scores)
    {
        if (scores.empty())
        {
            scores.push_back(0);
        }

        uint length = scores.size();
        for (uint i = 0; i < length; ++i)
        {
            int score = scores[i];
            scores[i] += card.minVal();

            if (card.minVal() != card.maxVal())
            {
                scores.push_back(score + card.maxVal());
            }
        }
    }

public:

    void addCard(BlackJackCard *card)
    {
        // Setup initial
        if (scores.empty())
        {
            scores.push_back(0);
        }

        // Update Scores
        for (auto &score : scores)
        {
            score += card->minVal();
        }

        if (card->isAce())
        {
            // can really only add 1 ace with value of 11, anymore will be greater than 21
            // can check here or track numAces globally as well
            int newScore = scores.back() + (card->maxVal() - card->minVal());
            if (newScore <= blackJack)
            {
                scores.push_back(newScore);
            }
        }

        Hand::addCard(card);
    }


    vector<int> possibleScores()
    {
        vector<int> scores;

        if (cards.size() == 0)
        {
            return scores;
        }

        for (auto card : cards)
        {
            addCardToScoreList((BlackJackCard)*card, scores);
        }

        return scores;
    }

    /* There are multiple possible scores for a blackjack hand, since aces have multiple values.
     * Return highest possible score that's under 21, or lowest score thats over */
    int score()
    {
        //vector<int> scores = possibleScores();

        int maxUnder = numeric_limits<int>::min();
        int minOver = numeric_limits<int>::max();

        for (auto score : scores)
        {
            if (score > blackJack && score < minOver)
            {
                minOver = score;
            }
            else if (score <= blackJack && score > maxUnder)
            {
                maxUnder = score;
            }
        }

        return (maxUnder == numeric_limits<int>::min()) ?
                minOver : maxUnder;
    }

    bool busted() {return score() > blackJack;}
    bool isBlackJack()
    {
        if (cards.size() > 2)
        {
            return false;
        }

        // 21 = 11 + 10
        BlackJackCard * card1 = cards[0];
        BlackJackCard * card2 = cards[1];

        return ( (card1->isAce() && card2->isFaceCard()) ||
                 (card2->isAce() && card1->isFaceCard()));
    }
};



#endif /* CHAP7OBJECTORIENTEDDESIGN_DECKOFCARDS_BLACKJACKHAND_H_ */
