#include "board.h"

int main()
{
    srand((unsigned)time(NULL));
    int option = 0;
    string CPUorHuman[2] = { "cpu", "player" };
    cout << "開始遊戲請輸入1，離開請輸入2:";
    while (cin >> option) {
        bool P[4] = { 0 }; // 0是電腦、1是玩家。
        // 選擇開始新遊戲or離開。
        if (option < 1 || option > 2) {
            cout << "輸入超出範圍，請重新輸入:";
            continue;
        }
        if (option == 2) {
            break;
        }

        // 建立盤面Class。
        Board board;

        // 選擇玩家數量(顏色隨機分配)。
        int human = 0;
        cout << "請選擇玩家數量(0~3):";
        while (cin >> human) {
            if (human >= 0 && human <= 3) {
                break;
            }
            cout << "輸入超出範圍，請重新輸入:";
        }
        int r = 0;
        r = rand() % 3 + 1;
        if (human == 3) {
            P[1] = 1, P[2] = 1, P[3] = 1;
        }
        else if (human == 2) {
            P[1] = 1, P[2] = 1, P[3] = 1;
            P[r] = 0;
        }
        else if (human == 1) {
            P[r] = 1;
        }
        cout << "Red:" << CPUorHuman[P[1]] << ", " << "Green:" << CPUorHuman[P[2]] << ", " << "Blue:" << CPUorHuman[P[3]] << endl;

        // 選擇電腦難度。
        if (human != 3) {
            int Level = 0;
            cout << "請選擇電腦難度: 1.easy, 2.medium, 3.hard: ";
            while (cin >> Level) {
                if (Level >= 1 && Level <= 3) {
                    break;
                }
                cout << "輸入超出範圍，請重新輸入:";
            }
            board.setLevel(Level, P);
        }
        cout << endl << endl << "初始盤面:" << endl << endl;

        double START, END;
        START = clock(); // 時間(開始)。

        int round = 1;           // 第幾回合
        int player = 1;          // 固定由紅方開始。
        while (board.getUnoccupied() > 0 && board.getStill() < 3) {
            board.PrintBoard();        // 印出盤面。
            pair<int, int> walk;
            // 在所有合法走步中計算出可以使當前下子玩家獲得最高分數的走法。
            if (P[player]) {
                walk = board.Select(player);
            }
            else {
                walk = board.judge(player);
            }

            cout << "Round" << round++ << ": " << board.getColor(player) << " " << board.getNumber(walk) << endl << endl;
            if (board.getEarly()) {
                break;
            }
            if (++player == 4) {
                player = 1;
            }
        }
        END = clock();
        cout << "Final Board:" << endl;
        board.PrintBoard();
        int winner = board.Final(); // 計算、印出哪位玩家獲勝。
        cout << "The winner is " << board.getColor(winner) << '.' << endl << endl;
        cout << "是否想要開始新遊戲？是請輸入1，否則輸入2:";
    }
}