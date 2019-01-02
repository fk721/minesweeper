/* TO RUN:
g++ minesweeper.cpp -o game.exe
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

/* GlOBALS */
const int ROWS(10);
const int COLS(10);
const int BOMBS(10);

struct Tile {
  Tile(): isVisible(false), isBomb(false), surroundingBombs(0) {}
  bool isVisible;
  bool isBomb;
  int surroundingBombs;
};

class Minesweeper {
public:
  Minesweeper(): tilesPlayed(0) {
    for (size_t i = 0; i < ROWS; ++i) {
      vector<Tile> currTile;
      for (size_t j = 0; j < COLS; ++j) { currTile.push_back(Tile()); }
      board.push_back(currTile);
    }
    srand(time(NULL));
    int count(0);
    while (count < BOMBS) {
  		int x = rand() % ROWS;
  		int y = rand() % COLS;
      if (!board[x][y].isBomb) {
        board[x][y].isBomb = true;
        board[x][y].surroundingBombs = -1;
        count += 1;
      }
    }
    for (size_t row = 0; row < ROWS; ++ row) {
      for (size_t col = 0; col < COLS; ++ col) {
        if (!board[row][col].isBomb) {
          for (int h = -1; h <= 1; ++h) {
            for (int v = -1; v <= 1; ++v) {
              if (validRow(row + h) && validCol(col+v)) {
                if (board[row+h][col+v].isBomb) {
                  board[row][col].surroundingBombs +=1;
                }
              }
            }
          }
        }
      }
    }
  }

  bool validRow(int row) const {return (row >=0 && row < ROWS); }
  bool validCol(int col) const {return (col >=0 && col < COLS); }
  bool done() const { return (tilesPlayed + BOMBS == 100);}
  bool isVisible(int row, int col) const {return board[row][col].isVisible;}

  void display(bool b) const {
    if (b) {
      for (size_t row = 0; row < ROWS; ++row) {
        for (size_t col = 0; col < COLS; ++col) {
          if (board[row][col].isBomb) {cout << "B "; }
          else {cout << board[row][col].surroundingBombs << " "; }
        }
        cout << "\n";
      }
    }
    else {
      for (size_t row = 0; row < ROWS; ++row) {
        for (size_t col = 0; col < COLS; ++col) {
          if (!board[row][col].isVisible) {cout << "X "; }
          else {cout << board[row][col].surroundingBombs << " "; }
        }
        cout << "\n";
      }
    }
    cout << "TILES PLAYED: " << tilesPlayed << endl;
  }

  bool play(int row, int col) {
    if (board[row][col].isBomb) {return false;}
    else if (board[row][col].surroundingBombs != 0) {
      board[row][col].isVisible = true;
      tilesPlayed +=1;
      return true;
    }
    vector<vector<int>> suitors;
    suitors.push_back({row,col});
    while (suitors.size() > 0) {
      int row = suitors[suitors.size() - 1][0];
      int col = suitors[suitors.size() - 1][1];
      if (!board[row][col].isVisible) {
        board[row][col].isVisible = true;
        tilesPlayed +=1;
      }
      suitors.pop_back();
      for (int h = -1; h <= 1; ++h) {
        for (int v = -1; v <= 1; ++v) {
          if (validRow(row+h) && validCol(col+v)) {
            if (!board[row+h][col+v].isBomb && !board[row+h][col+v].isVisible) {
              if (board[row+h][col+v].surroundingBombs != 0) {
                board[row+h][col+v].isVisible = true;
                tilesPlayed +=1;
              }
              else {
                suitors.push_back({row+h,col+v});
              }
            }
          }
        }
      }
    }
    return true;
  }
private:
  vector<vector<Tile>> board;
  int tilesPlayed;
};

int main() {
    Minesweeper sweeper;
    //sweeper.display(true);
    cout << '\n';
    while (!sweeper.done()) {
        sweeper.display(false);
        int row_sel = -1, col_sel = -1;
        while (row_sel == -1) {
            int r, c;
            cout << "row? ";
            cin >> r;
            r = r - 1;
            if (!sweeper.validRow(r)) {
                cout << "Row out of bounds\n";
                continue;
            }
            cout << "col? ";
            cin >> c;
            c = c - 1;
            if (!sweeper.validCol(c)) {
                cout << "Column out of bounds\n";
                continue;
            }
            if (sweeper.isVisible(r,c)) {
                cout << "Square already visible\n";
                continue;
            }
            row_sel = r;
            col_sel = c;
        }
        if (!sweeper.play(row_sel,col_sel)) {
            cout << "Sorry, you died..\n";
            sweeper.display(true);
            exit(0);
        }
    }
    cout << "You won!!!!\n";
    sweeper.display(true);
}
