#include <SFML/Graphics.hpp> // SFML grafik kütüphanesi
#include <vector>            // Birden fazla düşmanı tutmak için vector kullanacağız
#include <string>            // String işlemleri için

int main()
{
    // Oyun penceresi oluşturuyoruz
    sf::RenderWindow window(sf::VideoMode(800, 600), "Galaga Clone");


    window.setFramerateLimit(1500);


    // =========================
    // OYUNCU OLUŞTURMA
    // =========================

    // Oyuncu için dikdörtgen şekil oluşturuyoruz
    sf::RectangleShape player(sf::Vector2f(80.f, 20.f));

    // Oyuncunun rengini yeşil yapıyoruz
    player.setFillColor(sf::Color::Green);

    // Oyuncunun başlangıç pozisyonu
    player.setPosition(360.f, 520.f);



    // =========================
    // MERMİLER
    // =========================

    // Birden fazla mermi saklamak için vector
    std::vector<sf::RectangleShape> bullets;

    bool spacePressed = false;

    int score = 0;

    // =========================
    // FONT VE SKOR YAZISI
    // =========================

// Font
    sf::Font font;

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return -1;
    }

    // Yazı
    sf::Text scoreText;

    // Font ayarla
    scoreText.setFont(font);

    // Başlangıç yazısı
    scoreText.setString("Score: 0");

    // Yazı boyutu
    scoreText.setCharacterSize(24);

    // Yazı rengi
    scoreText.setFillColor(sf::Color::White);

    // Yazı pozisyonu
    scoreText.setPosition(620.f, 15.f);

    // =========================
    // DÜŞMANLAR
    // =========================

    // Birden fazla düşmanı saklamak için vector kullanıyoruz
    std::vector<sf::RectangleShape> enemies;

    // Düşmanların hareket yönü
    float enemySpeed = 0.2f;

    // Sağ mı gidiyorlar?
    bool moveRight = true;

    // 5 tane düşman oluşturacağız
    for (int i = 0; i < 5; i++)
    {
        // Her düşman için dikdörtgen oluştur
        sf::RectangleShape enemy(sf::Vector2f(50.f, 30.f));

        // Düşman rengi mavi
        enemy.setFillColor(sf::Color::Blue);

        // Düşmanların ekrandaki pozisyonları
        // i arttıkça düşmanlar sağa doğru dizilecek
        enemy.setPosition(120.f + i * 120.f, 80.f);

        // Düşmanı vector içine ekle
        enemies.push_back(enemy);
    }



    // =========================
    // ANA OYUN DÖNGÜSÜ
    // =========================

    while (window.isOpen())
    {
        sf::Event event;

        // Penceredeki olayları kontrol eder
        while (window.pollEvent(event))
        {
            // X tuşuna basılırsa pencere kapanır
            if (event.type == sf::Event::Closed)
                window.close();
        }



        // =========================
        // OYUNCU HAREKETİ
        // =========================

        // A tuşu basılıysa sola git
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(-0.5f, 0.f);

        // D tuşu basılıysa sağa git
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(0.5f, 0.f);



        // =========================
        // ATEŞ ETME
        // =========================

        // Space'e şu an basılıyor mu?
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            // Space'e ilk kez basıldıysa
            if (!spacePressed)
            {
                // Yeni mermi oluştur
                sf::RectangleShape bullet(sf::Vector2f(5.f, 20.f));

                bullet.setFillColor(sf::Color::Red);

                // Mermiyi oyuncunun üstüne yerleştir
                bullet.setPosition(
                    player.getPosition().x + 37.f,
                    player.getPosition().y
                );

                // Mermiyi vector içine ekle
                bullets.push_back(bullet);

                // Space basılı olarak işaretlenir
                spacePressed = true;
            }
        }
        else
        {
            // Space bırakıldıysa tekrar ateş edilebilir
            spacePressed = false;
        }



        // =========================
        // MERMİ HAREKETİ
        // =========================

        // Tüm mermileri hareket ettir
        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].move(0.f, -0.7f);

            // Ekrandan çıkan mermiyi sil
            if (bullets[i].getPosition().y < 0)
            {
                bullets.erase(bullets.begin() + i);
            }
        }


        // =========================
        // ÇARPIŞMA KONTROLÜ
        // =========================

        for (int i = 0; i < bullets.size(); i++)
        {
            for (int j = 0; j < enemies.size(); j++)
            {
                // Mermi düşmana değdi mi?
                if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
                {
                    // Düşmanı sil
                    enemies.erase(enemies.begin() + j);

                    // Skoru artır
                    score += 10;

                    // Mermiyi sil
                    bullets.erase(bullets.begin() + i);

                    break;
                }
            }
        }

// =========================
// DÜŞMAN HAREKETİ
// =========================

// Tüm düşmanları hareket ettir
        for (int i = 0; i < enemies.size(); i++)
        {
            // Eğer sağa gidiyorlarsa
            if (moveRight)
            {
                enemies[i].move(enemySpeed, 0.f);
            }
            else
            {
                enemies[i].move(-enemySpeed, 0.f);
            }
        }



        // Kenara değdiler mi kontrolü
        for (int i = 0; i < enemies.size(); i++)
        {
            // Sağ kenar
            if (enemies[i].getPosition().x > 750)
            {
                moveRight = false;
            }

            // Sol kenar
            if (enemies[i].getPosition().x < 0)
            {
                moveRight = true;
            }
        }

        // Skor yazısını güncelle
        scoreText.setString(
            "Score: " + std::to_string(score)
        );

        // =========================
        // EKRANI ÇİZME
        // =========================

        // Ekranı siyaha temizle
        window.clear();



        // =========================
        // SKOR KUTUSU
        // =========================

        // Skor için kutu
        sf::RectangleShape scoreBox(sf::Vector2f(160.f, 50.f));

        scoreBox.setFillColor(sf::Color(50, 50, 50));

        scoreBox.setPosition(610.f, 10.f);

        // Kutuyu çiz
        window.draw(scoreBox);

        // Yazıyı çiz
        window.draw(scoreText);




        // Oyuncuyu çiz
        window.draw(player);



        
        // Tüm mermileri çiz
        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i]);
        }



        // Tüm düşmanları tek tek çiz
        for (int i = 0; i < enemies.size(); i++)
        {
            window.draw(enemies[i]);
        }



        // Skoru pencere başlığında göster
        window.setTitle(
            "Galaga Clone - Score: " + std::to_string(score)
        );

        // Çizilen her şeyi ekrana göster
        window.display();
    }

    return 0;
}