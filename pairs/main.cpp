#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

/*
 * Projekti muistipeli
 *
 * Kuvaus:
 * Ohjelma suorittaa siis klassisen muistipelin, jossa pelaajat koittavat etsiä
 * pareja pelilaudalta kortteja vuorotellen kääntelemällä. Pelin loputtua eniten pareja
 * saanut pelaaja/pelaajat voittaa/voittavat pelin.
 * Pelin alussa pelilaudalle arvotaan pelaajalta kysyttyjen arvojen perusteella halutun
 * kokoinen pelilauta jolle kortit sijoitetaan satunnaisjärjestyksessä annetun siemenarvon
 * avulla. Itse peli kulkee siten että ohjelma kysyy pelaajilta vuorotellen kordinaatteja
 * minkä kortin haluavat laudalla kääntää. Jos pelaaja löytää parin hän saa siitä pisteen
 * ja saa jatkaa vuoroaan. Mikäli paria ei löydy siirtyy peli seuraavaan pelaajaan ja niin
 * edelleen. Ohjelma tutkii joka kierroksella onko pelilauta tyhjä, eli onko kaikki kortit
 * nostettu laudalta pois ja sitten kun näin on peli päättyy. Kun peli on päättynyt peli
 * julistaa laskettujen pisteiden perusteella voittajan. Ohjelmassa on useita tarkastajia
 * jotka tutkivat että pelaajien antamat syötteet ovat oikeanlaisia. Esimerkiksi mikäli
 * pelaaja syöttää haluamansa kortit kordinaatit virheellisesti ilmoittaa ohjelma
 * invalid card ilmoituksella siitä pelaajalle ja pyytää syöttämään kordinaatit uudelleen.
 *
 * Ohjelman tekijä:
 * Nimi: Matias Haapasalmi
 * E-mail: matias.haapasalmi@live.com
 * */



using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;
//pelaajat vektori jonka sisälle tulee pelaaja olioita.


// Muuntaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan
// (mikä johtaa laittomaan korttiin myöhemmin).
//
// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0
// (which leads to an invalid card later).
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Täyttää pelilaudan (kooltaan rows * columns) tyhjillä korteilla.
//
// Fills the game board, the size of which is rows * columns, with empty cards.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Etsii seuraavan tyhjän kohdan pelilaudalta (g_board) aloittamalla
// annetusta kohdasta start ja jatkamalla tarvittaessa alusta.
// (Kutsutaan vain funktiosta init_with_cards.)
//
// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Aloitetaan annetusta arvosta
    //
    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }
    // Jatketaan alusta
    //
    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }
    // Tänne ei pitäisi koskaan päätyä
    //
    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
//
// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, int seed)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Arvotaan täytettävä sijainti
    //
    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);
    // Hylätään ensimmäinen satunnaisluku (joka on aina jakauman alaraja)
    //
    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siitä seuraava tyhjä paikka.
    // (Seuraava tyhjä paikka haetaan kierteisesti funktion next_free avulla.)
    //
    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisätään kaksi samaa korttia (parit) pelilaudalle
        //
        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Tulostaa annetusta merkistä c koostuvan rivin,
// jonka pituus annetaan parametrissa line_length.
// (Kutsutaan vain funktiosta print.)
//
// Prints a line consisting of the given character c.
// The length of the line is given in the parameter line_length.
// (Called only by the function print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Tulostaa vaihtelevankokoisen pelilaudan reunuksineen.
//
// Prints a variable-length game board with borders.
void print(const Game_board_type& g_board)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;
    print_line_with_char('-', columns);
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}

// Kysyy käyttäjältä tulon ja sellaiset tulon tekijät, jotka ovat
// mahdollisimman lähellä toisiaan.
//
// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void ask_product_and_calculate_factors(unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

