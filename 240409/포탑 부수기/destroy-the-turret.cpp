#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<cstring>

using namespace std;

int N, M, K;
int map[10][10];
int when_attack[10][10] = { 0 }, isattack[10][10] = {0};
//우/하/좌/상의 우선순위대
int dy[] = { 0,1,0,-1 }, dx[] = { 1,0,-1,0 };
int dy_8[] = { -1,-1,0,1,1,1,0,-1 }, dx_8[] = { 0,1,1,1,0,-1,-1,-1 };
struct point {
	int y, x;
};
vector<point>turret;
int path_map[10][10] = { 0 };
bool cmp(point a, point b) {
	if (map[a.y][a.x] == map[b.y][b.x]) {
		if (when_attack[a.y][a.x] == when_attack[b.y][b.x]) {
			if (a.y + a.x == b.y + b.x) {
				return a.x < b.x;
			}
			return a.y + a.x < b.y + b.x;
		}
		return when_attack[a.y][a.x] < when_attack[b.y][b.x];
	}
	return map[a.y][a.x] < map[b.y][b.x];
}
void input() {
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cin >> map[i][j];
		}
	}

}

int sorted() {
	turret.clear();
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (map[i][j] != 0) {
				turret.push_back({ i,j });
			}
		}
	}
	sort(turret.begin(), turret.end(), cmp);
	return map[turret.back().y][turret.back().x];
}

bool laser_valid() {
	queue<point>q;
	memset(path_map, -1, sizeof(path_map));
	q.push(turret.front());

	while (!q.empty()) {
		point now = q.front();
		q.pop();

		if (now.y == turret.back().y && now.x == turret.back().x) {
			return true;
		}

		for (int i = 0; i < 4; i++)
		{
			int ny = (now.y + dy[i]+N) % N;
			int nx = (now.x + dx[i]+M) % M;

			if (map[ny][nx] == 0) continue;
			if (path_map[ny][nx] != -1)continue;
			path_map[ny][nx] = (i + 2) % 4;
			q.push({ ny, nx });
		}
	}
	return false;
}

int main() {
	int max;
	input();
	sorted();

	for (int t = 1; t <= K; t++)
	{
		memset(isattack, 0, sizeof(isattack));
		map[turret.front().y][turret.front().x] += N + M;
		isattack[turret.front().y][turret.front().x] = 1;
		when_attack[turret.front().y][turret.front().x]++;
		int dmg = map[turret.front().y][turret.front().x];
		if (laser_valid()) {
			point end = turret.back();
			map[end.y][end.x] -= dmg;
			isattack[end.y][end.x] = 1;
			if (map[end.y][end.x] < 0) map[end.y][end.x] = 0;
			while (1) {
				int dir = path_map[end.y][end.x];
				end.y = (end.y + dy[dir]+N) % N;
				end.x = (end.x + dx[dir]+M) % M;
				if (end.y == turret.front().y && end.x == turret.front().x)break;
				map[end.y][end.x] -= dmg / 2;
				isattack[end.y][end.x] = 1;
				if (map[end.y][end.x] < 0) map[end.y][end.x] = 0;
			}
		}

		else {
			point tar = turret.back();
			map[tar.y][tar.x] -= dmg;
			isattack[tar.y][tar.x] = 1;
			if (map[tar.y][tar.x] < 0) map[tar.y][tar.x] = 0;
			for (int i = 0; i < 8; i++)
			{
				int ny = (tar.y + dy_8[i]+N) % N;
				int nx = (tar.x + dx_8[i]+M) % M;
				if (ny == turret.front().y && nx == turret.front().x) {
					continue;
				}
				if (map[ny][nx] == 0)continue;

				map[ny][nx] -= dmg / 2;
				isattack[ny][nx] = 1;
				if (map[ny][nx] < 0) map[ny][nx] = 0;
			}
		} 
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				if (map[i][j] != 0 && isattack[i][j] != 1) {
					map[i][j]++;
				}
			}
		}
		max = sorted();
		if (turret.size() == 1) 
			break;
	}

	cout << max;
	return 0;
}