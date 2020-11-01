#include <bits/stdc++.h>
using namespace std;
using PII = pair<int,int>;
int const R = 6;
int const C = 6;
int AI_ID;

int board[R][C];

void initialize(int my_id){
    for(int i = 0; i < R; i++){
        if(i == 0 or  i + 1 == R){
            for(int j = 1; j + 1< C; j++){
                board[i][j] = 1;
            }
        }
        else {
            board[i][0] = board[i][C-1] = 2;
        }
    }
}
void print(){
    for(int i = 0; i < R; i++){
        for(int j = 0; j < C; j++){
            cerr << board[i][j] << " ";
        }
        cerr << endl;
    }
}

void make_random_move(){
    PII empty = {-1, -1}, ai = {-1, -1};
    for(int i = 0; i < R; i++){
        for(int j = 0; j < C; j++){
            if(board[i][j] == 0) empty = {i,j};
            else if(board[i][j] == AI_ID) ai = {i, j};
        }
    }
    board[empty.first][empty.second] = board[ai.first][ai.second];
    board[ai.first][ai.second] = 0;
    cout << ai.first << " " << ai.second << " " << empty.first << " " << empty.second << endl; 
}
int main(int argc, char** argv){
    string s;
    getline(cin, s);
    getline(cin, s);

    cerr << "argc : " << argc << endl;
    int ROW, COL;
    ROW = atoi(argv[1]);
    COL = atoi(argv[2]);

    AI_ID = atoi(argv[3]);
    cerr << ROW << " " << COL << " " << AI_ID << endl;
    int cur = 1;
    initialize(AI_ID);
    print();

    while(true){
        int which_player;
        cin >> which_player;
        cerr << "cpp player " << which_player << endl;

        if(which_player == AI_ID){
            make_random_move();
            cur = ( AI_ID == 2)? 1 : 2;                
        }
        else {
            int fx, fy, tx, ty;
            cin >> fx >> fy >> tx >> ty;
            cerr << "cpp " << fx << " " << fy << " " << tx << " " << ty << endl;
            board[tx][ty] = board[fx][fy];
            board[fx][fy] = 0;
            cur = AI_ID;            
        }
    }
    

}