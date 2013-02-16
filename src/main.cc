/*
 * Description: Plays a game of FreeCell
 */

#include "FreeCell.h"

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;


int main() {
    srand( ( unsigned int ) time(NULL) );

    FreeCell game(8,2,10,4);

    //FreeCell game(8,2,5,4);

    game.print();
    while ( ! game.isOver() )
    {
        cout << endl;

        // display promt and receive location of target card
        game.displayPromptAndReceiveInput(0);



        if ( ! game.playerResigned() )
        {
            // prompt, receive destination for the card
            int cancel = game.displayPromptAndReceiveInput(1);
            if ( cancel != 0 )
            {
                continue;
            }
            // check for valid move. if it is move target card to desintaion
            game.moveCard();

            // place a Bunch of spaces. hope this helps with readibility
            cout << endl << endl << endl << endl << endl << endl << endl;

            //display the reserved cells, foundation cells, tableau cards, and cards remaining
            game.print();
        }
    } // end while
    game.printEnding();

    return 0;
}
