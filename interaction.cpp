#include "interaction.h"
#include "map.h"
Engine::Engine()
{
	
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	//текстура карты
	
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	s_map.setTexture(map);//заливаем текстуру спрайтом
	allImage.loadFromFile("images/robots.png");
	//загрузили изображения

	clock.restart(); //перезагружает время

	////
	//Создаем объект героя
	Hero = new Player(allImage, 500, 500, 70, 80, "hero");
	////

	////
	//Создаем объект Enemy
	Enemy *anotherEnemy = new Enemy(allImage, 200, 200, 80, 80, "flybot", TileMapMy);
    enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
	
	//Bullet* anotherBullet = new Bullet(allImage, 100, 100, 16, 16, Hero->GetRotation(), "HeroBullet");
	//bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila

	GameOver = false;
}

Engine::~Engine()
{
	delete Hero;
	enemies.clear();
	bullets.clear();
}

void Engine::start()
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "Game", sf::Style::Fullscreen);
	float timer = 0;
	float gunTimer = 0;
	while (window.isOpen() && !GameOver)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {//если нажата клавиша 
			//	GameOver = true;
			//}
		}
		time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		timer += time;
		gunTimer += time;
 		//
		if (timer > 10000 && enemies.size()<4)
		{
			Enemy* anotherEnemy = new Enemy(allImage, 200, 200, 80, 80, "flybot",TileMapMy);
			enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
			timer = 0;
		}

		Hero->update(time, TileMapMy, event);
		sf::Vector2f HeroXY = Hero->GetgunXY();
		if (gunTimer > 1000 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Bullet* anotherBullet = new Bullet(allImage, HeroXY.x, HeroXY.y, 16, 16, Hero->GetRotation(), "HeroBullet");
			bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila
			gunTimer = 0;
		}
		sf::Vector2f EnemyXY;
		std::vector<Enemy>::iterator iterEnemies = enemies.begin();
		std::vector<Bullet>::iterator iterBullet = bullets.begin();
		while (iterBullet != bullets.end())
		{
			if (iterBullet->isAlive())
			{
				iterBullet->update(time, TileMapMy);
				++iterBullet;
			}
			else
			{
				iterBullet = bullets.erase(iterBullet); //стереть из списка
			}
		}
    	while (iterEnemies != enemies.end()) 
		{
			if (iterEnemies->isAlive())
			{
				iterEnemies->SetAim(Hero->GetXY());
				iterEnemies->update(time, TileMapMy);
				//EnemyXY = iterEnemies->GetXY();

				iterBullet = bullets.begin();
				while (iterBullet != bullets.end())
				{
					if (iterBullet->isAlive())
					{
						if (iterBullet->GetRect().intersects(iterEnemies->GetRect()))
						{
							iterEnemies->struck(iterBullet->GetDamage());
						}
						++iterBullet;
					}
					else
					{
						iterBullet = bullets.erase(iterBullet); //стереть из списка
					}
				}

				if(Hero->GetRect().intersects(iterEnemies->GetRect()))
				{
					iterEnemies->struck(100);
					
					if (iterEnemies->GetStatus() != "AlahuuuAkbar" && iterEnemies->GetStatus() != "killed")
					{
						Hero->struck(20);
					}
					iterEnemies->SetStatus("AlahuuuAkbar");
					
				}
				++iterEnemies;
				//if (!enemies.empty())
			}
			else 
			{
				iterEnemies = enemies.erase(iterEnemies); //стереть из списка
			}
			//if (!bullets.empty())
			//{
			//	iterBullet = bullets.begin();
			//	while (iterBullet != bullets.end())
			//	{
			//		if (iterBullet->isAlive())
			//		{
			//			iterBullet->update(time, TileMapMy);
			//			if (iterBullet->GetRect().intersects(iterEnemies->GetRect()))
			//			{
			//				iterEnemies->struck(iterBullet->GetDamage());
			//			}
			//			++iterBullet;
			//		}
			//		else
			//		{
			//			iterBullet = bullets.erase(iterBullet); //стереть из списка
			//		}
			//	}
			//}
			
		}
		GameOver = !Hero->isAlive();
		//drawing ->
		window.clear();
		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMapMy[i][j] == ' ')  s_map.setTextureRect(sf::IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			else if (TileMapMy[i][j] == '0')  s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));//если встретили символ 0, то рисуем 2й квадратик
			else if (TileMapMy[i][j] == 'b') s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));//если встретили символ b, то рисуем 3й квадратик
			else if (TileMapMy[i][j] == 'p') s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));//если встретили символ p, то рисуем 4й квадратик
			else if (TileMapMy[i][j] == 'h') s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));//если встретили символ h, то рисуем 5й квадратик
			else s_map.setTextureRect(sf::IntRect(160, 0, 32, 32));
			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. 
			//то есть задает каждому из них позицию.
			window.draw(s_map);//рисуем квадратики на экран
		}
		//window.draw(robotsprite);
		//window.draw(Hero->sprite);
		Hero->draw(window);
		iterEnemies = enemies.begin();
    	while (iterEnemies != enemies.end()) {
    		window.draw(iterEnemies->sprite);
			++iterEnemies;
    	}
		iterBullet = bullets.begin();
		while (iterBullet != bullets.end()) {
			window.draw(iterBullet->sprite);
			++iterBullet;
		}
		//window.draw(MyEnemy->sprite);
		window.display();
	}
}
