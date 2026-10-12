#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "gameMap.h"
#include <vector>
#include <cmath>

Game* Game::instance = NULL;

Image font;
Image minifont;
Image sprite;
Color bgcolor(130, 80, 100);
GameMap* map;
Image tileset;
Image bulletSpriteRight;
Image bulletSpriteLeft;
Image bulletSpriteUp;
Image bulletSpriteDown;
Image enemySprite;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	this->renderer = NULL;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	CharacterPos.x = 300;
	CharacterPos.y = 200;
	CameraPos.x = CharacterPos.x / 2;
	CameraPos.y = CharacterPos.y / 2;
	keyHas = false;
	doorOpen = false;
	gameOver = false;
	enemy.x = 310;
	enemy.y = 210;
	enemy.dead = false;
	enemy.directionTimer = 0;
	enemy.dirX = 0;
	enemy.dirY = 0;
	characterDirection = RIGHT;
	
	

	font.load("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.load("data/mini-font-white-4x6.tga"); //load bitmap-font image
	tileset.load("data/topdown_tileset_32x32.png");
	//std::cout << "Works" << std::endl;
	map = loadGameMap("data/mapa.tmj");
	sprite.load("data/32x32/t0099_player_flamethrower_east_a.png"); //example to load an sprite
	bulletSpriteRight.load("data/32x32/t0117_bullet_c.png");
	bulletSpriteLeft.load("data/32x32/bullet_left.png");
	bulletSpriteUp.load("data/32x32/bulllet_up.png");
	bulletSpriteDown.load("data/32x32/bullet_down.png");
	enemySprite.load("data/32x32/t0430_market_a_clerk_robot_a.png");


	//enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;
}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...

	//Update Camera Positions

	CameraPos.x = CharacterPos.x - 80;
	CameraPos.y = CharacterPos.y - 60;
	//some new useful functions
		framebuffer.fill( bgcolor );								//fills the image with one color
		//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
		//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
		drawMap(framebuffer);
		framebuffer.drawImage( sprite, CharacterPos.x-CameraPos.x, CharacterPos.y-CameraPos.y);	
		//draws a scaled image
		if(enemy.dead == false){
			framebuffer.drawImage(enemySprite, enemy.x - CameraPos.x, enemy.y - CameraPos.y);
		}
		
		//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
		framebuffer.drawText( "Level 0", 0, 0, font );				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		//framebuffer.drawText( toString(time), 1, 10, minifont,4,6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)

		for (Bullet& bullet : bullets)
		{
			if (bullet.bulletDir == UP) {
				framebuffer.drawImage(
					bulletSpriteUp,
					bullet.x - CameraPos.x,
					bullet.y - CameraPos.y
				);
			}
			if (bullet.bulletDir == DOWN) {
				framebuffer.drawImage(
					bulletSpriteDown,
					bullet.x - CameraPos.x,
					bullet.y - CameraPos.y
				);
			}

			if (bullet.bulletDir == LEFT) {
				framebuffer.drawImage(
					bulletSpriteLeft,
					bullet.x - CameraPos.x,
					bullet.y - CameraPos.y
				);
			}

			if (bullet.bulletDir == RIGHT) {
				framebuffer.drawImage(
					bulletSpriteRight,
					bullet.x - CameraPos.x,
					bullet.y - CameraPos.y
				);
			}

			
		}

	//send image to screen
	showFramebuffer(&framebuffer);
}




//Draw game map function
void Game::drawMap(Image& framebuffer)
{
	int num_tiles_x = tileset.width / map->tile_width;
	int num_tiles_y = tileset.height / map->tile_height;

	for (int layer = 0; layer < map->num_layers; layer++)
	{
		for (int x = 0; x < map->width; x++)
		{
			for (int y = 0; y < map->height; y++)
			{
				sCell& cell = map->getCell(x, y, layer);
				int type = (int)cell.type;

				// Empty tile
				if (type < 0)
					continue;

				int screenx =
					x * map->tile_width - CameraPos.x;

				int screeny =
					y * map->tile_height - CameraPos.y;

				// Don't draw tiles outside the screen
				if (screenx < -map->tile_width ||
					screenx >(int)framebuffer.width ||
					screeny < -map->tile_height ||
					screeny >(int)framebuffer.height)
				{
					continue;
				}

				int tile_x =
					(type % num_tiles_x) * map->tile_width;

				int tile_y =
					(type / num_tiles_x) * map->tile_height;

				Area area(
					tile_x,
					tile_y,
					map->tile_width,
					map->tile_height
				);

				framebuffer.drawImage(
					tileset,
					screenx,
					screeny,
					area
				);
			}
		}
	}
}




