/*
 * Sisältää tiedot GameBoard -luokan metodeista.
 */

#include "gameboard.hh"
#include <iostream>

// Luokan rakentaja
GameBoard::GameBoard():
    paused_(true), has_won_(false), high_score_(0)
{
}

// Luokan destruktori
GameBoard::~GameBoard()
{
    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

// Alustaa pelilaudan tyhjänä lautana
void GameBoard::init_empty()
{
    std::vector<NumberTile*> row;
    for( int i = 0; i < SIZE; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < SIZE; ++i)
    {
        board_.push_back(row);
    }
}

// Täyttää pelilaudan siemenluvun perusteella. Pelilauta voi olla tyhjä, täynnä
// tai jotain siltä väliltä.
// Parameter: int, seed; pelilaudan täyttämisen perustana oleva siemenluku
void GameBoard::fill(int seed)
{
    has_won_ = false;
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, SIZE - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            // NumberTilet täytyy luoda mikäli lautaa peliä pelataan ensimmäisen
            // kerran.
            if(board_.at(SIZE -1).at(SIZE-1) == nullptr)
            {
                board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
            }
            // Muussa tapauksessa NumberTilet alustetaan.
            else
            {
                board_.at(y).at(x)->reset();
            }
        }
    }

    for( int i = 0 ; i < SIZE ; ++i )
    {
        new_value();
    }
}

// Lisää pelilaudalle uuden luvun 2 arvoisen ruudun satunnaiseen kohtaan mikäli
// pelilauta ei ole täynnä tai tyhjä.
// Parameter: bool, check_if_empty; true, jos lauta on tyhjä
//                                  false, jos lauta on täynnä
void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}

// Tulostaa pelilaudan, jos peliä pelataan ilman graafista näkymää.
void GameBoard::print() const
{
    for( auto y : board_ )
    {
        std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
        for( auto x : y )
        {
            x->print(PRINT_WIDTH);
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
}

// Siirtää pelilaudan ruutuja haluttuun suuntaan
// Parameter 1: Coords, dir; sisältää tiedon suunnasta mihin ruutuja siirretään
// Parameter 2: int, goal; siältää tiedon maaliluvusta
// return: bool, true; jos peli on voitettu
//               false; jos peliä ei ole voitettu
bool GameBoard::move(Coords dir, int goal)
{
    has_won_ = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won_ = true;
            }
        }
    }
    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            tile->reset_turn();
        }
    }
    return has_won_;
}

// Palauttaa NumberTilen parametrina annettujen koordinaattien kohdasta.
// Parameter: Coords, coords; haetun NumberTilen sijainti
// return: Numbertile halutusta sijainnista.
NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}

// Tarkistaa, onko pelilauta tyhjä.
// return; bool, true; jos lauta on tyhjä.
//               false; jos lauta ei ole tyhjä.
bool GameBoard::is_empty()
{
    if (board_.empty()){
        return true;
    }
    else{
        return false;
    }
}

// Tarkistaa, onko peli meneillään.
// return: bool, true; mikäli peli on meneillään.
//               false; mikäli peli on tauolla, tai se on hävitty tai voitettu.
bool GameBoard::is_game_ongoing()
{
    if(paused_ or has_won_ or is_full()){
        return false;
    }
    else{
        return true;
    }
}

// Asettaa parametrin mukaan pelin tauolle tai pois tauolta.
// Parameter: bool, true; jos peli halutaan pauselle.
//                  false; jos peli halutaan pois pauselta.
void GameBoard::game_pause(bool pause)
{
    paused_ = pause;
}

// Palauttaa pelin pistemäärän
// return: int; pelin pistemäärä
int GameBoard::get_score()
{
    int score = 0;
    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            score = score +  board_.at(y).at(x)->get_points_();
        }
    }
    return score;
}

// Palauttaa pelin ennätspistemäärän.
int GameBoard::get_highscore_()
{
    return high_score_;
}

// Lukee ennätyspistemäärän tiedostosta
// return: int; ennätyspistemäärä
//            ; 0, jos tiedoston luku ei onnistu
int GameBoard::get_highscore_from_file()
{
    std::string filename = "scores.txt";
    std::string line;
    std::ifstream input_file(filename);

    if (not input_file)
    {
        return high_score_;
    }
    else
    {
        while ( getline(input_file, line))
        {
            bool check = false;

            // Tarkastetaan sisältääkö rivi muita kuin numeroita.
            for (uint i = 0; i <= line.length(); ++i)
            {
                if (isalpha(line[i]))
                {
                    check = true;
                    break;
                }
            }

            // Jos rivi sisälsi muita kuin numeroita, sen sisältö hylätään
            if (check){
                continue;
            }

            else if(line != "" and std::stoi(line)>high_score_){
                high_score_ = std::stoi(line);
            }
        }
        input_file.close();
        return high_score_;
    }
}

// Tallentaa pelissä saadun pistemäärän tiedostoon.
// Parameter: int, score; tallennettava ennätyspistemäärä
void GameBoard::save_score(int score)
{
    std::ofstream output_file("scores.txt", std::ofstream::app);
    if (output_file.is_open()){
        output_file << score << "\n";
        output_file.close();
    } else{
        return;
    }
}

// Palauttaa tiedon siitä, onko peli voitettu
// return: bool, true; jos peli on voitettu
//               false; jos peliä ei ole voitettu
bool GameBoard::is_won()
{
    return has_won_;
}

// Asettaa parametrina annetun pistemäärän uudeksi ennätyspistemääräksi
// Parameter; int, score: ennätyspistemääräksi asetettava pistemäärä
void GameBoard::set_highscore(int score)
{
    high_score_ = score;
}

// Tarkistaa, onko pelilauta täynnä
// return: bool, true; jos pelilauta on täynnä
//               false; jos pelilauta ei ole täynnä
bool GameBoard::is_full() const
{
    for( auto y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() )
            {
                return false;
            }
        }
    }
    return true;
}
