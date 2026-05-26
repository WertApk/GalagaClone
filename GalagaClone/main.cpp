#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Galaga Clone");

    // Oyuncu gemisi
    sf::RectangleShape player(sf::Vector2f(80.f, 20.f));

    player.setFillColor(sf::Color::Green);

    // Ekranın altına yerleştir
    player.setPosition(360.f, 520.f);

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

        // Hareket
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-0.3f, 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(0.3f, 0.f);
        }

        window.clear();

        // Oyuncuyu çiz
        window.draw(player);

        window.display();
    }

    return 0;
}