#ifndef POKERGAME_H
#define POKERGAME_H

#include <vector>
#include "cardGame.h"
class pokerGame :
	public cardGame
{
public:
	enum pokerHands { Nothing, One_pair, Two_pair, Three_of_a_kind, Straight, Flush, Full_house,
		Four_of_a_kind, Straight_flush, Royal_flush };

	pokerGame();
	~pokerGame();

	void play(player &);

private:
	void sortHand();
	pokerHands checkHand();
	pokerHands checkHand(std::vector<card> *);

	bool isColor(std::vector<card> *);
	bool isStraight(std::vector<card> *);
	bool isPair(std::vector<card> *);
	bool isTwoPair(std::vector<card> *);
	bool isThree(std::vector<card> *);
	bool isFour(std::vector<card> *);
	bool isFullHouse(std::vector<card> *);
	void writeScore(pokerHands, player &);
	void cleanTable();

	void changeCards(player &);

	static std::string pokerHandsNames[];
	static int rewardMult[];
		
};

#endif

