#include <iostream>
#include "Game.h"

//ctor
Game::Game(unsigned short player)
{
    my_color = player;
    foreign_color = (player == 1) ? 2 : 1;
}

Board Game::initilize() {
    Board res{};
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            res[i][j] = 0;
        }
    }
    res[3][3] = res[4][4] = 2;
    res[3][4] = res[4][3] = 1;
    return res;
}

bool Game::is_goal(Board f) {
    if (get_moves(f, 1).empty() && get_moves(f, 0).empty()) {
        return true;
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (f[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool Game::enemy_has_moves() {
    return get_moves(currentState, 0).empty();
}

int Game::move_to_int(std::string move) {
    int res = 0;
    switch (move[0])
    {
    case 'a':
        res = 0;
        break;
    case 'b':
        res = 1;
        break;
    case 'c':
        res = 2;
        break;
    case 'd':
        res = 3;
        break;
    case 'e':
        res = 4;
        break;
    case 'f':
        res = 5;
        break;
    case 'g':
        res = 6;
        break;
    case 'h':
        res = 7;
        break;
    default:
    {
        std::cout << "You cheating..." << std::endl;
        system("pause");
    }
    }
    return res + (std::stoi(move.substr(1, 1)) - 1) * 8;
}

std::string Game::get_str_move(int move)
{
    std::string s = "";
    switch (move % 8) {
    case 0:
        s += "a";
        break;
    case 1:
        s += "b";
        break;
    case 2:
        s += "c";
        break;
    case 3:
        s += "d";
        break;
    case 4:
        s += "e";
        break;
    case 5:
        s += "f";
        break;
    case 6:
        s += "g";
        break;
    case 7:
        s += "h";
        break;
    }
    return s + std::to_string(move / 8 + 1);
}

bool switcher = 1;
void Game::print_board() {
    auto moves = get_moves(currentState, switcher);
    switcher = switcher ? 0 : 1;
    std::cout << "  ";
    for (char c = 'A'; c <= 'H'; ++c) {
        std::cout << "  " << c << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << i + 1;
        for (int j = 0; j < 8; ++j) {
            switch (currentState[i][j])
            {
            case 0:
                if (moves.find(i * 8 + j) != moves.end()) {
                    std::cout << "  " << get_str_move(i * 8 + j);
                    continue;
                }
                std::cout << "   -";
                break;
            case 1:
                std::cout << "   X";
                break;
            default:
                std::cout << "   Y";
                break;
            }
        }
        std::cout << std::endl;
    }


}


bool Game::evalDirection(int dir, int& i, int& j) {
    switch (dir) {
    case 1:
        return --j >= 0;
    case 2:
        return (--j >= 0) && (--i >= 0);
    case 3:
        return --i >= 0;
    case 4:
        return (--i >= 0) && (++j < 8);
    case 5:
        return ++j < 8;
    case 6:
        return (++i < 8) && (++j < 8);
    case 7:
        return ++i < 8;
    case 8:
        return (++i < 8) && (--j >= 0);
    default:
        return false;
    }
}

std::map<int, std::vector<std::pair<int, int>>> Game::get_moves(Board f, int flag) const {
    int color = flag ? my_color : foreign_color;
    std::map<int, std::vector<std::pair<int, int>>> res{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (f[i][j] == color) {
                for (int dir = 1; dir <= 8; dir++) {
                    int k = i, l = j, c = 0;
                    while (evalDirection(dir, k, l) && (f[k][l] != color) && (f[k][l] != 0)) {
                        c++;
                    }
                    if ((k < 8) && (l < 8) && (k >= 0) && (l >= 0) && (f[k][l] == 0) && (c > 0)) {
                        res[k * 8 + l].push_back({ i * 8 + j, dir });
                    }
                }
            }
        }
    }
    return res;
}

Board Game::makeMove(const Board& f, int move, const std::vector<std::pair<int, int>>& affectedCheckers, bool isOurMove) const {
    Board newField;
    std::copy(f.begin(), f.end(), newField.begin());
    for (std::pair<int, int> pos : affectedCheckers) {
        int i = pos.first / 8, j = pos.first % 8;
        while ((i != move / 8) || (j != move % 8)) {
            evalDirection(pos.second, i, j);
            newField[i][j] = isOurMove ? my_color : foreign_color;
        }
    }
    return newField;
}

void Game::makeBotMove(const std::string& botMove, bool isOurMove) {
    int move = move_to_int(botMove);
    int currentColor = isOurMove ? my_color : foreign_color;
    for (int dir = 1; dir <= 8; dir++) {
        int k = move / 8, l = move % 8, c = 0;
        while (evalDirection(dir, k, l) && (currentState[k][l] != currentColor) && (currentState[k][l] != 0)) {
            c++;
        }
        if ((k < 8) && (l < 8) && (k >= 0) && (l >= 0) && (currentState[k][l] == currentColor) && (c > 0)) {
            int i = move / 8, j = move % 8;
            currentState[i][j] = currentColor;
            while ((i != k) || (j != l)) {
                evalDirection(dir, i, j);
                currentState[i][j] = currentColor;
            }
        }
    }
}


int Game::h(Board f) const {
    double mobility = 0;
    double piece_count = 0;
    int corner = 0;
    int mc = 0;
    int fc = 0;

    //ќценка угловых и крайних клеток:  летки на углах и кра€х имеют особую важность в игре Reversi, 
    //так как они более устойчивы и могут привести к захвату большего количества клеток. 
    //ћы можем назначить им больший вес при оценке позиции дл€ улучшени€ стратегии »».

    mc += (f[0][0] == my_color) ? 1 : (f[0][0] == foreign_color) ? 0 : 0;
    fc += (f[0][0] == foreign_color) ? 1 : (f[0][0] == my_color) ? 0 : 0;

    mc += (f[0][7] == my_color) ? 1 : (f[0][7] == foreign_color) ? 0 : 0;
    fc += (f[0][7] == foreign_color) ? 1 : (f[0][7] == my_color) ? 0 : 0;

    mc += (f[7][0] == my_color) ? 1 : (f[7][0] == foreign_color) ? 0 : 0;
    fc += (f[7][0] == foreign_color) ? 1 : (f[7][0] == my_color) ? 0 : 0;

    mc += (f[7][7] == my_color) ? 1 : (f[7][7] == foreign_color) ? 0 : 0;
    fc += (f[7][7] == foreign_color) ? 1 : (f[7][7] == my_color) ? 0 : 0;

    corner = 25 * (mc - fc);

    mc = get_moves(f, 1).size();
    fc = get_moves(f, 0).size();
    //ћобильность  пытаетс€ 
    //зафиксировать относительную разницу между количеством 
    //возможных ходов дл€ максимального и минимального игроков с целью 
    //ограничени€ мобильности противника и увеличени€ собственной мобильности.
    mobility = (mc > fc) ? (100.0 * mc) / (mc + fc) : (mc < fc)
        ? -(100.0 * fc) / (mc + fc) : 0.0;

    //ћатериальное преимущество: Ёта эвристика оценивает разницу в количестве фишек между игроками. 
    //„ем больше фишек у игрока, тем лучше его позици€. ќднако, в конце игры, 
    //когда больша€ часть доски уже заполнена, материальное преимущество может быть менее значимым.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (f[i][j] == my_color) {
                piece_count++;
            }
            else if (f[i][j] == foreign_color) {
                piece_count--;
            }
        }
    }
    return (int)(1500 * corner + 500 * mobility + 500 * piece_count);
}

