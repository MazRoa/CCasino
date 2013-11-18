#pragma once
#include "cardGame.h"

class blackjackGame :
	public cardGame
{
public:
	blackjackGame();// player &);
	~blackjackGame();

	void play(player &);
private:
	
	std::list<card> gamblerHandSplitted;
	std::list<card> dealerHand;
	bool splitted;
	int insurance;

	void writeScore(int , int );
	int checkScore(list<card> &);
	bool checkSplitPossibility();
	void bjSplit();
	int calcResults(int, std::string *);
	void cleanTable();
	void writePlayerScore();
	void checkForReshuffling();
	void hit(list<card> *);
	void dealerTurn(const bool &);
	void decisionPrompt(list<card> *_ptr_hand, bool _b_double, bool _b_split, bool _b_insurance);
	
};

