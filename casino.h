#ifndef CASINO_H
#define CASINO_H

#include <string>
#include <iostream>
#include "player.h"
#include "blackjackGame.h"
#include "pokerGame.h"

class casino
{
public:
	casino();
	~casino();

	void enter();  // when you enter to the casino you will return with nothing :)
};

#endif
