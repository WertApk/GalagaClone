#include <SFML/Graphics.hpp> // SFML grafik kütüphanesi
#include <vector>            // Birden fazla düşmanı tutmak için vector kullanacağız

int main()
{
    // Oyun penceresi oluşturuyoruz
    sf::RenderWindow window(sf::VideoMode(800, 600), "Galaga Clone");



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
    // MERMİ OLUŞTURMA
    // =========================

    // Mermi için küçük dikdörtgen
    sf::RectangleShape bullet(sf::Vector2f(5.f, 20.f));

    // Merminin rengi kırmızı
    bullet.setFillColor(sf::Color::Red);

    // Mermi aktif mi kontrolü
    bool bulletActive = false;



    // =========================
    // DÜŞMANLAR
    // =========================

    // Birden fazla düşmanı saklamak için vector kullanıyoruz
    std::vector<sf::RectangleShape> enemies;

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

        // Space'e basılırsa ve ekranda mermi yoksa
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bulletActive)
        {
            // Mermiyi oyuncunun üstüne yerleştir
            bullet.setPosition(
                player.getPosition().x + 37.f,
                player.getPosition().y
            );

            // Mermiyi aktif yap
            bulletActive = true;
        }



        // =========================
        // MERMİ HAREKETİ
        // =========================

        if (bulletActive)
        {
            // Mermiyi yukarı hareket ettir
            bullet.move(0.f, -0.7f);

            // Eğer ekranın dışına çıktıysa
            if (bullet.getPosition().y < 0)
            {
                // Mermiyi kapat
                bulletActive = false;
            }
        }

        // =========================
        // ÇARPIŞMA KONTROLÜ
        // =========================

        // Eğer mermi aktifse düşmanlara çarpıp çarpmadığını kontrol et
        if (bulletActive)
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                // Merminin sınırları ile düşmanın sınırları kesişiyor mu?
                if (bullet.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
                {
                    // Çarpılan düşmanı listeden sil
                    enemies.erase(enemies.begin() + i);

                    // Mermiyi de pasif yap
                    bulletActive = false;

                    // Düşman silindiği için döngüden çık
                    break;
                }
            }
        }


        // =========================
        // EKRANI ÇİZME
        // =========================

        // Ekranı siyaha temizle
        window.clear();



        // Oyuncuyu çiz
        window.draw(player);



        // Mermi aktifse çiz
        if (bulletActive)
            window.draw(bullet);



        // Tüm düşmanları tek tek çiz
        for (int i = 0; i < enemies.size(); i++)
        {
            window.draw(enemies[i]);
        }



        // Çizilen her şeyi ekrana göster
        window.display();
    }

    return 0;
}