#include "player.h"

player::player(int ch, std::string s) : chips(ch), name(s)
{
}


player::~player()
{
}

int player::bet(int maxbet, bool autoBet)
{
	int b = maxbet;
	//std::cout << "Enter number (0 - " << maxbet << "): ";
	
	while (!autoBet)
	{
		std::cin.clear();
		std::cin.sync();
		std::cin >> b;
		if (std::cin.fail() || b > maxbet) return -1;
		break;
	} 
	chips -= b;
	return b;
}

void player::reward(int r)
{
	chips += r;
}
