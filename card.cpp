#include "card.h"

std::string card::rankNames[] = { "Two ", "Three ", "Four ", "Five ", "Six ", "Seven ", "Eight ",
"Nine ", "Ten ", "Jack ", "Queen ", "King ", "Ace ", "Joker " };
std::string card::suitNames[] = { "Hearts ", "Diamonds ", "Clubs ", "Spades " };
std::string card::rankSymbols = "234567891JQKA;";
int card::suitSybmols[] = { 3, 4, 5, 6 };

char card::getRankSymbol()
{
	if (!face)
		return '-';
	else return rankSymbols[static_cast<int>(rank)];
}

char card::getSuitSymbol()
{
	if (!face)
		return '-';
	else return suitSybmols[static_cast<int>(suit)];
}
