#include <iostream>
#include <queue>
#include <vector>
#include <thread>  
#include <chrono>  
#include <algorithm>

using namespace std;

const int ROWS = 21;
const int COLUMNS = 41;
char GRID[ROWS][COLUMNS];

struct Point 
{
    int x, y;
};

void printGrid() 
{
    system("clear"); // Use "cls" for Windows
    for (int i=1; i<ROWS; i++) 
    {
        for (int j=1; j<COLUMNS; j++) 
        {
            cout << GRID[i][j];
        }
        cout<<endl;
    }
}

bool isValid(int x, int y) 
{
    return x >0 && x<ROWS && y>0 && y<COLUMNS;
}

const vector<Point> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};

vector<Point> bfs(Point start, Point end) 
{
    vector<vector<bool>> visited(ROWS, vector<bool>(COLUMNS, false));
    vector<vector<Point>> parent(ROWS, vector<Point>(COLUMNS, {-1, -1}));

    queue<Point> q;
    q.push(start);
    visited[start.x][start.y] = true;

    while (!q.empty()) 
    {
        Point current = q.front(); 
        q.pop();
        if (current.x == end.x && current.y == end.y) break;

        for (const auto& dir : directions) 
        {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (isValid(nx, ny) && !visited[nx][ny]) 
            {
                visited[nx][ny] = true;
                parent[nx][ny] = current;
                q.push({nx, ny});
            }
        }
    }

    vector<Point> path;
    Point current = end;
    while (!(current.x == start.x && current.y == start.y)) 
    {
        path.push_back(current);
        current = parent[current.x][current.y];
        if (current.x == -1 && current.y == -1) {return {};} // No path found
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() 
{
    // Initialize grid
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            GRID[i][j] = '-';
        }
    }

    int xa, ya, xb, yb;
    cout << "Enter A coordinates : ";
    cin >> xa >> ya;
    cout << "Enter B coordinates : ";
    cin >> xb >> yb;

    //xa--; ya--; xb--; yb--; // convert to 0 index

    if (!isValid(xa, ya) || !isValid(xb, yb) || (xa == xb && ya == yb)) {
        cout << "Invalid coordinates." << endl;
        return 1;
    }

    GRID[xa][ya] = 'A';
    GRID[xb][yb] = 'B';

    vector<Point> path = bfs({xa, ya}, {xb, yb});

    if (path.empty()) {
        cout << "No path found." << endl;
        return 1;
    }

    printGrid();

    for (const auto& p : path) 
    {
        if(GRID[p.x][p.y] != 'B') 
        {
            GRID[p.x][p.y] = 'x';
        }
        printGrid();
        GRID[p.x][p.y] = '-';
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    GRID[xb][yb] = 'x'; // Mark the end point
    printGrid();
    cout << "Path found!" << endl;

    return 0;
}
