#include "pokerGame.h"

std::string pokerGame::pokerHandsNames[] = { "nothing", "one pair", "two pair",
"three of a kind", "straight", "flush", "full house", "four of a kind",
"straight flush", "royal flush" };

int pokerGame::rewardMult[] = { 0, 1, 2, 4, 10, 20, 40, 100, 200, 400 };

inline bool operator<(card &cardA, card &cardB)
{
	if (cardA.getRank() < cardB.getRank())
		return true;
	else if (cardA.getRank() == cardB.getRank())
		return (cardA.getSuit() < cardB.getSuit());
	else return false;
}

pokerGame::pokerGame()
{
}

pokerGame::~pokerGame()
{
}

void pokerGame::play(player &gambler)
{
	char c;
	pokerHands result;
	displayTable(&gamblerHand, gambler, pot, "Poker");
	while (true)
	{
		if (!gambler.getChips()) // if player is broken - goodbye
		{
			break;
		}

		bettingPrompt();
		pot = gambler.bet(gambler.getChips());
		if (!pot) break;
		else if (pot < 0) continue;

		for (int i = 0; i < 5; ++i) // initial deal
		{
			gamblerHand.push_back(deal());
			gamblerHand.back().turnFace();
		}
		gamblerHand.sort();

		result = checkHand();

		changeCards(gambler);

		if (gambler.getChips() >= pot && (result <= pokerHands::One_pair) )
		{
			std::cout << " \nDouble bet? (y/n)";
			c = _getch();
			if (c == 'y')
			{
				pot += gambler.bet(pot, true);
			}
		}

		gamblerHand.sort();
		displayTable(&gamblerHand, gambler, pot, "Poker");

		result = checkHand();
		gambler.reward(pot * rewardMult[static_cast<int>(result)]);
		displayTable(&gamblerHand, gambler, 0, "Poker");
		writeScore(result, gambler);
		pot = 0;
		cleanTable();

		reshuffle();

	}
}

void pokerGame::changeCards(player &gambler)
{
	char c;
	int cardNum;
	while (true)
	{
		displayTable(&gamblerHand, gambler, pot, "Poker");

		std::cout << " Mark cards for change ( 1 - 5 )";

		c = _getch();
		if (c >= '1' && c <= '5')
		{
			cardNum = atoi(&c) - 1;
			list<card>::iterator iter = gamblerHand.begin();
			for (int i = 0; i < cardNum; ++i, ++iter){}
			iter->turnFace();
		}
		if (c == '\x0d') break;
	}

	for (int i = 0; i < 5; ++i)
	{
		for (auto iter = gamblerHand.begin(); iter != gamblerHand.end(); ++iter)
		{
			if (!iter->faceOn())
			{
				restDeck.splice(restDeck.end(), gamblerHand, iter);
				gamblerHand.splice(gamblerHand.end(), deck, deck.cbegin());
				gamblerHand.back().turnFace();
				break;
			}
		}
	}
}

bool pokerGame::isColor(std::vector<card> *hand) //
{
	for (int i = 0; i < 4; ++i)
	{
		if (hand->at(i).getSuit() != hand->at(i+1).getSuit())
		{
			return false;
		}
	}
	return true;

}

bool pokerGame::isStraight(std::vector<card> *hand) //
{
	for (int i = 0; i < 2; ++i)  // checking 4 cards from left
	{
		if (hand->at(i+1).getRank() - hand->at(i).getRank() != 1) 
		{
			return false;
		}
	}
	
	if ((hand->front().getRank() == card::two)
		&& ((hand->back().getRank() == card::ace) || (hand->back().getRank() == card::six)))
	{// Ace can be the lowest or highest card in straight
		return true; 
	}

	else if ((hand->at(4).getRank() - hand->at(3).getRank()) != 1)
	{
		return false; 
	}
	else return true;
}

bool pokerGame::isPair(std::vector<card> *hand)
{
	for (int i = 0; i < 4; i++)
	{
		if ((hand->at(i).getRank() == hand->at(i+1).getRank()))
		{
			return true;
		}
	}
	return false;
}

