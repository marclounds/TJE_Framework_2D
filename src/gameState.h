#pragma once

#include "utils.h"
#include <vector>


enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};



struct Enemy {
	float x;
	float y;
	float dirX;
	float dirY;
	bool dead;
	float directionTimer;
	float initialPosX;
	float initialPosY;
};

struct Bullet
{
	float x;
	float y;
	float dirX;
	float dirY;
	Direction bulletDir;
};

struct GameState {
	vec2 CharacterPos;
	bool keyHas;
	bool doorOpen;
	Enemy enemy;
	Enemy enemy2;
	Direction characterDirection;

	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;

	//score
	int score;
};