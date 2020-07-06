#include "PA6.h"

// displays menu and takes in user option
int menu_select(void)
{
	int option = 0;

	printf("Please enter an option:\n\n");
	printf("1. Print Game Rules\n");
	printf("2. Play Game\n");
	printf("3. Exit\n");

	scanf("%d", &option);

	return option;
}

// print game rules
void print_rules(void)
{
	printf("Battleship is a two player Navy game. The objective of the game is to sink all ships in your enemy's fleet.\n"
		"The Player to sink his/her enemy's fleet first wins. Both players' fleets consist of 5 ships that are hidden from the enemy.\n"
		"Each ship may be differentiated by its \"size\" (besides the Cruiser and Submarine) or number of cells it expands on the game board.\n"
		"The Carrier has 5 cells, Battleship has 4 cells, Cruiser has 3 cells, Submarine has 3 cells, and the Destroyer has 2 cells.\n"
		"You (Player 1) and the computer (Player 2) will take turns targeting cells to attack. You will be told if an attack hits its target or misses. You will also be told if a hit sinks a ship.\n"
		"After an attack, the attacked board will be updated with an \"m\" or an \"*\" so you can see all past attacks and their results at any time.\n\n");
}

// starts game, conducts game, prints outcome
void play_game(void)
{
	FILE* outfile = NULL;
	char player1_board[10][10], player2_board[10][10], manual = 0, player1_alive = 0, user_return_to_menu = 0;
	int player_start = 0;
	outfile = fopen("battleship.log", "w");

	initialize_game_board(player1_board);
	initialize_game_board(player2_board);

	print_hidden_board(player2_board);
	print_board(player1_board);

	// places user's board manually or randomly at user's discrtion
	manual = choose_manual_or_random();

	if (manual == 'y')
	{
		user_place_ships(player1_board);
	}
	else
	{
		place_ships_randomly(player1_board);
	}
	place_ships_randomly(player2_board);

	system("cls");
	print_hidden_board(player2_board);
	print_board(player1_board);

	player_start = choose_starting_player();

	// loop of turns function, where game is truly played
	player1_alive = turns(player1_board, player2_board, player_start, outfile);

	// tells user who won and lost
	if (player1_alive == 'y')
	{
		fclose(outfile);
		printf("You sunk all the computer's ships.\n\n"
			"\nYou win!\nCongrats!\n\n"
			"Enter any non-whitespace character to return to the menu:\n");
		scanf(" %c", &user_return_to_menu);
	}
	else
	{
		fclose(outfile);
		printf("The computer sunk all of your ships.\n\n"
			"\nThe computer wins.\nBummer...\n\n"
			"Enter any non-whitespace character to return to the menu:\n");
		scanf(" %c", &user_return_to_menu);
	}
	system("cls");
}

// set all game board spaces to '-'
void initialize_game_board(char board[][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = '-';
		}
	}
}

// calls user_place_ship function for each ship
void user_place_ships(char player1_board[][10])
{
	user_place_ship(player1_board, 5, "carrier", 'c');
	system("cls");
	print_board(player1_board);
	user_place_ship(player1_board, 4, "battleship", 'b');
	system("cls");
	print_board(player1_board);
	user_place_ship(player1_board, 3, "cruiser", 'r');
	system("cls");
	print_board(player1_board);
	user_place_ship(player1_board, 3, "submarine", 's');
	system("cls");
	print_board(player1_board);
	user_place_ship(player1_board, 2, "destroyer", 'd');
}

// prompts user to place a ship
void user_place_ship(char player1_board[][10], int ship_size, char ship_name[], char ship_letter)
{
	int ship_coordinates[4];
	char ship_good = '\0';

	do {
		// prompts for coordinates
		get_ship_coordinates(ship_coordinates, ship_size, ship_name);

		// changes order so that x values are in ascending order and y values are in ascending order
		re_order_coordinates(ship_coordinates);

		// calls function to check that ship is in correct range, is in correct number of cells, is not placed on another ship, and is not diagonoal
		ship_good = check_ship(player1_board, ship_coordinates, ship_size);

		// loops if any problem with ship
	} while (ship_good == 'n');

	// after checking that ship is good, actually changes the two dimensional list with corresponding ship letter now placed in
	for (int i = ship_coordinates[0]; i < ship_coordinates[2] + 1; i++)
	{
		for (int j = ship_coordinates[1]; j < ship_coordinates[3] + 1; j++)
		{
			player1_board[i][j] = ship_letter;
		}
	}
}

