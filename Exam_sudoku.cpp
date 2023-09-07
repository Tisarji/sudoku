#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#define N 9

struct SudokuBoard {
    int board[N][N];
};

vector<SudokuBoard> scoreHistory;

int sudokuboard[N][N] =
{
    {0, 0, 6, 5, 0, 8, 4, 0, 0},
    {5, 2, 0, 0, 0, 1, 0, 0, 0},
    {0, 7, 8, 0, 0, 0, 0, 0, 1},
    {0, 0, 4, 0, 1, 0, 0, 8, 0},
    {9, 0, 0, 8, 2, 3, 0, 0, 5},
    {0, 5, 0, 0, 9, 0, 6, 0, 0},
    {1, 3, 0, 0, 0, 0, 2, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 7, 4},
    {0, 0, 5, 2, 0, 6, 3, 0, 0}
};

void initializeBoard(SudokuBoard &board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board.board[i][j] = sudokuboard[i][j];
        }
    }
}

bool checkColumn(int c, int value) {
    for (int r = 0; r < N; r++) {
        if (sudokuboard[r][c] == value)
            return true;
    }
    return false;
}

bool checkRow(int r, int value) {
    for (int c = 0; c < N; c++) {
        if (sudokuboard[r][c] == value)
            return true;
    }
    return false;
}

bool checkBox(int initial_row, int initial_column, int value) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (sudokuboard[r + initial_row][c + initial_column] == value)
                return true;
        }
    }
    return false;
}

void showGame(const SudokuBoard &board, const vector<string> &moves) {
    cout << "\033[2J\033[1;1H";  // Clear the screen
    cout << "\033[1;36m --!* Sudoku Board *!--\033[0m" << endl;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (c == 3 || c == 6)
                cout << " | ";
            if (board.board[r][c] == 0)
                cout << "\033[1;33m" << "0" << "\033[0m" << " ";
            else
                cout << "\033[1;32m" << board.board[r][c] << "\033[0m" << " ";
        }

        if (r < moves.size()) {
            cout << "  |  " << moves[r];
        }

        cout << endl;

        if (r == 2 || r == 5)
            cout << "---------------------" << endl;
    }

    cout << "\033[1;35mAvailable commands: '\033[1;33msave\033[1;35m', '\033[1;33mundo\033[1;35m', '\033[1;33mhistory\033[1;35m'\033[0m" << endl;
}

bool findEmptyLocation(int &r, int &c, const SudokuBoard &board) {
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            if (board.board[r][c] == 0)
                return true;
        }
    }
    return false;
}

bool isValidMove(int r, int c, int value, const SudokuBoard &board) {
    return !checkRow(r, value) && !checkColumn(c, value) && !checkBox(r - r % 3, c - c % 3, value);
}

void displayHistory(const vector<string> &moves) {
    cout << "\n--- Move History ---\n";
    for (size_t i = 0; i < moves.size(); i++) {
        cout << "Move " << i + 1 << ":\n";
        cout << moves[i] << "\n\n";
    }
}

void writeHistoryToFile(const string &filename, const vector<string> &moves) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    outFile << "--- Move History ---\n";
    for (size_t i = 0; i < moves.size(); i++) {
        outFile << "Move " << i + 1 << ":\n";
        outFile << moves[i] << "\n\n";
    }

    outFile.close();
    cout << "Move history saved to " << filename << endl;
}

bool playSudoku(SudokuBoard &board, vector<string> &moves) {
    while (true) {
        int r, c;
        if (!findEmptyLocation(r, c, board))
            return true;

        showGame(board, moves);
        cout << "Enter your move (row column value) or 'undo' to undo your last move or 'history' to display history or 'save' to save history to a file: ";
        string input;
        cin >> input;

        if (input == "undo") {
            if (moves.empty()) {
                cout << "No moves to undo." << endl;
                continue;
            }

            string lastMove = moves.back();
            moves.pop_back();
            int prevR = lastMove[0] - '0';
            int prevC = lastMove[2] - '0';
            board.board[prevR][prevC] = 0;

            continue;
        } else if (input == "history") {
            displayHistory(moves);
            continue;
        } else if (input == "save") {
            cout << "Enter a filename to save the history: ";
            string filename;
            cin >> filename;
            writeHistoryToFile(filename, moves);
            continue;
        }

        int value = stoi(input);
        if (r < 0 || r >= N || c < 0 || c >= N || value < 1 || value > 9 || !isValidMove(r, c, value, board)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        SudokuBoard currentBoard;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                currentBoard.board[i][j] = board.board[i][j];
            }
        }
        scoreHistory.push_back(currentBoard);

        board.board[r][c] = value;
        moves.push_back(to_string(r) + " " + to_string(c) + " " + to_string(value));
    }
}

int main() 
{
    SudokuBoard currentBoard;
    initializeBoard(currentBoard);
    vector<string> moves;

    cout << "Welcome to Sudoku!" << endl;
    if (playSudoku(currentBoard, moves)) {
        cout << "Congratulations! You solved the Sudoku puzzle!" << endl;
        showGame(currentBoard, moves);
    } else {
        cout << "Sorry, the puzzle is not solvable." << endl;
    }
}