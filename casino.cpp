#include "casino.h"


casino::casino()
{
}


casino::~casino()
{
}

void casino::enter()
{
	std::string name;
	bool timeToGoHome = false;

	//blackjackGame game;
	
	//std::cout << "Enter your name: ";
	//std::cin >> name;
	name = "Player";
	player gambler(100, name);

	while (!timeToGoHome)
	{
		char input;
		system("CLS");
		std::cout << "\t#=- Console Casino -=#";
		std::cout << "  Choose game:\n\n(B)lackJack\n(P)oker - Five-card draw\n\n(e)xit";

		input = _getch();
		tolower(input);

		switch (input)
		{
		case 'b':
		{
			blackjackGame *game = new blackjackGame;
			game->play(gambler);
			delete game;
			break;
		}
		case 'p':
		{
			pokerGame *game = new pokerGame;
			game->play(gambler);
			delete game;
			break;
		}
		case 'e':
		{
			timeToGoHome = true;
			break;
		}
		default: break;
		}
		if (!gambler.getChips())
		{
			std::cout << " \n\nSorry, you don't have any money left. ";
			break;
		}
	}
	std::cout << "\nGoodbye!";
	std::cout << "\nPress any key.";
	_getch();
}
