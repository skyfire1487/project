#include "board.h" //左上    右上     左       右      左下     右下
int dir[6][2] = { {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0} };

int Board::getUnoccupied() {
    return unoccupied;
}

int Board::getStill() {
    return still;
}

bool Board::getEarly() {
    return early;
}

void Board::updateWeight(int player) {
    vector<pair<int, int>> MostValue; // 當前盤面最有價值的地方。
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            Weight[i][j] = 1;
        }
    }
    float Max = 10, Min = 0.5, Max2 = 3; // Max:外角落權重、Min:角落相鄰點權重、Max2:內角落權重。

    // 外圈角落。
    for (int i = 0; i < 6; i++) {
        int r = outerCorner[i].first, c = outerCorner[i].second;
        for (int j = 0; j < 2; j++) {
            int x = r, y = c, d = CorDir[i][j];
            int dx = dir[d][0], dy = dir[d][1];
            bool ifValue = true;
            int meet = 0;
            while (board[x][y] && board[x][y] != -1) {
                if (board[x][y] != player) {
                    if ((x == r && y == c) || (meet != 0 && board[x][y] != meet)) {
                        ifValue = false;
                        break;
                    }
                    meet = board[x][y];
                }
                x += dx, y += dy;
            }
            if (ifValue && board[x][y] != -1) {
                Weight[x][y] = Max;
                MostValue.emplace_back(x, y);
            }
        }
    }

    if (AdjStep) { // 內圈角落。
        for (int i = 0; i < 6; i++) {
            int r = innerCorner[i].first, c = innerCorner[i].second;
            if (board[r][c]) {
                continue;
            }
            bool ifValue = true;
            for (int j = 0; j < 3; j++) {
                int dx = dir[neighborDir[i][j]][0], dy = dir[neighborDir[i][j]][1];
                int x = r + dx, y = c + dy;
                if (board[x][y] != player) {
                    ifValue = false;
                    break;
                }
            }
            if (ifValue) {
                Weight[r][c] = Max2;
            }
        }
    }

    // 把權重值高的點的鄰居權重值設為0.5
    for (const auto& i : MostValue) {
        int x = i.first, y = i.second;
        for (auto& j : dir) {
            int dx = j[0], dy = j[1];
            int row = x + dx, col = y + dy;
            if (Weight[row][col] == 1) {
                Weight[row][col] = Min;
            }
        }
    }

    /*for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            cout << setw(4) << Weight[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;*/
}

void Board::Path(int x, int y) {
    PathTable[x][y] = 0;
    // 建立、更新路徑表。
    queue<pair<int, int>> occupied;
    occupied.emplace(x, y);
    size_t turn = 0;
    int distance = 1;
    while (!occupied.empty()) {
        turn = occupied.size();
        while (turn--) {
            int row = occupied.front().first, col = occupied.front().second;
            occupied.pop();
            for (int i = 0; i < 6; i++) {
                int r = row + dir[i][0], c = col + dir[i][1];
                if (distance >= PathTable[r][c] || r < 1 || c < 1 || r > 12 || c > 12) {
                    continue;
                }
                PathTable[r][c] = distance;
                occupied.emplace(r, c);
            }
        }
        distance++;
    }
}

void Board::PrintBoard() { // 印出初始盤面值
    cout << "Board:" << endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            cout << setw(4) << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}

