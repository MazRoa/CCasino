#include "blackjackGame.h"

blackjackGame::blackjackGame() : splitted(false), insurance(0)
{
	
}


blackjackGame::~blackjackGame()
{
}

int blackjackGame::checkScore(list<card> &hand)
{
	int score(0);
	card::ranks value;

	for (list<card>::iterator iter = hand.begin(); iter != hand.end(); ++iter)
	{
		value = iter->getRankEnum();
		if (value <= card::ten) score += static_cast<int>(value) + 2;
		else if (value <= card::king) score += 10;
	}
	for (list<card>::iterator iter = hand.begin(); iter != hand.end(); ++iter)
	{
		value = iter->getRankEnum();
		if (value == card::ace) // an Ace has different points depending o overall hand score
		{
			if (score < 11) score += 11;
			else score += 1;
		}
	}
	return score;
}

// i know this function is long but it contains all BlackJack mechanics
void blackjackGame::play(player &gambler)
{
	bool b_double, b_insurance, b_split, stand;
	b_double = b_insurance = b_split = stand = false;
	char c;
	int score(0);
	displayTable(&gamblerHand, gambler, pot, "BlackJack");
	while (true)
	{
		if (!gambler.getChips()) break;

		splitted = false;
		insurance = 0;
		b_double = b_insurance = b_split = stand = false;

		bettingPrompt();
		pot = gambler.bet(gambler.getChips());
		if (!pot) break;
		else if (pot < 0) continue;

		// initial dealing
		hit(&gamblerHand);
		hit(&dealerHand);
		hit(&gamblerHand);
		dealerHand.push_back(deal());

		list<card> *ptr_hand = &gamblerHand;
		
		while (true)
		{
			displayTable(&gamblerHand, gambler, pot, "BlackJack", &dealerHand, &gamblerHandSplitted, insurance);
			writePlayerScore();
			
			if (checkScore(*ptr_hand) >= 21 || stand)
			{
				if (gamblerHandSplitted.empty() || ptr_hand == &gamblerHandSplitted)
					break;
				else
				{
					ptr_hand = &gamblerHandSplitted;
					stand = false;
				}
			}

			if (!splitted && (gamblerHand.size() == 2) && (checkScore(gamblerHand) <= 10) 
				&& (gambler.getChips() >= pot)) b_double = true;
			else b_double = false;

			if (gamblerHandSplitted.empty() && checkSplitPossibility() && gambler.getChips() >= pot)
				b_split = true;
			//else b_split = false;

			if (!insurance && dealerHand.front().getRankEnum() == card::ace && gamblerHand.size() == 2 && 
				gamblerHandSplitted.empty() && (gambler.getChips() > pot / 2)) b_insurance = true;
			else b_insurance = false;

			decisionPrompt(ptr_hand, b_double, b_split, b_insurance);

			c = _getch();
			switch (c)
			{
			case 'h': {
						  hit(ptr_hand);
						  break;
			}
			case 's': {
						  stand = true;
						  break;
			}
			case 'd': {
						  if (b_double)
						  {
							  hit(ptr_hand);
							  pot = gambler.bet(pot, true) * 2;
							  stand = true;
						  }
						  break;
			}
			case 'p': {
						  if (b_split)
						  {
							  bjSplit();
							  pot += gambler.bet(pot, true);
						  }
						  b_split = false;
						  splitted = true;
						  break;
			}
			case 'i': {
						  if (b_insurance)
						  {
							  insurance = pot / 2;
							  gambler.bet(insurance, true);
						  }
						  break;
			}
			default: break;
			}
		}
		
		if ((checkScore(gamblerHand) > 21) && (!splitted || checkScore(gamblerHandSplitted) > 21))
		{
			std::cout << "\n  Cash: " << gambler.getChips();
		}
		else dealerTurn(splitted);
		
		std::string *dealerText = new std::string;
		score = calcResults(insurance, dealerText);
		gambler.reward(score);
				
		displayTable(&gamblerHand, gambler, 0, "BlackJack", &dealerHand, &gamblerHandSplitted);
		writePlayerScore();
		
		if (dealerHand.back().faceOn())
		{
			std::cout << " Dealer: ";
			writeScore(checkScore(dealerHand), dealerHand.size());
		}

		std::cout << *dealerText;
		delete dealerText;

		std::cout << "\n Reward: " << score;// << "  Cash: " << gambler.getChips();

		cleanTable();
		checkForReshuffling();
	}
}

void blackjackGame::decisionPrompt(list<card> *_ptr_hand, bool _b_double, bool _b_split, bool _b_insurance)
{
	if (!gamblerHandSplitted.empty())
	{
		std::cout << (_ptr_hand == &gamblerHand ? " Upper hand decision: " : " Lower hand decision: ");
	}
	else std::cout << " Decision: ";

	std::cout << "(H)it (S)tand " << (_b_double ? "(D)ouble " : "")
		<< (_b_split ? "S(p)lit " : "") << (_b_insurance ? "(I)nsurance" : "");
}

