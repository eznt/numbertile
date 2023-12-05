/*
 * Gameboard -luokan header tiedosto. Luokka on itse 2048 -peli ja siinä
 * ilmenee pelin toimintalogiikka.
 */

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>
#include <fstream>
#include <cctype>


const int SIZE = 4;
const int PRINT_WIDTH = 5;
const int NEW_VALUE = 2;
const int DEFAULT_GOAL = 2048;

const Coords DEFAULT_DIR = {0, 0};
const Coords LEFT = {0, -1};
const Coords UP = {-1, 0};
const Coords RIGHT = {0, 1};
const Coords DOWN = {1, 0};

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs.
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers.
    void fill(int seed);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

    // Palauttaa true, mikäli pelilauta on tyhjä ja false, mikäli se ei ole.
    bool is_empty();

    // Palauttaa true, mikäli peli on meneillään ja false, jos ei.
    bool is_game_ongoing();

    // Asettaa parametrin mukaan pelin tauolle tai pois tauolta.
    void game_pause(bool pause);

    // Palauttaa pelin pistemäärän.
    int get_score();

    // Palauttaa pelin ennätyspistemäärän.
    int get_highscore_();

    // Lukee lähdetiedostosta elin ennätyspistemäärän. Palauttaa 0, jos
    // tiedostoa ei ole, tai jos tiedosto ei sisällä pistemäärää oikeassa
    // muodossa.
    int get_highscore_from_file();

    // Tallentaa parametrina annetun pistemäärän tiedostoon.
    void save_score(int score);

    // Palauttaa true, jos peli on voitettu, muulloin false
    bool is_won();

    // Asettaa parametrina annetun pistemäärän uudeksi ennätyspistemääräksi.
    void set_highscore(int score);

private:

    // Kertoo, onko peli tauolla.
    bool paused_;

    // Kertoo, onko peli voitettu.
    bool has_won_;

    // Pelin ennätyspistemäärä.
    int high_score_;

    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;
};

#endif // GAMEBOARD_HH
