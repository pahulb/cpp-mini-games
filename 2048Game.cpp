#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h> // for _getch()
using namespace std;

const int SIZE = 4;
int board[SIZE][SIZE] = {0};

// Forward declarations
void drawBoard();
bool moveLeft();
bool moveRight();
bool moveUp();
bool moveDown();
void addTile();
bool canMove();
bool compress(int row[]);
bool merge(int row[]);

int main() {
    srand(time(0));
    addTile();
    addTile();

    while (true) {
        system("cls"); // clear screen
        drawBoard();

        if (!canMove()) {
            cout << "\nGame Over! No more moves.\n";
            break;
        }

        cout << "\nUse W/A/S/D to move (↑ ← ↓ →): ";
        char move = _getch();

        bool moved = false;
        switch (move) {
            case 'a': case 'A': moved = moveLeft(); break;
            case 'd': case 'D': moved = moveRight(); break;
            case 'w': case 'W': moved = moveUp(); break;
            case 's': case 'S': moved = moveDown(); break;
            default: continue; // ignore invalid keys
        }

        if (moved)
            addTile(); // only add a new tile if player moved
    }

    return 0;
}

void drawBoard() {
    cout << "-----------------------------\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                cout << setw(6) << ".";
            else
                cout << setw(6) << board[i][j];
        }
        cout << "\n";
    }
    cout << "-----------------------------\n";
}

void addTile() {
    int emptyCount = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                emptyCount++;

    if (emptyCount == 0) return;

    int pos = rand() % emptyCount;
    int value = (rand() % 10 < 9) ? 2 : 4;

    emptyCount = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                if (emptyCount == pos) {
                    board[i][j] = value;
                    return;
                }
                emptyCount++;
            }
        }
    }
}

// Compress row to the left (remove zeros)
bool compress(int row[]) {
    int temp[SIZE] = {0};
    int idx = 0;
    bool changed = false;

    for (int i = 0; i < SIZE; i++) {
        if (row[i] != 0) {
            temp[idx++] = row[i];
        }
    }

    for (int i = 0; i < SIZE; i++) {
        if (row[i] != temp[i]) {
            changed = true;
            row[i] = temp[i];
        }
    }
    return changed;
}

// Merge tiles in row (left direction)
bool merge(int row[]) {
    bool changed = false;
    for (int i = 0; i < SIZE - 1; i++) {
        if (row[i] != 0 && row[i] == row[i + 1]) {
            row[i] *= 2;
            row[i + 1] = 0;
            changed = true;
        }
    }
    return changed;
}

bool moveLeft() {
    bool changed = false;
    for (int i = 0; i < SIZE; i++) {
        bool c1 = compress(board[i]);
        bool m = merge(board[i]);
        bool c2 = compress(board[i]);
        if (c1 || m || c2)
            changed = true;
    }
    return changed;
}

bool moveRight() {
    bool changed = false;
    for (int i = 0; i < SIZE; i++) {
        int row[SIZE];
        for (int j = 0; j < SIZE; j++) row[j] = board[i][SIZE - 1 - j];

        bool c1 = compress(row);
        bool m = merge(row);
        bool c2 = compress(row);

        for (int j = 0; j < SIZE; j++) board[i][SIZE - 1 - j] = row[j];

        if (c1 || m || c2)
            changed = true;
    }
    return changed;
}

bool moveUp() {
    bool changed = false;
    for (int j = 0; j < SIZE; j++) {
        int col[SIZE];
        for (int i = 0; i < SIZE; i++) col[i] = board[i][j];

        bool c1 = compress(col);
        bool m = merge(col);
        bool c2 = compress(col);

        for (int i = 0; i < SIZE; i++) board[i][j] = col[i];

        if (c1 || m || c2)
            changed = true;
    }
    return changed;
}

bool moveDown() {
    bool changed = false;
    for (int j = 0; j < SIZE; j++) {
        int col[SIZE];
        for (int i = 0; i < SIZE; i++) col[i] = board[SIZE - 1 - i][j];

        bool c1 = compress(col);
        bool m = merge(col);
        bool c2 = compress(col);

        for (int i = 0; i < SIZE; i++) board[SIZE - 1 - i][j] = col[i];

        if (c1 || m || c2)
            changed = true;
    }
    return changed;
}

bool canMove() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                return true;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE - 1; j++)
            if (board[i][j] == board[i][j + 1] || board[j][i] == board[j + 1][i])
                return true;

    return false;
}
