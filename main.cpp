#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include <unordered_set>
using namespace std;


class state{
public:

    state* lead;

    bool board[8][8];
    state(bool C[8][8]){
        int i,j;
        for(i = 0; i < 8; i++)
            for(j = 0; j < 8; j++)
                board[i][j] = C[i][j];
        lead = NULL;
    }
    state(bool b[8][8], state* p){
        int i,j;
        for(i = 0; i < 8; i++)
            for(j = 0; j < 8; j++)
                board[i][j] = b[i][j];
        lead = p;
    }

    // method that encodes bool array to uint64_t
    uint64_t encode(){
        uint64_t uni = 0;
        int i, j;
        for(i = 0; i < 8; i++){
            for(j = 0; j < 8; j++){
                uni = (uni<<1) | (board[i][j]?1:0);
            }
        }
        return uni;
    }

    // method that gets the score for the computer to figure out what the next best move would be
    int getScore(){
        int score = 0;
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(board[i][j] == 1) score++;
            }
        }
        return score;
    }

    // click the board at spot x and y
    state* click(int x, int y){
        bool move[8][8];
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                move[i][j] = board[i][j];
            }
        }

        //flip 1 to 0 and vice versa
        if(y>0) move[x][y-1] = 1-move[x][y-1];
        if(x>0) move[x-1][y] = 1-move[x-1][y];
        if (x<7) move[x+1][y] = 1-move[x+1][y];
        if (y<7) move[x][y+1] = 1- move[x][y+1];
        return new state(move);
    }

    //print method that prints board
    void print(){
        int i,j;
        for(i = 0; i<8; i++){
            for(j=0; j<8; j++){
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    // method that prints how many steps were taken
    void printLast(){
        state *p = this;
        int steps = 0;
        while(p != NULL){
            p->print();
            cout << endl;
            p = p->lead;
            steps++;
        }
        cout << "Number of steps = " << steps << endl;
    }

    // checks for all 0's on the board
    bool isGoal(){
        int i,j;
        for(i=0;i<8;i++){
            for(j=0;j<8;j++){
                if(board[i][j] == 1) return false;
            }
        }
        return true;
    }
};

class statewrapper{
public:
    state* s;
    statewrapper(state *s) : s(s) {}
};

bool operator<(const statewrapper& s1, const statewrapper& s2) { // s1 is less than s2
    // compute the distance all pieces to their final place
    int score1 = s1.s->getScore();
    int score2 = s2.s->getScore();
    if (score1 < score2) return false;
    return true;
}

int main() {

    //initial board
    //change the 1's and 0's to your liking
    bool board[8][8] = {0, 1, 0, 0, 0, 1, 0, 0,
                        1, 1, 0, 0, 1, 0, 1, 0,
                        1, 0, 1, 0, 0, 0, 0, 0,
                        0, 1, 0, 0, 1, 1, 1, 0,
                        0, 0, 0, 0, 1, 0, 0, 0,
                        0, 0, 0, 0, 1, 0, 1, 1,
                        0, 0, 0, 0, 0, 1, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 0 };

    state *init = new state(board);

    priority_queue<statewrapper> OPEN;
    unordered_set<uint64_t > unOr;

    OPEN.push(statewrapper(init));

    while(!OPEN.empty()){

        state *s = OPEN.top().s;
        OPEN.pop();

        if(s->isGoal()){
            s->printLast();
            cout<< "Solution found" << endl;
            break;
        }

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){

                state* newstate = s->click(i,j);

                // use set unOr to stored all the states that have been pushed
                // only push when a new state is not in unOr
                if(newstate && unOr.find(newstate->encode()) == unOr.end()) {

                    newstate->lead = s;
                    OPEN.push(statewrapper(newstate));
                    unOr.insert(newstate->encode());
                }
            }
        }
    }
    cout << "Number of states: " << unOr.size() << endl;

    return 0;
}
