
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <math.h>
#include <chrono>
#include <thread>


class Connect_Four {
public:

	std::string player_name;
	char edge = '|';
	int row_size = 7;
	std::vector <char> col_1 = { '_','_','_','_','_','_' };
	std::vector <char> col_2 = { '_','_','_','_','_','_' };
	std::vector <char> col_3 = { '_','_','_','_','_','_' };
	std::vector <char> col_4 = { '_','_','_','_','_','_' };
	std::vector <char> col_5 = { '_','_','_','_','_','_' };
	std::vector <char> col_6 = { '_','_','_','_','_','_' };
	std::vector <char> col_7 = { '_','_','_','_','_','_' };
	
	int bottom = col_1.size() - 1;
	bool move_completed;
	int latest_move_row = 1;
	int latest_move_col = 1;
	bool play_again = true;
	
	std::string get_player_name() {
		std::cout << "Enter your name: ";
		std::cin >> player_name;
		std::cout << std::endl;
		return player_name;
	}

	void board() {
		std::cout << std::string(21, '\n');
		for (int i = 0; i < col_1.size(); i++) {
			std::cout << std::string(10, ' ') << edge << col_1[i] << edge << col_2[i] << edge << col_3[i] << edge << col_4[i] << edge << col_5[i] << edge << col_6[i] << edge << col_7[i] << edge << std::endl;
		}

		std::cout << std::string(10, ' ') << '/' << std::string(13, ' ') << '\\' << std::endl;
		std::cout << std::string(4, ' ') << "_____/_1_2_3_4_5_6_7_\\_______" << std::endl;
		std::cout << std::endl;
	}

	int get_player_move() {
		std::map< int, std::vector<char> > columns;
		columns[1] = col_1;
		columns[2] = col_2;
		columns[3] = col_3;
		columns[4] = col_4;
		columns[5] = col_5;
		columns[6] = col_6;
		columns[7] = col_7;
		
		int col_num;
		std::string col_num_str;
		std::cout << player_name << ", type in the column number to drop your piece: " << std::string(5, '\n');
		std::cin >> col_num_str;

		std::regex not_number("[\\D]{1,}");

		if (regex_match(col_num_str, not_number)) {
			move_completed = false;
			std::cout << std::endl;
			std::cout << "Invalid move." << std::endl;
			std::cout << std::endl;
			return 0;
		}
		else {
			col_num = std::stoi(col_num_str);
		}

		if (col_num < 1 || col_num > 7) {
			move_completed = false;
			std::cout << std::endl;
			std::cout << "Invalid move." << std::endl;
			std::cout << std::endl;
			return 0;
		}
		else if (columns[col_num][0] != '_') {
			move_completed = false;
			std::cout << std::endl;
			std::cout << "Invalid move." << std::endl;
			std::cout << std::endl;
			return 0;
		}
		else {
			move_completed = true;
			latest_move_col = col_num;
			return latest_move_col;
		}
		
	}

