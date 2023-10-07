//
// Created by niko1 on 19.10.2021.
//

#include <iostream>
#include "Game.h"

std::vector<std::vector<int>> Game::pagodaCoeffs = {{20, -4, 11, 8,  8,  11, -4, 20},
                                                    {-4, -7, -3, 1,  1,  -3, -7, -4},
                                                    {11, -3, 2,  2,  2,  2,  -3, 11},
                                                    {8,  1,  2,  -3, -3, 2,  1,  8},
                                                    {8,  1,  2,  -3, -3, 2,  1,  8},
                                                    {11, -3, 2,  2,  2,  2,  -3, 11},
                                                    {-4, -7, -3, 1,  1,  -3, -7, -4},
                                                    {20, -4, 11, 8,  8,  11, -4, 20}};

Game::Game(unsigned short ourColor) : ourColor(ourColor) {
    if (ourColor == 1) {
        opponentColor = 2;
    } else {
        opponentColor = 1;
    }
}

Field Game::getInitialState() {
    Field res{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            res[i][j] = 0;
        }
    }
    res[3][3] = 2;
    res[3][4] = 1;
    res[4][3] = 1;
    res[4][4] = 2;

    return res;
}

bool Game::isFinish(Field f) {
    //std::cout<< "isFinish " <<std::endl;
    if(getAvailableMoves(f).empty() && getOpponentMoves(f).empty()){
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

void Game::printField() {
    auto moves = getOpponentMoves(currentState);
    std::cout << "  ";
    for (int i = 0; i < 8; ++i) {
        std::cout << "  " << (char)('A' + i) << " ";
    }
    std::cout<<std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << i + 1;
        for (int j = 0; j < 8; ++j) {
            if (currentState[i][j] == 0) {
                if(moves.find(i*8+j) != moves.end()){
                    std::cout << "  " << botifyMove(i*8+j);
                    continue;
                }
                std::cout << "   -";
            } else if (currentState[i][j] == 1) {
                std::cout << "   x";
            } else {
                std::cout << "   o";
            }
        }
        std::cout << std::endl;
    }
}

bool Game::evalDirection(int dir, int &i, int &j) {
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

std::map<int, std::vector<std::pair<int, int>>> Game::getAvailableMoves(Field f) const {
    //std::cout<< "getAvailableMoves " <<std::endl;
    std::map<int, std::vector<std::pair<int, int>>> res{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (f[i][j] == ourColor) {
                for (int dir = 1; dir <= 8; dir++) {
                    int k = i, l = j, c = 0;
                    while (evalDirection(dir, k, l) && (f[k][l] != ourColor) && (f[k][l] != 0)) {
                        c++;
                    }
                    if ((k < 8) && (l < 8) && (k>=0) && (l>=0) && (f[k][l] == 0) && (c > 0)) {
                        res[k * 8 + l].push_back({i * 8 + j, dir});
                    }
                }
            }
        }
    }
    return res;
}

std::map<int, std::vector<std::pair<int, int>>> Game::getOpponentMoves(Field f) const {
    //std::cout<< "getOpponentMoves " <<std::endl;
    std::map<int, std::vector<std::pair<int, int>>> res{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (f[i][j] == opponentColor) {
                for (int dir = 1; dir <= 8; dir++) {
                    int k = i, l = j, c = 0;
                    while (evalDirection(dir, k, l) && (f[k][l] != opponentColor) && (f[k][l] != 0)) {
                        c++;
                    }
                    if ((k < 8) && (l < 8) && (k>=0) && (l>=0) && (f[k][l] == 0) && (c > 0)) {
                        res[k * 8 + l].push_back({i * 8 + j, dir});
                    }
                }
            }
        }
    }
    return res;
}

Field Game::makeMove(const Field &f, int move, const std::vector<std::pair<int, int>> &affectedCheckers, bool isOurMove) const {
    //currentState[move/8][move%8] = ourColor;
    //std::cout<< "makeMove " <<std::endl;
    Field newField;
    std::copy(f.begin(),  f.end(),newField.begin());
    for (std::pair<int, int> pos: affectedCheckers) {
        int i = pos.first / 8, j = pos.first % 8;
        while ((i != move / 8) || (j != move % 8)) {
            evalDirection(pos.second, i, j);
            newField[i][j] = isOurMove ? ourColor : opponentColor;
        }
    }
    return newField;
}

void Game::makeBotMove(const std::string &botMove, bool isOurMove) {
    //std::cout<< "makeBotMove " <<std::endl;
    int move = debotifyMove(botMove);
    int currentColor = isOurMove? ourColor: opponentColor;
    for (int dir = 1; dir <= 8; dir++) {
        int k = move / 8, l = move % 8, c = 0;
        while (evalDirection(dir, k, l) && (currentState[k][l] != currentColor) && (currentState[k][l] != 0)) {
            c++;
        }
        if ((k < 8) && (l < 8) && (k>=0) && (l>=0) &&(currentState[k][l] == currentColor) && (c > 0)) {
            int i = move / 8, j = move % 8;
            currentState[i][j] = currentColor;
            while ((i != k) || (j != l)) {
                evalDirection(dir, i, j);
                currentState[i][j] = currentColor;
            }
        }
    }
}

