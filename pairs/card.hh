/* Luokka: Card
 * ------------
 * Kuvaa yhtä korttia muistipelissä.
 *
 * COMP.CS.110 K2021
 * */
//
/* Class: Card
 * -----------
 * Represents a single card in pairs (memory) game.
 *
 * COMP.CS.110 K2021
 * */

/*
* Ohjelman tekijä:
* Nimi: Matias Haapasalmi
* E-mail: matias.haapasalmi@live.com
* */


#ifndef CARD_HH
#define CARD_HH

enum Visibility_type {OPEN, HIDDEN, EMPTY};
const char HIDDEN_CHAR = '#';
const char EMPTY_CHAR = '.';

class Card
{
public:
    // Oletusrakentaja: luo tyhjän kortin.
    //
    // Default constructor: creates an empty card.
    Card();

    // Rakentaja: luo piilotetun kortin, jonka kirjaimeksi tulee annettu merkki.
    //
    // Constructor: creates a hidden card with the given character.
    Card(const char c);

    // Ilmeiset setter- ja getter-metodit.
    //
    // Obvious setter and getter methods.
    void set_letter(const char c);
    void set_visibility(const Visibility_type visibility);
    char get_letter() const;
    Visibility_type get_visibility() const;

    // Kääntää kortin: vaihtaa näkyvyyden avoimesta piilotettuun ja päin vastoin.
    //
    // Turns a card: changes the visibility from open to hidden and vice versa.
    void turn();

    // Tulostaa kortin sen tämänhetkisen tilanteen (näkyvyyden) mukaisesti.
    //
    // Prints a card based on its current state (visibility).
    void print() const;

    // Poistaa kortin pelilaudalta: muuttaa näkyvyyden tyhjäksi.
    //
    // Removes a card from the game board: changes the visibility to empty.
    void remove_from_game_board();

private:
    char letter_;
    Visibility_type visibility_;
};

#endif // CARD_HH
