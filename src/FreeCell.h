/* 
 * File:   FreeCell.h
 * Author: Lucas
 *
 * Created on June 1, 2009, 2:23 AM
 */

#ifndef _FREECELL_H
#define	_FREECELL_H

#include "Card.h"
#include "Tableau.h"





const int MAX_RESERVED_CELLS = 8;
const int DEFAULT_NUMBER_OF_CASCADES = 8;
const int DEFAULT_NUMBER_OF_SUITS = 4;
const int DEFAULT_NUMBER_OF_CARDS_PER_SUIT = 13;
const int DEFAULT_NUMBER_OF_RESERVED_CELLS = 4;
const bool LOST = false;

const int FOUNDATION_INPUT_IDENTITY = 100;
const int RCELL_WIDTH = 2;





class Randomize
{
public:
    Randomize( Tableau &d );

private:
    Card cardsToShuffle[MAX_SUITS][MAX_RANK];
    int nSuits;
    int cardsPSuit;
    void setDefaults();
    void shuffleDefaults();
    void storeShuffledCards( Tableau & );
};

Randomize::Randomize( Tableau& d )
{
    nSuits = d.getNumberOfSuits();
    cardsPSuit = d.getNumberOfCardsPerSuit();

    setDefaults();
    shuffleDefaults();
    shuffleDefaults();
    storeShuffledCards( d );
}
void Randomize::setDefaults()
{
    char suitChar = 'x';
    // loop through each of the suits
    for ( int suit = 0; suit < nSuits; suit++ )
    {
        switch (suit) {
            case 0:
                suitChar = 'H';
                break;
            case 1:
                suitChar = 's';
                break;
            case 2:
                suitChar = 'D';
                break;
            case 3:
                suitChar = 'c';
                break;
        }
        // loop through all the card ranks
        for ( int cardRank = ACE; cardRank <= cardsPSuit; cardRank++ )
        {
           cardsToShuffle[suit][cardRank-1].setValues(cardRank, suitChar);
        }
    }
}
void Randomize::shuffleDefaults()
{
    Card temp;
    // randomize the cards in each suit
    for ( int suit = 0; suit < nSuits; suit++ )
    {
        // loop through all but the last card
        for ( int i = 0; i < cardsPSuit-1; i++)
        {
            // random int on [i,CARDS_PER_SUIT)
            int indexToSwithWith = (int) rand() % (cardsPSuit-i) + i;

            // swap cards at i and indexToSwitch for current suit
            temp = cardsToShuffle[suit][i];
            cardsToShuffle[suit][i] = cardsToShuffle[suit][indexToSwithWith];
            cardsToShuffle[suit][indexToSwithWith] = temp;
        }
    }
    // randomize the suits
    for ( int i = 0; i < cardsPSuit; i++ )
    {
        // loop through all but the last card
        for ( int suit = 0; suit < nSuits-1; suit++)
        {
            // random int on [suit,nSuits)
            int suitIndexToSwitchWith = (int) rand() % (nSuits-suit) + suit;

            // swap cards at suitIndexToSwitchW and i for current rank index
            temp = cardsToShuffle[suit][i];
            cardsToShuffle[suit][i] = cardsToShuffle[suitIndexToSwitchWith][i];
            cardsToShuffle[suitIndexToSwitchWith][i] = temp;
        }
    }

    return;
}
void Randomize::storeShuffledCards( Tableau& d )
{
    int rowForShuffledCards = 0, colForShuffledCards = 0;
    int rowForTableau = 0, colForTableau = 0;

    int cardsLeft = d.getNumberOfCardsPerSuit() * d.getNumberOfSuits();
    while( cardsLeft > 0 )
    {
        d.placeCardAt(rowForTableau, colForTableau, cardsToShuffle[rowForShuffledCards][colForShuffledCards]);

        colForTableau += 1;
        if ( (colForTableau) % d.getNumberOfCascades() == 0 && colForTableau != 0  )
        {
            rowForTableau += 1;
            colForTableau = 0;
        }

        colForShuffledCards += 1;
        if ( (colForShuffledCards) % (cardsPSuit) == 0 && colForShuffledCards != 0)
        {
            colForShuffledCards = 0;
            rowForShuffledCards += 1;
        }

        cardsLeft--;
    }
}














