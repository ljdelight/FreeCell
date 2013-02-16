/* 
 * File:   Tableau.h
 * Author: Lucas
 *
 * Created on June 1, 2009, 2:22 AM
 */

#ifndef _TABLEAU_H
#define	_TABLEAU_H


// used in randomize
const int MAX_SUITS = 4; // also used in class tableau
const int MAX_RANK = 13;
const int ACE = 1;
const int MAX_CASCADES = 9; // used in FreeCell
const int MAX_ROWS = 30;
const int WIDTH = 3;
#include "Card.h"

class Tableau
{

public:
    Tableau( int numCasc = 4, int numSuits = 4, int crdsPSuit = 13);

    void moveTo( int );
    void placeCardAt( int r, int casc, Card c);
    Card retrieveCardAt( int row, int cascade ) const;

    void deactivate( int cascade );
    int findRowIndexContainingExistingCard( int ) const; // given column, find row

    int getNumberOfSuits() const;
    int getNumberOfCardsPerSuit() const;
    int getRowsNeeded() const;
    int getNumberOfCascades() const;
    
    void decreaseTotalCards();

    void print();
private:
    int cardsPerSuit;
    int numberOfSuits;
    int numberOfCards;
    int numberOfCascades;
    int rowsNeeded;
    int rowsNeededPlusExtra;

    Card cardsInCascades[MAX_ROWS][MAX_CASCADES];
    int cardsRemaining;

};





Tableau::Tableau( int numCasc, int numSuits, int crdsPSuit)
{
    cardsPerSuit = crdsPSuit;

    // numberOfSuits includes the # of colors... (max of 4 suits!! )
    numberOfSuits = numSuits;
    numberOfCards = numberOfSuits * cardsPerSuit;

    numberOfCascades = numCasc;
    rowsNeededPlusExtra = 20;

    // this is the total number of cards that have not been placed into the foundation piles
    cardsRemaining = numberOfCards;
}
// places a card in a given row and cascade ONLY if there isn't a card there
void Tableau::placeCardAt( int row, int cascade, Card c )
{
    if ( ! cardsInCascades[row][cascade].isActive() )
    {
        // append values, card will now exist at this point
        cardsInCascades[row][cascade] = c;
    }
}
Card Tableau::retrieveCardAt( int row, int cascade ) const
{
    return cardsInCascades[row][cascade];
}
void Tableau::decreaseTotalCards()
{
    cardsRemaining -= 1;
}
void Tableau::deactivate( int casc )
{

    int r = findRowIndexContainingExistingCard(casc);
    cardsInCascades[r][casc].makeInactive();
}
// returns the row index of an existing card given a cascade to search in. Input and output MUST be zero based. returns -1 if no card in given cascade
int Tableau::findRowIndexContainingExistingCard( int cascade ) const
{
    assert( cascade >= 0 && cascade < numberOfCascades );

    for ( int i = rowsNeededPlusExtra; i >= 0; i-- )
    {
        if ( cardsInCascades[i][cascade].isActive() )
            return i;
    }

    return -1;
}
int Tableau::getNumberOfCascades() const
{
    return numberOfCascades;
}
int Tableau::getNumberOfSuits() const
{
    return numberOfSuits;
}
int Tableau::getNumberOfCardsPerSuit() const
{
    return cardsPerSuit;
}
void Tableau::print()
{
    cout <<endl;

    // display the cascade indices (!! one based)
    for ( int i = 0; i < numberOfCascades; i++ )
    {
        cout << setw( WIDTH ) << i+1;
        cout << " ";
    }
    cout << endl;
    for ( int i = 0; i < numberOfCascades; i++)
    {
        cout << setw( WIDTH ) << "--";
        cout << " ";
    }
    cout << endl;

    // display the cascades
    for ( int r = 0; r < rowsNeededPlusExtra; r++)
    {
        bool somethingPrinted = false;

        // loop through a row and display its elements
        for ( int cascade = 0; cascade < numberOfCascades; cascade++ )
        {
            if ( cardsInCascades[r][cascade].isActive() )
            {
                // print the card
                cout << setw( WIDTH ) << cardsInCascades[r][cascade].cardToString();
                somethingPrinted = true;
            }
            else
                cout << setw( WIDTH ) << " ";

            cout << " ";
        }
        cout << endl;

        // is this a good use of a break statement? prevents MANY blank lines
        if ( ! somethingPrinted )
            break;
    } // end outer for

    cout << "Cards Remaining: " << cardsRemaining << endl;


}


#endif	/* _TABLEAU_H */

