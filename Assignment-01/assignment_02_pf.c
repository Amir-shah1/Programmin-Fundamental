// === Include necessary header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// === Define game board dimensions ===
#define ROWS 6
#define COLUMNS 6
#define TOTAL_CARDS (ROWS * COLUMNS)
#define MAX_SCORE ((TOTAL_CARDS / 2) + 1) // +1 for the extra star card

// === Global variables for game state ===
int player1_score = 0;
int player2_score = 0;

// === Function to validate coordinates ===
bool validate_coordinates(char col, int row)
{
    return (col >= 'A' && col <= 'F') && (row >= 0 && row < ROWS);
}

// === Function to display the game board ===
void display_board(char board_value[ROWS][COLUMNS], char revealed_col1, int revealed_row1,
                   char revealed_col2, int revealed_row2)
{
    printf("\n     A   B   C   D   E   F\n");
    printf("   +---+---+---+---+---+---+\n");

    for (int i = 0; i < ROWS; i++)
    {
        printf("%2d |", i);
        for (int j = 0; j < COLUMNS; j++)
        {
            if ((i == revealed_row1 && j == (revealed_col1 - 'A')) ||
                (i == revealed_row2 && j == (revealed_col2 - 'A')))
            {
                printf(" %c |", board_value[i][j]);
            }
            else if (board_value[i][j] == ' ')
            {
                printf("   |");
            }
            else
            {
                printf(" # |");
            }
        }
        printf("\n   +---+---+---+---+---+---+\n");
    }
}

// === Function to display scores ===
void display_scores()
{
    printf("\n   Player 1 score: %d\n", player1_score);
    printf("   Player 2 score: %d\n\n", player2_score);
}

// === Function to handle player's turn ===
void handle_turn(int *score, bool *continue_turn, char board_value[ROWS][COLUMNS], int player_number)
{
    char col1, col2;
    int row1, row2;
    bool valid_input = false;

    // === Get and validate first coordinate ===
    do
    {
        printf("   Player %d turn\n", player_number);
        printf("   Enter first coordinate (e.g. B2): ");
        if (scanf(" %c%d", &col1, &row1) != 2)
        {
            printf("   Invalid input! Please enter a letter followed by a number (e.g. B2)\n");
            while (getchar() != '\n')
                ;
            // Clear input buffer
            continue;
        }

        col1 = toupper(col1);

        if (!validate_coordinates(col1, row1))
        {
            printf("   Invalid coordinates! Please enter a letter A-F and number 0-5\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (board_value[row1][col1 - 'A'] == ' ')
        {
            printf("   This cell is already empty! Please choose another\n");
            continue;
        }

        valid_input = true;
    } while (!valid_input);

    // === Get and validate second coordinate ===
    valid_input = false;
    do
    {
        printf("   Enter second coordinate (e.g. B2): ");
        if (scanf(" %c%d", &col2, &row2) != 2)
        {
            printf("   Invalid input! Please enter a letter followed by a number (e.g. B2)\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        col2 = toupper(col2);

        if (!validate_coordinates(col2, row2))
        {
            printf("   Invalid coordinates! Please enter a letter A-F and number 0-5\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (board_value[row2][col2 - 'A'] == ' ')
        {
            printf("   This cell is already empty! Please choose another\n");
            continue;
        }

        if (row1 == row2 && col1 == col2)
        {
            printf("   Cannot select the same cell twice! Please choose different coordinates\n");
            continue;
        }

        valid_input = true;
    } while (!valid_input);

    // === Convert column letters to array indices and get selected card values ===
    int col1_num = (col1 - 'A');
    int col2_num = (col2 - 'A');
    char first_value = board_value[row1][col1_num];
    char second_value = board_value[row2][col2_num];

    // === Display current game state ===
    display_scores();
    display_board(board_value, col1, row1, col2, row2);

    // === Check for matches and update scores ===
    *continue_turn = false;

    // Add delay to show cards
    printf("\nRevealed cards: %c and %c\n", first_value, second_value);
    printf("Press Enter to continue...");
    while (getchar() != '\n')
        ;
    getchar();

    if ((first_value == '*') || (second_value == '*'))
    {
        board_value[row1][col1_num] = ' ';
        board_value[row2][col2_num] = ' ';
        *score += 2;
        *continue_turn = true;
        printf("   Found a star! Extra turn granted!\n");
    }
    else if ((toupper(first_value) == toupper(second_value)) &&
             (first_value != ' ' && second_value != ' '))
    {
        board_value[row1][col1_num] = ' ';
        board_value[row2][col2_num] = ' ';
        (*score)++;
        *continue_turn = true;
        printf("   Match found! Extra turn granted!\n");
    }
    else
    {
        printf("   No match! Next player's turn.\n");
    }

    // === Check for end game condition and handle remaining star cards ===
    if (player1_score + player2_score >= MAX_SCORE - 2)
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (board_value[i][j] == '*')
                {
                    (*score)++;
                    board_value[i][j] = ' ';
                }
            }
        }
    }
}

// === Function to initialize the game board ===
void initialize_board(char board_value[ROWS][COLUMNS])
{
    char all_board_value[TOTAL_CARDS] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', '*', '*', 'a', 'b', 'c', 'd', 'e',
        'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q'};

    // === Randomly distribute cards on the board ===
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            bool put_value = false;
            while (!put_value)
            {
                int random_index = rand() % TOTAL_CARDS;
                if (all_board_value[random_index] != '0')
                {
                    board_value[i][j] = all_board_value[random_index];
                    all_board_value[random_index] = '0';
                    put_value = true;
                }
            }
        }
    }
}

// === Main function ===
int main(void)
{
    // === Initialize random number generator ===
    srand(time(NULL));

    // === Initialize game board ===
    char board_value[ROWS][COLUMNS] = {0};
    initialize_board(board_value);

    // === Display initial game state ===
    printf("\n=== Memory Game ===\n");
    printf("Find matching pairs of letters. Stars (*) are special cards worth 2 points!\n\n");
    display_scores();
    display_board(board_value, 'X', -1, 'X', -1); // No revealed cards initially

    // === Main game loop ===
    bool player1_turn = true;
    while (player1_score + player2_score < MAX_SCORE)
    {
        if (player1_turn)
        {
            handle_turn(&player1_score, &player1_turn, board_value, 1);
        }
        else
        {
            handle_turn(&player2_score, &player1_turn, board_value, 2);
        }

        if (!player1_turn)
        {
            printf("\nPlayer 2's turn\n");
        }
        else
        {
            printf("\nPlayer 1's turn\n");
        }
    }

    // === Display final results ===
    printf("\n=== Game Over! ===\n");
    printf("Final scores:\n");
    printf("   Player 1: %d\n", player1_score);
    printf("   Player 2: %d\n\n", player2_score);

    if (player1_score > player2_score)
    {
        printf("Player 1 wins!\n");
    }
    else
    {
        printf("Player 2 wins!\n");
    }

    return 0;
}