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
    return get_moves(board, 0).empty();
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

void Game::print_board() {
    auto moves = get_moves(board, switcher);
    switcher = switcher ? 0 : 1;
    std::cout << "  ";
    for (char c = 'A'; c <= 'H'; ++c) {
        std::cout << "  " << c << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << i + 1;
        for (int j = 0; j < 8; ++j) {
            switch (board[i][j])
            {
            case 0:
                if (moves.find(i * 8 + j) != moves.end()) {
                    std::cout << "  " << get_str_move(i * 8 + j);
                    continue;
                }
                std::cout << "   -";
                break;
            case 1:
                std::cout << "   x";
                break;
            default:
                std::cout << "   o";
                break;
            }
        }
        std::cout << std::endl;
    }


}


bool Game::check_dir(int dir, int& i, int& j) 
{
    switch (dir) 
    {
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

bool check_pos(int k, int l)
{
    if ((k < 8) && (l < 8) && (k >= 0) && (l >= 0))
    {
        return 1;
    }
    return 0;
}
VP Game::get_moves(Board board, int flag) const 
{
    int color = flag ? my_color : foreign_color;
    VP res{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == color) {
                for (int dir = 1; dir <= 8; dir++) 
                {
                    int k = i, l = j, c = 0;
                    while (check_dir(dir, k, l) && (board[k][l] != color) && (board[k][l] != 0)) 
                        c++;
                    if (check_pos(k,l) && (board[k][l] == 0) && (c > 0))
                        res[k * 8 + l].push_back({ i * 8 + j, dir });

                }
            }
        }
    }
    return res;
}
Board Game::get_move(const Board& board, int move, const std::vector<std::pair<int, int>>& vect_pair, bool is_my_move) const {
    Board b;
    std::copy(board.begin(), board.end(), b.begin());
    for (auto pos : vect_pair) {
        int i = pos.first / 8, j = pos.first % 8;
        while ((i != move / 8) || (j != move % 8)) 
        {
            check_dir(pos.second, i, j);
            b[i][j] = is_my_move ? my_color : foreign_color;
        }
    }
    return b;
}


void Game::bot_move(const std::string& str_move, bool is_my_color) {
    int move = move_to_int(str_move);
    int thiscolor = is_my_color ? my_color : foreign_color;
    for (int dir = 1; dir <= 8; dir++) 
    {
        int k = move / 8, l = move % 8, c = 0;
        while (check_dir(dir, k, l) && (board[k][l] != thiscolor) && (board[k][l] != 0)) 
        {
            c++;
        }
        if (check_pos(k, l) && (board[k][l] == thiscolor) && (c > 0))
        {
            int i = move / 8, j = move % 8;
            board[i][j] = thiscolor;
            while ((i != k) || (j != l)) 
            {
                check_dir(dir, i, j);
                board[i][j] = thiscolor;
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

    //Оценка угловых и крайних клеток: Клетки на углах и краях имеют особую важность в игре Reversi, 
    //так как они более устойчивы и могут привести к захвату большего количества клеток. 
    //Мы можем назначить им больший вес при оценке позиции для улучшения стратегии ИИ.

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
    //Мобильность  пытается 
    //зафиксировать относительную разницу между количеством 
    //возможных ходов для максимального и минимального игроков с целью 
    //ограничения мобильности противника и увеличения собственной мобильности.
    mobility = (mc > fc) ? (100.0 * mc) / (mc + fc) : (mc < fc)
        ? -(100.0 * fc) / (mc + fc) : 0.0;

    //Материальное преимущество: Эта эвристика оценивает разницу в количестве фишек между игроками. 
    //Чем больше фишек у игрока, тем лучше его позиция. Однако, в конце игры, 
    //когда большая часть доски уже заполнена, материальное преимущество может быть менее значимым.
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
    // Оценка стабильности
    //количество устойчивых фишек, которые не могут быть перевернуты противником.
    double stability = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (f[i][j] == my_color) {
                stability++;
                if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
                    stability += 2; // угловые клетки имеют большую стабильность
                }
                else if (i == 0 || i == 7 || j == 0 || j == 7) {
                    stability += 1; // крайние клетки имеют меньшую стабильность
                }
            }
            else if (f[i][j] == foreign_color) {
                stability--;
                if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
                    stability -= 2;
                }
                else if (i == 0 || i == 7 || j == 0 || j == 7) {
                    stability -= 1;
                }
            }
        }
    }
    return (int)(1500 * corner + 500 * mobility + 500 * piece_count + 0*stability);
}

// алгоритм альфа-бета отсечения для оценки лучшего хода в игре

int Game::alphaBeta(Board f, int depth, int alpha, int beta, bool is_winner, bool flag) {

    if (depth == 0 || is_goal(f)) {
        return h(f); // Возвращает оценку текущего состояния
    }

    if (is_winner) {
        int is_dangerous_move = -1;
        auto moves = get_moves(f, 1);
        int val = INT32_MIN;

        if (moves.empty())
            return alphaBeta(f, depth - 1, alpha, beta, false, false);

        // Проходим по всем возможным ходам
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child : moves) {
            // Выполняем альфа-бета отсечение для каждого потомка
            int res = alphaBeta(get_move(f, child.first, child.second, true), depth - 1, alpha, beta, false, false);

            // Обновляем значение лучшего хода и определяем ялвяется ли ход опасным
            if (val < res) {
                val = res;
                is_dangerous_move = child.first;
            }

            // Производим отсечение, если значение превысило beta
            if (val >= beta) {
                break;
            }
            // Обновляем значение alpha
            alpha = std::max(alpha, val);
        }

        // Возвращаем опасный ход, если флаг flag установлен, иначе возвращаем значение
        return flag ? is_dangerous_move : val;
    }
    else {
        auto availableMoves = get_moves(f, 0);

        if (availableMoves.empty())
            return alphaBeta(f, depth - 1, alpha, beta, true, false);

        int val = INT32_MAX;

        // Проходим по всем возможным ходам
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child : availableMoves) {
            // Выполняем альфа-бета отсечение для каждого потомка
            val = std::min(val, alphaBeta(get_move(f, child.first, child.second, false), depth - 1, alpha, beta, true, false));

            // Производим отсечение, если значение меньше или равно alpha
            if (val <= alpha) {
                break;
            }

            // Обновляем значение beta
            beta = std::min(beta, val);
        }

        // Возвращаем значение
        return val;
    }
}

std::string Game::decideHowToMove() {
    std::string state = get_winner();
    if (state != "") {
        return state;
    }
    if (get_moves(board, 1).size() == 0) {
        return "skip";
    }
    return get_str_move(alphaBeta(board, DEPTH, INT32_MIN, INT32_MAX, true, true));
}

std::string Game::get_winner() {
    if (is_goal(board)) {
        int cntOurs = 0, cntOpponent = 0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == my_color) {
                    cntOurs++;
                }
                else if (board[i][j] == foreign_color) {
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





