#include <bits/stdc++.h>
using namespace std;

int puzzle[4][4], temp[16], zero_r, zero_c;
bool even_parity = false;
int goal_pos1[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};
int goal_pos2[16] = {1, 2, 3, 4, 8, 7, 6, 5, 9, 10, 11, 12, 0, 15, 14, 13};
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
        }
    }

    print_puzzle();
} 

int check_parity(){
    int f1 = 0, f2 = 0, f = 0;
    
    for(int i = 0; i < 16; ++i){
        int num = temp[i];
        if(num == 0){
            zero_r = i/4;
            zero_c = i%4;
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
    cout << "f: " << f << endl;
    if(f % 2 == 0)  even_parity = true;
    else even_parity = false;
    cout << "parity: " << even_parity << endl;
}

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
    cout << "g prev thr " << g << " " << prev_direction << " " << threshold << endl; 
    int h = H();
    if(h == 0)  return -1;   // found
    if((g+h) > threshold) return g+h;     // fail

    int mn = 100000000;
    for(int i = 0; i < 4; ++i){
        if(i == (prev_direction ^ 1))   continue;

        int next_r = zero_r + r_dir[i];
        int next_c = zero_c + c_dir[i];
        if(next_r < 0 || next_r > 4 || next_c < 0 || next_c > 4) continue;

        path.push_back(dir[i]);
        swap(puzzle[next_r][next_c], puzzle[zero_r][zero_c]);
        swap(next_r, zero_r);
        swap(next_c, zero_c);
        int result = DFS(g+1, i, threshold);
        if(result == -1) return -1;
        if(result < mn) mn = result;
        swap(next_r, zero_r);
        swap(next_c, zero_c);
        swap(puzzle[next_r][next_c], puzzle[zero_r][zero_c]);
        path.pop_back();
    }
    return mn;
}

bool IDAstar(){
    int threshold = H();
    cout << threshold << endl;
    while(1){
        int t = DFS(0, -1, threshold);
        cout << "t " << t << endl;
        if(t == -1)   return true;
        if(t == 100000000) return false;
        if(t > 70) return false;
        threshold = t;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int m_distance = 0;
    cout << "Random puzzle:\n";
    generate_puzzle();
    check_parity();

    m_distance = H();
    cout << "Manhatann distance: " << m_distance << endl;
    if(!even_parity) return 0;
    else {
        cout << "even" << endl;
        // cout << IDAstar() << endl;
        // print_puzzle();
    }

}