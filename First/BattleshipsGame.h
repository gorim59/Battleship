#pragma once
#include "../First/battleships.h"
#include "../First/GameType.h"

class BattleshipsGame : public GameType
{
public:
	char empty = 'O';
	char ship = '#';
	char hit = 'X';
	char miss = '-';
	std::unique_ptr <connection> m_connection;
	char my_field[size_of_map][size_of_map];
	char enemy_field[size_of_map][size_of_map];
	int my_hit_points;
	int enemy_hit_points;
	bool finished;
	BattleshipsGame(std::unique_ptr<connection> conn) : m_connection{ std::move(conn) }, finished{ false }{
		my_hit_points = Corvette * 1 + Destroyer * 2 + Cruiser * 3 + Dreadnought * 4 + Yamato * 5;
		enemy_hit_points = Corvette * 1 + Destroyer * 2 + Cruiser * 3 + Dreadnought * 4 + Yamato * 5;
		for (int i = 0; i < size_of_map; i++) {
			for (int j = 0; j < size_of_map; j++) {
				my_field[i][j] = empty;
				enemy_field[i][j] = empty;
			}
		}
	}
	virtual void start() {
		std::cout << "Battleships Game Started\n";
		set_game();
		while (!finished) {
			fire();
			if (!finished) get_fired_at();
		}
		end();
	};
	virtual void join() {
		std::cout << "Battleships Game Joined\n";
		set_game();
		while (!finished) {
			get_fired_at();
			if (!finished) fire();
		}
		end();
	};
	void fire() {
		message msg;
		if (my_hit_points == 0) {
			std::cout << "\nYOU LOST\n";
			std::cout << "You lost all your assets\n";
			msg.header.id = MsgTypes::Surr;
			msg << my_hit_points;
			m_connection->write_msg(msg);
			finished = true;
			return;
		}
		print_enemy_field();
		std::cout << "Pick target\n";
		ship_placemet shot = get_shot_corr();
		while (enemy_field[shot.x][shot.y] != empty) {
			std::cout << "We already shot there, it's no use\n";
			shot = get_shot_corr();
		}
		msg.header.id = MsgTypes::Fire;
		std::cout << "Shot on " << shot.x << shot.y << "\n";
		msg << shot.x;
		msg << shot.y;
		m_connection->write_msg(msg);
		msg = m_connection->get_msg();
		int res;
		if (msg.header.id == MsgTypes::Resp) {
			msg >> res;
			if (res == 1) {
				std::cout << "HIT on " << shot.x + 1 << char('A' + shot.y) << "\n";
				enemy_field[shot.x][shot.y] = hit;
				enemy_hit_points--;
			}
			else {
				std::cout << "MISS on " << shot.x + 1 << char('A' + shot.y) << "\n";
				enemy_field[shot.x][shot.y] = miss;
			}
		}
		print_enemy_field();
	}
	void get_fired_at() {
		std::cout << "Brace\n";
		message msg(m_connection->get_msg());
		if (msg.header.id == MsgTypes::Surr) {
			int res;
			msg >> res;
			std::cout << "\nYOU WIN\n";
			std::cout << "Enemy surrendered with " << res << " hp left\n";
			finished = true;
			return;
		}
		if (msg.header.id != MsgTypes::Fire) {
			std::cout << "Unexpected\n";
		}
		int x, y;
		msg >> y;
		msg >> x;
		msg.header.id = MsgTypes::Resp;
		if (my_field[x][y] == ship) {
			std::cout << "You got hit at " << x + 1 << char('A' + y) << "\n";
			my_field[x][y] = hit;
			my_hit_points--;
			msg << 1;
		}
		else {
			std::cout << "Enemy missed at " << x + 1 << char('A' + y) << "\n";
			my_field[x][y] = miss;
			msg << 0;
		}
		print_my_field();
		m_connection->write_msg(msg);
	}
	void print_my_field() {
		std::cout << "YOUR FIELD   HP: " << my_hit_points << "\n";
		std::cout << "   A B C D E F G H I J\n";
		for (int i = 0; i < size_of_map; i++) {
			std::cout << i + 1;
			if (i < 9) {
				std::cout << "  ";
			}
			else {
				std::cout << " ";
			}
			for (int j = 0; j < size_of_map; j++) {
				std::cout << my_field[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	void print_enemy_field() {
		std::cout << "ENEMIES FIELD   HP: " << enemy_hit_points << "\n";
		std::cout << "   A B C D E F G H I J\n";
		for (int i = 0; i < size_of_map; i++) {
			std::cout << i + 1;
			if (i < 9) {
				std::cout << "  ";
			}
			else {
				std::cout << " ";
			}
			for (int j = 0; j < size_of_map; j++) {
				std::cout << enemy_field[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	void set_game() {
		print_my_field();
		for (int i = 0; i < Corvette; i++) {
			while (!place_ship(1)) {}
			print_my_field();
		}
		for (int i = 0; i < Destroyer; i++) {
			while (!place_ship(2)) {}
			print_my_field();
		}
		for (int i = 0; i < Cruiser; i++) {
			while (!place_ship(3)) {}
			print_my_field();
		}
		for (int i = 0; i < Dreadnought; i++) {
			while (!place_ship(4)) {}
			print_my_field();
		}
		for (int i = 0; i < Yamato; i++) {
			while (!place_ship(5)) {}
			print_my_field();
		}
	};
	bool place_ship(int size) {
		ship_placemet p = get_placement();
		for (int i = 0; i < size; i++) {
			if ((p.x + i >= size_of_map && p.direction == 0) || (p.y + i >= size_of_map && p.direction == 1)) {
				std::cout << "Out of field\n";
				return false;
			}
			if ((my_field[p.x + i][p.y] == ship && p.direction == 0) || (my_field[p.x][p.y + i] == ship && p.direction == 1)) {
				std::cout << "Field occupied by ship\n";
				return false;
			}
		}
		for (int i = 0; i < size; i++) {
			if (p.direction == 0) {
				my_field[p.x + i][p.y] = ship;
			}
			else if (p.direction == 1) {
				my_field[p.x][p.y + i] = ship;
			}
			else {
				std::cout << "Place ship error\n";
			}
		}
		return true;
	}
	void end() {
		std::cout << "Battleships Game Ended\n";
		system("pause");
	};
};