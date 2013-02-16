/* 
 * File:   Card.h
 * Author: Lucas
 *
 * Created on June 1, 2009, 2:24 AM
 */

#ifndef _CARD_H
#define	_CARD_H



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

class Card
{
public:
    Card();
    
    void print() const;
    string cardToString() const;

    int getRank() const;
    char getSuit() const;
    string getColor() const;
    
    bool isActive() const;
    void makeInactive();

    void setValues(int rank, char suit);
    
private:
    // (int)'H'=72 (int)'D'=68  <- RED (Heart or Diamond)
    // (int)'S'=83  (int)'C'=67 <- BLACK (Spade or Club)
    char suit;
    bool exists;

    // rank can be on [1,CARDS_PER_SUIT]
    int rank;
};




Card::Card()
{
    exists = false;

    // these are dummy values stored...
    rank = 123;
    suit = '.';
}
int Card::getRank() const
{
    return rank;
}
char Card::getSuit() const
{
    return suit;
}
string Card::getColor() const
{
    if ( suit == 's' || suit == 'c' )
        return "BLACK";
    else if ( suit == 'H'|| suit == 'D')
        return "RED";


    throw runtime_error( "ERROR: Card::getColor has invalid member value set on 'suit'");
    return "";
}
void Card::print() const
{
    cout << rank << suit;
}
void Card::setValues( int rnk, char st )
{
    // this cannot take over an existing card.
    assert( ! exists );

    exists = true;
    rank = rnk;
    suit = st;
}
void Card::makeInactive()
{
    exists = false;
}
string Card::cardToString() const
{
    assert( isActive() );

    string name = "";

    // card name begins with rank.
    switch( rank )
    {
        case 1:
            name = "A";
            break;
        case 10:
            name = "T";
            break;
        case 11:
            name = "J";
            break;
        case 12:
            name = "Q";
            break;
        case 13:
            name = "K";
            break;
        default:
        {
            // i couldn't find a better way to do this. The itoa() didn't work and C++ doesn't have .toString() as java
            ostringstream someObj;

            // sorta works like cout...
            someObj << rank;
            name = someObj.str();
        }
    }
    name += suit;

    return name;

}
bool Card::isActive() const
{
    return exists;
}

#endif	/* _CARD_H */

