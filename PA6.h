/*
Name: Chase Lewis
Date: 10/22/19
Description: A game of Battleship
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// for a player, stores the number of hits, number of misses, total shots, hit percentage (hits divided by total shots), and a string containing "Won." or "Lost."
typedef struct player_stats
{
	int hit;
	int miss;
	int shots;
	double hit_percent;
	char game_outcome[8];
} Player_stats;

// displays menu and takes in user option
int menu_select(void);

// print game rules
void print_rules(void);

// actual game function
void play_game(void);

// set all game board spaces to '-'
void initialize_game_board(char board[][10]);

// calls user_place_ship function for each ship
void user_place_ships(char player1_board[][10]);

// prompts user to place a ship
void user_place_ship(char player1_board[][10], int ship_size, char ship_name[], char ship_letter);

// prompts user if they want to place ships manually or have it randomly done for them. input validation
char choose_manual_or_random(void);

// check if ship placed in accepted spot
char check_ship(char player1_board[][10], int ship_coordinates[], int ship_size);

// changes order of x values and y values to be in ascending order so that list is easier to work with. (ex: 1 6 1 2 becomes 1 2 1 6)
void re_order_coordinates(int ship_coordinates[]);

// prompts user for coordinates
void get_ship_coordinates(int ship_coordinates[], int ship_size, char ship_name[]);

// prints board
void print_board(char board[][10]);

// prints hidden version of board, only showing hits and misses. non-attacked cells appear as "-"
void print_hidden_board(char board[][10]);

// randomly returns 1 or 2 and tells user who is going first
int choose_starting_player(void);

// randomly places ships
void place_ships_randomly(char board[][10]);

// gets random coordinates for ship placement
void random_ship_coordinates(int ship_coordinates[], int ship_size);

// calls random coordinates function and calls functions to check that the coords are good
void random_ship(char board[][10], int ship_size, char ship_name[], char ship_letter);

// checks if any cells to place ship in have already been filled. returns 1 if placement is good, 0 if one of the cells is filled
int random_check_ship(char board[][10], int ship_coordinates[]);

// loop through player turns, keeps track of stats
char turns(char player1_board[][10], char player2_board[][10], int player_start, FILE* outfile);

// gets coordinates from user for attack
void player1_choose_cell(int coordinates[]);

// checks that coordinates are in board range
char check_coordinates_in_range(int coordinates[]);

// checks that the cell has not been targeted before
char check_new_cell(char board[][10], int coordinates[]);

// computer randomly chooses a cell to attack
void random_choose_cell(int coordinates[]);

// after the coords are confirmed to be good, the attack happens and board is changed based on hit/miss. hits and misses are kept track of through pointers.
// attacks and results are outputed to file
int launch_attack(char board[][10], int coordinates[], int* hit_count, int *miss_count, FILE* outfile, int player);

// checks if a ship changed from existing to not existing to see if ship was sunk this turn. also checks if no more ships exist for a player, so that game can end
char check_remaining_ships(char board[][10], char ships_alive[], FILE* outfile, int player);

// after the game, the stats from custom structure for each player are printed to log file
void print_player_stats(FILE* outfile, Player_stats player_ending_stats, int player);
