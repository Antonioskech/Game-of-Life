#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int MAX_ROWS = 80;
const int MAX_COLS = 24;
const int MAX_COORDS = 80;
bool clearBoardCalled = false;
bool usedCoordinates[MAX_COORDS][MAX_COORDS] = { false };

char savedBoard[MAX_COLS][MAX_ROWS];
int savedM, savedN;
string savedFileName = "saved_game.txt";

void writeState(char board[MAX_ROWS][MAX_COLS], int m, int n, std::ostream& output);

void newGame(char board[MAX_ROWS][MAX_COLS], int& m, int& n) {
    // Default dimensions
    m = 8;
    n = 16;

    // Initialize the game board with empty cells
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = '-';
        }
    }

    // Display the current dimensions
    cout << "New game started with board size: " << n << "x" << m << endl;

    // Display the initial state of the board
    cout << "Initial state of the board:\n";
    writeState(board, m, n, cout);

    // Clear the used coordinates
    for (int i = 0; i < MAX_COORDS; ++i) {
        for (int j = 0; j < MAX_COORDS; ++j) {
            usedCoordinates[i][j] = false;
        }
    }
}

void readInitialState(char board[MAX_ROWS][MAX_COLS], int m, int n, ifstream& input) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> board[i][j];
        }
    }
}

void writeState(char board[MAX_ROWS][MAX_COLS], int m, int n, std::ostream& output) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            output << board[i][j];
        }
        output << std::endl;
    }
}

int countLiveNeighbors(char board[MAX_ROWS][MAX_COLS], int row, int col, int m, int n) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            if (i >= 0 && i < m && j >= 0 && j < n && !(i == row && j == col) && board[i][j] == '@') {
                count++;
            }
        }
    }
    return count;
}

void updateState(char board[MAX_ROWS][MAX_COLS], int& m, int& n) {
    char newBoard[MAX_ROWS][MAX_COLS];
    bool expandLeft = false, expandRight = false, expandTop = false, expandBottom = false;

    // Check if expansion is needed for each edge of the board
    for (int i = 0; i < m; ++i) {
        if (board[i][0] == '@') expandLeft = true;
        if (board[i][n - 1] == '@') expandRight = true;
    }
    for (int j = 0; j < n; ++j) {
        if (board[0][j] == '@') expandTop = true;
        if (board[m - 1][j] == '@') expandBottom = true;
    }

    // Update the board dimensions based on the expansion needs
    int newM = m + expandTop + expandBottom;
    int newN = n + expandLeft + expandRight;

    // Check if the new dimensions exceed the maximum limits
    if (newM > MAX_ROWS) newM = MAX_ROWS;
    if (newN > MAX_COLS) newN = MAX_COLS;

    // Initialize the new board with empty cells
    for (int i = 0; i < newM; ++i) {
        for (int j = 0; j < newN; ++j) {
            newBoard[i][j] = '-';
        }
    }

    // Copy the old board to the new one with appropriate displacement
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            newBoard[i + expandTop][j + expandLeft] = board[i][j];
        }
    }

    // Update the state of cells in the new board
    for (int i = 0; i < newM; ++i) {
        for (int j = 0; j < newN; ++j) {
            int liveNeighbors = countLiveNeighbors(newBoard, i, j, newM, newN);
            if (newBoard[i][j] == '@') {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    newBoard[i][j] = '-';
                }
            }
            else {
                if (liveNeighbors == 3) {
                    newBoard[i][j] = '@';
                }
            }
        }
    }

    // Copy the new state back to the original board
    m = newM;
    n = newN;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = newBoard[i][j];
        }
    }
}

