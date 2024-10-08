#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>
#include <set>
#include <map>
#include <sstream>

typedef std::array<std::array<unsigned short, 8>, 8> Board;
typedef std::map<int, std::vector<std::pair<int, int>>> VP;

class Game {
    Board board = initilize();
    unsigned short my_color = 2;
    unsigned short foreign_color = 1;
public:
    Game(unsigned short ourColor);

    Board initilize();
    int DEPTH = 5;

    static bool check_dir(int dir, int& i, int& j);

    std::map<int, std::vector<std::pair<int, int>>> get_moves(Board f, int flag) const;

    bool is_goal(Board f);

    void print_board();

    bool switcher = 1;

    Board get_move(const Board& f, int move, const std::vector<std::pair<int, int>>& affectedCheckers, bool isOurMove = true) const;

    void bot_move(const std::string& botMove, bool isOurMove);

    static std::string get_str_move(int move);

    static int move_to_int(std::string move);

    int h(Board f) const;

    int alphaBeta(Board f, int depth, int alpha, int beta, bool isMax, bool returnMove = false);

    std::string decideHowToMove();

    std::string get_winner();

    bool enemy_has_moves();

    bool checkMove(std::string move) {
        int parsedMove = move_to_int(move);
        return get_moves(board, 0).count(parsedMove) > 0;
    }
};


#endif
