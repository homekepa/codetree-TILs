#include<iostream>
#include<vector>
#include<queue>
#include<vector>
#include<algorithm>
#include<cstring>

using namespace std;

int L, N, Q; // L체스판 크기, N기사의 수, Q명령의 수
int trap_map[41][41], knight_map[41][41];
int total_dmg = 0, flag; //result
int dy[4] = {-1,0,1,0}, dx[4] = {0,1,0,-1};
int ismove[31] = {0,} ;
int life[31] = { 0 };
struct knight_info {
    /*
     * 기사의 위치 r, c 의 위치 좌측 상단
     * 기사의 위치 기준, h높이, w넓이
     * 기사의 체력 k
    */
    int r, c, h, w, k;
};
knight_info knight[31];
queue<pair<int, int>>cmd;

void input() {
    cin >> L >> N >> Q;
    for (int i = 1; i <= L; i++)
    {
        for (int  j = 1; j <= L; j++)
        {
            cin >> trap_map[i][j];
        }
    }
    int r, c, h, w, k;
     
    for (int i = 1; i <= N; i++)
    {
        cin >> r >> c >> h >> w >> k;
        knight[i] = { r,c,h,w,k };
    }
    int num, dir;
    for (int i = 0; i < Q; i++)
    {
        cin >> num >> dir;
        cmd.push({ num, dir });
    }
}

void draw() {
    for (int i = 1; i <= N; i++)
    {
        for (int j = knight[i].r; j <= knight[i].r + knight[i].h - 1; j++)
        {
            for (int k = knight[i].c; k <= knight[i].c + knight[i].w - 1; k++)
            {
                knight_map[j][k] = i;
            }
        }
    }
}
void del_knight(int num) {
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            if (knight_map[i][j] == num) {
                knight_map[i][j] = 0;
            }
        }
    }
}

void damegecount() {
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            int num = knight_map[i][j];
            if(trap_map[i][j] == 1 && num != 0 && !(cmd.front().first == num) && ismove[num] == 1) {
                knight[num].k--;
                life[num]++;
                if (knight[num].k <= 0) {
                    del_knight(num);
                }
            }
        }
    }
}

void move_knight(int dir) {
    for (int num = 1; num <= N; num++)
    {
        if(ismove[num] != 1)continue;
        knight[num].r += dy[dir];
        knight[num].c += dx[dir];
        del_knight(num);
        for (int j = knight[num].r; j <= knight[num].r + knight[num].h - 1; j++)
        {
            for (int k = knight[num].c; k <= knight[num].c + knight[num].w - 1; k++)
            {
                knight_map[j][k] = num;
            }
        }
    }
      
}

bool move_isvalid(int num, int dir) {
    int ny = knight[num].r + dy[dir];
    int nx = knight[num].c + dx[dir];
    for (int j = ny; j <= ny + knight[num].h - 1; j++)
    {
        for (int k = nx; k <= nx + knight[num].w - 1; k++)
        {
            
            if (knight_map[j][k] == num) continue;
            if (j <= 0 || k <= 0 || j > L || k > L)return false; // map이탈
            if (trap_map[j][k] == 2)return false;//벽에 부딪쳤을때
            if (knight_map[j][k] != 0){
                if (!move_isvalid(knight_map[j][k], dir))return false;
            }
        }
    }
    ismove[num] = 1;
    return true;
}

int main() {
    input();
    draw(); 
    while (!cmd.empty()) {
        memset(ismove, 0, sizeof(ismove));
        int num = cmd.front().first;
        if (knight[num].k == 0) { cmd.pop(); continue; }
        int dir = cmd.front().second;
        if (move_isvalid(num, dir)) {
            move_knight(dir);
            damegecount();
        }
        cmd.pop();
    }
    for (int i = 1; i <= N; i++)
    {
        if (knight[i].k > 0) total_dmg += life[i];
    }
    cout << total_dmg;
    return 0;
}