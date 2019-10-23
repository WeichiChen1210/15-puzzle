#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

int puzzle[4][4], origin[4][4], temp[16], zero_r, zero_c, temp_r, temp_c;
int goal_pos1[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};   // parity: even
int goal_pos2[4][4] = {{1, 2, 3, 4}, {8, 7, 6, 5}, {9, 10, 11, 12}, {0, 15, 14, 13}};   // parity: odd
vector<char> path;
int r_dir[4] = {0, 0, +1, -1};
int c_dir[4] = {+1, -1, 0, 0};
int dir[4] = {'R', 'L', 'D', 'U'};


void print_puzzle(){
    for(int j = 0; j < 4; ++j){
        for(int i = 0; i < 4; ++i)
            cout << setw(3) << puzzle[j][i];
        cout << endl;
    }
}

void generate_puzzle(){
    srand(time(NULL));
    for(int i = 0; i < 16; ++i)
        temp[i] = i;
    
    for(int i = 15; i > 0; --i){
        int j = rand() % i;
        int tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }
    int cnt = 0;
    for(int j = 0; j < 4; ++j){
        for(int i = 0; i < 4; ++i){
            puzzle[j][i] = temp[cnt++];
            origin[j][i] = puzzle[j][i];
        }
    }

    print_puzzle();
} 


bool check_parity(){
    bool even_parity = false;
    int f1 = 0, f2 = 0, f = 0;
    
    for(int i = 0; i < 16; ++i){
        int num = temp[i];
        if(num == 0){
            temp_r = zero_r = i/4;
            temp_c = zero_c = i%4;
            f2 = zero_r + 1;
            continue;
        }
        int sum = 0;
        for(int j = i+1; j < 16; ++j){
            if(temp[j] != 0 && temp[j] < num)
                sum++;
        }
        // cout << sum << " ";
        f1 += sum;
    }
    f = f1 + f2;
    // cout << "f: " << f << " " << f1 << " " << f2 << endl;
    if(f % 2 == 0)  even_parity = true;
    else even_parity = false;
    // cout << "parity: " << even_parity << endl;
    return even_parity;
}

// H function, calculate Manhattan distance
int H(){
    int distance = 0;

    for(int j = 0; j < 4; ++j){
        for(int i = 0; i < 4; ++i){
            int num = puzzle[j][i];
            if(num == 0)    continue;

            int goal_x = (num-1) % 4;
            int goal_y = (num-1) / 4;

            distance += abs(goal_x - i) + abs(goal_y - j);
        }
    }
    return distance;
}

int DFS(int g, int prev_direction, int threshold){
    // update h
    int h = H();
    if(h == 0)  return -1;   // found
    if((g+h) > threshold) return g+h; // return and increase threshold

    int mn = 100000000;
    //  test four directions
    for(int i = 0; i < 4; ++i){
        // avoid going back
        if(i == (prev_direction ^ 1))   continue;

        // calculate the next position to move
        int next_r = zero_r + r_dir[i];
        int next_c = zero_c + c_dir[i];
        if(next_r < 0 || next_r > 4 || next_c < 0 || next_c > 4) continue;

        // swap position
        path.push_back(dir[i]);
        swap(puzzle[next_r][next_c], puzzle[zero_r][zero_c]);
        swap(next_r, zero_r);
        swap(next_c, zero_c);

        // DFS to go down
        int result = DFS(g+1, i, threshold);
        // if return -1 then success
        if(result == -1) return -1;
        // else this move is not good, swap back
        if(result < mn) mn = result;
        swap(next_r, zero_r);
        swap(next_c, zero_c);
        swap(puzzle[next_r][next_c], puzzle[zero_r][zero_c]);
        path.pop_back();
    }
    return mn;
}

// IDA*
bool IDAstar(){
    // set up threshold with H()
    int threshold = H();
    // cout << threshold << endl;
    while(1){
        // DFS search
        int t = DFS(0, -1, threshold);
        cout << "t " << t << endl;
        if(t == -1)   return true;  // success
        // if(t == 100000000) return false;
        if(t > 60) return false;
        // increase depth and search again
        threshold = t;
    }
    return false;
}

// print out the moving processes
void print_move(){
    int steps = path.size();
    // cout << "steps: " << steps << endl;
    // resume the zero position
    zero_r = temp_r;
    zero_c = temp_c;

    for(int i = 0; i < steps; ++i){        
        char temp = path[i];
        int direction;
        cout << "============\nStep " << i+1 << ": " << temp << endl;
        // select direction
        switch (temp){
            case 'R':
                direction = 0;
                break;
            case 'L':
                direction = 1;
                break;
            case 'D':
                direction = 2;
                break;
            case 'U':
                direction = 3;
                break;
        }
        // swap blocks
        int next_r = zero_r + r_dir[direction];
        int next_c = zero_c + c_dir[direction];
        swap(origin[next_r][next_c], origin[zero_r][zero_c]);
        swap(next_r, zero_r);
        swap(next_c, zero_c);

        // print out puzzle
        for(int j = 0; j < 4; ++j){
            for(int i = 0; i < 4; ++i)
                cout << setw(3) << origin[j][i];
            cout << endl;
        }
    }
}

int main(){
    int m_distance = 0;
    cout << "Random puzzle:\n";
    generate_puzzle();

    bool is_solvable = check_parity();
    cout << "parity: " << is_solvable << endl;
    if(!is_solvable){
        cout << "this puzzle is not solvable\n";
        return 0;
    }
    else{
        m_distance = H();
        // cout << "Manhatann distance: " << m_distance << endl;
        // cout << "even" << endl;
        bool result = IDAstar();
        // too many steps
        if(!result){
            cout << "this puzzle is not solvable\n";
            return 0;
        }
        // cout << result << endl;
        for(int i = 0; i < path.size(); ++i)
            cout << path[i] << " ";
        cout << endl;
        // print the moving process
        print_move();
    }
    

}