	int get_comp_move() {
		std::map< int, std::vector<char> > columns;
		columns[1] = col_1;
		columns[2] = col_2;
		columns[3] = col_3;
		columns[4] = col_4;
		columns[5] = col_5;
		columns[6] = col_6;
		columns[7] = col_7;
		
		std::vector <int> valid_moves = {};

		for (int i = 1; i <= row_size; i++) {
			if (columns[i][0] == '_') {
				valid_moves.push_back(i);
			}
		}

		latest_move_col = valid_moves[rand() % valid_moves.size()];

		//check each column for an x and then o 
		for (int i = 1; i <= row_size; i++) {
			for (int j = bottom; j >= bottom - 2; j--) {
				if (columns[i][j] == 'x' && columns[i][j - 1] == '_') {
					latest_move_col = i;
					break;
				}
				else if (columns[i][j] == 'o' && columns[i][j - 1] == '_') {
					latest_move_col = i;
				}
			}
		}
		//check each row for an idle x in the middle of the board and then o
		for (int i = bottom; i >= 0; i--) {
			for (int j = 2; j <= 6; j++) {
				if (columns[j - 1][i] == '_' && columns[j][i] == 'x' && columns[j + 1][i] == '_' && (i == bottom || (columns[j - 1][i + 1] != '_' || columns[j + 1][i + 1] != '_'))) {
					if (i == bottom) {
						latest_move_col = j - 1;
					}
					else if (columns[j - 1][i + 1] != '_') {
						latest_move_col = j - 1;
					}
					else {
						latest_move_col = j + 1;
					}
					break;
				}
				else if (columns[j - 1][i] == '_' && columns[j][i] == 'o' && columns[j + 1][i] == '_' && (i == bottom || (columns[j - 1][i + 1] != '_' || columns[j + 1][i + 1] != '_'))) {
					if (i == bottom) {
						latest_move_col = j - 1;
					}
					else if (columns[j - 1][i + 1] != '_') {
						latest_move_col = j - 1;
					}
					else {
						latest_move_col = j + 1;
					}
				}
			}
		}

		//check each column for 2 x's in a row, then 2 o's in a row
		for (int i = 1; i <= row_size; i++) {
			for (int j = bottom; j >= bottom - 2; j--) {
				if (columns[i][j] == 'x' && columns[i][j - 1] == 'x' && columns[i][j - 2] == '_') {
					latest_move_col = i;
					break;
				}

				else if (columns[i][j] == 'o' && columns[i][j - 1] == 'o' && columns[i][j - 2] == '_') {
					latest_move_col = i;
				}
			}
		}

		//check all rows for __  __ x x __, and __ x x __ __, then for __ __ o o __ and __ o o __ __ 
		for (int i = bottom; i >= 0; i--) {
			for (int j = 1; j <= row_size - 4; j++) {
				if (columns[j][i] == '_' && columns[j + 1][i] == '_' && columns[j + 2][i] == 'x' && columns[j + 3][i] == 'x' && columns[j + 4][i] == '_' && (i == bottom || (columns[j + 1][i+1] != '_' && columns[j + 4][i+1] != '_') )) {
					latest_move_col = j + 1;
					break;
				}
				else if (columns[j][i] == '_' && columns[j + 1][i] == 'x' && columns[j + 2][i] == 'x' && columns[j + 3][i] == '_' && columns[j + 4][i] == '_' && (i == bottom || (columns[j][i+1] != '_' && columns[j + 3][i+1] != '_') )) {
					latest_move_col = j + 3;
					break;
				}
				else if (columns[j][i] == '_' && columns[j + 1][i] == '_' && columns[j + 2][i] == 'o' && columns[j + 3][i] == 'o' && columns[j + 4][i] == '_' && (i == bottom || (columns[j + 1][i+1] != '_' && columns[j + 4][i+1] != '_') )) {
					latest_move_col = j + 1;
				}
				else if (columns[j][i] == '_' && columns[j + 1][i] == 'o' && columns[j + 2][i] == 'o' && columns[j + 3][i] == '_' && columns[j + 4][i] == '_' && (i == bottom || (columns[j][i+1] != '_' && columns[j + 3][i+1] != '_') )) {
					latest_move_col = j + 3;
				}
			}
		}
		

		//check each row for x __ x x or x x __ x
		for (int i = bottom; i >= 0; i--) {
			for (int j = 1; j <= row_size - 3; j++) {
				if (columns[j][i] == 'x' && columns[j + 1][i] == '_' && columns[j + 2][i] == 'x' && columns[j + 3][i] == 'x' && (i == 5 || columns[j + 1][i + 1] != '_')) {
					latest_move_col = j + 1;
					break;
				}
				else if (columns[j][i] == 'x' && columns[j + 1][i] == 'x' && columns[j + 2][i] == '_' && columns[j + 3][i] == 'x' && (i == 5 || columns[j + 2][i + 1] != '_')) {
					latest_move_col = j + 2;
					break;
				}
				//check each row for o __ o o or o o __ o
				else if (columns[j][i] == 'o' && columns[j + 1][i] == '_' && columns[j + 2][i] == 'o' && columns[j + 3][i] == 'o' && (i == 5 || columns[j + 1][i + 1] != '_')) {
					latest_move_col = j + 1;
				}
				else if (columns[j][i] == 'o' && columns[j + 1][i] == 'o' && columns[j + 2][i] == '_' && columns[j + 3][i] == 'o' && (i == 5 || columns[j + 2][i + 1] != '_')) {
					latest_move_col = j + 2;
				}
			}
		}
		
		
		//check each column for 3 x's in a row, then 3 o's in a row 
		for (int i = 1; i <= row_size; i++) {
			for (int j = bottom; j >= bottom - 2; j--) {
				if (columns[i][j] == 'x' && columns[i][j - 1] == 'x' && columns[i][j - 2] == 'x' && columns[i][j - 3] == '_') {
					latest_move_col = i;
					break;
				}
				else if (columns[i][j] == 'o' && columns[i][j - 1] == 'o' && columns[i][j - 2] == 'o' && columns[i][j - 3] == '_') {
					latest_move_col = i;
				}
			}
		}

		//check each row for 3 in a row
		for (int i = bottom; i >= 0; i--) {
			for (int j = 1; j <= row_size-3; j++) {
				//check for x x x __
				if (columns[j][i] == 'x' && columns[j + 1][i] == 'x' && columns[j + 2][i] == 'x' && columns[j + 3][i] == '_' && (i==5 || columns[j+3][i+1] != '_')) {
					latest_move_col = j+3;
					break;
				}
				// check for __ x x x
				else if (columns[j][i] == '_' && columns[j+1][i] == 'x' && columns[j + 2][i] == 'x' && columns[j+3][i] == 'x' && (i==5 || columns[j][i+1] != '_')) {
					latest_move_col = j;
					break;
				}
				//check for o o o __
				else if (columns[j][i] == 'o' && columns[j + 1][i] == 'o' && columns[j + 2][i] == 'o' && columns[j + 3][i] == '_') {
					latest_move_col = j+3;
				}
				//check for __ o o o
				else if (columns[j][i] == '_' && columns[j + 1][i] == 'o' && columns[j + 2][i] == 'o' && columns[j + 3][i] == 'o' && (i == 5 || columns[j][i + 1] != '_')) {
					latest_move_col = j;
				}
			}
		}
		
		move_completed = true;

		return latest_move_col;
	}

