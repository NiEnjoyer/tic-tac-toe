#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

class Gamer {
public:
    int id, points;
    char sign;
private:
    int wins, draws, losses;
public:
    Gamer(int id, char sign) {
        this->id = id;
        this->sign = sign;
        this->wins = 0;
        this->draws = 0;
        this->losses = 0;
        this->points = 0;
    }
    Gamer(){};
    void add_one(std::string statement) {
        std::map<std::string, int> keys;
        keys["wins"] = 0;
        keys["draws"] = 1;
        keys["losses"] = 2;
        keys["points"] = 3;
        switch (keys[statement]) {
            case 0: {this->wins++; break;}
            case 1: {this->draws++; break;}
            case 2: {this->losses++; break;}
            case 3: {this->points++; break;}
            default: {std::cout<<"Sth wrong with adding"; break;}
        }
    }
    void show() {
        std::cout<<this->id<<" "<<this->sign<<" "<<this->wins<<" "<<this->draws<<" "<<this->losses<<" "<<this->points<<std::endl;
    }
    void show_title() {
        std::cout<<"ID Symbol Wygrane Remisy Porażki Punkty"<<std::endl;
    }
    void calc_points() {
        this->points = this->wins*2 + this->draws*1;
    }
    ~Gamer(){};
};

struct Data {
    std::vector<std::string> games;
    std::vector<std::pair<int, char>> players;
    Data(std::vector<std::string> game, std::vector<std::pair<int, char>> play) {
        games = game;
        players = play;
    }
    Data() {};
};

struct Result {
    char w_sign, l_sign, d_sign;
    bool is_draw;
    Result(char w_s, char l_s, bool is_d = false, char d_s = '.'){
        w_sign = w_s; l_sign = l_s; is_draw = is_d; d_sign = d_s;
    }
    Result(){};
};

Data handle_tic_tac_toe();
std::pair<char, char> detect_players(std::string str);
Result determine_game(std::string str);

int main() {
    bool exit = true;
    std::vector<std::pair<int, char>> players;
    std::vector<std::string> games;
    Data puller;
    puller = handle_tic_tac_toe();
    players = puller.players;
    games = puller.games;
    std::vector<Gamer> gamers;
    for (int i=0; i<players.size(); i++) {
        gamers.emplace_back(players.at(i).first, players.at(i).second);
    }
    auto labda_per_id = [](const Gamer& lhs, const Gamer& rhs) -> bool{return lhs.id < rhs.id;};
    auto labda_per_sign = [](const Gamer& lhs, const Gamer& rhs) -> bool{return lhs.sign < rhs.sign;};
    sort(gamers.begin(), gamers.end(), labda_per_id);

    for (auto& i : games) {
        Result tempo = determine_game(i);
        auto labda_sign_1 = [tempo](const Gamer &hs) -> auto { return hs.sign == tempo.w_sign; };
        auto labda_sign_2 = [tempo](const Gamer &hs) -> auto { return hs.sign == tempo.l_sign; };
        if (tempo.is_draw) {
            find_if(gamers.begin(), gamers.end(), labda_sign_1)->add_one("draws");
            find_if(gamers.begin(), gamers.end(), labda_sign_2)->add_one("draws");
        }
        else {
            find_if(gamers.begin(), gamers.end(), labda_sign_1)->add_one("wins");
            find_if(gamers.begin(), gamers.end(), labda_sign_2)->add_one("losses");
        }
    }
    for (auto& i : gamers) i.calc_points();
    sort(gamers.begin(), gamers.end(), [](const Gamer &lhs, const Gamer &rhs) { return lhs.points > rhs.points; });

    while(exit) {
        std::cout<<"1 - pokaz tabele"<<std::endl;
        std::cout<<"2 - pokaz wyniki"<<std::endl;
        std::cout<<"3 - pokaz statystyki gracza"<<std::endl;
        std::cout<<"4 - wyjdz z programu"<<std::endl;
        std::cout<<"Wybierz opcje: ";
        int wyb;
        std::cin>>wyb;
        switch(wyb) {
            case 1: {
                gamers.at(0).show_title();
                for (int i=0; i<gamers.size(); i++) {
                    gamers.at(i).show();
                }
                break;
            }
            case 2: {
                for (int i=0; i<games.size(); i++) {
                    std::cout<<games.at(i).substr(0,3)<<std::endl;
                    std::cout<<games.at(i).substr(4,3)<<std::endl;
                    std::cout<<games.at(i).substr(8,3)<<std::endl;
                    std::cout<<std::endl;
                }
                break;
            }
            case 3: {
                std::cout<<"Podaj ID: ";
                int choice;
                std::cin>>choice;
                auto obj = find_if(gamers.begin(), gamers.end(), [choice](const Gamer &hs){ return hs.id == choice;});
                std::cout<<std::endl;
                obj->show();
                break;
            }
            case 4: {
                exit = false;
                break;
            }
            case 5: { //Testing options | hidden
                std::cout<<"Podaj index: ";
                int choice;
                std::cin>>choice;
                Result kk = determine_game(games.at(choice));
                std::cout<<std::endl<<"Win "<<kk.w_sign;
                std::cout<<std::endl<<"Loss "<<kk.l_sign;
                std::cout<<std::endl<<"Draw "<<kk.is_draw;
                break;
            }
            case 6: {
                std::cout<<"Podaj index: ";
                int choice;
                std::cin>>choice;
                std::pair<char, char> kk = detect_players(games.at(choice));
                std::cout<<std::endl<<"First "<<kk.first;
                std::cout<<std::endl<<"Second "<<kk.second;
            }
        }
    }
    return 0;
}