int Game::debotifyMove(std::string move) {
    int res = 0;
    switch (move[0]) {
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
        default: {
            std::cout << "Have you just tried to fool me?? NO WAY." << std::endl;
            system("pause");
            throw std::exception();
        }
    }
    res += (std::stoi(move.substr(1, 1)) - 1) * 8;
    return res;
}

std::string Game::botifyMove(int move) {
    std::stringstream ss;
    switch (move % 8) {
        case 0:
            ss << "a";
            break;
        case 1:
            ss << "b";
            break;
        case 2:
            ss << "c";
            break;
        case 3:
            ss << "d";
            break;
        case 4:
            ss << "e";
            break;
        case 5:
            ss << "f";
            break;
        case 6:
            ss << "g";
            break;
        case 7:
            ss << "h";
            break;
    }
    ss << (move / 8 + 1);
    return ss.str();
}

/// https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
int Game::h(Field f) const {
    //std::cout<< "heuristics " <<std::endl;
    double hCoinParity = 0;
    double hPagodaDistribution = 0;
    double hMobility = 0;
    int me = 0, opponent = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (f[i][j] == ourColor) {
                hPagodaDistribution += pagodaCoeffs[i][j];
                me++;
            } else if (f[i][j] == opponentColor) {
                hPagodaDistribution -= pagodaCoeffs[i][j];
                opponent++;
            }
        }
    }
    if (me > opponent)
        hCoinParity = (100.0 * me) / (me + opponent);
    else if (me < opponent)
        hCoinParity = -(100.0 * opponent) / (me + opponent);

    me = opponent = 0;
    if(f[0][0] == ourColor) me++;
    else if(f[0][0] == opponentColor) opponent++;
    if(f[0][7] == ourColor) me++;
    else if(f[0][7] == opponentColor) opponent++;
    if(f[7][0] == ourColor) me++;
    else if(f[7][0] == opponentColor) opponent++;
    if(f[7][7] == ourColor) me++;
    else if(f[7][7] == opponentColor) opponent++;
    int hCornerOccupancy = 25 * (me - opponent);

    me = getAvailableMoves(f).size();
    opponent = getOpponentMoves(f).size();
    if(me > opponent)
        hMobility = (100.0 * me)/(me + opponent);
    else if(me < opponent)
        hMobility = -(100.0 * opponent)/(me + opponent);
    return (int) (29.954 * hCoinParity + 701.724 * hCornerOccupancy + 108.968 * hMobility + 159.354 * hPagodaDistribution);
}

int Game::alphaBeta(Field f, int depth, int alpha, int beta, bool isMax, bool returnMove) {
    //std::cout<< "alphaBeta " <<std::endl;
    if (depth == 0 || isFinish(f)) {
        return h(f);
    }
    if (isMax) {
        int value = INT32_MIN;
        int savedMove = -1;
        auto availableMoves = getAvailableMoves(f);
        if(availableMoves.empty()){
            return alphaBeta(f,depth - 1, alpha, beta, false, false);
        }
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child: availableMoves) {
            auto childF = makeMove(f,child.first,child.second, true);

            int childRes = alphaBeta(childF,depth - 1, alpha, beta, false, false);
            if (value < childRes){
                value = childRes;
                savedMove = child.first;
            }
            if(value >= beta){
                break;
            }
            alpha = std::max(alpha,value);
        }
        return returnMove? savedMove: value;
    } else{
        int value = INT32_MAX;
        auto availableMoves = getOpponentMoves(f);
        if(availableMoves.empty()){
            return alphaBeta(f,depth - 1, alpha, beta, true, false);
        }
        for (const std::pair<const int, std::vector<std::pair<int, int>>>& child: availableMoves) {
            Field childF = makeMove(f,child.first,child.second,false);
            value = std::min(value, alphaBeta(childF,depth - 1, alpha, beta, true, false));
            if(value <= alpha){
                break;
            }
            beta = std::min(beta,value);
        }
        return value;
    }
}

std::string Game::decideHowToMove() {
    std::string state = checkForWin();
    if (state!=""){
        return state;
    }
    if(getAvailableMoves(currentState).size() == 0){
        return "skip";
    }
    auto rankedMove = alphaBeta(currentState, ALPHA_BETA_DEPTH, INT32_MIN, INT32_MAX, true, true);
    return botifyMove(rankedMove);
}

std::string Game::checkForWin() {
    if(isFinish(currentState)){
        int cntOurs =0, cntOpponent =0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (currentState[i][j] == ourColor) {
                    cntOurs++;
                }else if (currentState[i][j] == opponentColor){
                    cntOpponent++;
                }
            }
        }
        if(cntOurs == cntOpponent){
            return "draw";
        }
        return cntOurs > cntOpponent? "win" : "lose";
    }
    return "";
}

bool Game::isOpponentSkipping() {
    return getOpponentMoves(currentState).empty();
}