void resizeBoard(char board[MAX_ROWS][MAX_COLS], int& m, int& n) {
    // Display the current dimensions
    cout << "Current board size: " << n << "x" << m << endl;

    int newX, newY;
    cout << "Enter new width (X): ";
    cin >> newY;
    cout << "Enter new height (Y): ";
    cin >> newX;

    if (newX <= 0 || newY <= 0) {
        cout << "Invalid dimensions. Please enter positive values." << endl;
        return;
    }

    // Create a temporary board with the new dimensions
    char newBoard[MAX_ROWS][MAX_COLS];
    for (int i = 0; i < min(m, newX); ++i) {
        for (int j = 0; j < min(n, newY); ++j) {
            newBoard[i][j] = board[i][j];
        }
    }

    // Update the board dimensions
    m = newX;
    n = newY;

    // Copy the new state back to the original board
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = newBoard[i][j];
        }
    }

    // Display the new board size
    cout << "New board size: " << n << "x" << m << endl;
    writeState(board, m, n, cout);
}

bool isCoordinateUsed(int x, int y) {
    return usedCoordinates[x][y];
}

void markCoordinateUsed(int x, int y) {
    usedCoordinates[x][y] = true;
}

void stepForward(char board[MAX_ROWS][MAX_COLS], int m, int n) {
    // Check if clearBoard has been called
    if (clearBoardCalled) {
        // Reset the flag
        clearBoardCalled = false;

        // Reset all used coordinates
        for (int i = 0; i < MAX_COORDS; ++i) {
            for (int j = 0; j < MAX_COORDS; ++j) {
                usedCoordinates[i][j] = false;
            }
        }

        cout << "Coordinates have been cleared!" << endl;
    }
    int x, y;

    do {
        // Ask for the X coordinate
        cout << "Enter row (X) coordinate to move: ";
        cin >> x;

        // Check for the validity of the X coordinate
        if (x < 0 || x >= m) {
            cout << "Invalid row coordinate. Please try again." << endl;
            continue;
        }

        // Ask for the Y coordinate
        cout << "Enter column (Y) coordinate to move: ";
        cin >> y;

        // Check for the validity of the Y coordinate
        if (y < 0 || y >= n) {
            cout << "Invalid column coordinate. Please try again." << endl;
            continue;
        }

        // Check if the coordinates have already been used
        if (usedCoordinates[x - 1][y - 1]) {
            cout << "These coordinates have already been used. Please choose different ones." << endl;
        }
        else {
            // Mark the coordinates as used
            usedCoordinates[x - 1][y - 1] = true;
            break;  // Exit the loop if the coordinates are valid and unused
        }
    } while (true);

    // Toggle the state of the cell
    board[x - 1][y - 1] = (board[x - 1][y - 1] == '@') ? '-' : '@';

    // Display the updated board and marked coordinates
    cout << "Current coordinates marked with +:" << endl << endl;

    // Display the board with "+" marks for the entered coordinates
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == x - 1 && j == y - 1) {
                cout << "+";
            }
            else {
                cout << board[i][j];
            }
        }
        cout << endl;
    }
}

void toggleCell(char board[MAX_ROWS][MAX_COLS], int m, int n) {
    int x, y;

    cout << "Enter row (x) coordinate to toggle cell state: ";
    cin >> x;

    // Validation for x
    if (x <= 0 || x > m) {
        cout << "Invalid row coordinate. Please try again." << endl;
        return;
    }

    cout << "Enter column (y) coordinate to toggle cell state: ";
    cin >> y;

    // Validation for y
    if (y <= 0 || y > n) {
        cout << "Invalid column coordinate. Please try again." << endl;
        return;
    }

    // changing the state of the cell
    board[x - 1][y - 1] = (board[x - 1][y - 1] == '@') ? '-' : '@';

    // Showing the updated board
    cout << "Updated board state:\n";
    writeState(board, m, n, cout);
}

void clearBoard(char board[MAX_ROWS][MAX_COLS], int& m, int& n, int& prevM, int& prevN) {
    // Store the previous dimensions
    prevM = m;
    prevN = n;

    // Set all cells to empty
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            board[i][j] = '-';
        }
    }

    // Update the flag
    cout << "Cleared board state:\n";
    writeState(board, m, n, cout);

    // Update the flag
    clearBoardCalled = true;
}