Data handle_tic_tac_toe() {
    std::vector<std::string> game;
    std::vector<std::pair<int, char>> player;

    std::ifstream tictactoe;
    tictactoe.open("tictactoe.txt");
    if (!tictactoe.is_open()) {
        std::cout<<"Couldn't open file... closing program";
        return Data();
    }
    std::string line_g = "";
    std::string line;
    int i = 0;
    while (getline(tictactoe,line)) {
        i++;
        if (i%4 == 0) {
            line_g = line_g.substr(1);
            game.push_back(line_g);
            line_g = "";
            continue;
        };
        line_g += "|" + line;
    }
    /*for (int i=0; i<game.size(); i++) {
        std::cout<<game.at(i)<<std::endl;
    }*/
    tictactoe.close();

    std::ifstream players;
    players.open("players.txt");
    if (!players.is_open()) {
        std::cout<<"Couldn't open file... closing program";
        return Data();       
    }
    line = "";
    while (getline(players, line)) {
        int sp_index = line.find(' ');
        int fr = stoi(line.substr(0,sp_index));
        std::string sd = line.substr(sp_index+1);
        //std::cout<<sc<<"|";
        player.emplace_back(fr, sd[0]);
        /*player.first = stoi(line.substr(0,sp_index));
        player.second = line.substr(sp_index);*/
        //player[stoi(line.substr(0,sp_index))] = line.substr(sp_index);
    }

    players.close();
    return Data(game, player);   
}

std::pair<char, char> detect_players(std::string str) {
    char fr = '.', sd = '.';
    for (int i=0; i<str.length(); i++) {
        if (fr == '.' || sd == '.') {
            if (str[i] != '|' && str[i] != '.') {
                if (fr == '.') fr = str[i];
                else if (str[i]!=fr) sd = str[i];
            }
        }
        else break;
    }
    return std::pair<char, char> {fr, sd};
}

Result determine_game(std::string str) {
    //std::cout<<str.length();
    if (str.length()<11) return Result();
    std::pair<char, char> players_pair = detect_players(str);
    std::vector<std::string> players {std::string(1,players_pair.first)+std::string(1,players_pair.first)+std::string(1,players_pair.first), std::string(1,players_pair.second)+std::string(1,players_pair.second)+std::string(1,players_pair.second)};
    std::vector<std::string> legitimate_wins;
    legitimate_wins.push_back(str.substr(0,3));
    legitimate_wins.push_back(str.substr(4,3));
    legitimate_wins.push_back(str.substr(8,3));
    legitimate_wins.push_back(str.substr(0,1)+str.substr(4,1)+str.substr(8,1));
    legitimate_wins.push_back(str.substr(1,1)+str.substr(5,1)+str.substr(9,1));
    legitimate_wins.push_back(str.substr(2,1)+str.substr(6,1)+str.substr(10,1));
    legitimate_wins.push_back(str.substr(0,1)+str.substr(5,1)+str.substr(10,1));
    legitimate_wins.push_back(str.substr(2,1)+str.substr(5,1)+str.substr(8,1));
    char winner = '.', losser = '.';
    bool found_winner = false;

    for (int i=0; i<players.size(); i++) {
        if (found_winner) break;
        for (auto& ii : legitimate_wins) {
            if (ii == players.at(i)) {
                winner = players.at(i)[0];
                if (i==0) losser = players.at(i+1)[0];
                else losser = players.at(i-1)[0];
                found_winner = true;
                break;
            }
        }
    }
    if (!found_winner)
        return Result(players_pair.first, players_pair.second, true);
    return Result(winner, losser);
}