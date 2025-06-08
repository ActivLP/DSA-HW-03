#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>

using namespace std;

const int ROWS = 21;   
const int COLUMNS = 41; 

char GRID[ROWS][COLUMNS];

struct Point 
{
    int x, y;
    int cost;

    bool operator>(const Point &other) const 
    {
        return cost > other.cost;
    }
};

bool isValid(int x, int y) 
{
    return x >= 1 && x <= 20 && y >= 1 && y <= 40 && GRID[x][y] != '*';
}

void printGrid() 
{
    system("clear"); // Use "cls" for Windows
    for (int i = 1; i <= 20; i++) 
    {
        for (int j = 1; j <= 40; j++) 
        {
            cout << GRID[i][j];
        }
        cout << endl;
    }
}

// get the path from start to end using the prev array
vector<pair<int,int>> getPath(pair<int,int> start, pair<int,int> end, vector<vector<pair<int,int>>> &prev) 
{
    vector<pair<int,int>> path;
    pair<int,int> current = end;

    while (current != start)
    {
        path.push_back(current);
        current = prev[current.first][current.second];
        if (current == make_pair(-1,-1)) return {}; // no path 
    }
    reverse(path.begin(), path.end());
    return path;
}

// dijkstra's algorithm to find the shortest path using a priority queue
vector<pair<int,int>> dijkstra(pair<int,int> start, pair<int,int> end) 
{
    vector<vector<int>> dist(ROWS, vector<int>(COLUMNS, 1000000));
    vector<vector<pair<int,int>>> prev(ROWS, vector<pair<int,int>>(COLUMNS, {-1,-1}));

    priority_queue<Point, vector<Point>, greater<Point>> pq;

    dist[start.first][start.second] = 0;
    pq.push({start.first, start.second, 0});

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!pq.empty()) 
    {
        Point cur = pq.top(); 
        pq.pop();

        if (make_pair(cur.x, cur.y) == end) break;

        for (int dir = 0; dir < 4; dir++) 
        {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];

            if (isValid(nx, ny) && dist[nx][ny] > cur.cost + 1) 
            {
                dist[nx][ny] = cur.cost + 1;
                prev[nx][ny] = {cur.x, cur.y};
                pq.push({nx, ny, dist[nx][ny]});
            }
        }
    }

    return getPath(start, end, prev);
}

// generate walls randomly
void placeWalls() 
{
    srand(time(nullptr));

    cout << "How many walls do you want to place ? (max 150) : ";
    int numWalls;
    cin >> numWalls;
    if (numWalls < 0 || numWalls > 150) 
    {
        cout << "Invalid number of walls." << endl;
        return;
    }

    for (int i = 0; i < numWalls; i++)
    {
        int x = rand() % 20 + 1; 
        int y = rand() % 40 + 1; 
        if(GRID[x][y] != '-') 
        {
            i--; // Retry if the cell is already occupied
            continue;
        }
        GRID[x][y] = '*'; 
    }
}

int main() 
{
    // Initialize grid
    for (int i=1; i <=ROWS; i++)
    {
        for (int j=1; j <=COLUMNS; j++)
        {
            GRID[i][j] = '-';
        }
    }

    int xa, ya, xb, yb;
    cout << "Enter A coordinates : ";
    cin >> xa >> ya;
    cout << "Enter B coordinates : ";
    cin >> xb >> yb;

    if (!isValid(xa, ya) || !isValid(xb, yb) || (xa == xb && ya == yb)) 
    {
        cout << "Invalid coordinates or A == B." << endl;
        return 1;
    }

    placeWalls();

    GRID[xa][ya] = 'A';
    GRID[xb][yb] = 'B';

    auto path = dijkstra({xa, ya}, {xb, yb});

    if (path.empty()) 
    {
        printGrid();
        cout << "No path found!" << endl;
        return 1;
    }

    printGrid();

    for (auto [x, y] : path) 
    {
        if(GRID[x][y] != 'B')
        {
            GRID[x][y] = 'x';
        }
        printGrid();
        GRID[x][y] = '-';
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    GRID[xb][yb] = 'x'; // Mark the end point
    printGrid();
    cout << "Path found!" << endl;

    return 0;
}
