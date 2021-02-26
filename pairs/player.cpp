#include "player.hh"
#include <cstdlib>
#include <iostream>

// Lisää luokan toteutus tähän.
// Kuhunkin julkiseen metodiin pitäisi riittää 1-2 koodiriviä.

// Add class implementation here.
// It should be enough to write only 1-2 code lines in each public method.

/*
* Ohjelman tekijä:
* Nimi: Matias Haapasalmi
* E-mail: matias.haapasalmi@live.com
* */


Player::Player(const std::string& name, unsigned int points):
    name_(name), points_(points){

}

std::string Player::get_name() const{
    std::string name = "";
    name = name_;
    return name;
}

unsigned int Player::number_of_pairs() const{
    unsigned int points = 0;
    points = points_ / 2;
    return points;
}

void Player::add_card(Card& card){
    points_ = points_ + 1;
    card.remove_from_game_board();
}