// Funktio kysyy pelaajien määrää ja palauttaa sen unsigned int muodossa.
// Se hyödyntää valmiiksi luotua stoi_with_check funktiota tutkiessaan,
// että syöte on numeraalinen
unsigned int ask_number_of_players(unsigned int& pelaajien_maara){
    while(not (pelaajien_maara > 0)){
        std::cout << INPUT_AMOUNT_OF_PLAYERS;
        std::string maara_str = "";
        std::getline(std::cin, maara_str);
        pelaajien_maara = stoi_with_check(maara_str);

    }
    return pelaajien_maara;
}

//funktio lisää luotuun vektoriin pelaajat, Player tyyppisiä olioita
//joille se alustaa nimen, käyttäjän antamien nimien perusteella.

void pelaajat_vektorin_luoja(std::vector<Player>& pelaajat, unsigned int pelaajien_maara){

    std::string nimi;
    for(unsigned int i = 0; i < pelaajien_maara; ++i){
        std::cin >> nimi;
        Player uusi_pelaaja(nimi);
        pelaajat.push_back(uusi_pelaaja);
    }
}

// funktio tarkastaa vuoro funktiossa käytettävästä syote vektorista, että
// siinä esiintyvät int arvot ovat oikeanlaisia. Eli siis funktio tarkastaa
// tuleeko invalid card vai ei.

bool syote_tarkastaja(std::vector<int>& syote, int rivit, int sarakkeet, Game_board_type& game_board){
    int y1 = syote.at(0);
    int x1 = syote.at(1);
    int y2 = syote.at(2);
    int x2 = syote.at(3);

    for(unsigned int i = 0; i < syote.size();i++){
        if(syote.at(i) < 1){
            return false;
        }
    }
    if(x1 > rivit || x2 > rivit || y1 > sarakkeet || y2 > sarakkeet){
        return false;
    }
    if(x1 == x2 and y1 == y2){
        return false;
    }

    Card kortti1 = game_board.at(x1 - 1).at(y1 - 1);
    Card kortti2 = game_board.at(x2 - 1).at(y2 - 1);
    if(kortti1.get_visibility() == EMPTY or kortti2.get_visibility() == EMPTY){
        return false;
    }

    return true;
}


// Funktio kääntää halutut kortit ja luo uuden laudan jonka se palauttaa
Game_board_type next_board(Game_board_type& game_board, std::vector<int> syote){
    int y1 = syote.at(0);
    int x1 = syote.at(1);
    int y2 = syote.at(2);
    int x2 = syote.at(3);

    game_board.at(x1 - 1).at(y1 - 1).turn();
    game_board.at(x2 - 1).at(y2 - 1).turn();

    return game_board;

}
// funktio selvittää koska peli on ohi käymällä kaikki laudan kortit läpi ja
// mikäli kaikki kortit ovat tyhjiä peli on ohi
bool peli_ohi(Game_board_type& game_board){
    for(Game_row_type row : game_board){
        for(Card kortti : row){
            if(kortti.get_visibility() != EMPTY){
                return false;
            }
        }
    }
   return true;
}
// Mikali peli on pelattu loppuun, siten että kortit loppuvat tämä funktio
// etsii voittajan/voittajat ja hänen/heidän pistemääränsä
void get_winner(std::vector<Player>& pelaajat){
    std::vector<int> voittaja_indeksi;
    unsigned int tulos = 0;
    for(unsigned int i = 0; i < pelaajat.size(); ++i){
        if(pelaajat.at(i).number_of_pairs() > tulos){
            voittaja_indeksi = { };
            voittaja_indeksi.push_back(i);
            tulos = pelaajat.at(i).number_of_pairs();

        }
        else if(pelaajat.at(i).number_of_pairs() == tulos){
            voittaja_indeksi.push_back(i);

        }
    }
    if(voittaja_indeksi.size() > 1){
        std::cout << "Game over!" << endl;
        std::cout << "Tie of " << voittaja_indeksi.size() << " players with " << tulos << " pairs." << endl;
    }
    else{
        Player voittaja = pelaajat.at(voittaja_indeksi.at(0));
        int voittaja_pisteet = voittaja.number_of_pairs();
        std::string voittaja_nimi = voittaja.get_name();
        std::cout << "Game over!" << std::endl;
        std::cout << voittaja_nimi << " has won with " << voittaja_pisteet << " pairs.";
    }
}