// prompts user if they want to place ships manually or have it randomly done for them. input validation
char choose_manual_or_random(void)
{
	char manual = 0;
	printf("Would you like to manually choose your ship positions? If not, they are placed randomly.\n"
		"Please enter 'y' for 'yes' or 'n' for 'no': \n");

	do {
		scanf(" %c", &manual);
		if (manual != 'y' && manual != 'n')
		{
			printf("\nThat is not an accepted input. Please enter 'y' or 'n': \n");
		}
	} while (manual != 'y' && manual != 'n');

	return manual;
}

// check if ship placed in accepted spot
char check_ship(char player1_board[][10], int ship_coordinates[], int ship_size)
{
	char ship_good = 'y';

	// checks coordinates chosen fit with ship size
	if ((ship_coordinates[2] - ship_coordinates[0] != ship_size - 1 && ship_coordinates[3] - ship_coordinates[1] != ship_size - 1) ||
		(ship_coordinates[2] - ship_coordinates[0] != 0 && ship_coordinates[3] - ship_coordinates[1] != 0))
	{
		printf("\nThe length of the line between these coordinates is incorrect or the line is diagonal.\n");
		return 'n';
	}

	// checks if corrdinates in grid range and not occupied by ship already
	for (int i = ship_coordinates[0]; i < ship_coordinates[2] + 1; i++)
	{
		if (i > 9 || i < 0)
		{
			printf("\nOne or more of the entered coordinates is outside of the grid range.\n");
			return 'n';
		}

		for (int j = ship_coordinates[1]; j < ship_coordinates[3] + 1; j++)
		{
			if (j > 9 || i < 0)
			{
				printf("\nOne or more of the entered coordinates is outside of the grid range.\n");
				return 'n';
			}
			if (player1_board[i][j] != '-')
			{
				printf("\nThe ship overlaps with a previously placed ship.\n");
				return 'n';
			}
		}
	}
	// returns 'y' if everything is good
	return 'y';
}

// changes order of x values and y values to be in ascending order so that list is easier to work with. (ex: 1 6 1 2 becomes 1 2 1 6)
void re_order_coordinates(int ship_coordinates[])
{
	int temp_coordinate = 0;

	if (ship_coordinates[0] > ship_coordinates[2])
	{
		temp_coordinate = ship_coordinates[0];
		ship_coordinates[0] = ship_coordinates[2];
		ship_coordinates[2] = temp_coordinate;
	}
	if (ship_coordinates[1] > ship_coordinates[3])
	{
		temp_coordinate = ship_coordinates[1];
		ship_coordinates[1] = ship_coordinates[3];
		ship_coordinates[3] = temp_coordinate;
	}
}

// prompts user for coordinates
void get_ship_coordinates(int ship_coordinates[], int ship_size, char ship_name[])
{
	int ship_start_vertical = 0, ship_start_horizontal = 0, ship_end_vertical = 0, ship_end_horizontal = 0;
	int example_ship_coordinate = 1 + ship_size;

	printf("\nPlease enter the beginning and ending coordinates of the %s. \n"
		"The %s must take up %d cells and cannot be placed diagonally. "
		"\nPlease enter coordinates in the form 'y1 x1 y2 x2'.\n"
		"Ex: 1 2 1 %d:\n", ship_name, ship_name, ship_size, example_ship_coordinate);
	scanf("%d%d%d%d", &ship_start_vertical, &ship_start_horizontal, &ship_end_vertical, &ship_end_horizontal);

	ship_coordinates[0] = ship_start_vertical;
	ship_coordinates[1] = ship_start_horizontal;
	ship_coordinates[2] = ship_end_vertical;
	ship_coordinates[3] = ship_end_horizontal;
}

