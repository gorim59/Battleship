#pragma once
#include "../First/common.h"
#define size_of_map 10
#define Corvette 0
#define Destroyer 1
#define Cruiser 0
#define Dreadnought 0
#define Yamato 0

struct ship_placemet
{
	int direction = -1;
	int x = 0;
	int y = 0;
};

ship_placemet get_placement() {
	ship_placemet res;
	std::string s;
	std::cout << "Place ship: ";
	std::cin >> s;
	if (s[0] == 'v') {
		res.direction = 0;
	}
	else if (s[0] == 'h') {
		res.direction = 1;
	}
	else {
		std::cout << "Wrong direction\n";
		return get_placement();
	}
	res.y = s[s.length() - 1] - 'A';
	s = s.substr(1, s.length() - 2);
	// std::cout << s << '\n';
	res.x = stoi(s) - 1;
	if (res.x < 0 || res.x > 9) {
		std::cout << "Wrong letter\n";
		return get_placement();
	}
	if (res.y < 0 || res.y > 9) {
		std::cout << "Wrong number\n";
		return get_placement();
	}
	std::cout << res.x << ' ';
	std::cout << res.y << ' ';
	std::cout << res.direction << '\n';
	return res;
};

ship_placemet get_shot_corr() {
	ship_placemet res;
	std::string s;
	std::cout << "Coordinates: ";
	std::cin >> s;

	res.y = s[s.length() - 1] - 'A';
	s = s.substr(0, s.length() - 1);
	res.x = stoi(s) - 1;
	std::cout << res.x << ' ';
	std::cout << res.y << '\n';
	if (res.x < 0 || res.x > 9) {
		std::cout << "Wrong letter\n";
		return get_placement();
	}
	if (res.y < 0 || res.y > 9) {
		std::cout << "Wrong number\n";
		return get_placement();
	}
	return res;
};