void Board::PrintPath() {
    cout << "Path：" << endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            cout << setw(4) << PathTable[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Board::PrintFliptable() {
    cout << "Flip：" << endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            cout << setw(4) << FlipTable[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Board::Legal(int p, bool legal[13][13], set<pair<int, int>> flip[13][13]) { // 佔中方、誰的回合。
    empty = true;
    int front = p - 1, next = p + 1;
    if (!front) front = 3;
    if (next == 4) next = 1;
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            if (board[i][j]) {
                continue;
            }
            for (int k = 0; k < 6; k++) {
                int nx = i + dir[k][0], ny = j + dir[k][1];
                if (board[nx][ny] == front || board[nx][ny] == next) {
                    int neighbor = board[nx][ny];
                    int x = nx, y = ny;
                    int dx = dir[k][0], dy = dir[k][1];
                    bool find = false;   // 碰到第二個子且不與第一個子同顏色 = true。
                    vector<pair<int, int>> tempFlip;
                    tempFlip.emplace_back(x, y);
                    x += dx, y += dy;
                    while (board[x][y] != -1 && board[x][y]) {
                        if (board[x][y] == neighbor) {
                            tempFlip.emplace_back(x, y);
                        }
                        else {
                            find = true; // 碰到第二種棋子且跟第一個碰到的不相同。
                            break;
                        }
                        x += dx, y += dy;
                    }
                    if (find) {
                        legal[i][j] = true;
                        empty = false;
                        for (int z = 0; z < tempFlip.size(); z++) {
                            flip[i][j].insert(tempFlip[z]);
                        }
                    }
                }
            }
        }
    }
    if (empty) {
        Adjacent(legal, p);
        AdjStep = true;
    }
}

void Board::PrintLegal(bool legal[13][13]) {
    cout << "Legal：" << endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            cout << setw(4) << legal[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Board::Printflip(set<pair<int, int>> flip[13][13]) {
    cout << "Flip:" << endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            size_t size = flip[i][j].size();
            if (size > 0) {
                printf("flip[%d][%d]:", i, j);
            }
            for (const auto& s : flip[i][j]) {
                printf("(%d,%d) ", s.first, s.second);
            }
            if (size > 0) {
                cout << endl;
            }
        }
    }
    cout << endl;
}

void Board::Adjacent(bool legal[13][13], int player) {
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            if (!board[i][j]) {
                for (int k = 0; k < 6; k++) {
                    int dx = i + dir[k][0], dy = j + dir[k][1];
                    if (board[dx][dy] == player) {
                        legal[i][j] = true;
                        break;
                    }
                }
            }
        }
    }
}

void Board::ScoreCount(int player, vector<float>& score) {
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            int p = board[i][j];
            if (p <= 0) {
                continue;
            }
            score[p] += Weight[i][j] / (PathTable[i][j] + FlipTable[i][j]);
        }
    }
    float sum = score[1] + score[2] + score[3];
    for (int i = 1; i < 4; i++) {
        score[i] = score[i] * Bound / sum;
    }
}


vector<float> Board::maxn(pair<int, int> node, set<pair<int, int>> Flip, int depth, int player, float bound) {
    vector<float> value(4, 0);
    int ni = node.first, nj = node.second;
    board[ni][nj] = player;
    vector<pair<int, int>> turn[4]; // 紀錄翻子位置。
    for (const auto& s : Flip) {
        int x = s.first, y = s.second;
        turn[board[x][y]].emplace_back(x, y);
        board[x][y] = player;
    }
    Node++;
    if (depth == N[root]) { // 到欲搜尋深度就不繼續call maxn function。
        ScoreCount(root, value);
        board[ni][nj] = 0; // 還原盤面。
        for (int i = 1; i < 4; i++) {
            for (const auto& s : turn[i]) {
                int x = s.first, y = s.second;
                board[x][y] = i;
            }
        }
        return value;
    }

    if (++player == 4) {
        player = 1;
    }
    vector<float> best(4, 0);
    bool legal[13][13] = { 0 };
    set<pair<int, int>> flip[13][13];
    Legal(player, legal, flip); // 看哪裡可以下子 -> legal() 合法走步。

    // 從所有合法走步中找到相對分數最高的點。
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            if (!legal[i][j]) {
                continue;
            }
            if (best[player] >= bound) {
                board[ni][nj] = 0; // 還原盤面。
                for (int k = 1; k < 4; k++) {
                    for (const auto& s : turn[k]) {
                        int x = s.first, y = s.second;
                        board[x][y] = k;
                    }
                }
                return best;
            }
            value = maxn(make_pair(i, j), flip[i][j], depth + 1, player, Bound - best[player]);
            if (value[player] >= best[player]) {
                best = value;
            }
        }
    }
    board[ni][nj] = 0; // 還原盤面。
    for (int i = 1; i < 4; i++) {
        for (const auto& s : turn[i]) {
            int x = s.first, y = s.second;
            board[x][y] = i;
        }
    }
    return best;
}