// prints board
void print_board(char board[][10])
{
	printf("\nYour board: \n");
	printf("\n  0 1 2 3 4 5 6 7 8 9");
	for (int i = 0; i < 10; i++)
	{
		printf("\n%d ", i);
		for (int j = 0; j < 10; j++)
		{
			printf("%c ", board[i][j]);
		}
	}
	printf("\n\n");
}

// prints hidden version of board, only showing hits and misses. non-attacked cells appear as "-"
void print_hidden_board(char board[][10])
{
	printf("\nComputer's board: \n");
	printf("\n  0 1 2 3 4 5 6 7 8 9");
	for (int i = 0; i < 10; i++)
	{
		printf("\n%d ", i);
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j] != 'm' && board[i][j] != '*')
			{
				printf("- ");
			}
			else
			{
				printf("%c ", board[i][j]);
			}
		}
	}
	printf("\n\n");
}

// randomly returns 1 or 2 and tells user who is going first
int choose_starting_player(void)
{
	int player_start = 0;

	player_start = rand() % 2 + 1;

	if (player_start == 1)
	{
		printf("\nYou (Player 1) have been randomly chosen to go first.\n");
	}
	else
	{
		printf("\nThe computer (Player 2) has been randomly chosen to go first.\n");
	}
	return player_start;
}

// randomly places ships
void place_ships_randomly(char board[][10])
{
	random_ship(board, 5, "carrier", 'c');
	random_ship(board, 4, "battleship", 'b');
	random_ship(board, 3, "cruiser", 'r');
	random_ship(board, 3, "submarine", 's');
	random_ship(board, 2, "destroyer", 'd');
}

// gets random coordinates for ship placement
void random_ship_coordinates(int ship_coordinates[], int ship_size)
{
	int vertical_or_horizontal = 0, random_vertical = 0, random_horizontal = 0;

	vertical_or_horizontal = rand() % 2;
	random_vertical = rand() % 10;
	random_horizontal = rand() % 10;

	ship_coordinates[0] = random_vertical;
	ship_coordinates[1] = random_horizontal;

	if (vertical_or_horizontal == 0)
	{
		ship_coordinates[2] = random_vertical + ship_size - 1;
		ship_coordinates[3] = random_horizontal;
	}
	else
	{
		ship_coordinates[2] = random_vertical;
		ship_coordinates[3] = random_horizontal + ship_size - 1;
	}
}

// calls random coordinates function and calls functions to check that the coords are good
void random_ship(char board[][10], int ship_size, char ship_name[], char ship_letter)
{
	int ship_coordinates[4], ship_good = 0;

	while (!ship_good)
	{
		random_ship_coordinates(ship_coordinates, ship_size);

		// reruns loops and gets new coords if they are out of board range
		if (ship_coordinates[2] > 9 || ship_coordinates[3] > 9)
		{
			continue;
		}

		// checks that chosen cell is new
		ship_good = random_check_ship(board, ship_coordinates);
	}

	// places ship letter in cells within coords
	for (int i = ship_coordinates[0]; i < ship_coordinates[2] + 1; i++)
	{
		for (int j = ship_coordinates[1]; j < ship_coordinates[3] + 1; j++)
		{
			board[i][j] = ship_letter;
		}
	}
}

// checks if any cells to place ship in have already been filled. returns 1 if placement is good, 0 if one of the cells is filled
int random_check_ship(char board[][10], int ship_coordinates[])
{
	for (int i = ship_coordinates[0]; i < ship_coordinates[2] + 1; i++)
	{

		for (int j = ship_coordinates[1]; j < ship_coordinates[3] + 1; j++)
		{
			if (board[i][j] != '-')
			{
				return 0;
			}
		}
	}
	return 1;
}

