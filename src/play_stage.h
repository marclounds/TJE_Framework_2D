#pragma once
#include "stage.h"
#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "input.h"
#include "gameState.h"





class PlayStage : public Stage
{
public:



	SDL_Window* window;
	SDL_Renderer* renderer;
	int window_width;
	int window_height;
	vec2 CameraPos;

	bool pause = false;
	//some globals
	long frame;
	float time;
	float elapsed_time;
	int fps;
	bool must_exit;
	bool gameOver;



	//Constructor 
	PlayStage(GameState& gamestate);

    void update(double dt) override;
    void render(Image& framebuffer) override;
    void drawMap(Image& framebuffer);
    bool CollisionBulletEnemy(Bullet bullet, Enemy enemy);
	bool isCollisionDoor(float x, float y);
	bool isCollision(float x, float y);
	bool hasKey(float x, float y);
	bool isCharacterCollision(float x, float y);
	bool characterEnemyCollision(vec2 CharacterPos, Enemy enemy);
	void generationEnemies();

	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);



	//audio stuff
	void enableAudio(); //opens audio channel to play sound


	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec); //called constantly to fill the audio buffer

private:
	GameState& gamestate;

};