bool Game::CollisionBulletEnemy(Bullet bullet) {
	//We calculate tile of enemy
	float enemyLeft = enemy.x;
	float enemyRight = enemy.x + 24;
	float enemyTop = enemy.y;
	float enemyBottom = enemy.y + 30;

	float bulletLeft = bullet.x;
	float bulletRight = bullet.x + 12;
	float bulletTop = bullet.y;
	float bulletBottom = bullet.y + 12;

	if (bulletLeft >= enemyLeft && bulletRight <= enemyRight && bulletTop >= enemyTop && bulletBottom <= enemyBottom) {
		return true;
	}
	else {
		return false;
	}



}

//Function that determines whether character collides with the key
bool Game::hasKey(float x, float y) {
	int collision_layer = 2;
	int tile_x = (int)(x / map->tile_width);
	int tile_y = (int)(y / map->tile_height);
    // Validate map pointer and coordinates to avoid undefined behaviour
	if (!map)
		return false;

	if (tile_x < 0 || tile_x >= map->width || tile_y < 0 || tile_y >= map->height)
		return false;

	sCell& cell = map->getCell(tile_x, tile_y, collision_layer);
	return cell.type >= 0;

	

}


//bool Game::isCollisionDoor(float x, float y) {
//	int collision_layer = 3;
//	int tile_x = (int)(x / map->tile_width);
//	int tile_y = (int)(y / map->tile_height);
//	sCell& cell = map->getCell(tile_x, tile_y, collision_layer);
//	return cell.type >= 0;
//
//	
//}

//
//bool Game::isCollisionDoor(float x, float y)
//{
//	int door_layer = 3;
//
//	int tile_x = (int)(x / map->tile_width);
//	int tile_y = (int)(y / map->tile_height);
//
//	// Prevent out-of-bounds access
//	if (tile_x < 0 || tile_x >= map->width || tile_y < 0 || tile_y >= map->height)
//		return false;
//
//	sCell& cell = map->getCell(tile_x, tile_y, door_layer);
//
//	return cell.type >= 0;
//}
// 
// 
bool Game::isCollisionDoor(float x, float y)
{
	int door_layer = 3;

	// SAME hitbox dimensions as isCharacterCollision()
	float left = x + 11;
	float right = x + 20;
	float top = y + 24;
	float bottom = y + 31;

	int leftTile = (int)(left / map->tile_width);
	int rightTile = (int)(right / map->tile_width);
	int topTile = (int)(top / map->tile_height);
	int bottomTile = (int)(bottom / map->tile_height);

	if (map->getCell(leftTile, topTile, door_layer).type >= 0)
		return true;

	if (map->getCell(rightTile, topTile, door_layer).type >= 0)
		return true;

	if (map->getCell(leftTile, bottomTile, door_layer).type >= 0)
		return true;

	if (map->getCell(rightTile, bottomTile, door_layer).type >= 0)
		return true;

	return false;
}
//void Game::openDoor() {
//	//We edit the position in which the door is to no collision and change animation
//	int collision_layer = 3;
//	
//
//}

bool Game::isCollision(float x, float y)
{
	int collision_layer = 1;

	int tile_x = (int)(x / map->tile_width);
	int tile_y = (int)(y / map->tile_height);

	// Outside map
	if (tile_x < 0 || tile_x >= map->width ||
		tile_y < 0 || tile_y >= map->height)
	{
		return true;
	}

	sCell& cell = map->getCell(tile_x, tile_y, collision_layer);

	return cell.type >= 0;
}

bool Game::isCharacterCollision(float x, float y)
{
	int width = 32;
	int height = 32;

	return isCollision(x, y) ||
		isCollision(x + width - 1, y) ||
		isCollision(x, y + height - 1) ||
		isCollision(x + width - 1, y + height - 1);
}

bool Game::characterEnemyCollision(vec2 CharacterPos, Enemy enemy) {
	
		int LeftCharacter = CharacterPos.x;
		int RightCharacter = CharacterPos.x + 32;
		int TopCharacter = CharacterPos.y;
		int LowCharacter = CharacterPos.y + 32;

		int LeftEnemy = enemy.x;
		int RightEnemy = enemy.y + 32;
		int TopEnemy = enemy.y;
		int LowEnemy = enemy.y + 32;


		if (LeftCharacter <= RightEnemy && RightCharacter >= LeftEnemy && TopCharacter <= LowEnemy && LowCharacter >= TopEnemy) {
			return true;
		}
		else {
			return false;
		}

	}
