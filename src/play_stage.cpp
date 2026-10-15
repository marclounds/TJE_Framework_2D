#include "play_stage.h"

#include "image.h"
#include "gameMap.h"
#include <vector>
#include <cmath>
#include <algorithm>


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
//Constructor

PlayStage::PlayStage(GameState& state )
	: gamestate(state)
{
this->window_width = window_width;
this->window_height = window_height;
this->window = window;
this->renderer = NULL;
//instance = this;
must_exit = false;

fps = 0;
frame = 0;
time = 0.0f;
elapsed_time = 0.0f;
gamestate.CharacterPos.x = 300;
gamestate.CharacterPos.y = 200;
CameraPos.x = gamestate.CharacterPos.x / 2;
CameraPos.y = gamestate.CharacterPos.y / 2;
gamestate.keyHas = false;
gamestate.doorOpen = false;
gamestate.characterDirection = RIGHT;
gamestate.score = 0;





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



}


void PlayStage::generationEnemies() {
    //gamestate.numEnemies = 0;
	gamestate.enemy.x = 340;
	gamestate.enemy.y = 240;
	gamestate.enemy.initialPosX = 340;
	gamestate.enemy.initialPosY = 240;

	gamestate.enemy.dead = false;
	gamestate.enemy.directionTimer = 0;
	gamestate.enemy.dirX = 0;
	gamestate.enemy.dirY = 0;

	gamestate.enemy2.x = 360;
	gamestate.enemy2.y = 270;
	gamestate.enemy2.initialPosX = 310;
	gamestate.enemy2.initialPosY = 210;
	gamestate.enemy2.dead = false;
	gamestate.enemy2.directionTimer = 0;
	gamestate.enemy2.dirX = 0;
	gamestate.enemy2.dirY = 0;

	gamestate.enemies.push_back(gamestate.enemy);
	gamestate.enemies.push_back(gamestate.enemy2);


}


//Draw game map function
void PlayStage::drawMap(Image& framebuffer)
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

