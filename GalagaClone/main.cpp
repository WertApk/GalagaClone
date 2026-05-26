#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Galaga Clone");

    // Oyuncu
    sf::RectangleShape player(sf::Vector2f(80.f, 20.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(360.f, 520.f);

    // Mermi
    sf::RectangleShape bullet(sf::Vector2f(5.f, 20.f));
    bullet.setFillColor(sf::Color::Red);

    bool bulletActive = false;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Oyuncu hareketi
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-0.5f, 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(0.5f, 0.f);
        }

        // Ateş etme
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bulletActive)
        {
            bullet.setPosition(
                player.getPosition().x + 37.f,
                player.getPosition().y
            );

            bulletActive = true;
        }

        // Mermi hareketi
        if (bulletActive)
        {
            bullet.move(0.f, -0.7f);

            // Ekrandan çıkınca sıfırla
            if (bullet.getPosition().y < 0)
            {
                bulletActive = false;
            }
        }

        window.clear();

        window.draw(player);

        if (bulletActive)
        {
            window.draw(bullet);
        }

        window.display();
    }

    return 0;
}