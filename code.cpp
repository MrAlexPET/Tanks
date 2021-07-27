#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace sf;

Clock bulletClock;
Clock bulletClock2;

class Picture
{
public:
	Texture texture;
	Sprite sprite;

	void Move(float x, float y) 
	{
		sprite.move(x, y);
	}
};

class Player : public Picture
{
public:
	bool canShoot;
	FloatRect bounds;

	Player() 
	{
		texture.loadFromFile("tank.png");
		sprite.setTexture(texture);
		sprite.setPosition(560, 70);
		sprite.setOrigin(16, 16);
		sprite.setScale(0.8, 0.8);
		canShoot = true;
		bounds = sprite.getGlobalBounds();
	}

	void Move(float a, float b) 
	{
		sprite.move(a, b);
	}
}player;

class Enemy : public Picture
{
public:
	bool canShoot;
	FloatRect bounds;

	Enemy()
	{
		texture.loadFromFile("player.png");
		sprite.setTexture(texture);
		sprite.setPosition(60, 560);
		sprite.setOrigin(16, 16);
		sprite.setScale(0.8, 0.8);
		canShoot = true;
		bounds = sprite.getGlobalBounds();
	}

	void Move(float a, float b)
	{
		sprite.move(a, b);
	}
}enemy;

class Bullet : public Picture
{
public:
	bool exists;
	int direction;
	FloatRect bounds;

	Bullet(int _direction)
	{
		texture.loadFromFile("bullet.png");
		sprite.setTexture(texture);
		exists = false;
		direction = _direction;
	}

	void Shoot()
	{
		exists = true;
		sprite.setPosition((player.sprite.getPosition().x), (player.sprite.getPosition().y));
		sprite.setRotation(player.sprite.getRotation());

		player.canShoot = false;

		bulletClock.restart();
	}

	void Move(float a, float b)
	{
		sprite.move(a, b);
	}
};

class EnemyBullet : public Picture
{
public:
	bool exists;
	int direction;
	FloatRect bounds;

	EnemyBullet(int _direction)
	{
		texture.loadFromFile("bullet.png");
		sprite.setTexture(texture);
		exists = false;
		direction = _direction;
	}

	void Shoot()
	{
		exists = true;
		sprite.setPosition((enemy.sprite.getPosition().x), (enemy.sprite.getPosition().y));
		sprite.setRotation(enemy.sprite.getRotation());
		enemy.canShoot = false;

		bulletClock2.restart();
	}

	void Move(float a, float b)
	{
		sprite.move(a, b);
	}
};

class Wall : public Picture
{
public:
	FloatRect bounds;

	Wall() 
	{
		texture.loadFromFile("wall.png");
		sprite.setTexture(texture);
	}
};


