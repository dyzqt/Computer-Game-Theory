#include<iostream>
#include<Windows.h>
#include<vector>

using namespace std;
/*
789
456
123
move = 1~9
board[(move-1)%3][(move-1)/3]
*/
//人为X，计算机为O
char board[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
char  Player = 'X';
int win = -100;

// 画棋盘
static void printBoard(char board[3][3]) {
    system("cls");//清空上一次图片
    for (int i = 2; i >= 0; i--) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    Sleep(10);//100毫秒刷新
}

// 判断胜利
static int checkWin(char player) {
    // 横向
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            if (player == 'X') return 1;
            else return -1;
        }
    }

    // 纵向
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            if (player == 'X') return 1;
            else return -1;
        }
    }

    // 交叉
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        if (player == 'X') return 1;
        else return -1;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        if (player == 'X') return 1;
        else return -1;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return -100; // 游戏未结束
            }
        }
    }
    return 0; // 平局
}

static bool chack(int move) {
    // 输入是否合理
    if (move < 1 || move > 9 || board[(move - 1) / 3][(move - 1) % 3] == 'X' || board[(move - 1) / 3][(move - 1) % 3] == 'O') {
        printBoard(board);
        cout << "重新输入" << endl;
        return false;
    }

    // 画棋子
    board[(move - 1) / 3][(move - 1) % 3] = Player;
    return true;
}
//赢了+1，输了-1，平局0
static int eval() {
    int ris = 0; //进行评分
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            if (board[i][j] == (i * 3 + j + 1) + '0') ris++;
    //int win = checkWin( Player);
    if (win == -1) return ris + 1;
    else if (win == 1) return -(ris + 1);
    else if (win == 0) return 0;

}
//记录每一层赢的可能性，每次返回最大的数。
//奇数为计算机，偶数为人
//bug:在交换时判断胜负会判断错，如：已经下的‘O’，需要判断‘O’是否赢，但plays已经调换变成‘X’
static int MiniMax(int &x,int high) { //一个记录下的位置，一个记录层数 &可以修改x值并传回 引用传递，修改 x 会影响外部的原始变量
    int num; //记录最大值
    if (high % 2 == 1) num = -100; //计算机时要最高的
    else num = 100; //人时要最低的
    //判断胜负
    char plays = high % 2 == 0 ? 'O' : 'X';
    win = checkWin(plays);
    /*if (checkWin(plays) == 1 || checkWin(plays) == -1) return eval();*/
    if (win == 1 || win == -1) return eval();

    vector<int> set; //记录可下位置
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            if (board[i][j] != 'O' && board[i][j] != 'X') set.push_back(i * 3 + j + 1);

    if (set.size() == 0) return eval(); //平局
    for (int i = 0;i < set.size();i++) { //全部遍历完
        int a = set[i]; //记录可下位置
        int b = a; //记录上一步
        board[(a - 1) / 3][(a - 1) % 3] = high % 2 == 1 ? 'O' : 'X'; //奇数为计算机走的
        int max = MiniMax(a,high + 1);
        board[(b - 1) / 3][(b - 1) % 3] = b + '0'; //因为共用一个表格，将上一步还原 表格时字符串，还原也要还原成字符

        //将要下的位置
        if (high % 2 == 1) {
            if (num < max) {
                num = max;
                if (high == 1) x = set[i];
            }
        }
        else {
            if (num > max) {
                num = max;
            }
        }
    }
    return num;
}
void com() {
    int x = {};
    MiniMax(x, 1);
    board[(x - 1) / 3][(x - 1) % 3] = 'O';
}
int main() {
    int move;
    int high = 1;
    while (true) {
        printBoard(board);
        if (Player == 'X') {
            while (1) {
                cout << "玩家 " << Player << ",请下棋 (1-9): ";
                cin >> move;
                if (chack(move)) break;
            }
        }
        else {
            com();
        }
        int win = checkWin(Player);
        // 检查胜利
        if (win == 1) {
            printBoard(board);
            cout << "玩家 X 赢了" << endl;
            //break;
        }
        if (win == -1) {
            printBoard(board);
            cout << "玩家 O 赢了" << endl;
            //break;
        }
        // 检查格子是否平
        if (win == 0) {
            printBoard(board);
            cout << "平局" << endl;
            /*break;*/
        }

        // 交换
        Player = (Player == 'X') ? 'O' : 'X';
        //是否继续游戏
        if (win == 0 || win == 1 || win == -1) {
            char x;
            cout << "是否继续游戏 y/n" << endl;
            cin >> x;
            if (x == 'n') break;
            else {
                Player = 'X';
                win = -100;
                for (int i = 0;i < 3;i++)
                    for (int j = 0;j < 3;j++)   
                        board[i][j] = (i * 3 + j + 1) + '0';

                printBoard(board);
            }
        }

    }
    return 0;
}