// loop through player turns, keeps track of stats
char turns(char player1_board[][10], char player2_board[][10], int player_start, FILE* outfile)
{

	char player1_alive = 'y', player2_alive = 'y', let_computer_go = 0;
	int coordinates[2], player1_hit_count = 0, player2_hit_count = 0, player1_miss_count = 0, player2_miss_count = 0, player1_shots = 0, player2_shots = 0, hit_or_miss = 0;
	double player1_hit_percent = 0, player2_hit_percent = 0;
	char good_range = 0, new_cell = 0;
	char player1_ships_alive[] = { 'y', 'y', 'y', 'y', 'y' };
	char player2_ships_alive[] = { 'y', 'y', 'y', 'y', 'y' };

	// alternates between players
	for (; player1_alive == 'y' && player2_alive == 'y'; player_start++)
	{
		// player 1 (user) turn
		if (player_start % 2 == 1)
		{
			// checks that cell is in range and not targeted already
			do {
				// player chooses cell, and cell is checked by functions for range and repetition
				random_choose_cell(coordinates);
				good_range = check_coordinates_in_range(coordinates);
				new_cell = check_new_cell(player2_board, coordinates);

				if (new_cell == 'n')
				{
					printf("\nYou have already chosen that cell before.\n");
				}
			} while (good_range == 'n' || new_cell == 'n');

			//clear screen between turns to get rid of junk
			system("cls");
			printf("\nYou launch your attack...");
			// changes attacked cell to "m" or "*" and prints hit/miss
			hit_or_miss = launch_attack(player2_board, coordinates, &player1_hit_count, &player1_miss_count, outfile, 1);

			// checks that player is still alive after attack
			player2_alive = check_remaining_ships(player2_board, player2_ships_alive, outfile, 2);

			print_hidden_board(player2_board);
			print_board(player1_board);
		}

		// player 2 (cpu) turn
		else
		{
			//printf("\nEnter any non-whitespace character to allow the computer to do its turn:\n");
			//scanf(" %c", &let_computer_go);
			// checks that cell is not targeted already. no need to check range since the random numbers will always be within range
			do {
				// computer randomly chooses cell, cell is checked by function for repetition
				random_choose_cell(coordinates);
				new_cell = check_new_cell(player1_board, coordinates);
			} while (new_cell == 'n');

			// same functions as for user
			system("cls");
			printf("The computer chooses the cell: %d %d\n", coordinates[0], coordinates[1]);
			printf("The computer launches its attack...");
			hit_or_miss = launch_attack(player1_board, coordinates, &player2_hit_count, &player2_miss_count, outfile, 2);

			player1_alive = check_remaining_ships(player1_board, player1_ships_alive, outfile, 1);

			print_hidden_board(player2_board);
			print_board(player1_board);
		}
	}
	// at end of game, builds stats structures
	fprintf(outfile, "\nGame Over.\n");
	player1_shots = player1_hit_count + player1_miss_count;
	player2_shots = player2_hit_count + player2_miss_count;
	player1_hit_percent = ((double) player1_hit_count / (player1_shots)) * 100;
	player2_hit_percent = ((double) player2_hit_count / (player2_shots)) * 100;

	Player_stats player1_stats = { player1_hit_count, player1_miss_count, player1_shots, player1_hit_percent, "" };
	Player_stats player2_stats = { player2_hit_count, player2_miss_count, player2_shots, player2_hit_percent, "" };

	if (player1_alive == 'y')
	{
		strcpy(player1_stats.game_outcome, "Won.");
		strcpy(player2_stats.game_outcome, "Lost.");
	}
	else
	{
		strcpy(player1_stats.game_outcome, "Lost.");
		strcpy(player2_stats.game_outcome, "Won.");
	}

	// outputs stats to file
	print_player_stats(outfile, player1_stats, 1);
	print_player_stats(outfile, player2_stats, 2);

	// returns whether player 1 is alive or not, used to determine who won and lost
	return player1_alive;
}

// gets coordinates from user for attack
void player1_choose_cell(int coordinates[])
{
	printf("\nPlease enter the coordinates for a cell to strike. Use y1 x1 format. Ex: 3 4: \n");
	scanf("%d%d", &coordinates[0], &coordinates[1]);
}

