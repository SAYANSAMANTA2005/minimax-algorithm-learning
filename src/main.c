#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define SIZE 3
#define EMPTY 0
#define PLAYER_X 1 // Maximizing player
#define PLAYER_O 2 // Minimizing player

int board[SIZE][SIZE] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY}
};

void print_board() {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == PLAYER_X) printf("X ");
            else if (board[r][c] == PLAYER_O) printf("O ");
            else printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

int evaluate_board() {
    // Rows and columns
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == PLAYER_X) return +10;
            else if (board[i][0] == PLAYER_O) return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == PLAYER_X) return +10;
            else if (board[0][i] == PLAYER_O) return -10;
        }
    }

    // Diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return +10;
        else if (board[0][0] == PLAYER_O) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return +10;
        else if (board[0][2] == PLAYER_O) return -10;
    }

    return 0; // No one wins
}

bool is_moves_left() {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (board[r][c] == EMPTY)
                return true;
    return false;
}

int minimax(int depth, bool isMaximizingPlayer) {
    int score = evaluate_board();

    // If Maximizing player has won
    if (score == 10) return score - depth;

    // If Minimizing player has won
    if (score == -10) return score + depth;

    // If no moves left
    if (!is_moves_left()) return 0;

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] == EMPTY) {
                    board[r][c] = PLAYER_X;
                    int eval = minimax(depth + 1, false);
                    board[r][c] = EMPTY;
                    maxEval = (eval > maxEval) ? eval : maxEval;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] == EMPTY) {
                    board[r][c] = PLAYER_O;
                    int eval = minimax(depth + 1, true);
                    board[r][c] = EMPTY;
                    minEval = (eval < minEval) ? eval : minEval;
                }
            }
        }
        return minEval;
    }
}

typedef struct {
    int row, col;
} Move;

Move find_best_move() {
    int bestVal = INT_MIN;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == EMPTY) {
                board[r][c] = PLAYER_X;
                int moveVal = minimax(0, false);
                board[r][c] = EMPTY;
                if (moveVal > bestVal) {
                    bestMove.row = r;
                    bestMove.col = c;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int main() {
    print_board();
    while (is_moves_left() && evaluate_board() == 0) {
        // Player X (AI) move
        Move bestMove = find_best_move();
        board[bestMove.row][bestMove.col] = PLAYER_X;
        printf("AI move: %d %d\n", bestMove.row, bestMove.col);
        print_board();
        
        if (!is_moves_left() || evaluate_board() != 0) break;

        // Player O (Human) move
        int row, col;
        printf("Enter your move (row and column): ");
        scanf("%d %d", &row, &col);
        if (board[row][col] == EMPTY) {
            board[row][col] = PLAYER_O;
        } else {
            printf("Invalid move, try again.\n");
            continue;
        }
        print_board();
    }

    int score = evaluate_board();
    if (score == 10)
        printf("AI wins!\n");
    else if (score == -10)
        printf("Human wins!\n");
    else
        printf("Draw!\n");

    return 0;
}