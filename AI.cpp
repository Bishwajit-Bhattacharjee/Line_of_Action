#include <bits/stdc++.h>
using namespace std;
using PII = pair<int,int>;
int const R = 6;
int const C = 6;
int AI_ID;

int board[R][C];

int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

ofstream ai_log;

 
struct Move {
    int fx, fy, tx, ty;
    Move (const int _fx, const int _fy, const int _tx, const int _ty){
        this->fx = _fx; 
        this->fy = _fy;
        this->tx = _tx;
        this->ty = _ty;
    }
    Move (){}
};

struct State {

    int row_count;
    int col_count;
    vector < vector < int > > board;
    int cur_player;

    State (const int &row, const int &col, const int &cur_p){
        this->row_count = row;
        this->col_count = col;
        this->cur_player = cur_p;
        this->board = vector < vector <int > > (this->row_count, vector < int > (this->col_count, 0));
        this->initialize();        
    }

    void initialize(){
        for(int i = 0; i < this->row_count; i++){
            if(i == 0 or  i + 1 == this->row_count){
                for(int j = 1; j + 1< this->col_count; j++){
                    board[i][j] = 1;
                }
            }
            else {
                board[i][0] = board[i][this->col_count - 1] = 2;
            }
        }
    }

    State (const State &state){
        this->board = state.board;
        this->row_count = state.row_count;
        this->col_count = state.col_count;
        this->rotate_move(); // expecting this will be called by alternating player
    }

    void rotate_move(){
        this->cur_player = (this->cur_player == 1) ? 2 : 1 ; 
    }
    vector < Move > get_possible_moves () {
        vector < Move > moves;

        for (int r = 0; r < this->row_count ; r++){
            for (int c = 0; c < this->col_count ; c++){
                int el = this->board[r][c];

                if(el == this->cur_player){
                    vector < Move > cur_pos_moves = get_moves_from_some_position(r, c);
                    moves.insert (moves.end(), cur_pos_moves.begin(), cur_pos_moves.end()) ;           
                }
            }
        }
        return moves;
    }

    vector < Move > get_moves_from_some_position (int row, int col){
        assert(this->board[row][col] == this->cur_player);
        vector < Move > moves;
        for(int dir = 0; dir < 8; dir++){
            PII what_pos = go_a_direction(row, col, dir);

            if(what_pos == PII(-1, -1)) continue;
            moves.emplace_back(row, col, what_pos.first, what_pos.second);
        }
        return moves;
    }

    PII go_a_direction (int row, int col, int dir){
        PII target = PII(-1, -1); // flag to say you cannot go in this direction
        int cnt = get_count_in_a_line(row, col, dir); // line can horizontal, vertical, diagonal

        // first try to go cnt - 1 steps, if there is any opposition's
        // checker, then you need to stop , but the cnt-th step is special.
        // You can move on that square even it there is an opposition's checker.
        while( cnt > 1){
            row += dx[dir], col += dy[dir] ;
            if(!is_inside_the_board(row, col)) return target; 
            if(this->board[row][col] and this->board[row][col] != this->cur_player) return target; // stepping on opponent's guti
            cnt--;
        }
        assert(cnt == 1);
        row += dx[dir];
        col += dy[dir];
        
        if(! is_inside_the_board(row, col)) return target;
        if(this->board[row][col] == this->cur_player) // stepping on my own guti, invalid
            return target;

        return {row, col}; // can go in this direction

    }

    int get_count_in_a_line (int row, int col, const int &dir){
        int nx = -dx[dir], ny = -dy[dir];

        return get_count_till_border(row, col, dx[dir], dy[dir]) + 
               get_count_till_border(row, col, nx, ny) +
               1;
    }

    int get_count_till_border (int row, int col, const int ddx, const int ddy){
        int cnt = 1;

        while(is_inside_the_board( row + ddx , col + ddy) ){
            row += ddx, col += ddy;
            cnt += (this->board[row][col] != 0);            
        }
        return cnt;
    }    

    bool is_inside_the_board (const int &row, const int &col){
        return row >= 0 and row < this->row_count and col >= 0 and col < this->col_count;
    }

    void print (){
        ai_log << "Current player " << endl;
        for(int r = 0; r < this->row_count; r++){
            for(int c = 0; c < this->col_count; c++){
                ai_log << this->board[r][c] << " ";
            }
            ai_log << endl;
        }
    }

    void next_moves_debug(){

        this->print();
        
        for(int r = 0; r < this->row_count; r++){
            for(int c = 0; c < this->col_count; c++){
                int cur = this->board[r][c];
                if(cur == AI_ID){
                    ai_log << "player pos(" << r << ", " << c  << " )" << endl;
                    ai_log << "valid moves : \n";
                    vector < Move > moves = get_moves_from_some_position(r, c);
                    for(auto x : moves){
                        ai_log << x.tx << " " << x.ty << endl;
                    }
                }
            }
        }
    } 

    void make_random_move(){
        PII empty = {-1, -1}, ai = {-1, -1};
        for(int i = 0; i < this->row_count; i++){
            for(int j = 0; j < this->col_count ; j++){
                if(board[i][j] == 0) empty = {i,j};
                else if(board[i][j] == AI_ID) ai = {i, j};
            }
        }
        board[empty.first][empty.second] = board[ai.first][ai.second];
        board[ai.first][ai.second] = 0;

        this->rotate_move();

        cout << ai.first << endl;
        cout << ai.second << endl; 
        cout << empty.first << endl; 
        cout << empty.second << endl; 
        
    }

    void handle_an_hooman_move (const int &fx, const int &fy, const int &tx, const int &ty) {
        board[tx][ty] = board[fx][fy];
        board[fx][fy] = 0; 
        this->rotate_move();
    }

};



int main(int argc, char** argv){
    
    ai_log.open("ai_log.txt", ios::out);

    // getline(cin, s);
    // getline(cin, s);

    cerr << "argc : " << argc << endl;
    int ROW, COL;
    ROW = atoi(argv[1]);
    COL = atoi(argv[2]);

    AI_ID = atoi(argv[3]);
    cerr << ROW << " " << COL << " " << AI_ID << endl;
    int cur = 1;
    State game = State(ROW, COL, 1);

    game.print();

    while(true){

        int which_player;
        cin >> which_player;
        cerr << "cpp player " << which_player << endl;

        if(which_player == AI_ID){
            game.next_moves_debug();
            game.make_random_move();
            cur = ( AI_ID == 2)? 1 : 2;                
        }
        else {
            int fx, fy, tx, ty;
            cin >> fx >> fy >> tx >> ty;
            cerr << "cpp " << fx << " " << fy << " " << tx << " " << ty << endl;
            game.handle_an_hooman_move(fx, fy, tx, ty);
            cur = AI_ID;            
        }
    }
    
    ai_log.close();
}