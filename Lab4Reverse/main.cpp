#include <iostream>
#include "Game.h"
using namespace std;
void PVE(Game g, int player)
{
    while (true)
    {

        auto move = g.decideHowToMove();
        if (move == "skip") {
            std::cout << "No moves " << std::endl;
        }
        if (move == "draw") {
            std::cout << "It's draw!" << std::endl;
            system("pause");
        }
        if (move == "win") {
            std::cout << "Easy Win for me! " << std::endl;
            system("pause");
        }
        if (move == "lose") {
            system("pause");
            std::cout << "I lose this round!" << std::endl;
        }
        if (move != "lose" && move != "win" && move != "skip")
        {
            g.makeBotMove(move, true);
            std::cout << " Our move: " << move << std::endl;
            g.print_board();
            std::string state = g.get_winner();
            switch (state[0])
            {
            case 'd':
                std::cout << "It's draw!" << std::endl;
            case 'w':
                std::cout << "Easy Win for me! " << std::endl;
                system("pause");
            case 'l':
                std::cout << "I lose this round!" << std::endl;
                system("pause");
            default:
                break;
            }

        }

        std::string oppMove;

        if (g.enemy_has_moves()) {
            std::cout << "Oh, u have no moves!" << std::endl;
            continue;
        }
        cout << "Make ur move, human" << endl;
        std::cin >> oppMove;
        if (!g.checkMove(oppMove)) {
            std::cout << "Unfair Move!" << std::endl;
            std::cin >> oppMove;
        }
        if (!g.checkMove(oppMove)) {
            std::cout << "Well, I am offended right now. Bye." << std::endl;
            system("pause");
            throw std::exception("CHEATER!...");
        }
        g.makeBotMove(oppMove, false);
        std::cout << " Opponent move: " << oppMove << std::endl;
        g.print_board();
        std::cout << std::endl << std::endl;
    }
}

int main(int argc, char* argv[]) 
{
    setlocale(LC_ALL, "Russian");
    int player = 1;
    // Если запущена дуэль
    if (argc > 1)
    {
        player = (int)(*argv[1] - '0') + 1;
        Game g(player);
        g.print_board();
        if (player == 2) {
            std::string oppMove;
            std::cin >> oppMove;
            g.makeBotMove(oppMove, false);
            std::cout << "Opponent move: " << oppMove << std::endl;
            g.print_board();
            std::cout << std::endl << std::endl;
        }
        while (true)
        {
            auto move = g.decideHowToMove();
            if (move == "skip") {
                std::cout << "No moves " << std::endl;
            }
            if (move == "draw") {
                std::cout << "It's draw!" << std::endl;
                return 4;
            }
            if (move == "win") {
                std::cout << "Easy Win for me! " << std::endl;
                return 0;
            }
            if (move == "lose") {
                std::cout << "I lose this round!" << std::endl;
                return 3;
            }

            if (move != "lose" && move != "win" && move != "skip")
            {
                g.makeBotMove(move, true);
                std::cout << player;
                std::cout << " Our move: " << move << std::endl;
                std::cerr << move << std::endl;
                g.print_board();
                std::string state = g.get_winner();
                std::cout << "state: " << state << std::endl;
                switch (state[0])
                {
                case 'd':
                    std::cout << "It's draw!" << std::endl;
                    return 4;
                case 'w':
                    std::cout << "Easy Win for me! " << std::endl;
                    return 0;
                case 'l':
                    std::cout << "I lose this round!" << std::endl;
                    return 3;
                default:
                    break;
                }
            }
            std::string oppMove;

            if (g.enemy_has_moves()) {
                std::cout << "Oh, u have no moves!" << std::endl;
                continue;
            }
            std::cout << "I'm " << player << " waiting for ur move" << std::endl;
            std::cin >> oppMove;
            if (!g.checkMove(oppMove)) {
                std::cout << "Unfair Move!" << std::endl;
                std::cin >> oppMove;
            }
            if (!g.checkMove(oppMove)) {
                std::cout << "Well, I am offended right now. Bye." << std::endl;
                system("pause");
                throw std::exception("CHEATER!...");
            }
            g.makeBotMove(oppMove, false);
            std::cout << player;
            std::cout << " Opponent move: " << oppMove << std::endl;
            g.print_board();
            std::cout << std::endl << std::endl;
        }
    }
    // Одиночная игра с ботом
    else
    {
        Game g(player);
        g.print_board();
        if (player == 2) {
            std::string oppMove;
            std::cin >> oppMove;
            g.makeBotMove(oppMove, false);
            std::cout << "Opponent move: " << oppMove << std::endl;
            g.print_board();
            std::cout << std::endl << std::endl;
        }
        PVE(g, player);
    }
    
    return 1;
}
