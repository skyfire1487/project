#include "board.h"
// 紅方先行(玩家1)，綠方次之(玩家2)，藍方最後(玩家3)。
string Color[4] = { "None", "Red", "Green", "Blue" };
// 初始盤面編號
string num[13][13] = { {"N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N"},
                       {"N", "N", "N", "N", "N", "N", "A01", "A02", "A03", "A04", "A05", "A06", "N"},
                       {"N", "N", "N", "N", "N", "B01", "B02", "B03", "B04", "B05", "B06", "B07", "N"},
                       {"N", "N", "N", "N", "C01", "C02", "C03", "C04", "C05", "C06", "C07", "C08", "N"},
                       {"N", "N", "N", "D01", "D02", "D03", "D04", "D05", "D06", "D07", "D08", "D09", "N"},
                       {"N", "N", "E01", "E02", "E03", "E04", "E05", "E06", "E07", "E08", "E09", "E10", "N"},
                       {"N", "F01", "F02", "F03", "F04", "F05", "F06", "F07", "F08", "F09", "F10", "F11", "N"},
                       {"N", "G01", "G02", "G03", "G04", "G05", "G06", "G07", "G08", "G09", "G10", "N", "N"},
                       {"N", "H01", "H02", "H03", "H04", "H05", "H06", "H07", "H08", "H09", "N", "N", "N"},
                       {"N", "J01", "J02", "J03", "J04", "J05", "J06", "J07", "J08", "N", "N", "N", "N"},
                       {"N", "K01", "K02", "K03", "K04", "K05", "K06", "K07", "N", "N", "N", "N", "N"},
                       {"N", "L01", "L02", "L03", "L04", "L05", "L06", "N", "N", "N", "N", "N", "N"},
                       {"N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N"} };

int main()
{
    srand((unsigned)time(NULL));
    double START, END;
    int totalGame = 1; // 總共要跑幾輪。
    double Time[35] = { 0 };
    int Nodes[35] = { 0 };

    float WinRound[4] = { 0 }; // 各玩家總共贏幾場。
    for (int i = 0; i < totalGame; i++) {
        int count = 0;
        START = clock(); // 時間(開始)。
        Board board;
        srand((unsigned)time(NULL));
        int round = 1;           // 第幾回合
        int player = 1;          // 固定由紅方開始。

        while (board.getUnoccupied() > 0 && board.getStill() < 3) {
            //board.PrintBoard();        // 印出盤面。

            // 在所有合法走步中計算出可以使當前下子玩家獲得最高分數的走法。
            pair<int, int> walk = board.judge(player, Nodes, Time, count);

            //board.PrintPath();         // 印出每個位置的最短路徑。

            cout << "Round" << round++ << ": " << Color[player] << " " << num[walk.first][walk.second] << endl << endl;
            if (board.getEarly()) {
                break;
            }
            if (++player == 4) {
                player = 1;
            }
        }
        END = clock();
        /*if (board.getUnoccupied() >= 71) {
            i--;
            continue;
        }*/
        cout << "Final Board:" << endl;
        board.PrintBoard();
        int winner = board.Final(); // 計算、印出哪位玩家獲勝。
        cout << "total runtime: " << (END - START) / CLOCKS_PER_SEC << endl;
        cout << "The winner is " << Color[winner] << '.' << endl;
        WinRound[winner]++;
    }
    for (int i = 1; i < 4; i++) {
        cout << Color[i];
        cout << " 勝率: " << WinRound[i] / totalGame << endl;
    }
    cout << "Node:" << endl;
    for (int i = 0; i < 35; i++) {
        cout << Nodes[i] / totalGame << " ";
    }
    cout << endl << endl;
    cout << "Time:" << endl;
    for (int i = 0; i < 35; i++) {
        cout << Time[i] / totalGame << " ";
    }
    cout << endl;
}