#pragma once
#include <cstdlib>
#include <list>
#include <iostream>
#include <string>
#include <conio.h>
#include "card.h"
#include "player.h"

using std::list;

class cardGame
{

public:
	cardGame();
	virtual ~cardGame();
	
protected:
	virtual card deal();

	void shuffle(std::list<card> &d);
	void displayTable(const list<card> *, player &, const int &,
		const std::string &, const list<card> * = nullptr, const list<card> * = nullptr,
		const int & = 0);
	virtual void cleanTable();
	void reshuffle();
	void bettingPrompt();

	std::list<card> deck;
	std::list<card> gamblerHand;
	std::list<card> restDeck;
	int pot;

private:
	void initDeck();
	void displayHand(const list<card> );
	void displayCardRank(int &, list<card>::iterator );
};

