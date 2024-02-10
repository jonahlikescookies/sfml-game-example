#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>
#include <vector>
#include <random>

class BULLET {
    public:
        sf::Rect<int> collider;
        sf::Vector2f pos;
        sf::Vector2f velocity;
        sf::Sprite sprite;
    public:
        void update() {
            (*this).sprite.move((*this).velocity);
            (*this).pos = (*this).sprite.getPosition();
            (*this).collider.left = (*this).pos.x;
            (*this).collider.top = (*this).pos.y;
        }
};

class ENEMY {
    public:
        sf::Rect<int> collider;
        sf::Vector2f pos;
        sf::Vector2f velocity;
        sf::Sprite sprite;
        bool dead;
    public:
        void update() {
            (*this).sprite.move((*this).velocity);
            (*this).pos = (*this).sprite.getPosition();
            (*this).collider.left = (*this).pos.x;
            (*this).collider.top = (*this).pos.y;
        }
};

int main(int argc, const char* argv[]) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> x(0, 1920);
    std::uniform_int_distribution<> y(0, 1080);
    std::uniform_int_distribution<> scoreToAdd(150, 351);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "The Battle of Terranova");
    sf::Texture texture;
    texture.loadFromFile("src/Player.png");
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("src/Enemy.png");
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("src/Bullet.png");
    std::vector<ENEMY> enemies;
    std::vector<BULLET> bullets;
    char layout = 'm';
    sf::Font roboto;
    roboto.loadFromFile("src/roboto.ttf");
    sf::Clock enemyTimer;
    sf::Clock scoreTimer;
    sf::Clock bulletTimer;
    sf::Text scoreText;
    scoreText.setCharacterSize(100);
    scoreText.setFont(roboto);
    scoreText.setString("Score: 0");
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setFillColor(sf::Color::White);
    sf::Text title;
    title.setFont(roboto);
    title.setString("The Battle of Terranova");
    title.setCharacterSize(150);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    int level = 0;
    sf::Sprite playerSprite;
    playerSprite.setTexture(texture);
    sf::Vector2f velocity(0, 0);
    sf::Vector2f pos(-960, -540);
    window.setFramerateLimit(60);
    sf::Text startText;
    startText.setFont(roboto);
    startText.setString("Press any key to begin.");
    startText.setFillColor(sf::Color::White);
    startText.setCharacterSize(80);
    startText.setOrigin(-600, -600);
    sf::Texture gameOverBGTexture;
    gameOverBGTexture.loadFromFile("src/Backdrop-2_Zarael_Captured.png");
    sf::Sprite gameOverBG;
    gameOverBG.setTexture(gameOverBGTexture);
    sf::Texture bgTexture;
    bgTexture.loadFromFile("src/Backdrop-1_Alien_Invaders_Arrive.png");
    sf::Sprite bg;
    bg.setTexture(bgTexture);
    int score = 0;
    int scoreFromEnemies = 0;
    sf::Rect<float> playerRect{pos, sf::Vector2f{128.0, 128.0}};
    while (window.isOpen()) {
        sf::Event event;
        score = scoreFromEnemies + floor(scoreTimer.getElapsedTime().asSeconds());
        scoreText.setString("Score: " + std::to_string(score));
        if (enemyTimer.getElapsedTime().asSeconds() >= 5) {
            enemyTimer.restart();
            for (int i = 0; i < ceil(level/10); i++) {
                int x1 = x(generator);
                int y2 = y(generator);
                enemies.push_back(ENEMY{sf::Rect<int>{x1, y2, 128, 128}, sf::Vector2f{float(x1), float(y2)}, sf::Vector2f{0,0}, sf::Sprite{enemyTexture}, false});
                enemies.back().sprite.setPosition(enemies.back().pos);  
                if (enemies.back().collider.intersects(sf::Rect<int>{int(lroundf(playerRect.left)), int(lroundf(playerRect.top)), int(lroundf(playerRect.width)), int(lroundf(playerRect.height))})) {
                    enemies.back().sprite.setPosition(x(generator), y(generator));
                }
            }
        }
        while (window.pollEvent(event)) {
            sf::Vector2u size = playerSprite.getTexture()->getSize();
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                switch (int(event.text.unicode)) {
                    case 22:
                        velocity.y = -7;
                        break;
                    case 0:
                        playerSprite.setTextureRect(sf::IntRect(size.x, 0, -size.x, size.y));
                        velocity.x = -7;
                        break;
                    case 18:
                        velocity.y = 7;
                        break;
                    case 3:
                        playerSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
                        velocity.x = 7;
                        break;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (layout == 'm') layout = 'g';
                else if (layout == 'o') layout = 'm';
                else {
                    switch (event.text.unicode) {
                        case 22:
                        case 18:
                            velocity.y = 0;
                            break;
                        case 0:
                        case 3:
                            velocity.x = 0;
                            break;
                    }
                }
            }    
        }
        window.clear();
        switch (layout) {
            case 'g':
                {
                    playerSprite.move(velocity);
                    pos = playerSprite.getPosition();
                    playerRect.left = pos.x;
                    playerRect.top = pos.y;
                    window.draw(playerSprite);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && bulletTimer.getElapsedTime().asMilliseconds() >= 250) {
                        bulletTimer.restart();
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        double angle = atan2(mousePos.y-pos.y, mousePos.x-pos.x);
                        bullets.push_back(BULLET{sf::Rect<int>{int(lroundf(pos.x+128)), int(lroundf(pos.y+128)), 30, 8}, sf::Vector2f{pos.x+128, pos.y+128}, sf::Vector2f{float(30.0*float(cos(angle))), float(30.0*float(sin(angle)))}, sf::Sprite{bulletTexture}});
                        bullets.back().sprite.setPosition(bullets.back().pos);
                    }
                    printf("ENEMY COUNT: %d\n", enemies.size());
                    for(int i = 0; i < bullets.size(); i++) {
                        bullets[i].update();
                        window.draw(bullets[i].sprite);
                    }
                    int enemiesAlive = 0;
                    for(int i = 0; i < enemies.size(); i++) {
                        if (!enemies[i].dead) {
                            enemiesAlive++;
                            printf("PLAYER POS: (%f, %f)\nPLAYER RECT POS: (%f, %f)\nPLAYER SPRITE POS: (%f, %f)\nENEMY SPRITE POS: (%f, %f)\n", pos.x, pos.y, playerRect.left, playerRect.top, playerSprite.getPosition().x, playerSprite.getPosition().y, enemies[i].sprite.getPosition().x, enemies[i].sprite.getPosition().y);
                            if (enemies[i].sprite.getPosition().y > pos.y) {
                                enemies[i].velocity.y = -1;
                            } else {
                                enemies[i].velocity.y = 1;
                            }
                            if (enemies[i].sprite.getPosition().x > pos.x) {
                                enemies[i].velocity.x = -1;
                            } else {
                                enemies[i].velocity.x = 1;
                            }
                            printf("VELOCITY: sf::Vector2f(%f, %f)\n", enemies[i].velocity.x, enemies[i].velocity.y);
                            enemies[i].update();
                            for(auto bullet : bullets){
                                if (enemies[i].collider.intersects(bullet.collider)) {
                                    enemies[i].dead = true;
                                    int s = scoreToAdd(generator);
                                    scoreFromEnemies+=s-(s%50);
                                }
                            }
                            if (enemies[i].collider.intersects(sf::Rect<int>{int(lroundf(playerRect.left)), int(lroundf(playerRect.top)), int(lroundf(playerRect.width)), int(lroundf(playerRect.height))})) {
                                layout = 'o';
                                level = 1;
                                scoreTimer.restart();
                                score = 0;
                                scoreFromEnemies = 0;
                                enemies.clear();
                                bullets.clear();
                                playerSprite.setPosition(sf::Vector2f{0,0});
                                break;
                            }
                            window.draw(enemies[i].sprite);
                        }
                    }
                    if (!enemiesAlive) {
                        level++;
                    }

                    window.draw(scoreText);
                    break;
                }
            case 'm':
                window.draw(bg);
                window.draw(title);
                window.draw(startText);
                break;
            case 'o':
                window.draw(gameOverBG);
                break;
        }
        window.display();
    }

    return 0;
}
