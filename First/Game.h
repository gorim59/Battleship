#pragma once
#include "../First/connection.h"
#include "../First/TicTacToeGame.h"
#include "../First/BattleshipsGame.h"

class Game
{
public:
	std::unique_ptr <connection> m_connection;
	//char my_field[size_of_map][size_of_map];
	//char enemy_field[size_of_map][size_of_map];
	//int my_hit_points;
	//int enemy_hit_points;
	bool finished;
	Game(std::unique_ptr<connection> conn) : m_connection{ std::move(conn) }, finished{ false }{
		//my_hit_points = Corvette * 1 + Destroyer * 2 + Cruiser * 3 + Dreadnought * 4 + Yamato * 5;
		//enemy_hit_points = Corvette * 1 + Destroyer * 2 + Cruiser * 3 + Dreadnought * 4 + Yamato * 5;
		//for (int i = 0; i < size_of_map; i++) {
		//	for (int j = 0; j < size_of_map; j++) {
		//		my_field[i][j] = empty;
		//		enemy_field[i][j] = empty;
		//	}
		//}
	}
	void start() {
		std::cout << "Game Started\n";
		message msg;
		do{
			msg = m_connection->get_msg();
		} while (msg.header.id != MsgTypes::Start);
		int type;
		msg >> type;
		GameType *game = nullptr;
		if (type == 1) {
			game = new BattleshipsGame(std::move(m_connection));
		}
		else if (type == 2) {
			game = new TicTacToeGame(std::move(m_connection));
		}
		else {
			std::cout << "Unknown game\n";
			system("pause");
			m_connection->Disconnect();
			end();
		}
		std::cout << "Game Started pre\n";
		if (game != nullptr)
		game->start();
		delete game;
		end();
	};
	void join() {
		std::cout << "Game Joined\n";
		message msg;
		msg.header.id = MsgTypes::Start;
		int type;
		std::cout << "Pick game battleships[1]; tic-tac-toe[2]:";
		std::cin >> type;
		std::cout << "Type:" << type <<"\n";
		msg << type;
		m_connection->write_msg(msg);
		GameType *game = nullptr;
		std::cout << "Type:" << type << "\n";
		if (type == 1) {
			std::cout << "BattleshipsGame:\n";
			game = new BattleshipsGame(std::move(m_connection));
		}
		else if (type == 2) {
			std::cout << "TicTacToeGame:\n";
			game = new TicTacToeGame(std::move(m_connection));
		}
		else {
			std::cout << "Unknown game\n";
			system("pause");
			m_connection->Disconnect();
			end();
		}
		std::cout << "Game Joined pre\n";
		if (game != nullptr)
		game->join();
		delete game;
		end();
	};
	void end() {
		std::cout << "Game Ended\n";
		finished = true;
	};
	//void fire() {
	//	std::cout << "Pick target\n";
	//	ship_placemet shot = get_shot_corr();
	//	std::cout << "t0\n";
	//	while (enemy_field[shot.x][shot.y] != empty) {
	//		std::cout << "We already shot there, it's no use\n";
	//		shot = get_shot_corr();
	//	}
	//	std::cout << "t1\n";
	//	message msg;
	//	std::cout << "t2\n";
	//	msg.header.id = MsgTypes::Fire;
	//	std::cout << "Shot on " << shot.x << shot.y << "\n";
	//	msg << shot.x;
	//	msg << shot.y;
	//	m_connection->write_msg(msg);
	//	msg = m_connection->get_msg();
	//	int res;
	//	if (msg.header.id == MsgTypes::Resp) {
	//		msg >> res;
	//		if (res == 1) {
	//			std::cout << "HIT on "<< shot.x+1 << char('A'+shot.y) <<"\n";
	//			enemy_field[shot.x][shot.y] = hit;
	//		}
	//		else {
	//			std::cout << "MISS on " << shot.x + 1 << char('A' + shot.y) << "\n";
	//			enemy_field[shot.x][shot.y] = miss;
	//		}
	//	}
	//	get_fired_at();
	//}
	//void get_fired_at() {
	//	std::cout << "Brace\n";
	//	message msg(m_connection->get_msg());
	//	if (msg.header.id != MsgTypes::Fire) {
	//		std::cout << "Unexpected\n";
	//	}
	//	int x, y;
	//	msg >> x;
	//	msg >> y;
	//	msg.header.id = MsgTypes::Resp;
	//	if (my_field[x][y] == '#') {
	//		std::cout << "You got hit at "<< x+1 << char('A'+y) <<"\n";
	//		msg << 1;
	//	}
	//	else {
	//		std::cout << "Enemy missed at " << x + 1 << char('A' + y) << "\n";
	//		msg << 1;
	//	}
	//	m_connection->write_msg(msg);
	//	fire();
	//}
	//void set_game() {
	//	print_my_field();
	//	for (int i = 0; i < Corvette; i++) {
	//		while (!place_ship(1)) {}
	//		print_my_field();
	//	}
	//	for (int i = 0; i < Destroyer; i++) {
	//		while (!place_ship(2)) {}
	//		print_my_field();
	//	}
	//	for (int i = 0; i < Cruiser; i++) {
	//		while (!place_ship(3)) {}
	//		print_my_field();
	//	}
	//	for (int i = 0; i < Dreadnought; i++) {
	//		while (!place_ship(4)) {}
	//		print_my_field();
	//	}
	//	for (int i = 0; i < Yamato; i++) {
	//		while (!place_ship(5)) {}
	//		print_my_field();
	//	}
	//};
	//bool place_ship(int size) {
	//	ship_placemet p = get_placement();
	//	for (int i = 0; i < size; i++) {
	//		if ((p.x + i >= size_of_map && p.direction == 0) || (p.y + i >= size_of_map && p.direction == 1)) {
	//			std::cout << "Out of field\n";
	//			return false;
	//		}
	//		if ((my_field[p.x + i][p.y] == ship && p.direction == 0) || (my_field[p.x][p.y + i] == ship && p.direction == 1)) {
	//			std::cout << "Field occupied by ship\n";
	//			return false;
	//		}
	//	}
	//	for (int i = 0; i < size; i++) {
	//		if (p.direction == 0) {
	//			my_field[p.x + i][p.y] = ship;
	//		}
	//		else if (p.direction == 1) {
	//			my_field[p.x][p.y + i] = ship;
	//		}
	//		else {
	//			std::cout << "Place ship error\n";
	//		}
	//	}
	//	return true;
	//}
	//void print_my_field() {
	//	std::cout << "YOUR FIELD   HP: "<< my_hit_points<<"\n";
	//	std::cout << "   A B C D E F G H I J\n";
	//	for (int i = 0; i < size_of_map; i++){
	//		std::cout << i + 1;
	//		if (i < 9) {
	//			std::cout << "  ";
	//		}
	//		else {
	//			std::cout << " ";
	//		}
	//		for (int j = 0; j < size_of_map; j++) {
	//			std::cout << my_field[i][j] << " ";
	//		}
	//		std::cout << "\n";
	//	}
	//	std::cout << "\n";
	//}
	//void print_enemy_field() {
	//	std::cout << "ENEMIES FIELD   HP: " << enemy_hit_points << "\n";
	//	std::cout << "   A B C D E F G H I J\n";
	//	for (int i = 0; i < size_of_map; i++) {
	//		std::cout << i + 1;
	//		if (i < 9) {
	//			std::cout << "  ";
	//		}
	//		else {
	//			std::cout << " ";
	//		}
	//		for (int j = 0; j < size_of_map; j++) {
	//			std::cout << enemy_field[i][j] << " ";
	//		}
	//		std::cout << "\n";
	//	}
	//	std::cout << "\n";
	//}
};