int main() 
{
	RenderWindow window(VideoMode(608, 608, 32), "Window");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Wall **walls = new Wall * [19];
	std::vector <Bullet*> bulls;
	std::vector <EnemyBullet*> bulls2;
	float speed = 0.05;
	float speedPl = 0.25;

	Picture plwon = Picture();
	plwon.texture.loadFromFile("plwon2.png");
	plwon.sprite.setTexture(plwon.texture);
	bool playerWon = false;

	Picture enemyWon = Picture();
	enemyWon.texture.loadFromFile("enemyWon2.png");
	enemyWon.sprite.setTexture(enemyWon.texture);
	bool enWon = false;

	int nextBullet = 0;
	int nextBullet2 = 0;
	int totalBullets = 10;
	int direction = 3;
	int direction2 = 3;
	String map[] = {"*******************",
					"*-----------------*",
					"*--***--*--*-***--*",
					"*--***--*--*-***--*",
					"*--***--*--*-***--*",
					"*-----------------*",
					"*--**---*--*-***--*",
					"*--**---****-***--*",
					"*-------*--*------*", 
					"*--***-------***--*",
					"*-------*--*------*", 
					"*--***--****--**--*",
					"*--***--*--*--**--*", 
					"*-----------------*",
					"*--***--*--*--**--*", 
					"*--***--*--*--**--*",
					"*--***--*--*--**--*", 
					"*-----------------*",
					"*******************", };
	// Set map
	for (int i = 0; i < 19; i++)
	{
		walls[i] = new Wall[19];
	}
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map[i][j] == '*')
			{
				walls[i][j].sprite.setPosition(Vector2f(j * 32, i * 32));
				walls[i][j].bounds = walls[i][j].sprite.getGlobalBounds();
			}
		}
		
	}

	Clock clock;
	// Main game loop
	Event event;
	while (window.isOpen()) 
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if(event.type == Event::KeyPressed)
			{
				if(event.key.code == Keyboard::Escape) window.close();
			}
		}
		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 800;

		//Player1 Input
		///////////////
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			direction = 0;
			player.sprite.setRotation(-90);
			player.Move(0, -speed * time);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) 
		{ 
			direction = 1;
			player.sprite.setRotation(90);
			player.Move(0, speed * time); 
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left)) 
		{ 
			direction = 2;   
			player.sprite.setRotation(180);
			player.Move(-speed * time, 0); 
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) 
		{
			direction = 3;
			player.sprite.setRotation(0);
			player.Move(speed * time, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) 
		{
			if(player.canShoot)bulls.push_back(new Bullet(direction));
			if (!bulls.at(nextBullet)->exists) bulls.at(nextBullet)->Shoot();
		}


		// Player2 Input
		////////////////
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			direction2 = 0;
			enemy.sprite.setRotation(-90);
			enemy.Move(0, -speed * time);
		}
		else if (Keyboard::isKeyPressed(Keyboard::S))
		{
			direction2 = 1;
			enemy.sprite.setRotation(90);
			enemy.Move(0, speed * time);
		}
		else if (Keyboard::isKeyPressed(Keyboard::A))
		{
			direction2 = 2;
			enemy.sprite.setRotation(180);
			enemy.Move(-speed * time, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::D))
		{
			direction2 = 3;
			enemy.sprite.setRotation(0);
			enemy.Move(speed * time, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q))
		{
			if (enemy.canShoot)bulls2.push_back(new EnemyBullet(direction2));
			if (!bulls2.at(nextBullet2)->exists) bulls2.at(nextBullet2)->Shoot();
		}

		// Collision
		player.bounds = player.sprite.getGlobalBounds();
		enemy.bounds = enemy.sprite.getGlobalBounds();
		for (int i = 0; i < 19; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				if (walls[i][j].bounds.intersects(player.bounds) )
				{
					if (direction == 0) { player.Move(0, speed * time); }
					else if (direction == 1) { player.Move(0, -speed * time); }
					else if (direction == 2) { player.Move(speed * time, 0); }
					else if (direction == 3) { player.Move(-speed * time, 0); }
				}
				if (walls[i][j].bounds.intersects(enemy.bounds))
				{
					if (direction2 == 0) { enemy.Move(0, speed * time); }
					else if (direction2 == 1) { enemy.Move(0, -speed * time); }
					else if (direction2 == 2) { enemy.Move(speed * time, 0); }
					else if (direction2 == 3) { enemy.Move(-speed * time, 0); }
				}
				for (int k = 0; k < bulls.size(); k++)
				{
					if (walls[i][j].bounds.intersects(bulls[k]->bounds))
					{
						bulls[k]->sprite.setPosition(-100, -100);
					}
					else if(bulls[k]->bounds.intersects(enemy.bounds))
					{
						enemy.sprite.setPosition(-100, -100);
						bulls[k]->sprite.setPosition(-100, -100);
						playerWon = true;
					}
					
				}
				for (int k = 0; k < bulls2.size(); k++)
				{
					if (walls[i][j].bounds.intersects(bulls2[k]->bounds))
					{
						bulls2[k]->sprite.setPosition(-100, -100);
					}
					else if(bulls2[k]->bounds.intersects(player.bounds))
					{
						player.sprite.setPosition(-100, -100);
						bulls2[k]->sprite.setPosition(-100, -100);
						enWon = true;
					}

				}
			}
		}
		
		
		window.clear(Color(0,0,0));

		// Draw
		for (int i = 0; i < bulls.size(); i++)
		{
			if (bulls[i]->exists) window.draw(bulls[i]->sprite);
			bulls[i]->bounds = bulls[i]->sprite.getGlobalBounds();
		}

		for (int i = 0; i < bulls2.size(); i++)
		{
			if (bulls2[i]->exists) window.draw(bulls2[i]->sprite);
			bulls2[i]->bounds = bulls2[i]->sprite.getGlobalBounds();
		}

		window.draw(player.sprite);
		window.draw(enemy.sprite);

		for (int i = 0; i < 19; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				window.draw(walls[i][j].sprite);
			}
		}

		if(playerWon)
		{
			plwon.sprite.setPosition(204, 254);
			window.draw(plwon.sprite);
		}
		if(enWon)
		{
			enemyWon.sprite.setPosition(204, 254);
			window.draw(enemyWon.sprite);
		}

		window.display();
		
		

		//MoveBullet(bulls.size(), (int)bulls);
		for (int i = 0; i < bulls.size(); i++)
		{
			if (bulls.at(i)->exists) 
			{
				switch (bulls.at(i)->direction)
				{
					case 0: bulls.at(i)->Move(0, -speedPl * time); break;
					case 1: bulls.at(i)->Move(0, speedPl * time); break;
					case 2: bulls.at(i)->Move(-speedPl * time, 0); break;
					case 3: bulls.at(i)->Move(speedPl * time, 0); break;
				}
			}
		}

		for (int i = 0; i < bulls2.size(); i++)
		{
			if (bulls2.at(i)->exists)
			{
				switch (bulls2.at(i)->direction)
				{
				case 0: bulls2.at(i)->Move(0, -speedPl * time); break;
				case 1: bulls2.at(i)->Move(0, speedPl * time); break;
				case 2: bulls2.at(i)->Move(-speedPl * time, 0); break;
				case 3: bulls2.at(i)->Move(speedPl * time, 0); break;
				}
			}
		}

		// Check bullet clock(reload)
		if (player.canShoot == false) 
		{
			if (bulletClock.getElapsedTime().asSeconds() > 0.4)
			{
				if (nextBullet <= totalBullets) nextBullet++;
				if (nextBullet > totalBullets) { 
					bulls.resize(0);
					nextBullet = 0; 
				}
				player.canShoot = true;
				bulletClock.restart();
			}
		}

		if (enemy.canShoot == false)
		{
			if (bulletClock2.getElapsedTime().asSeconds() > 0.4)
			{
				if (nextBullet2 <= totalBullets) nextBullet2++;
				if (nextBullet2 > totalBullets) {
					bulls2.resize(0);
					nextBullet2 = 0;
				}
				enemy.canShoot = true;
				bulletClock2.restart();
			}
		}

		//std::cout<< player.sprite.getPosition().x<<std::endl;
	}

	return 0;
}
