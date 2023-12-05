/*
 * Sisältää tiedot NumberTile -luokan metodeista.
 */

#include "numbertile.hh"
#include "gameboard.hh"
#include <iomanip>
#include <iostream>

Coords operator+(Coords lhs, Coords rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

// Rakentaja
NumberTile::NumberTile(int value, Coords coords, GameBoard* board):
    value_(value), points_(0), coords_(coords), board_(board), is_merged_(false)
{
}

// Destruktori
NumberTile::~NumberTile()
{
}

// Tulostaa peliruudun, jos peliä ei pelata graafisella näkymällä
void NumberTile::print(int width)
{
    std::cout << "|" << std::setw(width - 1) << value_;
}

// Liikuttaa peliruutua parametrina annettuun suuntaan ja yhdistää sen toiseen
// peliruutuun, mikäli se on saman arvoinen
// Parameter 1: Coords, direction; suunta, johono ruutua siirretään
// Parameter 2: int, goal_val; maaliluku
// return: bool, true; jos maaliluku saavutettiin
//               false; jos maalilukua ei saavutettu
bool NumberTile::move(Coords direction, const int goal_val)
{
    Coords curr_loc = coords_;
    Coords new_loc = coords_ + direction;
    while( is_on_board( new_loc ) )
    {
        NumberTile* curr = board_->get_item(curr_loc);
        NumberTile* dest = board_->get_item(new_loc);
        if( dest->value_ == 0 )
        {
            dest->value_ = curr->value_;
            dest->is_merged_ = curr->is_merged_;
            curr->value_ = 0;
            curr->is_merged_ = false;
        }
        else if( dest->value_ == curr->value_ and
                 not dest->is_merged_ and
                 not curr->is_merged_ )
        {
            points_ = points_ + curr->value_;
            dest->value_ = 2 * curr->value_;
            curr->value_ = 0;
            dest->is_merged_ = true;
            return dest->value_ == goal_val;
        }
        curr_loc = new_loc;
        new_loc = dest->coords_ + direction;
    }
    return false;
}

// Antaa peliruudulle uuden arvon, jos peliruudun aikasempi arvo oli nolla.
// Parameter: int, new_val; peliruudulle asetettava uusi arvo
// return: bool, true; mikäli uuden arvon asetus onnistui
//               false; mikäli uuden arvon asetus epäonnistui
bool NumberTile::new_value(int new_val)
{
    if( value_ == 0 )
    {
        value_ = new_val;
        return true;
    }
    return false;
}

// Palauttaa tiedon siitä, onko peliruudun arvo nolla.
// return: bool, true; jos arvo on nolla
//               false; jos arvo ei ole nolla
bool NumberTile::is_empty()
{
    return value_ == 0;
}

// Asettaa peliruudun tideon siitä, onko se yhdistynyt, falseksi.
void NumberTile::reset_turn()
{
    is_merged_ = false;
}

// Palauttaa peliruudun arvon.
// return: int, value; peliruudun arvo
int NumberTile::get_value_()
{
    return value_;
}

// Alustaa peliruudun uudelleen
void NumberTile::reset()
{
    value_ = 0;
    points_ = 0;
}

// Palauttaa peliruudusta saadun pistemäärän.
// return; int, peliruudusta saatu pistemäärä.
int NumberTile::get_points_()
{
    return points_;
}

// Palauttaa tiedon siitä, onko parametrina annetussa sijainnissa oleva
// peliruutu laudalla.
// Parameter: Coords, coords; peliruudun koordinaatit
// return: bool, true; jos peliruutu on laudalla
//               false; jos peliruutu ei ole laudalla
bool NumberTile::is_on_board(Coords coords)
{
    return coords.first >= 0 and coords.first < SIZE and
           coords.second >= 0 and coords.second < SIZE;
}