bool PlayStage::CollisionBulletEnemy(Bullet bullet, Enemy enemy) {
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
bool PlayStage::hasKey(float x, float y) {
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


bool PlayStage::isCollisionDoor(float x, float y)
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


bool PlayStage::isCollision(float x, float y)
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

bool PlayStage::isCharacterCollision(float x, float y)
{
	int width = 32;
	int height = 32;

	return isCollision(x, y) ||
		isCollision(x + width - 1, y) ||
		isCollision(x, y + height - 1) ||
		isCollision(x + width - 1, y + height - 1);
}


bool PlayStage::characterEnemyCollision(vec2 CharacterPos, Enemy enemy) {

	int LeftCharacter = CharacterPos.x;
	int RightCharacter = CharacterPos.x + 20;
	int TopCharacter = CharacterPos.y;
	int LowCharacter = CharacterPos.y + 20;

	int LeftEnemy = enemy.x;
	int RightEnemy = enemy.x + 20;
	int TopEnemy = enemy.y;
	int LowEnemy = enemy.y + 20;


	if (LeftCharacter <= RightEnemy && RightCharacter >= LeftEnemy && TopCharacter <= LowEnemy && LowCharacter >= TopEnemy) {
		return true;
	}
	else {
		return false;
	}

}



void PlayStage::update(double dt)
{
    float speed = 100.0f;

    float new_x = gamestate.CharacterPos.x;
    float new_y = gamestate.CharacterPos.y;


    // ENEMY MOVEMENT

    for (Enemy& enemy : gamestate.enemies)
    {
        if (!enemy.dead)
        {
            float enemySpeed = 40.0f;

            enemy.directionTimer -= dt;

            if (enemy.directionTimer <= 0)
            {
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

                enemy.directionTimer = 1.0f + (rand() % 3);
            }

            float newEnemyX = enemy.x + enemy.dirX * enemySpeed * dt;
            float newEnemyY = enemy.y + enemy.dirY * enemySpeed * dt;

            // X collision
            if (!isCharacterCollision(newEnemyX, enemy.y))
            {
                enemy.x = newEnemyX;
            }
            else
            {
                enemy.dirX *= -1;
            }

            // Y collision
            if (!isCharacterCollision(enemy.x, newEnemyY))
            {
                enemy.y = newEnemyY;
            }
            else
            {
                enemy.dirY *= -1;
            }
        }
    }


    // CHARACTER MOVEMENT

    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT))
    {
        sprite.load("data/32x32/t0099_player_flamethrower_east_a.png");
        new_x += speed * dt;
        gamestate.characterDirection = RIGHT;
    }

    if (Input::isKeyPressed(SDL_SCANCODE_LEFT))
    {
        new_x -= speed * dt;
        sprite.load("data/32x32/t0215_player_flamethrower_west_a.png");
        gamestate.characterDirection = LEFT;
    }

    if (Input::isKeyPressed(SDL_SCANCODE_UP))
    {
        new_y -= speed * dt;
        sprite.load("data/32x32/t0231_player_flamethrower_north_a.png");
        gamestate.characterDirection = UP;
    }

    if (Input::isKeyPressed(SDL_SCANCODE_DOWN))
    {
        new_y += speed * dt;
        sprite.load("data/32x32/t0199_player_flamethrower_south_a.png");
        gamestate.characterDirection = DOWN;
    }


    // SHOOTING

    if (Input::wasKeyPressed(SDL_SCANCODE_A))
    {
        Bullet bullet{};

        bullet.x = gamestate.CharacterPos.x;
        bullet.y = gamestate.CharacterPos.y;

        if (gamestate.characterDirection == UP)
        {
            bullet.dirX = 0.0f;
            bullet.dirY = -1.0f;
            bullet.bulletDir = UP;
        }

        if (gamestate.characterDirection == DOWN)
        {
            bullet.dirX = 0.0f;
            bullet.dirY = 1.0f;
            bullet.bulletDir = DOWN;
        }

        if (gamestate.characterDirection == LEFT)
        {
            bullet.dirX = -1.0f;
            bullet.dirY = 0.0f;
            bullet.bulletDir = LEFT;
        }

        if (gamestate.characterDirection == RIGHT)
        {
            bullet.dirX = 1.0f;
            bullet.dirY = 0.0f;
            bullet.bulletDir = RIGHT;
        }

        gamestate.bullets.push_back(bullet);
    }


    // PICK UP KEY

    if (hasKey(new_x, gamestate.CharacterPos.y) ||
        hasKey(gamestate.CharacterPos.x, new_y))
    {
        std::cout << "has key" << std::endl;
        gamestate.keyHas = true;
    }


    // X MOVEMENT AND DOOR COLLISION

    bool doorCollisionX =
        isCollisionDoor(new_x, gamestate.CharacterPos.y);

    if (isCharacterCollision(new_x, gamestate.CharacterPos.y))
    {
        std::cout << "Character collision not door" << std::endl;
    }

    if (!isCharacterCollision(new_x, gamestate.CharacterPos.y))
    {
        if (!doorCollisionX || gamestate.keyHas == true)
        {
            gamestate.CharacterPos.x = new_x;

            if (doorCollisionX && gamestate.keyHas == true)
            {
                std::cout << "door opened" << std::endl;

                gamestate.doorOpen = true;

                // Open all door tiles
                for (int i = 0; i < map->width; i++)
                {
                    for (int j = 0; j < map->height; j++)
                    {
                        if (map->getCell(i, j, 3).type != -1)
                        {
                            map->getCell(i, j, 3).type = -1;
                            map->getCell(i, j, 2).type = 143;
                        }
                    }
                }
            }
        }
    }


    // Y MOVEMENT AND DOOR COLLISION

    bool doorCollisionY =
        isCollisionDoor(gamestate.CharacterPos.x, new_y);

    if (isCharacterCollision(gamestate.CharacterPos.x, new_y))
    {
        std::cout << "Character collision not door" << std::endl;
    }

    if (!isCharacterCollision(gamestate.CharacterPos.x, new_y))
    {
        if (!doorCollisionY || gamestate.keyHas == true)
        {
            gamestate.CharacterPos.y = new_y;

            if (doorCollisionY && gamestate.keyHas == true)
            {
                gamestate.doorOpen = true;

                std::cout << "door opened" << std::endl;

                // Open all door tiles
                for (int i = 0; i < map->width; i++)
                {
                    for (int j = 0; j < map->height; j++)
                    {
                        if (map->getCell(i, j, 3).type != -1)
                        {
                            map->getCell(i, j, 3).type = -1;
                            map->getCell(i, j, 2).type = 143;
                        }
                    }
                }
            }
        }
    }


    // CAMERA FOLLOWS CHARACTER

    CameraPos.x = gamestate.CharacterPos.x - 80;
    CameraPos.y = gamestate.CharacterPos.y - 60;


    // BULLET MOVEMENT AND ENEMY COLLISION

    float bulletSpeed = 200.0f;

    for (Bullet& bullet : gamestate.bullets)
    {
        bullet.x += bullet.dirX * bulletSpeed * dt;
        bullet.y += bullet.dirY * bulletSpeed * dt;

        for (Enemy& enemy : gamestate.enemies)
        {
            if (!enemy.dead)
            {
                if (CollisionBulletEnemy(bullet, enemy))
                {
                    enemy.dead = true;

                    gamestate.score += 100;

                    std::cout << "enemy dead" << std::endl;
                }
            }
        }
    }


    // CHARACTER AND ENEMY COLLISION

    for (Enemy& enemy : gamestate.enemies)
    {
        if (!enemy.dead &&
            characterEnemyCollision(gamestate.CharacterPos, enemy))
        {
            gameOver = true;

            std::cout << "game over" << std::endl;
            //We must remove all enemies because we will regenerate them at the beggining again
            for (Enemy& enemy : gamestate.enemies) {

                enemy.dead = true;
            }
            
        }
    }


    // REMOVE DEAD ENEMIES

    gamestate.enemies.erase(
        std::remove_if(
            gamestate.enemies.begin(),
            gamestate.enemies.end(),
            [](const Enemy& enemy)
            {
                return enemy.dead;
            }
        ),
        gamestate.enemies.end()
    );

    if (Input::wasKeyPressed(SDL_SCANCODE_Z))

    {
        pause = true;
    }
}

void PlayStage::render(Image& framebuffer)
{
    // Draw map
    // Draw player
    // Draw enemies
    // Draw bullets

	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...

	//Update Camera Positions

	CameraPos.x = gamestate.CharacterPos.x - 80;
	CameraPos.y = gamestate.CharacterPos.y - 60;
	//some new useful functions
	framebuffer.fill(bgcolor);								//fills the image with one color
	//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
	//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
	drawMap(framebuffer);
	framebuffer.drawImage(sprite, gamestate.CharacterPos.x - CameraPos.x, gamestate.CharacterPos.y - CameraPos.y);
	//draws a scaled image
	for (Enemy& enemy : gamestate.enemies) {
		if (enemy.dead == false) {
			framebuffer.drawImage(enemySprite, enemy.x - CameraPos.x, enemy.y - CameraPos.y);
		}
	}
	
	
	//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
	framebuffer.drawText("Score: " + std::to_string(gamestate.score), 0, 0, font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
	//framebuffer.drawText( toString(time), 1, 10, minifont,4,6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)

	for (Bullet& bullet : gamestate.bullets)
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




}