// "four of a kind" and "full house" must be checked before this function 
bool pokerGame::isTwoPair(std::vector<card> *hand) 
{// there are 3 posibilities for two pair: 22334, 22344, 23344
	
	if (hand->at(0).getRank() == hand->at(1).getRank())
	{
		if (hand->at(2).getRank() == hand->at(3).getRank() || hand->at(3).getRank() == hand->at(4).getRank())
		{
			return true; 
		}
	}
	else if (hand->at(1).getRank() == hand->at(2).getRank() && hand->at(3).getRank() == hand->at(4).getRank())
	{
		return true;
	}
	return false;
}

bool pokerGame::isThree(std::vector<card> *hand)
{
	for (int i = 0; i < 3; ++i)
	{
		if ((hand->at(i).getRank() == hand->at(i + 1).getRank()) && (hand->at(i).getRank() == hand->at(i + 2).getRank()))
		{
			return true;
		}
	}
	return false;
}

bool pokerGame::isFour(std::vector<card> *hand)
{
	for (int i = 0; i < 2; ++i)
	{
		if ((hand->at(i).getRank() == hand->at(i + 1).getRank())
			&& (hand->at(i).getRank() == hand->at(i + 2).getRank())
			&& (hand->at(i).getRank() == hand->at(i + 3).getRank()))
		{
			return true;
		}
	}
	return false;
}

bool pokerGame::isFullHouse(std::vector<card> *hand)
{
	if ((hand->at(0).getRank() == hand->at(1).getRank())
		&& (hand->at(2).getRank() == hand->at(3).getRank()) && (hand->at(2).getRank() == hand->at(4).getRank()))
	{
		return true;
	}

	if ((hand->at(0).getRank() == hand->at(1).getRank()) && (hand->at(0).getRank() == hand->at(2).getRank())
		&& (hand->at(3).getRank() == hand->at(4).getRank()))
	{
		return true;
	}
	return false;
}

pokerGame::pokerHands pokerGame::checkHand()
{ // create temporary vector from list - it's easier to check poker hands with randomaccesssiterator
	std::vector<card> *v_gamblerHand = new std::vector<card>[5];
	//auto iter = gamblerHand.begin();
	for (auto iter = gamblerHand.begin(); iter != gamblerHand.end(); ++iter)
	{
		v_gamblerHand->push_back(*iter);
	}
	pokerHands p = checkHand(v_gamblerHand);
	delete [] v_gamblerHand;
	return p;
}

pokerGame::pokerHands pokerGame::checkHand(std::vector<card> *v_gamblerHand)
{
	if (isColor(v_gamblerHand))
	{
		if (isStraight(v_gamblerHand))
		{
			if (v_gamblerHand->back().getRank() == card::ace)
			{
				return Royal_flush;
			}
			return Straight_flush;
		}
		return Flush;
	}
	else if (isStraight(v_gamblerHand))
	{
		return Straight;
	}
	else if (isPair(v_gamblerHand))
	{
		if (isThree(v_gamblerHand))
		{
			if (isFullHouse(v_gamblerHand))
			{
				return Full_house;
			}
			else if (isFour(v_gamblerHand))
			{
				return Four_of_a_kind;
			}
			return Three_of_a_kind;
		}
		else if (isTwoPair(v_gamblerHand))
		{
			return Two_pair;
		}
		return One_pair;
	}
	return Nothing;
}

void pokerGame::writeScore(pokerHands hand, player &gambler)
{
	std::cout << "\n You have " << pokerHandsNames[static_cast<int>(hand)] << "!";
	if (hand)
	{
		std::cout << "\n Reward: " << pot * rewardMult[static_cast<int>(hand)];
	}
}

void pokerGame::cleanTable()
{
	restDeck.splice(restDeck.end(), gamblerHand);
	for (auto iter = restDeck.begin(); iter != restDeck.end(); ++iter)
	{
		if (iter->faceOn()) iter->turnFace();
	}
}
