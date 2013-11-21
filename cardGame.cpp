#include "cardGame.h"

cardGame::cardGame() : pot(0)
{
	initDeck();
	shuffle(deck);
}

cardGame::~cardGame()
{
}

void cardGame::initDeck()
{
	for (int i = 0; i < 52; ++i)
	{
		deck.push_back(card(i % 13, i / 13));
	}
}

void cardGame::shuffle(std::list<card> &d)
{
	int s = d.size();
	for (int i = s;i ; --i)
	{
		list<card>::iterator iter = d.begin();
		for (int x = rand() % i; x; --x, ++iter){}
		d.push_back(*iter);
		d.erase(iter);
	}
}

card cardGame::deal()
{
	card c = deck.front();
	deck.pop_front();
	return c;
}

void cardGame::displayTable(const list<card> *gamHand, player &gam, const int &pot,
	const std::string &gameName, const list<card> *dealer, const list<card> *gamHand2, const int &ins)
{
	system("CLS");
	std::cout << "\t\t" << gameName << " table";
	std::cout << "   Pot: " << pot;
	if (ins)
		std::cout << "  Insurance: " << ins;
	
	if (dealer)
	{
		std::cout << "\n  Dealer hand:";
		displayHand(*dealer);
	}

	if (!gamHand->empty()) std::cout << "\n  " << gam.getName() << "'s hand: ";
	std::cout << "  Chips: " << gam.getChips();
	displayHand(*gamHand);
	if (gamHand2) displayHand(*gamHand2);
}

void cardGame::displayHand(list<card> hand)
{
	if (hand.empty()) return;
	char s, c; 
	int b;
	std::string cardDesign[14] = { "02020", "20202", "50005", "50205", "50505",
		"52505", "52525", "57505", "57525", "71152", "25563", "55655", "00200", "joker" };
	// each digit in five-digit numbers is one line of current drawn card 
	// i.e '5' (binary - 101) = sign, no sign, sign
		
	for (int rows = 0; rows < 9; ++rows)
	{
		std::cout << std::endl;
		for (list<card>::iterator iter = hand.begin(); iter != hand.end(); ++iter)
		{
			if (rows == 0 || rows == 8)
			{
				std::cout << "+-----+ " ;
				continue;
			}
			if (!iter->faceOn())
			{
				std::cout << ":=====: ";
				continue;
			}
			if (rows == 1 || rows == 7)
			{
				displayCardRank(rows, iter);

				continue;
			}
			std::cout << ": ";
			if (iter->getRankEnum() == card::joker)
			{
				std::cout << " " << cardDesign[12][rows - 2] << "  : ";
				continue;
			}
			s = static_cast<char>(iter->getSuit() + 3); // calculate card sign ASCII code
			b = iter->getRank();
			for (int colbit = 4; colbit > 0; colbit >>= 1)
			{
				c = cardDesign[b][rows - 2];
				std::cout << (atoi(&c) & colbit ? s : ' '); // calculate whether print or not card sign in current line
			}

			std::cout << " : ";		
		}
	}
	std::cout << std::endl;
}

void cardGame::displayCardRank(int &_rows, list<card>::iterator _iter)
{ // top-line and bottom-line with card rank
	if (_rows == 1) std::cout << ":" << _iter->getRankSymbol() << (_iter->getRank() == card::ten ? "0   : " : "    : ");
	else 
	{
		std::cout << ":   ";
		if (_iter->getRank() == card::ten)
			std::cout << "10: ";
		else std::cout << " " << _iter->getRankSymbol() << ": ";
	}
}

void cardGame::reshuffle()
{
	deck.splice(deck.end(), restDeck);
	shuffle(deck);
}

void cardGame::bettingPrompt()
{
	std::cout << "\n Enter your bet (0 - exit): ";
}