class FreeCell
{
public:
    FreeCell( int = DEFAULT_NUMBER_OF_CASCADES, int =DEFAULT_NUMBER_OF_SUITS,
            int = DEFAULT_NUMBER_OF_CARDS_PER_SUIT, int = DEFAULT_NUMBER_OF_RESERVED_CELLS);
    void print();
    void printEnding();
    int displayPromptAndReceiveInput(int );
    void convertTarget();
    void convertDestination();
    void moveCard();
    void validateTarget( int );
    void validateDestination( int );
    bool canMoveToFoundation( Card );
    bool validTableauMove( int targ, Card destC );
    void moveToFoundation( Card );
    bool isOver();
    bool isAvailable( int inputTarget );
    bool playerResigned() const;
private:
    Tableau tableau;
    Card foundationPile[MAX_SUITS];
    Card reservedCells[MAX_RESERVED_CELLS];
    int numberOfFoundationPiles;
    int numberOfReservedCells;
    int target;
    int targetDestination;

    bool gameWon, resigned;
    bool isToFoundation;
    bool isToTableau;
    bool isToReserved;
    bool fromReserved;
    bool fromTableau;
};









// this is the constructor for FreeCell
FreeCell::FreeCell( int nCasc, int nSuits, int nCrdsPerSuit, int nRsvCells )
{
    // this checks the constants and the values used to initialize this class. i think i got 'em all.
    assert( nSuits <= MAX_SUITS && nSuits > 0 );
    assert( nCasc <= MAX_CASCADES && nCasc > 0);
    assert( nRsvCells <= MAX_RESERVED_CELLS && nRsvCells >= 0);
    assert( nCrdsPerSuit <= MAX_RANK && nCrdsPerSuit > 0);

    // used to determine when to stop looping
    gameWon = false;
    resigned = false;

    numberOfReservedCells = nRsvCells;
    numberOfFoundationPiles = nSuits;

    // create a temporaty deck
    Tableau t( nCasc, nSuits, nCrdsPerSuit);

    // store the temp deck
    tableau = t;

    // shuffle the tableau pile
    Randomize shuffle( tableau );
}
// void FreeCell::printEnding() -- prints whether the player won or lost
void FreeCell::printEnding()
{
    if ( resigned )
        cout << "Player resigned. Please play again!";
    else
        cout << "WINNER! Woah!! I'm almost surprised YOU won. Awesome.";

    cout << endl;
}
bool FreeCell::playerResigned()  const
{
    return resigned;
}
// void FreeCell::moveToFoundation( Card cardToMove )
//
// PRE: none
// POST: moves given card to the foundation stack.
void FreeCell::moveToFoundation( Card cardToMove )
{
    // since we are removing a card from play, we can decrease the number of playing cards
    tableau.decreaseTotalCards();

    for ( int i = 0; i < numberOfFoundationPiles; i++ )
    {
        if ( ! foundationPile[i].isActive() )
        {
            if ( cardToMove.getRank() == ACE)
            {
                // if the card is an ace and the foundation is empty, we will store the card
                foundationPile[i] = cardToMove;
                return;
            }
        }
        else if ( foundationPile[i].getSuit() == cardToMove.getSuit() )
        {
            // the incoming card must be one HIGHER than the card with the same suit
            if ( foundationPile[i].getRank() == cardToMove.getRank()-1 )
            {

                foundationPile[i] = cardToMove;
                return;
            }
        }
    }
}
// bool FreeCell::canMoveToFoundation( Card cardToMove )
// -- this function is the checker for the one above.
// PRE: none
// POST: returns true if a given card can be placed into the tableau
bool FreeCell::canMoveToFoundation( Card cardToMove )
{
    for ( int i = 0; i < numberOfFoundationPiles; i++ )
    {
        if ( ! foundationPile[i].isActive() )
        {
            if ( cardToMove.getRank() == ACE)
            {
                return true;
            }
        }
        else if ( foundationPile[i].getSuit() == cardToMove.getSuit() )
        {
            if ( foundationPile[i].getRank() == cardToMove.getRank()-1 )
            {
                return true;
            }
        }
    }
    return false;
}

