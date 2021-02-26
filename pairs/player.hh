/* Luokka: Player
 * --------------
 * Kuvaa yhtä pelaajaa muistipelissä.
 *
 * COMP.CS.110 K2021
 * */
//
/* Class: Player
 * -------------
 * Represents a single player in pairs (memory) game.
 *
 * COMP.CS.110 K2021
 * */

/*
* Ohjelman tekijä:
* Nimi: Matias Haapasalmi
* E-mail: matias.haapasalmi@live.com
* */


#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

class Player
{
public:
    // Rakentaja: luo annetun nimisen pelaajan.
    //
    // Constructor: creates a player with the given name.
    //Pelaajalla on aina nimi ja pisteet, jotka alustetaan nollaksi.
    Player(const std::string& name, unsigned int points = 0);

    // Palauttaa pelaajan nimen.
    //
    // Returns the name of the player.
    std::string get_name() const;

    // Palauttaa pelaajan tähän asti keräämien parien määrän.
    //
    // Returns the number of pairs collected by the player so far.
    unsigned int number_of_pairs() const;

    // Siirtää annetun kortin pelilaudalta pelaajalle,
    // eli lisää kortin pelaajan keräämiin kortteihin
    // ja poistaa sen pelilaudalta.
    //
    // Moves the given card from the game board for the player,
    // i.e. inserts it to the collected cards of the player and
    // removes it from the game board.
    void add_card(Card& card);

    // Tulostaa pelaajan tilanteen: nimen ja tähän asti kerättyjen parien määrän.
    //
    // Prints the game status of the player: name and collected pairs so far.
    void print() const;

private:
    // Lisää tarvittavat attribuutit tähän
    // Add necessary attributes here
    const std::string name_;
    unsigned int points_;
};

#endif // PLAYER_HH