//void Game::drawMap(Image& framebuffer)
//{
//	int num_tiles_x = tileset.width / map->tile_width;
//	int num_tiles_y = tileset.height / map->tile_height;
//	int map_layer_id = 0;
//
//	for (int x = 0; x < map->width; x++) {
//		for (int y = 0; y < map->height; y++) {
//
//			sCell& cell = map->getCell(x, y, map_layer_id);
//			//Now we need to get the value that is in the cell to see its type
//			int type = (int)cell.type;
//
//
//			// Screen position with camera offset
//			int screenx = x * map->tile_width - CameraPos.x;
//			int screeny = y * map->tile_height - CameraPos.y;
//
//			// Avoid rendering out of screen stuff
//			if (screenx < -map->tile_width ||
//				screenx >(int)framebuffer.width ||
//				screeny < -map->tile_height ||
//				screeny >(int)framebuffer.height)
//				continue;
//
//
//
//			//Now we got to find this cell type in tilemap
//			int tile_x = (type % num_tiles_x) * map->tile_width;
//			int tile_y = (type / num_tiles_x) * map->tile_height;
//
//			//We get the actual image we have in that position in tilemap
//			Area area(tile_x, tile_y, map->tile_width, map->tile_height);
//
//			// Draw region of tileset inside framebuffer
//			framebuffer.drawImage(tileset, screenx, screeny, area);
//
//
//		}
//
//	}
//}
void Game::update(double dt)
{
	float speed = 100.0f;

	float new_x = CharacterPos.x;
	float new_y = CharacterPos.y;



	//Movement of enemy should be mnore or less random and has to take collision into account
	if (!enemy.dead) {
		float enemySpeed = 40.0f;

		enemy.directionTimer -= dt;

		if (enemy.directionTimer <= 0) {
			int direction = rand() % 4;


			if (direction == 0) // UP
			{
				enemy.dirX = 0;
				enemy.dirY = -1;
			}
			else if (direction == 1) // DOWN
			{
				enemy.dirX = 0;
				enemy.dirY = 1;
			}
			else if (direction == 2) // LEFT
			{
				enemy.dirX = -1;
				enemy.dirY = 0;
			}
			else // RIGHT
			{
				enemy.dirX = 1;
				enemy.dirY = 0;
			}

			// Keep this direction for 1–3 seconds
			enemy.directionTimer = 1.0f + (rand() % 3);
		}
		float newEnemyX, newEnemyY;
		newEnemyX = enemy.x +enemy.dirX * enemySpeed * dt;
		newEnemyY = enemy.y + enemy.dirY * enemySpeed * dt;

		//Have to take collisions into account then we reverse the direction of movement of the enemy

		if (!isCharacterCollision(newEnemyX, enemy.y)) {
			enemy.x = newEnemyX;
		}

		if (!isCharacterCollision(enemy.x, newEnemyY)) {
			enemy.y = newEnemyY;
		}

		if (isCharacterCollision(newEnemyX, enemy.y)) {
			enemy.dirX *= -1;
		}
		if (isCharacterCollision(enemy.x, newEnemyY)) {
			enemy.dirY *= -1;
		}
		
	}

	// Desired movement
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		sprite.load("data/32x32/t0099_player_flamethrower_east_a.png");
		new_x += speed * dt;
		characterDirection = RIGHT;
	}

		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
			new_x -= speed * dt;
			sprite.load("data/32x32/t0215_player_flamethrower_west_a.png");
			characterDirection = LEFT;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
			new_y -= speed * dt;
			sprite.load("data/32x32/t0231_player_flamethrower_north_a.png");
			characterDirection = UP;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
			new_y += speed * dt;
			sprite.load("data/32x32/t0199_player_flamethrower_south_a.png");
			characterDirection = DOWN;
		}


		if (Input::wasKeyPressed(SDL_SCANCODE_A))
		{
			Bullet bullet;

			bullet.x = CharacterPos.x;
			bullet.y = CharacterPos.y;

			// For now, bullet travels right
			if (characterDirection == UP) {
				bullet.dirX = 0.0f;
				bullet.dirY = -1.0f;
				bullet.bulletDir = UP;
			}
			if (characterDirection == DOWN) {
				bullet.dirX = 0.0f;
				bullet.dirY = 1.0f;
				bullet.bulletDir = DOWN;
			}
			if (characterDirection == LEFT) {
				bullet.dirX = -1.0f;
				bullet.dirY = 0.0f;
				bullet.bulletDir = LEFT;
			}
			if (characterDirection == RIGHT) {
				bullet.dirX = 1.0f;
				bullet.dirY = 0.0f;
				bullet.bulletDir = RIGHT;

			}
			

			//Adds bullet object to the list 
			bullets.push_back(bullet);
		}


	// Pick up key
	if (hasKey(new_x, CharacterPos.y) ||
		hasKey(CharacterPos.x, new_y))
	{
		std::cout << "has  key" << std::endl;
		keyHas = true;
	}


	// X movement
	bool doorCollisionX =
		isCollisionDoor(new_x, CharacterPos.y);
	//if (doorCollisionX) {
	//	std::cout << "door openedD" << std::endl;
	//}
	//
	if (isCharacterCollision(new_x, CharacterPos.y)) {
		std::cout << "Character collision not door" << std::endl;
	}

	if (!isCharacterCollision(new_x, CharacterPos.y))
	{

		if (!doorCollisionX || keyHas == true)
		{
			CharacterPos.x = new_x;
			//std::cout << "door collision: " << doorCollisionX << std::endl;
			
			if (doorCollisionX && keyHas == true) {
				std::cout << "door opened" << std::endl;
				doorOpen = true;
				int doorX = (int)(new_x / map->tile_width);
				int doorY = (int)(CharacterPos.y / map->tile_height);

				map->getCell(doorX, doorY, 3).type = -1;
				map->getCell(doorX, doorY, 2).type = 143;
			}
		}
	}


	// Y movement
	bool doorCollisionY =
		isCollisionDoor(CharacterPos.x, new_y);
	//if (doorCollisionY) {
	//	std::cout << "door openedD" << std::endl;
	//}


	if (isCharacterCollision(CharacterPos.x, new_y)) {
		std::cout << "Character collision not door" << std::endl;
	}

	if (!isCharacterCollision(CharacterPos.x, new_y))
	{
		if (!doorCollisionY || keyHas == true)
		{
			//std::cout << "key: " << keyHas << std::endl;
			CharacterPos.y = new_y;

			//std::cout << "door collision: " << doorCollisionY << std::endl;

			if (doorCollisionY && keyHas == true) {
				doorOpen = true;

				std::cout << "door opened" << std::endl;
			
				int doorX = (int)(CharacterPos.x / map->tile_width);
				int doorY = (int)(new_y / map->tile_height);
				map->getCell(doorX, doorY, 3).type = -1;
				map->getCell(doorX, doorY, 2).type = 143;
			}
		}
	}


	// Camera follows character
	CameraPos.x = CharacterPos.x - 80;
	CameraPos.y = CharacterPos.y - 60;

	float bulletSpeed = 200.0f;

	for (Bullet& bullet : bullets)
	{
		bullet.x += bullet.dirX * bulletSpeed * dt;
		bullet.y += bullet.dirY * bulletSpeed * dt;
		if (CollisionBulletEnemy(bullet)) {
			enemy.dead = true;
			std::cout << "enemy dead" << std::endl;

		}
	}

	if (characterEnemyCollision(CharacterPos, enemy)) {
		gameOver = true;
		std::cout << "game over" << std::endl;
	}

	



}
//void Game::update(double dt)
//{
//	float speed = 100.0f;
//
//	float new_x = CharacterPos.x;
//	float new_y = CharacterPos.y;
//
//	// Calculate desired position
//	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT))
//		new_x += speed * dt;
//
//	if (Input::isKeyPressed(SDL_SCANCODE_LEFT))
//		new_x -= speed * dt;
//
//	if (Input::isKeyPressed(SDL_SCANCODE_UP))
//		new_y -= speed * dt;
//
//	if (Input::isKeyPressed(SDL_SCANCODE_DOWN))
//		new_y += speed * dt;
//
//	// This will detect if character has reached the key. Once we have the key the door no longer collides
//
//	if (hasKey(new_x, CharacterPos.y))
//		key = 1;
//
//	if (hasKey(CharacterPos.x, new_y))
//		key = 1;
//
//	if (isCollisionDoor(new_x, CharacterPos.y) && key == 1) {
//		doorOpen = 1;
//		CharacterPos.x = new_x;
//
//	}
//
//	if (isCollisionDoor(CharacterPos.x, new_y) && key == 1) {
//		doorOpen = 1;
//		CharacterPos.y = new_y;
//
//	}
//
//	// Only move if there is no collision
//	if (!isCharacterCollision(new_x, CharacterPos.y) && !isCollisionDoor(new_x, CharacterPos.y))
//		CharacterPos.x = new_x;
//
//	if (!isCharacterCollision(CharacterPos.x, new_y) && !isCollisionDoor(CharacterPos.x, new_y))
//		CharacterPos.y = new_y;
//
//	// Camera follows character
//	CameraPos.x = CharacterPos.x - 80;
//	CameraPos.y = CharacterPos.y - 60;
//}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	#ifdef USE_OPENGL
		glViewport( 0,0, width, height );
	#endif
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

#ifdef USE_OPENGL
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();
#else
	static SDL_Texture* texture = NULL;
	static int tex_width = 0;
	static int tex_height = 0;
	if (!texture || tex_width != img->width || tex_height != img->height)
	{
		if(texture)
			SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_TARGET, img->width, img->height);
		tex_width = img->width;
		tex_height = img->height;
	}

	//SDL_RenderClear(renderer);
	SDL_Rect rect = { 0, 0, (int)img->width, (int)img->height };
	SDL_UpdateTexture(texture, &rect, img->pixels, img->width*4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

#endif
	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(nullptr, 0, &audio_spec, nullptr, 0);
	if (!audio_device) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudioDevice(audio_device, 0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