// void FreeCell::moveCard() -- moves a card from target to destination
//
// PRE: there is a checker function that was already called (called validateDestination)
// POST: moves a card to target.
void FreeCell::moveCard()
{
    convertTarget();
    convertDestination();

    Card replacement;
    int oldRow;
    int newRow;
    if ( fromReserved )
    {
        if ( isToFoundation )
        {
            // save the card
            replacement = reservedCells[target];

            // since we are moving to the foundation, we call that fctn
            moveToFoundation( replacement );

            // disables the card that we just moved
            reservedCells[target].makeInactive();
        }
        else if ( isToReserved )
        {
            replacement = reservedCells[target];
            reservedCells[target].makeInactive();

            // store the card to the reserved cell
            reservedCells[targetDestination] = replacement;
        }
        else if ( isToTableau )
        {
            replacement = reservedCells[target];
            reservedCells[target].makeInactive();

            // find the row that the destination card will fall on
            newRow = tableau.findRowIndexContainingExistingCard(targetDestination) + 1;

            // places the card at given location
            tableau.placeCardAt( newRow, targetDestination, replacement );
        }
    }
    // move the card from the tableau to...
    else if ( fromTableau )
    {
        if ( isToFoundation )
        {
            int row =tableau.findRowIndexContainingExistingCard(target);
            replacement = tableau.retrieveCardAt( row , target );
            moveToFoundation( replacement );
            tableau.deactivate( target );
        }
        else if ( isToReserved )
        {

            // find the row that the target card is on
            oldRow =tableau.findRowIndexContainingExistingCard(target);

            // copy the card that needs to be moved
            replacement = tableau.retrieveCardAt( oldRow , target );
            // deactivate that card
            tableau.deactivate( target );

            reservedCells[targetDestination] = replacement;
        }
        else if ( isToTableau )
        {
            // find the row that the card that is moving is on
            oldRow = tableau.findRowIndexContainingExistingCard(target);

            // copy the card that needs to be moved
            replacement = tableau.retrieveCardAt( oldRow , target );
            // deactivate that card
            tableau.deactivate(target );

            // find the row that the destination card will fall on
            newRow = tableau.findRowIndexContainingExistingCard(targetDestination) + 1;
            tableau.placeCardAt( newRow, targetDestination, replacement );
        }
    }// end else-if
}
// bool FreeCell::validTableauMove( int trgt, Card destinationCard )
//
// PRE:
// PRE: returns true if card can be placed onto the tableau
bool FreeCell::validTableauMove( int trgt, Card destinationCard )
{
    assert ( trgt >=0 );
    int row;

    row = tableau.findRowIndexContainingExistingCard(trgt);
    Card targetCard = tableau.retrieveCardAt(row, trgt);

    // compare colors of the cards
    if ( targetCard.getColor() == destinationCard.getColor() )
    {
        return false;
    }

    // target card MUST be one less than the rank of destination card
    else if (!(targetCard.getRank() == destinationCard.getRank() - 1) )
    {
        return false;
    }

    return true;
}
// void FreeCell::validateTarget( int prompt)
// ---This is a little confusing b/c we need to check all the rules of FreeCell
// PRE: promt must be 0 or 1
// POST: This will continuously ask for a valid card to move and will store this value (NOT zero based) when it finds it
void FreeCell::validateTarget( int prompt)
{
    assert( prompt == 0 || prompt == 1 );

    // this means the player has RESIGNED.
    if ( target == 0 )
        resigned = true;

    // if target is a reserved cell, check that it is an actual tableau AND it has a card
    else if ( target >= 10 && target <= 10 * numberOfReservedCells )
    {
        // if there is a remained then this indicates the operator input something like 11
        if ( target % 10 != 0 )
        {
            cout << endl << "!! Location " << target << " is invalid. " << endl;
            displayPromptAndReceiveInput(prompt);
        }
        else if (!reservedCells[target / 10 - 1].isActive() )
        {
            cout << endl << "!! There isn't a card at Location " << target << ". " << endl;
            displayPromptAndReceiveInput(prompt);
        }
        // possible bad values have been checked so we know this is true
        else
        {
            fromReserved = true;
            fromTableau = false;
        }
    }
    // this ensures that the card to be moved is at an actual place. must be on [1,numCascades]
    else if ( target > 0 && target <= tableau.getNumberOfCascades() )
    {
        // if the cascade chosen doesn't have a card, request a new one
        if ( tableau.findRowIndexContainingExistingCard(target - 1) < 0 )
        {
            cout << endl << "Cascade " << target << " does not contain a card." << endl;
            displayPromptAndReceiveInput(prompt);
        }
        else
        {
            fromReserved = false;
            fromTableau = true;
        }
    }
    else
    {
        cout << endl << "!! Location " << target << " is invalid. " << endl;
        displayPromptAndReceiveInput(prompt);
    }

}