void randomizeBoard(char board[MAX_ROWS][MAX_COLS], int& m, int& n) {
    int shuffleLevel;
    cout << "Choose shuffle level:\n1 - Weakly shuffled\n2 - Moderately shuffled\n3 - Highly shuffled\nEnter your choice: ";
    cin >> shuffleLevel;

    // Validation for the level
    if (shuffleLevel < 1 || shuffleLevel > 3) {
        cout << "Invalid shuffle level. Using default (moderately shuffled)." << endl;
        shuffleLevel = 2;
    }
    
    srand(time(0));

    // If m and n are not initialised, set default values
    if (m <= 0 || n <= 0) {
        m = 8;
        n = 16;
    }

    //Randomize
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            // Probabilities
            int randNum = rand() % 100;  // Generating numbers from 0 to 99
            int probability;

            //Depending on the shuffleLevel, create different probabilities for alive cell
            switch (shuffleLevel) {
            case 1:  // weakly shuffled
                probability = 20;
                break;
            case 2:  // default shuffle
                probability = 50;
                break;
            case 3:  // highly shuffled
                probability = 80;
                break;
            default:
                probability = 50; 
            }

            //Cell status depending on the probability
            board[i][j] = (randNum < probability) ? '@' : '-';
        }
    }

    // Show result after randomizing
    cout << "Randomized board state:\n";
    writeState(board, m, n, cout);
}

void saveToFile(char board[MAX_ROWS][MAX_COLS], int m, int n) {
    string fileName;
    cout << "Enter the filename to save the current state: ";
    cin >> fileName;

    ofstream outputFile(fileName);
    if (outputFile.is_open()) {
        // Save the size of the board
        outputFile << m << " " << n << endl;

        // Save the current state of the board
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                outputFile << board[i][j];
            }
            outputFile << endl;
        }

        // Close the file
        outputFile.close();

        // Save the current state and dimensions for the last saved game
        savedM = m;
        savedN = n;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                savedBoard[i][j] = board[i][j];
            }
        }
        savedFileName = fileName;

        cout << "Board state saved to file." << endl;
    }
    else {
        cout << "Error opening file for writing." << endl;
    }
}

void loadFile(char board[MAX_ROWS][MAX_COLS], int& m, int& n) {
    string fileName;

    cout << "Enter the filename: ";
    cin >> fileName;

    ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        // Read the size of the loaded board
        inputFile >> m >> n;

        // Read the initial state
        readInitialState(board, m, n, inputFile);

        // Close the file
        inputFile.close();

        // Display the loaded board state
        cout << "Loaded board state from file:\n";
        writeState(board, m, n, cout);

        // Start a new game with the loaded data
        cout << "Starting a new game with loaded data.\n";
    }
    else {
        cout << "Error opening file." << endl;
    }
}

int main() {
    int m, n, generations;
    char board[MAX_ROWS][MAX_COLS];

    int choice;
    do {
        cout << "\nMenu:\n1. New game\n2. Load file\n3. Step forward\n4. Resize\n5. Toggle cell\n6. Clear board\n7. Randomize\n8. Save to file\n9. End\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            newGame(board, m, n);
            break;
        case 2:
            loadFile(board, m, n);
            break;
        case 3:
            stepForward(board, m, n);
            break;
        case 4:
            resizeBoard(board, m, n);
            break;
        case 5:
            toggleCell(board, m, n);
            break;
        case 6:
            int prevM, prevN;
            clearBoard(board, m, n, prevM, prevN);
            break;
        case 7:
            randomizeBoard(board, m, n);
            break;
        case 8:
            saveToFile(board, m, n);
            break;
        case 9:
            cout << "Ending the game. Returning to the initial menu." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 9);

    return 0;
}