pair<int, int> Board::judge(int player, int *Nodes, double *Time, int &c) {
    double START, END;
    START = clock(); // 時間(開始)。

    root = player;
    bool legal[13][13] = { 0 };
    set<pair<int, int>> flip[13][13];   // 下每一個地方有其他哪些點會被翻。
    Legal(player, legal, flip);
    //PrintBoard();
    //PrintLegal(legal);  // 印出合法的走步(bool)。
    //Printflip(flip);     // 印出下該位置會造成哪些位置棋子的翻轉。
    updateWeight(player);

    Node = 0;
    int fx = 0, fy = 0;
    vector<float> best(4, 0), value(4, 0);

    if (N[player] > 0) {
        for (int i = 1; i < 12; i++) {
            for (int j = 1; j < 12; j++) {
                if (!legal[i][j]) {
                    continue;
                }
                value = maxn(make_pair(i, j), flip[i][j], 1, player, Bound);
                if (value[player] >= best[player]) {
                    best = value;
                    fx = i, fy = j;
                }
            }
        }
    }
    else {
        vector<pair<int, int>> options;
        for (int i = 1; i < 12; i++) {
            for (int j = 1; j < 12; j++) {
                if (legal[i][j]) {
                    options.emplace_back(i, j);
                }
            }
        }
        
        int size = options.size();
        if (!size) {
            still++;
            return make_pair(0, 0);
        }

        random_device rd; // 隨機設備
        mt19937 m(rd());    // 先得到一個隨機數生成器。
        uniform_int_distribution<> dist(1, size);    // 隨機數分布。

        int r = dist(m);
        for (const auto& s : options) { // set無法直接用[]取得元素。
            if (!(--r)) {
                fx = s.first, fy = s.second;
                break;
            }
        }
    }
    cout << "check node: " << Node;
    totalNode += Node;

    if (fx > 0 && fy > 0) {
        updateBoard(fx, fy, player, flip[fx][fy]);
    }
    else {
        still++;
    }
    END = clock();
    cout << ", run time: " << (END - START) / CLOCKS_PER_SEC << endl;
    if (player == 2) {
        Nodes[c] += Node;
        Time[c] += (END - START) / CLOCKS_PER_SEC;
        c++;
    }

    return make_pair(fx, fy);
}

void Board::updateBoard(int fx, int fy, int player, set<pair<int, int>> Flip) {
    unoccupied--;
    // 接近終局時根據設定的N來改變搜尋深度。
    N[1] = min(unoccupied, N[1]), N[2] = min(unoccupied, N[2]), N[3] = min(unoccupied, N[3]);

    still = 0;
    board[fx][fy] = player;

    for (const auto& s : Flip) {
        board[s.first][s.second] = player;
    }

    int count[4] = { 0 };
    Count(count);
    if ((!count[1] && !count[2]) || (!count[1] && !count[3]) || (!count[2] && !count[3])) {
        early = true;
        return;
    }

    Path(fx, fy);     // 此輪下完子後更新路徑表。
    //PrintBoard();
    //PrintPath();

    /*vector<float> score(4, 0);
    ScoreCount(player, score);
    cout << "Score: ";
    for (int i = 1; i < 4; i++) {
        cout << score[i] << "  ";
    }
    cout << endl;*/
}

void Board::Count(int *count) {
    for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
            if (board[i][j] <= 0) {
                continue;
            }
            count[board[i][j]]++;
        }
    }
}

int Board::Final() {
    int winner = 0, count[4] = { 0 }, over[4] = { -1 };
    Count(count);
    if (!count[1] && !count[2]) {
        return 3;
    }
    else if (!count[1] && !count[3]) {
        return 2;
    }
    else if (!count[2] && !count[3]) {
        return 1;
    }
    for (int i = 1; i < 4; i++) {
        over[i] = count[i] - WinLow[i];
    }
    int max = -1;
    for (int i = 1; i < 4; i++) {
        cout << count[i] << " ";
        if (over[i] > max) {
            max = over[i];
            winner = i;
        }
    }
    cout << endl;
    cout << "total check node: " << totalNode << endl;
    return winner;
}