// void FreeCell::validateDestination(int prompt) -- this checks that both the target card and destination card are compatible for the move
// !!!--- This is a little confusing b/c we need to check all the rules of FreeCell
// PRE:
// POST: will store boolean values to respective destinations
void FreeCell::validateDestination(int prompt)
{
    // if they decide the do not want to move the same card they can input 0 to select a new one
    if ( targetDestination == 0 )
    {
        // ditch the original target and get a new one.
        displayPromptAndReceiveInput(0);
    }

    // card destination is a foundation
    if ( targetDestination == FOUNDATION_INPUT_IDENTITY )
    {
        Card replacement;
        // determines where the target originated so these if statemenst can determine if a legal move is available
        if ( fromReserved )
        {
            replacement = reservedCells[target / 10 - 1];
        }
        else  if(fromTableau)
        {
            int row =tableau.findRowIndexContainingExistingCard(target-1);
            replacement = tableau.retrieveCardAt(row, target-1);
        }
        if ( canMoveToFoundation(replacement) )
        {
            isToFoundation = true;
            isToTableau = false;
            isToReserved = false;
        }
        else
        {
            cout << "!! Cannot move card at " << target << " to a foundaion pile" << endl;
            displayPromptAndReceiveInput(prompt);
        }



    }// end if for destination Foundation cards

    // if targetDestination is a potential reserved cell...
    else if ( targetDestination >= 10 && targetDestination <= 10 * numberOfReservedCells )
    {
        // Ensure that it is an actual location AND does NOT have a card
        if ( targetDestination % 10 != 0 )
        {
            cout << endl << "!! Location " << targetDestination << " is invalid. " << endl;
            displayPromptAndReceiveInput(prompt);
        }
        else if ( reservedCells[targetDestination / 10 - 1].isActive() )
        {
            cout << endl << "!! There is already a card at Location " << targetDestination << ". " << endl;
            displayPromptAndReceiveInput(prompt);
        }
        else
        {

            isToFoundation = false;
            isToTableau = false;
            isToReserved = true;

        }

    }
    // this is the case for destination as a tableau card
    else if ( targetDestination > 0 && targetDestination <= tableau.getNumberOfCascades() )
    {
        if ( fromTableau)
        {
            int row =tableau.findRowIndexContainingExistingCard(targetDestination-1);
            // indicates that the destination cascade is empty so we can add the card without checking anything.
            if ( row < 0 )
            {

                isToFoundation = false;
                isToTableau = true;
                isToReserved = false;

                // this is needed to prevent errors that would occur if destinationCard attempted to be created
                return;
            }

            Card destinationCard = tableau.retrieveCardAt(row,targetDestination-1);
            // moving from tableau to tablueu so make them zero based
            if ( validTableauMove( target-1, destinationCard ) )
            {
                isToFoundation = false;
                isToTableau = true;
                isToReserved = false;
            }
            else
            {
                cout << "That is not a legal move" << endl;
                displayPromptAndReceiveInput(prompt);
            }
        }
        // moving reserved card to tableau
        else if ( fromReserved )
        {
            // make a copy of cascade card
            int row = tableau.findRowIndexContainingExistingCard(targetDestination - 1);
            if ( row < 0 )
            {
                isToFoundation = false;
                isToTableau = true;
                isToReserved = false;
                return;
            }
            Card destinationCard = tableau.retrieveCardAt(row, targetDestination - 1);

            // make a copy of reserved cell card
            Card targetCard = reservedCells[target / 10 - 1];

            // compare colors of the cards
            if ( targetCard.getColor() == destinationCard.getColor() )
            {
                cout << "That is not a legal move" << endl;
                displayPromptAndReceiveInput(prompt);
            }
            // target card MUST be one less than the rank of destination card
            else if ( !(targetCard.getRank() == destinationCard.getRank() - 1) )
            {
                cout << "That is not a legal move" << endl;
                displayPromptAndReceiveInput(prompt);
            }
            else
            {
                isToFoundation = false;
                isToTableau = true;
                isToReserved = false;
            }
        }


    }
    else
    {
        cerr << endl << "!! Location " << targetDestination << " is invalid";
        displayPromptAndReceiveInput(prompt);
    }
}
int FreeCell::displayPromptAndReceiveInput( int prompt)
{
    if ( cin.fail() )
            exit(1);

    string inputStuff = "";

    switch( prompt )
    {
        case 0: {
            // prompt
            cout << "From which pile do you wish to take a card? (1-" << tableau.getNumberOfCascades()
                    << ", 10-" << 10*numberOfReservedCells << ", 0 to resign): ";
            // receive
            cin >> target;
            validateTarget(prompt);

            break;
        } // end switch, case: 0
        case 1: {
            // prompt
            cout  << endl << "Moving card from " << target <<". Where do you wish to place this card? (-1 to cancel) ";
            // receive
            cin >> targetDestination;
            if ( targetDestination < 0 )
            {
                return -1;
            }
            validateDestination(prompt);

            break;
        } // end switch, case: 1
    } // end switch

    return 0;

}
void FreeCell::convertTarget()
{
    if (fromReserved)
        target = target / 10 - 1;
    else if (fromTableau)
        target -= 1;
}
void FreeCell::convertDestination()
{
    if (isToFoundation)
        targetDestination = 0;
    else if (isToTableau)
        targetDestination -= 1;
    else if (isToReserved)
        targetDestination = targetDestination / 10 - 1;
}
bool FreeCell::isOver()
{
    // if the player resigned, the game is over
    if ( resigned )
        return true;

    // checks if one of the foundation cards aren't equal to the max rank
    for ( int i = 0; i < numberOfFoundationPiles; i++ )
    {
        if ( foundationPile[i].getRank() != tableau.getNumberOfCardsPerSuit() )
            return false;
    }
    // if the above for doesn't return false, then all reserved cells have the max values. gameWon
    gameWon = true;
    return true;
}
void FreeCell::print()
{
    // display reserved cells
    for ( int g = 0; g < numberOfReservedCells; g++ )
    {
        cout << 10*(g+1) << ":";
        if ( reservedCells[g].isActive() )
        {
            cout << setw( RCELL_WIDTH ) << reservedCells[g].cardToString();
        }
        else
            cout << setw( RCELL_WIDTH ) << "__";
        cout << " ";
    }

    cout << "   ";

    // display foundation stacks
    cout << FOUNDATION_INPUT_IDENTITY << ":";
    for ( int g = 0; g < numberOfFoundationPiles; g++ )
    {
        if ( foundationPile[g].isActive() )
        {
            cout << left << setw( RCELL_WIDTH ) << foundationPile[g].cardToString();
        }
        else
            cout << left << setw( WIDTH ) << "__";
        cout << " ";
    }
    cout << endl;

    tableau.print();
}

#endif	/* _FREECELL_H */

