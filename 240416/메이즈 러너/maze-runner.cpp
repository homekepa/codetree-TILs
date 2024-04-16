#include<iostream>
#include<vector>
#include<queue>
#include<cmath>
#include<climits>
#define MAX INT_MAX
using namespace std;

int map[11][11];
int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,1,-1 };
int N, M, K;
int exit_y, exit_x;
int out_cnt;
struct Info {
    int y, x, score;
    bool isOut;
};

struct Info2 {
    int y, x, dist;
};

struct Cmp {
    bool operator()(Info2 a, Info2 b) {
        if (a.dist != b.dist) {
            return a.dist > b.dist;
        }
        else {
            if (a.y != b.y) {
                return a.y > b.y;
            }
            else {
                return a.x > b.x;
            }
        }
    }
};

vector<Info>Players;
void input() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> map[i][j];
        }
    }
    int y, x;
    Players.push_back({ 0,0,0,true });
    for (int i = 1; i <= M; i++) {
        cin >> y >> x;
        Players.push_back({ y,x,0,false });
    }
    cin >> exit_y >> exit_x;
    map[exit_y][exit_x] = -1;
}

bool isValid(int y, int x) {
    return y >= 1 && x >= 1 && y <= N && x <= N && (map[y][x] == -1 || map[y][x] == 0);
}

bool movePlayers() {


    for (int i = 1; i < Players.size(); i++) {
        if (Players[i].isOut)continue;
        int cur_y = Players[i].y;
        int cur_x = Players[i].x;

        int cur_distance = abs(cur_y - exit_y) + abs(cur_x - exit_x);
        for (int j = 0; j < 4; j++) {
            int next_y = cur_y + dy[j];
            int next_x = cur_x + dx[j];
            if (!isValid(next_y, next_x))continue;

            int next_distance = abs(next_y - exit_y) + abs(next_x - exit_x);

            if (next_distance >= cur_distance)continue;
            else {
                Players[i].score++;

                if (next_distance == 0) {
                    out_cnt++;
                    Players[i].isOut = true;
                    if (out_cnt == M)return false;
                }
                else {
                    Players[i].y = next_y;
                    Players[i].x = next_x;
                }

                break;
            }

        }
    }
    return true;
}

priority_queue<Info2, vector<Info2>, Cmp> calAlldis() {
    priority_queue<Info2, vector<Info2>, Cmp>pq;

    for (int i = 1; i < Players.size(); i++) {
        if (Players[i].isOut)continue;
        int dis_y = abs(Players[i].y - exit_y);
        int dis_x = abs(Players[i].x - exit_x);
        int dis = max(dis_y, dis_x);

        int r = max(Players[i].y, exit_y) - dis;
        int c = max(Players[i].x, exit_x) - dis;
        if (r < 1)r = 1;
        if (c < 1)c = 1;
        pq.push({ r,c,dis });

    }
    return pq;
}

void rotateMap() {

    priority_queue<Info2, vector<Info2>, Cmp>pq = calAlldis();
    Info2 rotPlayer = pq.top();
    int r = rotPlayer.y;
    int c = rotPlayer.x;
    int dis = rotPlayer.dist;
    vector<int> c_pmap[11][11];
    for (int i = 1; i < Players.size(); i++) {
        if (Players[i].isOut)continue;
        int y = Players[i].y;
        int x = Players[i].x;
        c_pmap[y][x].push_back(i);
    }

    int c_map[11][11] = { 0, };
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            c_map[i][j] = map[i][j];
        }
    }

    int rot_x = c + dis;
    for (int i = r; i <= r + dis; i++) {
        int rot_y = r;
        for (int j = c; j <= c + dis; j++) {
            if (c_map[i][j] > 0)c_map[i][j]--;
            map[rot_y][rot_x] = c_map[i][j];

            if (!c_pmap[i][j].empty()) {
                for (auto p : c_pmap[i][j]) {
                    int player_num = p;
                    Players[player_num].y = rot_y;
                    Players[player_num].x = rot_x;
                }
            }

            if (map[rot_y][rot_x] == -1) {
                exit_y = rot_y;
                exit_x = rot_x;
            }
            rot_y++;
        }
        rot_x--;
    }


}

int main() {
    cin >> N >> M >> K;
    input();
    for (int i = 0; i < K; i++) {
        if (movePlayers()) {
            rotateMap();
        }
        else {
            break;
        }
    }
    int ans = 0;
    for (int i = 1; i <= M; i++) {
        ans += Players[i].score;
    }
    cout << ans << "\n";
    cout << exit_y << " " << exit_x;
    return 0;
}