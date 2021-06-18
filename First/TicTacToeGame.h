#pragma once
#include "../First/GameType.h"
char symbols[2] = { 'X','O' };
char empty = '-';

class TicTacToeGame: public GameType
{
public:
	char symbols[2] = { 'X','O' };
	char your_symbol;
	char enemy_symbol;
	char winner;
	char empty = '-';
	std::unique_ptr <connection> m_connection;
	char board[3][3];
	TicTacToeGame(std::unique_ptr<connection> conn) : m_connection{ std::move(conn) } {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				board[i][j] = empty;
			}
		}
	}
	virtual void start() {
		std::cout << "Tic-Tac-Toe Game Started\n";
		your_symbol = symbols[0];
		enemy_symbol = symbols[1];
		while (!finished()) {
			move();
			if (!finished()) receive_move();
		}
		end();
	};
	virtual void join() {
		std::cout << "Tic-Tac-Toe Game Joined\n";
		your_symbol = symbols[1];
		enemy_symbol = symbols[0];
		while (!finished()) {
			receive_move();
			if (!finished()) move();
		}
		end();
	};
	void move() {
		draw();
		int x;
		do{
			std::cout << "Your move: ";
			std::cin >> x;
			x--;
		} while(board[x / 3][x % 3] != empty);
		board[x / 3][x % 3] = your_symbol;
		message msg;
		msg << x;
		m_connection->write_msg(msg);
	};
	void receive_move() {
		draw();
		int x;
		message msg;
		msg = m_connection->get_msg();
		msg >> x;
		board[x / 3][x % 3] = enemy_symbol;
	};
	bool finished() {
		if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != empty) {
			winner = board[0][0];
			return true;
		}
		if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] != empty) {
			winner = board[0][0];
			return true;

		}
		if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] != empty) {
			winner = board[1][0];
			return true;

		}
		if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] != empty) {
			winner = board[2][0];
			return true;

		}
		if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] != empty) {
			winner = board[0][0];
			return true;

		}
		if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] != empty) {
			winner = board[0][1];
			return true;

		}
		if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] != empty) {
			winner = board[0][2];
			return true;

		}
		if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] != empty) {
			winner = board[2][0];
			return true;

		}
		if (board[0][0] != empty &&
			board[1][0] != empty &&
			board[2][0] != empty &&
			board[0][1] != empty &&
			board[1][1] != empty &&
			board[2][1] != empty &&
			board[0][2] != empty &&
			board[1][2] != empty &&
			board[2][2] != empty) {
			winner = empty;
			return true;

		}
		return false;
	}
	void draw() {
		std::cout << "Board:\n";
		std::cout << "Symbol: "<<your_symbol<<" \n";
		for (int i = 0; i < 9; i++) {
			if (board[i / 3][i % 3] == empty) {
				std::cout << i + 1;
			}
			else {
				std::cout << board[i / 3][i % 3];
			}
			std::cout << ' ';
			if (i % 3 == 2) {
				std::cout << '\n';
			}
		}
	}
	void end() {
		draw();
		if (your_symbol == winner) {
			std::cout << "\nYOU WON\n";
		}
		else if (winner == empty) {
			std::cout << "\nDRAW\n";
		}
		else {
			std::cout << "\nYOU LOST\n";
		}
		std::cout << "Tic-Tac-Toe Game Ended\n";
		system("pause");
	};
};