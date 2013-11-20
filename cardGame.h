#ifndef CARDGAME_H
#define CARDGAME_H

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

	void shuffle(list<card> &d);
	void displayTable(const list<card> *, player &, const int &,
		const std::string &, const list<card> * = nullptr, const list<card> * = nullptr,
		const int & = 0);
	virtual void cleanTable();
	void reshuffle();
	void bettingPrompt();

	list<card> deck;
	list<card> gamblerHand;
	list<card> restDeck;
	int pot;

private:
	void initDeck();
	void displayHand(const list<card> );
	void displayCardRank(int &, list<card>::iterator );
};

#endif