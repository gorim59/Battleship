#pragma once
#include "../First/connection.h"

class GameType
{
public:
	GameType() { }
	virtual void start() = 0;
	virtual void join() = 0;
};