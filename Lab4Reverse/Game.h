//
// Created by niko1 on 19.10.2021.
//

#ifndef REVERSI_GAME_H
#define REVERSI_GAME_H

#include <array>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <iostream>

typedef std::array<std::array<unsigned short, 8>, 8> Board;

class Game {
    Board currentState = getInitialState();
    // 0 - empty 1 = black, 2 = white
    unsigned short my_color = 2;
    unsigned short foreign_color = 1;
    static std::vector<std::vector<int>> pagodaCoeffs;
    const int ALPHA_BETA_DEPTH = 5;
public:
    Game(unsigned short ourColor);

    Board getInitialState();

    static bool evalDirection(int dir, int &i, int &j);

    std::map<int, std::vector<std::pair<int, int>>> get_moves(Board f,int flag) const;

    bool is_goal(Board f);

    void printField();

    Board makeMove(const Board &f, int move, const std::vector<std::pair<int, int>> &affectedCheckers, bool isOurMove = true) const;

    void makeBotMove(const std::string &botMove, bool isOurMove);

    static std::string get_str_move(int move);

    static int debotifyMove(std::string move);

    int h(Board f) const;

    int alphaBeta(Board f, int depth, int alpha, int beta, bool isMax, bool returnMove = false);

    std::string decideHowToMove();

    std::string get_winner();

    bool enemy_has_moves();

    void printSituation(){
        std::cout << get_moves(currentState, 0).size() << " " << get_moves(currentState,1).size() << std::endl;
    }

    bool checkMove(std::string move){
        int parsedMove = debotifyMove(move);
        return get_moves(currentState, 0).count(parsedMove) > 0;
    }
};


#endif //REVERSI_GAME_H
