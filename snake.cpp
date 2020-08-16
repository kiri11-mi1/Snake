#include <SFML\Graphics.hpp>
#include "TileMap.h"
#include <ctime>
#include <iostream>
#include <sstream>

using namespace sf;

int size(53);
int w = size*W;
int h = size * H;

int dir = -1, num = 1;
int score = 0;

struct {
	int x; int y;
} snake[100];

struct {
	int x; int y;
} fruit;

void fruitPosition() {
	fruit.x = rand() % 18 + 1;
	fruit.y = rand() % 10 + 1;
	if (Map[fruit.y][fruit.x] == ' ' ) Map[fruit.y][fruit.x] = 'F'; 
}
void interaction() {
	////////////////Удар головы о стену//////////////////
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Map[i][j] == '0' && snake[0].x == j && snake[0].y == i) {
				score = 0;
				num = 1;
				snake[0].x = 3;
				snake[0].y = 3;
				dir = -1;
			}
		}
	}
	///////////////////////////////////////////////////////

	////////////////////Поедание фрукта///////////////////
	if (snake[0].x == fruit.x && snake[0].y == fruit.y) { 
		num++; 
		score++; 
		Map[fruit.y][fruit.x] = ' ';
		fruitPosition();
	}
	//////////////////////////////////////////////////////////

	////////////////////Самопоедание/////////////////////////
	for (int i = num; i > 0; i--) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			score = 0;
			num = 1;
			snake[0].x = 3;
			snake[0].y = 3;
			dir = -1;
		}
	}
	////////////////////////////////////////////////////////
}

void Tick() {
	for (int i = num; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
	if (dir == 0) snake[0].y += 1;//down
	if (dir == 1) snake[0].x -= 1;//left
	if (dir == 2) snake[0].x += 1;//right
	if (dir == 3) snake[0].y -= 1;//up
	interaction();
}

int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1055, 636), "Snake");

	///////////////////////Установка текста//////////////
	Font font;
	font.loadFromFile("font.otf");
	Text text("", font, 24);
	text.setStyle(sf::Text::Bold);
	////////////////////////////////////////////////////

	////////////////////Map////////////////////////
	Image mapImage;
	mapImage.loadFromFile("images/fone.png");
	mapImage.saveToFile("images/fone.png");
	Texture mapTexture;
	mapTexture.loadFromImage(mapImage);
	Sprite mapSprite;
	mapSprite.setTexture(mapTexture);
	///////////////////////////////////////////////

	///////////Fruit////////////////////
	fruitPosition();
	///////////////////////////////////

	////////////////////Player///////////////////
	Image playuerIm;
	playuerIm.loadFromFile("images/hero.png");
	playuerIm.saveToFile("images/hero.png");
	Texture playerT;
	playerT.loadFromImage(playuerIm);
	Sprite player(playerT);
	player.setTextureRect(IntRect(0,0,53,53));
	snake[0].x = snake[0].y = 3;
	////////////////////////////////////////////
	Clock clock; 
	float timer = 0, speed = 0.1;

	while (window.isOpen()) {

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) window.close();

		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))  dir = 0;
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))  dir = 1;
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) dir = 2;
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))    dir = 3;

		if (timer > speed) { timer = 0; Tick(); } //поэксперементировтаь

		window.clear();
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (Map[i][j] == '0') mapSprite.setTextureRect(IntRect(53,0,53,53));
				if (Map[i][j] == ' ') mapSprite.setTextureRect(IntRect(0, 0, 53, 53));
				if (Map[i][j] == 'F') mapSprite.setTextureRect(IntRect(107, 0, 53, 53));
				mapSprite.setPosition(j*53, i*53);
				window.draw(mapSprite);
			}
		}
		for (int i = 0; i < num; i++) {
			player.setPosition(snake[i].x * size, snake[i].y * size); 
			window.draw(player);
		}
		std::ostringstream scoreString;
		scoreString << score;
		text.setString("Score"+scoreString.str());
		text.setPosition(100,0);
		window.draw(text);
		window.display();
	}
	return 0;
} 