int Game::alphaBeta(Board f, int depth, int alpha, int beta, bool isMax, bool returnMove) {
    if (depth == 0 || is_goal(f)) {
        return h(f);
    }
    if (isMax) {
        int value = INT32_MIN;
        int savedMove = -1;
        auto availableMoves = get_moves(f, 1);
        if (availableMoves.empty()) {
            return alphaBeta(f, depth - 1, alpha, beta, false, false);
        }
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child : availableMoves) {
            auto childF = makeMove(f, child.first, child.second, true);

            int childRes = alphaBeta(childF, depth - 1, alpha, beta, false, false);
            if (value < childRes) {
                value = childRes;
                savedMove = child.first;
            }
            if (value >= beta) {
                break;
            }
            alpha = std::max(alpha, value);
        }
        return returnMove ? savedMove : value;
    }
    else {
        int value = INT32_MAX;
        auto availableMoves = get_moves(f, 0);
        if (availableMoves.empty()) {
            return alphaBeta(f, depth - 1, alpha, beta, true, false);
        }
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child : availableMoves) {
            Board childF = makeMove(f, child.first, child.second, false);
            value = std::min(value, alphaBeta(childF, depth - 1, alpha, beta, true, false));
            if (value <= alpha) {
                break;
            }
            beta = std::min(beta, value);
        }
        return value;
    }
}

std::string Game::decideHowToMove() {
    std::string state = get_winner();
    if (state != "") {
        return state;
    }
    if (get_moves(currentState, 1).size() == 0) {
        return "skip";
    }
    return get_str_move(alphaBeta(currentState, ALPHA_BETA_DEPTH, INT32_MIN, INT32_MAX, true, true));
}

std::string Game::get_winner() {
    if (is_goal(currentState)) {
        int cntOurs = 0, cntOpponent = 0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (currentState[i][j] == my_color) {
                    cntOurs++;
                }
                else if (currentState[i][j] == foreign_color) {
                    cntOpponent++;
                }
            }
        }
        if (cntOurs == cntOpponent) {
            return "draw";
        }
        return cntOurs > cntOpponent ? "win" : "lose";
    }
    return "";
}





