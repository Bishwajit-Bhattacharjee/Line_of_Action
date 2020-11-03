#include <bits/stdc++.h>
using namespace std;
using PII = pair<int,int>;
int AI_ID;
ofstream ai_log;
int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

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

Move best_move;
int dep = 4;

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
        this->cur_player = state.cur_player;
        // this->rotate_move(); // expecting this will be called by alternating player
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
            PII what_pos = go_to_a_direction(row, col, dir);

            if(what_pos == PII(-1, -1)) continue;
            moves.emplace_back(row, col, what_pos.first, what_pos.second);
        }
        return moves;
    }

    PII go_to_a_direction (int row, int col, int dir){
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
        int cnt = 0;

        while(is_inside_the_board( row + ddx , col + ddy) ){
            row += ddx, col += ddy;
            cnt += (this->board[row][col] != 0);            
        }
        return cnt;
    }    

    bool is_inside_the_board (const int &row, const int &col){
        return row >= 0 and row < this->row_count and col >= 0 and col < this->col_count;
    }

    int bfs (PII src) {
        int sz = 1;
        vector < vector < bool > > vis( this->row_count, vector<bool> (this->col_count, false));
        queue < PII > q;

        vis[src.first][src.second] = 1;
        q.push(src);
        
        while (q.size()) {
            PII front = q.front(); q.pop();
            for (int dir = 0; dir < 8; dir++) {
                int x = front.first + dx[dir];
                int y = front.second + dy[dir];

                if (!is_inside_the_board (x, y) ) continue;
                if (vis[x][y] or this->board[x][y] != this->board[src.first][src.second]) continue;
                sz++;
                q.push({x,y});
                vis[x][y] = 1;
            }
        }
        return sz;
    }


    int winner (){
        int one_count = 0, two_count = 0;
        PII one_pos = PII(-1, -1), two_pos = PII(-1, -1);

        for (int r = 0; r < this->row_count; r++){
            for (int c = 0; c < this->col_count; c++){
                int el = this->board[r][c];
                if (el == 0) continue;
                else if (el == 1) one_count++, one_pos = PII(r, c);
                else if (el == 2) two_count++, two_pos = PII(r, c);
            }
        }
        assert(one_count >= 1 && two_count >= 1);

        int one_comp_size = bfs(one_pos);
        int two_comp_size = bfs(two_pos);
        
        bool one_can_win = (one_comp_size == one_count);
        bool tow_can_win = (two_comp_size == two_count);

        if(one_can_win && tow_can_win){
            return (this->cur_player == 1? 2 : 1);
        }
        if(one_can_win) return 1;
        if(tow_can_win) return 2;
        return 0;
    }

    void print (){
        ai_log << "Current player " << this->cur_player << endl;
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

    void make_random_move (){
        best_move = Move(-1, -1, -1, -1);
        int max_found = minimax(*this, INT_MIN, INT_MAX, dep);
        ai_log << " VALUE FOUND : " << max_found << endl;
        ai_log << best_move.fx << " " << best_move.fy << " " << best_move.tx << " " << best_move.ty << endl; 

        this->handle_an_ai_move(best_move); 
    }

    void handle_an_ai_move (const Move &move, bool should_print = true) {
        assert(this->board[move.fx][move.fy] == AI_ID && this->board[move.tx][move.ty]);

        board[move.tx][move.ty] = board[move.fx][move.fy];
        board[move.fx][move.fy] = 0;

        this->rotate_move();

        if (should_print){
            cout << move.fx << endl;
            cout << move.fy << endl;
            cout << move.tx << endl;
            cout << move.ty << endl;

            ai_log << "Move : " ;
            ai_log << move.fx << " " << move.fy << " " << move.tx << " " << move.ty << endl;
        }
        // Debug 
    }

    void handle_an_hooman_move (const int &fx, const int &fy, const int &tx, const int &ty) {
        board[tx][ty] = board[fx][fy];
        board[fx][fy] = 0; 
        this->rotate_move();
    }

};

int eval_func (State &state) {
    return rand() % 120;
}

int minimax (State state, int alpha, int beta, int depth) {

    int winner = state.winner();
    if (winner != 0) {
        if (winner == AI_ID ) return 1e8 ;
        else return -1e8;
    }
    if (depth == 0) {
        return eval_func(state);
    }
    if (state.cur_player == AI_ID ) { // maximizing player
        int max_val = INT_MIN;
        vector < Move > moves = state.get_possible_moves();

        for (auto move : moves){
            State child(state);
            child.handle_an_ai_move(move, false);
            assert (child.cur_player != state.cur_player);
            int cur = minimax (child, alpha, beta, depth - 1);

            if (cur > max_val and depth == dep) {
                best_move = move;
            }

            max_val = max (max_val, cur);
            alpha = max (alpha, cur);
            if (beta <= alpha) break;
        }
        return max_val;        
    }
    else {
        int min_val = INT_MAX;
        vector < Move > moves = state.get_possible_moves();

        for (auto move : moves){
            State child(state);
            child.handle_an_ai_move(move, false);
            assert (child.cur_player != state.cur_player);
            int cur = minimax (child, alpha, beta, depth - 1);
            min_val = min (min_val, cur);
            beta = min (beta, cur);
            if (beta <= alpha) break;
        }
        return min_val;        
    }
}

int main(int argc, char** argv){
    srand(time(NULL));

    ai_log.open("ai_log.txt", ios::out);

    cerr << "argc : " << argc << endl;
    int ROW, COL;
    ROW = atoi(argv[1]);
    COL = atoi(argv[2]);
    AI_ID = atoi(argv[3]);

    cerr << ROW << " " << COL << " " << AI_ID << endl;
    int cur = 1;
    State game = State(ROW, COL, 1);

    game.print();

    while (true){

        int which_player;
        cin >> which_player;
        cerr << "cpp player " << which_player << endl;

        if ( game.winner() != 0) {
            ai_log << "BODDA " << game.winner() << " jiti gese " << endl;    
            exit(0);
        }

        if (which_player == AI_ID){
            // game.next_moves_debug();
            game.make_random_move();
        }
        else {
            int fx, fy, tx, ty;
            cin >> fx >> fy >> tx >> ty;
            game.handle_an_hooman_move(fx, fy, tx, ty);
        }
    }
    
    ai_log.close();
}