// Tämä funktio tuli lopulta vähän pitkäksi, kuitenkin
// funktion ideana on pyörittää itse vuoroja eteenpäin eli saada peli pyörimään
// Siinä on loputon pää for looppi joka käy vuoron läpi, joka katkaistaan peli ohi funktion
// avulla.
int vuoro(std::vector<Player>& pelaajat, unsigned int rivit, unsigned int sarakkeet,
          Game_board_type& game_board){

    std::string vuorossa = "";
    int numero = 0;
    std::vector<int> syote;
    unsigned int in_turn = 0;

    for(;;){

    if(peli_ohi(game_board) == true){
        break;
    }

    if(in_turn == pelaajat.size()){
        in_turn = 0;
    }

    vuorossa = pelaajat.at(in_turn).get_name();

    for(;;){
    syote = { };

        std::cout << vuorossa << ": Enter two cards (x1, y1, x2, y2), or q to quit: ";
        for(int i = 0 ; i < 4; i++){
            std::string syot_str = "";
            std::cin >> syot_str;
            if (syot_str == "q"){
                std::cout << "Why on earth you are giving up the game?" << endl;
                return EXIT_SUCCESS;
            }
            else{
            numero = stoi_with_check(syot_str);
            syote.push_back(numero);
            }
        }
        if(syote_tarkastaja(syote, rivit, sarakkeet, game_board) == true){

            break;
        }
        if(syote_tarkastaja(syote, rivit, sarakkeet, game_board) == false){
                std::cout << "Invalid card." << endl;

        }

    }
        next_board(game_board, syote);
        print(game_board);

        int y1 = syote.at(0) - 1;
        int x1 = syote.at(1) - 1;
        int y2 = syote.at(2) - 1;
        int x2 = syote.at(3) - 1;
        if(game_board.at(x1).at(y1).get_letter() == game_board.at(x2).at(y2).get_letter()){
            pelaajat.at(in_turn).add_card(game_board.at(x1).at(y1));
            pelaajat.at(in_turn).add_card(game_board.at(x2).at(y2));
            std::cout << FOUND << endl;
            for(unsigned int i = 0; i < pelaajat.size(); ++i){
                std::cout << "*** " << pelaajat.at(i).get_name() << " has " << pelaajat.at(i).number_of_pairs() << " pair(s)." << endl;
            }
          }

        else{
            game_board.at(x1).at(y1).turn();
            game_board.at(x2).at(y2).turn();
            in_turn = in_turn + 1;
            std::cout << NOT_FOUND << endl;
            for(unsigned int i = 0; i < pelaajat.size(); ++i){
                std::cout << "*** " << pelaajat.at(i).get_name() << " has " << pelaajat.at(i).number_of_pairs() << " pair(s)." << endl;      
        }
    }
        print(game_board);
    }
    return 2;
}

int main()
{
    Game_board_type game_board;
    //pelaajat vektori jonka sisälle tulee pelaaja olioita.
    std::vector<Player> pelaajat;
    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    unsigned int pelaajien_maara = 0;
    ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);
    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);
    ask_number_of_players(pelaajien_maara);
    std::cout << "List " << pelaajien_maara << " players: ";
    pelaajat_vektorin_luoja(pelaajat, pelaajien_maara);
    print(game_board);
    int vuoroSuccess = vuoro(pelaajat, factor1, factor2, game_board);
    if(vuoroSuccess != EXIT_SUCCESS){
         get_winner(pelaajat);
    }




    return EXIT_SUCCESS;
}

