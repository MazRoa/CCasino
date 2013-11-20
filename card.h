#ifndef CARD_H
#define CARD_H

#include <string>
class card
{
public:
	enum ranks {two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, joker};
	enum suits {hearts, diamonds, clubs, spades};

	card(short r = 0, short s = 0) : rank(static_cast<ranks>(r)), suit(static_cast<suits>(s)), face(false)
	{}

	card(ranks r , suits s ) : rank(r), suit(s), face(false)
	{}

	~card()
	{}

	short getRank() { return rank; }
	short getSuit() { return suit; }
	ranks getRankEnum() { return static_cast<ranks>(rank); }
	suits getSuitEnum() { return static_cast<suits>(suit); }
	std::string* getRankName() { return &rankNames[rank]; }
	std::string* getSuitName() { return &suitNames[suit]; }
	std::string getCardName() { return rankNames[rank] + "of " + suitNames[suit]; }
	char getRankSymbol();
	char getSuitSymbol();
	bool faceOn() { return face; }
	void turnFace() { (face) ? face = false : face = true; }

	static std::string rankNames[];
	static std::string suitNames[];
	static std::string rankSymbols;
	static int suitSybmols[];

private:
	ranks rank;
	suits suit;
	bool face;

};


#endif

