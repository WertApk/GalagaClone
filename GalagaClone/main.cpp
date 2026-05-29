#include <SFML/Graphics.hpp> // SFML grafik kütüphanesi
#include <vector>            // Birden fazla düşmanı tutmak için vector kullanacağız
#include <string>            // String işlemleri için
#include <cmath>             // Sinüs fonksiyonunu kullanmak için
#include <cstdlib>           // Rastgele sayı üretmek için
#include <ctime>             // Rastgeleliği zamana bağlamak için



int main()
{
    // Rastgele sayı sistemini başlatıyoruz
    srand(time(0));
    
    // Oyun penceresi oluşturuyoruz
    sf::RenderWindow window(sf::VideoMode(800, 600), "Galaga Clone");


    window.setFramerateLimit(800);


    // =========================
    // OYUNCU OLUŞTURMA
    // =========================

    // Oyuncu için dikdörtgen şekil oluşturuyoruz
    sf::RectangleShape player(sf::Vector2f(65.f, 15.f));

    // Oyuncunun rengini yeşil yapıyoruz
    player.setFillColor(sf::Color::Green);

    // Oyuncunun başlangıç pozisyonu
    player.setPosition(360.f, 520.f);



    // =========================
    // MERMİLER
    // =========================

    // Birden fazla mermi saklamak için vector
    std::vector<sf::RectangleShape> bullets;

    // Düşmanların attığı mermileri tutar
    std::vector<sf::RectangleShape> enemyBullets;

    bool spacePressed = false;

    int score = 0;

    // Oyuncu vuruldu mu kontrolü
    bool playerGotShot = false;
    
    bool levelComplete = false;
    int levelCompleteTimer = 0;

    // Kaçıncı seviyede olduğumuzu tutar
    int level = 1;

    // Oyuncu vurulduktan sonra geçen süreyi sayacağız
    int gameOverTimer = 0;

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

    // Oyuncu vurulunca ekranda gösterilecek yazı
    sf::Text hitText;
    hitText.setFont(font);
    hitText.setString("You Got Shot");
    hitText.setCharacterSize(40);
    hitText.setFillColor(sf::Color::Red);
    hitText.setPosition(270.f, 280.f);

    // Seviye tamamlanınca ekranda gösterilecek yazı
    sf::Text levelCompleteText;
    levelCompleteText.setFont(font);
    levelCompleteText.setString("LEVEL " + std::to_string(level) + " COMPLETE");
    levelCompleteText.setCharacterSize(40);
    levelCompleteText.setFillColor(sf::Color::Green);
    levelCompleteText.setPosition(180.f, 280.f);


    // =========================
    // DÜŞMANLAR
    // =========================

    // Birden fazla düşmanı saklamak için vector kullanıyoruz
    std::vector<sf::RectangleShape> enemies;

    // Düşmanların başlangıç Y konumlarını saklıyoruz
    // Çünkü sinüs hareketinde düşmanları bu başlangıç yüksekliğinin etrafında oynatacağız
    std::vector<float> enemyBaseY;

    // Düşmanların hareket yönü
    float enemySpeed = 0.2f;

    // Sağ mı gidiyorlar?
    bool moveRight = true;

    // Dalga hareketi için zaman değişkeni
    // Bu değer oyun çalıştıkça artacak ve sinüs hareketini oluşturacak
    float waveTime = 0.f;

    // Düşmanların belli aralıklarla ateş etmesi için sayaç
    int enemyShootTimer = 0;

    // 5 tane düşman oluşturacağız
    for (int i = 0; i < 5; i++)
    {
        // Her düşman için dikdörtgen oluştur
        sf::RectangleShape enemy(sf::Vector2f(40.f, 22.f));

        // Düşman rengi mavi
        enemy.setFillColor(sf::Color::Blue);

        // Düşmanların ekrandaki pozisyonları
        // i arttıkça düşmanlar sağa doğru dizilecek
        enemy.setPosition(120.f + i * 120.f, 80.f);

        // Düşmanı vector içine ekle
        enemies.push_back(enemy);

        // Bu düşmanın başlangıç Y konumunu kaydediyoruz
        // Şu an tüm düşmanlar 80.f yüksekliğinde başladığı için 80.f ekliyoruz
        enemyBaseY.push_back(80.f);
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
        {
            // Oyuncu ekranın solundan çıkmasın
            if (player.getPosition().x > 0)
            {
                player.move(-0.5f, 0.f);
            }
        }

        // D tuşu basılıysa sağa git
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // Oyuncu ekranın sağından çıkmasın
            if (player.getPosition().x < 735)
            {
                player.move(0.5f, 0.f);
            }
        }



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
            bullets[i].move(0.f, -0.4f);

            // Ekrandan çıkan mermiyi sil
            if (bullets[i].getPosition().y < 0)
            {
                bullets.erase(bullets.begin() + i);
            }
        }


        // =========================
        // DÜŞMAN MERMİ HAREKETİ
        // =========================

        for (int i = 0; i < enemyBullets.size(); i++)
        {
            // Düşman mermileri aşağı doğru gider
            enemyBullets[i].move(0.f, 0.3f);

            // Ekranın altından çıkan düşman mermisini sil
            if (enemyBullets[i].getPosition().y > 600)
            {
                enemyBullets.erase(enemyBullets.begin() + i);
            }
        }

        // =========================
        // DÜŞMAN MERMİSİ - OYUNCU ÇARPIŞMASI
        // =========================

        for (int i = 0; i < enemyBullets.size(); i++)
        {
            // Düşman mermisi oyuncuya değdi mi?
            if (enemyBullets[i].getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                // Eğer oyuncu ilk kez vurulduysa
                if (!playerGotShot)
                {
                    playerGotShot = true;

                    // Sayaç sadece ilk vurulmada sıfırlanır
                    gameOverTimer = 0;
                }

                enemyBullets.erase(enemyBullets.begin() + i);
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

                    // Düşmanın başlangıç Y bilgisini de siliyoruz
                    enemyBaseY.erase(enemyBaseY.begin() + j);

                    

                    // Skoru artır
                    score += 10;

                    // Mermiyi sil
                    bullets.erase(bullets.begin() + i);

                    break;
                }
            }
        }

        // Dalga hareketi için zamanı artırıyoruz
        // Bu değer arttıkça std::sin() farklı sonuçlar üretir
        waveTime += 0.005f;


        // =========================
        // DÜŞMAN ATEŞ ETME
        // =========================

        // Sayaç her frame artar
        enemyShootTimer++;

        // Düşmanların ateş etme sıklığını seviyeye göre ayarlıyoruz
        int enemyShootLimit = 900;

        // Eğer 2. seviyeye geçildiyse düşmanlar daha sık ateş etsin
        if (level == 2)
        {
            enemyShootLimit = 300;
        }

        // Eğer sayaç belirli bir değere ulaştıysa ve hala düşman varsa
        if (enemyShootTimer >= enemyShootLimit && enemies.size() > 0)
        {
            // Rastgele bir düşman seçiyoruz
            int randomEnemyIndex = rand() % enemies.size();

            // Seçilen düşmandan mermi oluşturuyoruz
            sf::RectangleShape enemyBullet(sf::Vector2f(5.f, 20.f));

            // Düşman mermisinin rengi sarı
            enemyBullet.setFillColor(sf::Color::Yellow);

            // Mermiyi seçilen düşmanın alt orta kısmından başlatıyoruz
            enemyBullet.setPosition(
                enemies[randomEnemyIndex].getPosition().x + 18.f,
                enemies[randomEnemyIndex].getPosition().y + 22.f
            );

            // Mermiyi vector içine ekliyoruz
            enemyBullets.push_back(enemyBullet);

            // Sayacı sıfırlıyoruz
            enemyShootTimer = 0;
        }


        // =========================
        // DÜŞMAN HAREKETİ
        // =========================

        // Tüm düşmanları hareket ettir
        for (int i = 0; i < enemies.size(); i++)
        {
            // Eğer düşmanlar sağa gidiyorsa X ekseninde sağa hareket ettiriyoruz
            if (moveRight)
            {
                enemies[i].move(enemySpeed, 0.f);
            }
            else
            {
                // Eğer sağa gitmiyorlarsa sola hareket ettiriyoruz
                enemies[i].move(-enemySpeed, 0.f);
            }

            // Sinüs fonksiyonu -1 ile +1 arasında değer üretir
            // Bu değeri 20 ile çarparak düşmanı 20 piksel yukarı-aşağı oynatıyoruz
            // waveTime değiştikçe hareket sürekli devam eder
            // + i ekleyerek her düşmanın dalga hareketini biraz farklı başlatıyoruz
            float waveOffset = std::sin(waveTime + i) * 25.f;

            // Düşmanın X konumunu koruyoruz
            // Y konumunu ise başlangıç Y değeri + sinüs dalgası yapıyoruz
            enemies[i].setPosition(
                enemies[i].getPosition().x,
                enemyBaseY[i] + waveOffset
            );
        }

        // =========================
        // DÜŞMAN BOŞLUK KAPATMA
        // =========================

        // Seviyeye göre düşmanlar arası olması gereken mesafe
        float enemySpacing = 120.f;

        if (level == 2)
        {
            enemySpacing = 100.f;
        }

        // Düşmanlar arasında boşluk oluştuysa bunu yavaşça kapatıyoruz
        for (int i = 1; i < enemies.size(); i++)
        {
            // Bir önceki düşmana göre olması gereken X konumu
            float targetX = enemies[i - 1].getPosition().x + enemySpacing;

            // Eğer arada normalden fazla boşluk varsa sağdaki düşman sola doğru yaklaşır
            if (enemies[i].getPosition().x > targetX)
            {
                enemies[i].move(-0.1f, 0.f);
            }
        }

        // =========================
        // KENAR KONTROLÜ
        // =========================

        // Düşmanlar arasında hâlâ boşluk var mı?
        bool hasGap = false;

        for (int i = 1; i < enemies.size(); i++)
        {
            if (enemies[i].getPosition().x - enemies[i - 1].getPosition().x > enemySpacing + 5.f)
            {
                hasGap = true;
            }
        }

        // Kenara değdiler mi kontrolü
        for (int i = 0; i < enemies.size(); i++)
        {
            // Sağ kenara gelindiyse ama arada boşluk varsa hemen yön değiştirme
            if (enemies[i].getPosition().x > 750 && !hasGap)
            {
                moveRight = false;
            }

            // Sol kenara gelindiyse ama arada boşluk varsa hemen yön değiştirme
            if (enemies[i].getPosition().x < 0 && !hasGap)
            {
                moveRight = true;
            }
        }

        // Eğer hiç düşman kalmadıysa ve seviye daha önce tamamlanmadıysa
        if (enemies.size() == 0 && !levelComplete)
        {
            // Seviyeyi tamamlandı olarak işaretliyoruz
            levelComplete = true;

            // Sayaç sıfırlanıyor
            levelCompleteTimer = 0;
        }

        // Seviye tamamlandıysa süre saymaya başla
        if (levelComplete)
        {
            levelCompleteTimer++;
        }

        // Seviye tamamlandıktan yaklaşık 3 saniye sonra yeni seviyeye geç
        if (levelComplete && levelCompleteTimer > 3900)
        {
            // Seviyeyi 1 artırıyoruz
            level++;

            // Level complete durumunu kapatıyoruz
            levelComplete = false;

            // Sayaç sıfırlanıyor
            levelCompleteTimer = 0;

            // Eski mermileri temizliyoruz
            bullets.clear();
            enemyBullets.clear();

            // Düşmanları ve başlangıç Y bilgilerini temizliyoruz
            enemies.clear();
            enemyBaseY.clear();

            // Yeni seviyede daha fazla düşman oluşturuyoruz
            for (int i = 0; i < 7; i++)
            {
                sf::RectangleShape enemy(sf::Vector2f(40.f, 22.f));

                enemy.setFillColor(sf::Color::Blue);

                enemy.setPosition(60.f + i * 100.f, 80.f);

                enemies.push_back(enemy);

                enemyBaseY.push_back(80.f);
            }

            // Yeni level yazısını güncelliyoruz
            levelCompleteText.setString("LEVEL " + std::to_string(level) + " COMPLETE");
        }


        // Eğer oyuncu vurulduysa süre saymaya başla
        if (playerGotShot)
        {
            gameOverTimer++;
        }

        // Eğer oyuncu vurulduysa süre saymaya başla
        if (playerGotShot)
        {
            gameOverTimer++;
        }

        // Yaklaşık 3 saniye sonra oyun döngüsünden çık
        if (playerGotShot && gameOverTimer > 3900)
        {
			continue;// Eğer break olursa oyun bitince pencere kapanır, ama böyle oyun donup pencere açık kalır
        }

        // Skor yazısını güncelle
        scoreText.setString(
            "Score: " + std::to_string(score)
        );

        // Yaklaşık 3 saniye sonra oyunu durdur
		// Oyun 1300 fps e limitli old için 3900 sayısı yaklaşık 3 saniyeye denk gelir
        if (playerGotShot && gameOverTimer > 3900)
        {
            break;
        }


        // =========================
        // EKRANI ÇİZME
        // =========================

        // Ekranı siyaha temizle
        window.clear();



        // =========================
        // SKOR KUTUSU
        // =========================

        // Skor için kutu
        sf::RectangleShape scoreBox(sf::Vector2f(160.f, 40.f));

        scoreBox.setFillColor(sf::Color(50, 50, 50));

        scoreBox.setPosition(610.f, 10.f);

        // Kutuyu çiz
        window.draw(scoreBox);

        // Yazıyı çiz
        window.draw(scoreText);

        // Eğer oyuncu vurulduysa uyarı yazısını çiz
        if (playerGotShot)
        {
            window.draw(hitText);
        }


        // Oyuncuyu çiz
        window.draw(player);

        // Seviye tamamlandıysa LEVEL COMPLETE yazısını çiz
        if (levelComplete)
        {
            window.draw(levelCompleteText);
        }

        
        // Tüm mermileri çiz
        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i]);
        }

        // Tüm düşman mermilerini çiz
        for (int i = 0; i < enemyBullets.size(); i++)
        {
            window.draw(enemyBullets[i]);
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