// checks that coordinates are in board range
char check_coordinates_in_range(int coordinates[])
{
	if (coordinates[0] >= 0 && coordinates[0] <= 9)
	{
		if (coordinates[1] >= 0 && coordinates[1] <= 9)
		{
			return 'y';
		}
	}
	printf("\nThat cell is not in the board range.\n");
	return 'n';
}

// checks that the cell has not been targeted before
char check_new_cell(char board[][10], int coordinates[])
{
	if (board[coordinates[0]][coordinates[1]] == 'm' || board[coordinates[0]][coordinates[1]] == '*')
	{
		return 'n';
	}
	return 'y';
}

// computer randomly chooses a cell to attack
void random_choose_cell(int coordinates[])
{
	coordinates[0] = rand() % 10;
	coordinates[1] = rand() % 10;
}

// after the coords are confirmed to be good, the attack happens and board is changed based on hit/miss. hits and misses are kept track of through pointers.
// attacks and results are outputed to file
int launch_attack(char board[][10], int coordinates[], int *hit_count, int *miss_count, FILE* outfile, int player)
{
	if (board[coordinates[0]][coordinates[1]] == '-')
	{
		(board[coordinates[0]][coordinates[1]] = 'm');
		printf("\nThe attack missed.\n");
		*miss_count += 1;
		fprintf(outfile, "Player %d targeted cell (%d, %d). Miss.\n", player, coordinates[0], coordinates[1]);
		return 0;
	}
	else
	{
		(board[coordinates[0]][coordinates[1]] = '*');
		printf("\nThe attack hit!\n");
		*hit_count += 1;
		fprintf(outfile, "Player %d targeted cell (%d, %d). Hit.\n", player, coordinates[0], coordinates[1]);
		return 1;
	}
}

// checks if a ship changed from existing to not existing to see if ship was sunk this turn. also checks if no more ships exist for a player, so that game can end
char check_remaining_ships(char board[][10], char ships_alive[], FILE* outfile, int player)
{
	int ship_counts[] = { 0, 0, 0, 0, 0 };
	char ship_names[][11] = { "carrier", "battleship", "cruiser", "submarine", "destroyer" };

	// loops to make counter of how many spaces each ship is taking up (a ship takes up one less cell that usual per time it has been hit)
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j] == 'c')
			{
				ship_counts[0]++;
			}
			if (board[i][j] == 'b')
			{
				ship_counts[1]++;
			}
			if (board[i][j] == 'r')
			{
				ship_counts[2]++;
			}
			if (board[i][j] == 's')
			{
				ship_counts[3]++;
			}
			if (board[i][j] == 'd')
			{
				ship_counts[4]++;
			}
		}
	}

	// loops and checks if ship changed from existing to not existing. this way it prints that the ship sunk, but doesn't print this every subsequent turn thereafter
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		if (ships_alive[i] == 'y' && ship_counts[i] == 0)
		{
			printf("\nPlayer %d's %s has been sunk!\n", player, ship_names[i]);
			ships_alive[i] = 'n';
			fprintf(outfile, "Player %d's %s sank.\n", player, ship_names[i]);
		}
	}

	// checks if no more ships for this player, returns "n" if no more ships so loop in turn function can break and game will end
	if (ship_counts[0] == 0 && ship_counts[1] == 0 && ship_counts[2] == 0 && ship_counts[3] == 0 && ship_counts[4] == 0)
	{
		return 'n';
	}
	return 'y';
}

// after the game, the stats from custom structure for each player are printed to log file
void print_player_stats(FILE* outfile, Player_stats player_ending_stats, int player)
{
	fprintf(outfile, "\nPlayer %d Stats\n"
		"Hits: %d\n"
		"Misses: %d\n"
		"Shots: %d\n"
		"Hit Percentage: %.1lf%%\n"
		"%s\n", 
		player, player_ending_stats.hit, player_ending_stats.miss, player_ending_stats.shots, player_ending_stats.hit_percent, player_ending_stats.game_outcome);
}