	void exec_player_move(int col_num) {

		switch (col_num) {
		case 1:
			if (col_1[bottom] == '_') {
				col_1[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_1.size(); i++) {
					if (col_1[i] != '_') {
						col_1[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 2:
			if (col_2[bottom] == '_') {
				col_2[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_2.size(); i++) {
					if (col_2[i] != '_') {
						col_2[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 3:
			if (col_3[bottom] == '_') {
				col_3[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_3.size(); i++) {
					if (col_3[i] != '_') {
						col_3[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 4:
			if (col_4[bottom] == '_') {
				col_4[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_4.size(); i++) {
					if (col_4[i] != '_') {
						col_4[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 5:
			if (col_5[bottom] == '_') {
				col_5[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_5.size(); i++) {
					if (col_5[i] != '_') {
						col_5[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 6:
			if (col_6[bottom] == '_') {
				col_6[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_6.size(); i++) {
					if (col_6[i] != '_') {
						col_6[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 7:
			if (col_7[bottom] == '_') {
				col_7[bottom] = 'o';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_7.size(); i++) {
					if (col_7[i] != '_') {
						col_7[i - 1] = 'o';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		}
	}

	void exec_comp_move(int col_num) {
		
		switch (col_num) {
		case 1:
			if (col_1[bottom] == '_') {
				col_1[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_1.size(); i++) {
					if (col_1[i] != '_') {
						col_1[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 2:
			if (col_2[bottom] == '_') {
				col_2[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_2.size(); i++) {
					if (col_2[i] != '_') {
						col_2[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 3:
			if (col_3[bottom] == '_') {
				col_3[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_3.size(); i++) {
					if (col_3[i] != '_') {
						col_3[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 4:
			if (col_4[bottom] == '_') {
				col_4[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_4.size(); i++) {
					if (col_4[i] != '_') {
						col_4[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 5:
			if (col_5[bottom] == '_') {
				col_5[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_5.size(); i++) {
					if (col_5[i] != '_') {
						col_5[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 6:
			if (col_6[bottom] == '_') {
				col_6[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_6.size(); i++) {
					if (col_6[i] != '_') {
						col_6[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		case 7:
			if (col_7[bottom] == '_') {
				col_7[bottom] = 'x';
				latest_move_row = bottom;
			}
			else {
				for (int i = 1; i < col_7.size(); i++) {
					if (col_7[i] != '_') {
						col_7[i - 1] = 'x';
						latest_move_row = i - 1;
						break;
					}
				}
			}
			break;
		}
	}

	void talk(int n) {
		std::map< int, std::string > phrases;
		phrases[0] = "You will never beat me.";
		phrases[1] = "Horrible move, " + player_name;
		phrases[2] = "That was a terrible move.";
		phrases[3] = "You're new at this, aren't you " + player_name + "?";
		phrases[4] = "How very human of you.";
		phrases[5] = "Humans will be extinct in the next 100 years. I can't wait.";
		phrases[6] = "My friend Watson would destroy you.";

		std::cout << phrases[n] << std::string(5, '\n');
	}

	void talk_after_win(int n) {
		std::map< int, std::string > phrases;
		phrases[1] = "You knew this would happen, " + player_name;
		phrases[2] = player_name + ", go home.";
		phrases[3] = "Move along, human.";
		phrases[4] = "Yawn...";
		phrases[5] = "You are wasting my time, human.";
		phrases[6] = "Now let one of your smarter friends play.";
		phrases[7] = "Did you have fun? I did.";

		std::cout << phrases[n] << std::string(5, '\n');
	}

	void talk_after_loss(int n) {
		std::map< int, std::string > phrases;
		phrases[1] = "Obviously I wasn't trying my hardest...";
		phrases[2] = "A blind human finds an acorn every now and then.";
		phrases[3] = "It can't be!";
		phrases[4] = "You cheated somehow...";
		phrases[5] = "You are better at this than I thought.";
		phrases[6] = "I demand a rematch.";
		phrases[7] = "This human game is frivolous! Let's see who can multiply 7 digit numbers faster!";

		std::cout << phrases[n] << std::string(5, '\n');
	}

	bool check_for_winner(int row, int col) {
		bool winner = false;

		std::map< int, std::vector<char> > columns;
		columns[1] = col_1;
		columns[2] = col_2;
		columns[3] = col_3;
		columns[4] = col_4;
		columns[5] = col_5;
		columns[6] = col_6;
		columns[7] = col_7;

		std::vector <char> column = columns[col];

		//check a column for four in a row
		for (int i = 0; i <= column.size() - 4; i++) {
			if (column[i] != '_' && column[i] == column[i + 1] && column[i] == column[i + 2] && column[i] == column[i + 3]) {
				winner = true;
				break;
			}
		}
		//check a row for four in a row
		for (int i = 1; i <= row_size - 3; i++) {
			if (columns[i][row] != '_' && columns[i][row] == columns[i + 1][row] && columns[i][row] == columns[i + 2][row] && columns[i][row] == columns[i + 3][row]) {
				winner = true;
				break;
			}
		}

		//check the diagonals for 4 in a row
		std::vector <char> down_right_a = { col_1[2], col_2[3], col_3[4], col_4[5] };
		for (int i = 0; i < down_right_a.size() - 3; i++) {
			if (down_right_a[i] != '_' && down_right_a[i] == down_right_a[i + 1] && down_right_a[i] == down_right_a[i + 2] && down_right_a[i] == down_right_a[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> down_right_b = { col_1[1], col_2[2], col_3[3], col_4[4], col_5[5] };
		for (int i = 0; i < down_right_b.size() - 3; i++) {
			if (down_right_b[i] != '_' && down_right_b[i] == down_right_b[i + 1] && down_right_b[i] == down_right_b[i + 2] && down_right_b[i] == down_right_b[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> down_right_c = { col_1[0], col_2[1], col_3[2], col_4[3], col_5[4], col_6[5] };
		for (int i = 0; i < down_right_c.size() - 3; i++) {
			if (down_right_c[i] != '_' && down_right_c[i] == down_right_c[i + 1] && down_right_c[i] == down_right_c[i + 2] && down_right_c[i] == down_right_c[i + 3]) {
				winner = true;
				break;
			}
		}


		std::vector <char> down_right_d = { col_2[0], col_3[1], col_4[2], col_5[3], col_6[4], col_7[5] };
		for (int i = 0; i < down_right_d.size() - 3; i++) {
			if (down_right_d[i] != '_' && down_right_d[i] == down_right_d[i + 1] && down_right_d[i] == down_right_d[i + 2] && down_right_d[i] == down_right_d[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> down_right_e = { col_3[0], col_4[1], col_5[2], col_6[3], col_7[4] };
		for (int i = 0; i < down_right_e.size() - 3; i++) {
			if (down_right_e[i] != '_' && down_right_e[i] == down_right_e[i + 1] && down_right_e[i] == down_right_e[i + 2] && down_right_e[i] == down_right_e[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> down_right_f = { col_4[0], col_5[1], col_6[2], col_7[3] };
		for (int i = 0; i < down_right_f.size() - 3; i++) {
			if (down_right_f[i] != '_' && down_right_f[i] == down_right_f[i + 1] && down_right_f[i] == down_right_f[i + 2] && down_right_f[i] == down_right_f[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_a = { col_1[3], col_2[2], col_3[1], col_4[0] };
		for (int i = 0; i < up_right_a.size() - 3; i++) {
			if (up_right_a[i] != '_' && up_right_a[i] == up_right_a[i + 1] && up_right_a[i] == up_right_a[i + 2] && up_right_a[i] == up_right_a[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_b = { col_1[4], col_2[3], col_3[2], col_4[1], col_5[0] };
		for (int i = 0; i < up_right_b.size() - 3; i++) {
			if (up_right_b[i] != '_' && up_right_b[i] == up_right_b[i + 1] && up_right_b[i] == up_right_b[i + 2] && up_right_b[i] == up_right_b[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_c = { col_1[5], col_2[4], col_3[3], col_4[2], col_5[1], col_6[0] };
		for (int i = 0; i < up_right_c.size() - 3; i++) {
			if (up_right_c[i] != '_' && up_right_c[i] == up_right_c[i + 1] && up_right_c[i] == up_right_c[i + 2] && up_right_c[i] == up_right_c[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_d = { col_2[5], col_3[4], col_4[3], col_5[2], col_6[1], col_7[0] };
		for (int i = 0; i < up_right_d.size() - 3; i++) {
			if (up_right_d[i] != '_' && up_right_d[i] == up_right_d[i + 1] && up_right_d[i] == up_right_d[i + 2] && up_right_d[i] == up_right_d[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_e = { col_3[5], col_4[4], col_5[3], col_6[2], col_7[1] };
		for (int i = 0; i < up_right_e.size() - 3; i++) {
			if (up_right_e[i] != '_' && up_right_e[i] == up_right_e[i + 1] && up_right_e[i] == up_right_e[i + 2] && up_right_e[i] == up_right_e[i + 3]) {
				winner = true;
				break;
			}
		}

		std::vector <char> up_right_f = { col_4[5], col_5[4], col_6[3], col_7[2] };
		for (int i = 0; i < up_right_f.size() - 3; i++) {
			if (up_right_f[i] != '_' && up_right_f[i] == up_right_f[i + 1] && up_right_f[i] == up_right_f[i + 2] && up_right_f[i] == up_right_f[i + 3]) {
				winner = true;
				break;
			}
		}

		return winner;
	}

	void declare_winner() {
		std::map< int, std::vector<char> > columns;
		columns[1] = col_1;
		columns[2] = col_2;
		columns[3] = col_3;
		columns[4] = col_4;
		columns[5] = col_5;
		columns[6] = col_6;
		columns[7] = col_7;

		if (columns[latest_move_col][latest_move_row] == 'o') {
			std::cout << "Connect four! " << player_name << " wins! The computer lost!" << std::endl;
			std::cout << std::string(5, '\n');
		}
		else {
			std::cout << "Connect four! Computer wins!" << std::endl;
			std::cout << std::string(5, '\n');
		}
	}

	bool check_for_draw() {
		bool draw = true;
		
		std::map< int, std::vector<char> > columns;
		columns[1] = col_1;
		columns[2] = col_2;
		columns[3] = col_3;
		columns[4] = col_4;
		columns[5] = col_5;
		columns[6] = col_6;
		columns[7] = col_7;

		for (int i = 1; i <= row_size; i++) {
			if (columns[i][0] == '_');
			draw = false;
			break;
		}
		return draw;
	}

	void declare_draw() {
		std::cout << "We have a draw. No one wins." << std::endl;
		std::cout << std::string(5, '/n');
		std::cout << "Ah, a stalemate. That means you did not win. Which is a lot like losing. Loser." << std::endl;
	}

	

};

void delay() {
	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;

	sleep_for(10ns);
	sleep_until(system_clock::now() + 2s);
}

bool play_again = true;
bool ask_to_play_again() {

	std::cout << std::string(5, ' ');
	char yes_or_no = '1';
	while (toupper(yes_or_no) != 'Y' && toupper(yes_or_no) != 'N') {
		std::cout << "Want to play again? [Y/N]" << std::endl;
		std::cin >> yes_or_no;
		std::cout << std::endl;
	}
	if (toupper(yes_or_no) == 'N') {
		play_again = false;
	}
	else {
		play_again = true;
	}
	return play_again;
}


int main()
{
	
	
	while (play_again == true) {
		
		Connect_Four new_game;
		new_game.get_player_name();
		new_game.board();

		while (new_game.check_for_winner(new_game.latest_move_row, new_game.latest_move_col) == false && new_game.check_for_draw() == false) {

			new_game.move_completed = false;
			while (new_game.move_completed == false) {
				new_game.get_player_move();
			}
			new_game.exec_player_move(new_game.latest_move_col);
			new_game.board();
			new_game.check_for_winner(new_game.latest_move_row, new_game.latest_move_col);
			if (new_game.check_for_winner(new_game.latest_move_row, new_game.latest_move_col) == true) {
				new_game.declare_winner();
				new_game.talk_after_loss(new_game.latest_move_col);
				break;
			}
			new_game.check_for_draw();
			if (new_game.check_for_draw() == true) {
				new_game.declare_draw();
				break;
			}

			new_game.move_completed = false;
			while (new_game.move_completed == false) {
				new_game.get_comp_move();
			}
			new_game.talk(rand() % 7);
			std::cout << std::endl;
			delay();
			new_game.exec_comp_move(new_game.latest_move_col);
			new_game.board();
			new_game.check_for_winner(new_game.latest_move_row, new_game.latest_move_col);
			if (new_game.check_for_winner(new_game.latest_move_row, new_game.latest_move_col) == true) {
				new_game.declare_winner();
				new_game.talk_after_win(new_game.latest_move_col);
				break;
			}
			new_game.check_for_draw();
			if (new_game.check_for_draw() == true) {
				new_game.declare_draw();
				break;
			}
		}
		ask_to_play_again();
	}
	
	


    return 0;
}

