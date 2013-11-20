#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include <iostream>
#include "card.h"

class player
{
	friend class blackjackGame;
public:
	player(int ch = 0, std::string = "Dealer");
	~player();


	std::string getName() { return name; }
	int getChips(){ return chips; }
	int bet(int, bool = false);
	void reward(int);
	
private:
	int chips;
	std::string name;
};

#endif