void blackjackGame::dealerTurn(const bool &splitted)
{
	dealerHand.back().turnFace();
	if (splitted || !((checkScore(gamblerHand) == 21) && (gamblerHand.size() == 2)))
	{
		while (checkScore(dealerHand) < 17)
		{
			dealerHand.push_back(deal());
			dealerHand.back().turnFace();
		}
	}
}

void blackjackGame::hit(list<card> *ptr_hand)
{
	ptr_hand->push_back(deal());
	ptr_hand->back().turnFace();
}

bool blackjackGame::checkSplitPossibility()
{
	if (gamblerHand.size() == 2)
	{
		card::ranks value1 = gamblerHand.front().getRankEnum();
		card::ranks value2 = gamblerHand.back().getRankEnum();
		if (value1 == value2 || value1 >= card::ten && value1 <= card::king && value2 >= card::ten && value2 <= card::king)
			return true;

	}
	return false;
}

void blackjackGame::bjSplit()
{
	gamblerHandSplitted.push_front(gamblerHand.back());
	gamblerHand.pop_back();
}

void blackjackGame::writeScore(int score, int cards)
{
	if (score > 21) std::cout << "Busted! ";
	else if (score == 21 && cards == 2) 
		std::cout << "BlackJack! ";
	else std::cout << score;
}

int blackjackGame::calcResults(int insurance, std::string *text)
{
	int reward(0);
	int gamblerScore( checkScore(gamblerHand) );
	int gamblerScoreSplit( checkScore(gamblerHandSplitted) );
	int dealerScore( checkScore(dealerHand) );
	int gamblerCards( gamblerHand.size() );
	int gamblerCardsSplit( gamblerHandSplitted.size() );
	int dealerCards( dealerHand.size() );

	bool firstHandWon(false);
	bool secondHandWon(false);

	gamblerScore = (gamblerScore > 21) ? 0 : gamblerScore;
	gamblerScoreSplit = (gamblerScoreSplit > 21) ? 0 : gamblerScoreSplit;
	dealerScore = (dealerScore > 21) ? 0 : dealerScore;

	if (gamblerScore == 21 && gamblerCards == 2 && !gamblerCardsSplit)// && gamblerCards == 2)
	{
		if (dealerScore == 21 && dealerCards == 2)
		{
			*text = " Push!";
			return pot;
		}
		else return pot * 3;
	}

	if (dealerScore == 21 && dealerCards == 2)
	{
		*text = (insurance ? " Insurance returned" : " You lose!");
		return (insurance ? insurance * 2 : 0);
	}

	if (gamblerScore > dealerScore)
	{
		if (!gamblerCardsSplit)
		{
			*text = " You won!";
			return pot * 2;
		}
		else
		{
			reward += pot;
			firstHandWon = true;
		}
	}
	else if (gamblerScore && (gamblerScore == dealerScore))
	{
		if (!gamblerCardsSplit) 
		{
			*text = " Push!";
			return pot;
		}
		else reward += pot / 2;
	}

	if (gamblerScoreSplit > dealerScore)
	{
		reward += pot;
		secondHandWon = true;
	}
	else if (gamblerScoreSplit && (gamblerScoreSplit == dealerScore))
	{
		reward += pot / 2;
	}

	if (firstHandWon && secondHandWon)
	{
		*text = " You won! ";
	}
	else if (firstHandWon != secondHandWon)
	{
		*text = " One hand won ";
	}
	else if (gamblerScore == dealerScore && gamblerScoreSplit == dealerScore)
	{
		*text = " Push! ";
	}
	else *text = " You lose! ";

	return reward;
}

void blackjackGame::cleanTable()
{
	restDeck.splice(restDeck.end(), dealerHand);
	restDeck.splice(restDeck.end(), gamblerHand);
	restDeck.splice(restDeck.end(), gamblerHandSplitted);
	for (auto iter = restDeck.begin(); iter != restDeck.end(); ++iter)
	{
		if (iter->faceOn()) 
			iter->turnFace();
	}
}

void blackjackGame::writePlayerScore()
{
	if (!gamblerHandSplitted.empty())
	{
		std::cout << "Upper hand: ";
		writeScore(checkScore(gamblerHand), gamblerHand.size());
		std::cout << " Lower hand: ";
		writeScore(checkScore(gamblerHandSplitted), gamblerHandSplitted.size());
	}
	else
	{
		std::cout << "Points: ";
		writeScore(checkScore(gamblerHand), gamblerHand.size());
	}
}

void blackjackGame::checkForReshuffling()
{
	if (deck.size() < 15)
	{
		std::cout << "\n Deck will be reshuffled ";
		reshuffle();
	}
}
