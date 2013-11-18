//CCasino is a simple game where you are gambler at a casino with a few card games
//
//
//TO DO: 
//- create method/class for create/save player profile
//- more games
//- use a multiplatform library for console input / output


#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "casino.h"

//using namespace std;

int main()
{
    srand(time(nullptr));

	casino WCCasino;
	WCCasino.enter();
	
    return 0;
}
