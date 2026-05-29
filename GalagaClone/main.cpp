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

    // Oyuncunun can sayısı
    int lives = 3;

    // Oyuncu vuruldu mu kontrolü
    bool playerGotShot = false;
    
    bool levelComplete = false;
    int levelCompleteTimer = 0;

    // Kaçıncı seviyede olduğumuzu tutar
    int level = 1;

    // Oyunun hangi ekranda olduğunu tutuyoruz
    enum GameState
    {
        MENU,
        KEYS,
        GAMEPLAY_INFO,
        PLAYING,
        GAME_OVER,
        YOU_WIN
    };

    // Oyun ilk açıldığında menüden başlayacak
    GameState currentState = MENU;

    // Menüde seçili olan satır
    int selectedMenuItem = 0;

    // Menüde tuş basılı tutulunca çok hızlı geçiş olmasın diye kontrol değişkenleri
    bool upPressed = false;
    bool downPressed = false;

    // Enter tuşu basılı tutulunca seçim tekrar tekrar çalışmasın diye
    bool enterPressed = false;
    
    // R tuşu basılı tutulunca tekrar tekrar çalışmasın
    bool rPressed = false;

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

    // Can yazısı
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setString("Lives: 3");
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(20.f, 15.f);


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
    // MENÜ YAZILARI
    // =========================

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("GALAGA CLONE");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(220.f, 100.f);

    sf::Text playText;
    playText.setFont(font);
    playText.setString("PLAY");
    playText.setCharacterSize(35);
    playText.setPosition(330.f, 220.f);

    sf::Text keysText;
    keysText.setFont(font);
    keysText.setString("KEYS");
    keysText.setCharacterSize(35);
    keysText.setPosition(330.f, 280.f);

    sf::Text gameplayText;
    gameplayText.setFont(font);
    gameplayText.setString("GAMEPLAY");
    gameplayText.setCharacterSize(35);
    gameplayText.setPosition(330.f, 340.f);

    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("QUIT");
    quitText.setCharacterSize(35);
    quitText.setPosition(330.f, 400.f);

    // =========================
    // KEYS EKRANI YAZILARI
    // =========================

    sf::Text keysInfoText;
    keysInfoText.setFont(font);
    keysInfoText.setString("KEYS\n\nA  - Move Left\nD  - Move Right\nSPACE - Shoot\n\nPress ESC to return menu");
    keysInfoText.setCharacterSize(28);
    keysInfoText.setFillColor(sf::Color::White);
    keysInfoText.setPosition(180.f, 140.f);

    // =========================
    // GAMEPLAY EKRANI YAZILARI
    // =========================

    sf::Text gameplayInfoText;
    gameplayInfoText.setFont(font);
    gameplayInfoText.setString(
        "GAMEPLAY\n\n"
        "Destroy all enemies to complete the level.\n"
        "Enemy bullets reduce your lives.\n"
        "If your lives reach 0, the game is over.\n"
        "Each destroyed enemy gives 10 points.\n\n"
        "Press ESC to return menu"
    );
    gameplayInfoText.setCharacterSize(26);
    gameplayInfoText.setFillColor(sf::Color::White);
    gameplayInfoText.setPosition(90.f, 120.f);

    // =========================
    // GAME OVER EKRANI
    // =========================

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString(
        "GAME OVER\n\n"
        "Press R to Retry\n"
        "Press ESC for Menu"
    );
    gameOverText.setCharacterSize(35);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(220.f, 200.f);

    // =========================
    // YOU WIN EKRANI
    // =========================

    sf::Text youWinText;
    youWinText.setFont(font);
    youWinText.setString(
        "YOU WIN\n\n"
        "Press R to Play Again\n"
        "Press ESC for Menu"
    );
    youWinText.setCharacterSize(35);
    youWinText.setFillColor(sf::Color::White);
    youWinText.setPosition(200.f, 200.f);


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

        // KEYS veya GAMEPLAY ekranındayken ESC ile menüye dön
        if (currentState == KEYS || currentState == GAMEPLAY_INFO)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                currentState = MENU;
            }
        }

        // GAME OVER ekranında R ile yeniden başlat
        if (currentState == GAME_OVER)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                if (!rPressed)
                {
                    // Oyun değişkenlerini sıfırla
                    score = 0;
                    lives = 3;
                    level = 1;

                    playerGotShot = false;
                    levelComplete = false;

                    gameOverTimer = 0;
                    levelCompleteTimer = 0;

                    enemyShootTimer = 0;

                    bullets.clear();
                    enemyBullets.clear();

                    enemies.clear();
                    enemyBaseY.clear();

                    // Level 1 düşmanlarını yeniden oluştur
                    for (int i = 0; i < 5; i++)
                    {
                        sf::RectangleShape enemy(sf::Vector2f(40.f, 22.f));

                        enemy.setFillColor(sf::Color::Blue);

                        enemy.setPosition(120.f + i * 120.f, 80.f);

                        enemies.push_back(enemy);

                        enemyBaseY.push_back(80.f);
                    }

                    // Oyuncuyu başlangıç konumuna taşı
                    player.setPosition(360.f, 520.f);

                    // Oyuna geri dön
                    currentState = PLAYING;

                    rPressed = true;
                }
            }
            else
            {
                rPressed = false;
            }
        }

        // GAME OVER ekranında ESC ile menüye dön
        if (currentState == GAME_OVER)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                currentState = MENU;
            }
        }


        // =========================
        // MENÜ KONTROLÜ
        // =========================

        if (currentState == MENU)
        {
            // Yukarı ok tuşuna basıldıysa
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (!upPressed)
                {
                    selectedMenuItem--;

                    // En üstten yukarı çıkarsa en alta geçsin
                    if (selectedMenuItem < 0)
                    {
                        selectedMenuItem = 3;
                    }

                    upPressed = true;
                }
            }
            else
            {
                upPressed = false;
            }

            // Aşağı ok tuşuna basıldıysa
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (!downPressed)
                {
                    selectedMenuItem++;

                    // En alttan aşağı inerse en üste geçsin
                    if (selectedMenuItem > 3)
                    {
                        selectedMenuItem = 0;
                    }

                    downPressed = true;
                }
            }
            else
            {
                downPressed = false;
            }
            // Enter tuşuna basıldıysa seçili menü elemanını çalıştır
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (!enterPressed)
                {
                    if (selectedMenuItem == 0)
                    {
                        // Yeni oyun başlatılırken temel değerleri sıfırlıyoruz
                        score = 0;
                        lives = 3;
                        level = 1;

                        playerGotShot = false;
                        levelComplete = false;

                        gameOverTimer = 0;
                        levelCompleteTimer = 0;
                        enemyShootTimer = 0;

                        // Ekrandaki eski mermileri temizliyoruz
                        bullets.clear();
                        enemyBullets.clear();

                        // Eski düşmanları temizliyoruz
                        enemies.clear();
                        enemyBaseY.clear();

                        // Level 1 düşmanlarını yeniden oluşturuyoruz
                        for (int i = 0; i < 5; i++)
                        {
                            sf::RectangleShape enemy(sf::Vector2f(40.f, 22.f));

                            enemy.setFillColor(sf::Color::Blue);

                            enemy.setPosition(120.f + i * 120.f, 80.f);

                            enemies.push_back(enemy);

                            enemyBaseY.push_back(80.f);
                        }

                        // Oyuncuyu başlangıç konumuna taşıyoruz
                        player.setPosition(360.f, 520.f);

                        // Level complete yazısını tekrar Level 1'e göre ayarlıyoruz
                        levelCompleteText.setString("LEVEL " + std::to_string(level) + " COMPLETE");

                        // Oyunu başlatıyoruz
                        currentState = PLAYING;
                    }

                    // KEYS seçiliyse tuşlar ekranına geç
                    if (selectedMenuItem == 1)
                    {
                        currentState = KEYS;
                    }

                    // GAMEPLAY seçiliyse oyun açıklaması ekranına geç
                    if (selectedMenuItem == 2)
                    {
                        currentState = GAMEPLAY_INFO;
                    }

                    // QUIT seçiliyse pencereyi kapat
                    if (selectedMenuItem == 3)
                    {
                        window.close();
                    }

                    enterPressed = true;
                }
            }
            else
            {
                enterPressed = false;
            }
        }

        // Eğer oyun PLAYING durumunda değilse oyun kodlarını çalıştırma
        if (currentState != PLAYING)
        {
            window.clear();

            if (currentState == MENU)
            {
                playText.setFillColor(selectedMenuItem == 0 ? sf::Color::Green : sf::Color::White);
                keysText.setFillColor(selectedMenuItem == 1 ? sf::Color::Green : sf::Color::White);
                gameplayText.setFillColor(selectedMenuItem == 2 ? sf::Color::Green : sf::Color::White);
                quitText.setFillColor(selectedMenuItem == 3 ? sf::Color::Green : sf::Color::White);

                window.draw(titleText);
                window.draw(playText);
                window.draw(keysText);
                window.draw(gameplayText);
                window.draw(quitText);
            }

            if (currentState == KEYS)
            {
                window.draw(keysInfoText);
            }

            if (currentState == GAMEPLAY_INFO)
            {
                window.draw(gameplayInfoText);
            }

            if (currentState == GAME_OVER)
            {
                window.draw(gameOverText);
            }

            if (currentState == YOU_WIN)
            {
                window.draw(youWinText);
            }


            window.display();
            continue;
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
                // Oyuncunun canını 1 azaltıyoruz
                lives--;

                // Eğer can kalmadıysa oyun bitiyor
                if (lives <= 0 && !playerGotShot)
                {
                    playerGotShot = true;

                    // Game over sayacı başlıyor
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

        // Eğer 3. seviyeye geçildiyse düşmanlar daha da sık ateş etsin
        if (level == 3)
        {
            enemyShootLimit = 220;
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

        if (level == 3)
        {
            enemySpacing = 80.f;
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
            // Seviye tamamlandıktan yaklaşık 3 saniye sonra yeni seviyeye geç
            if (levelComplete && levelCompleteTimer > 3900)
            {
                // Eğer 3. seviye bittiyse oyuncu oyunu kazandı
                if (level == 3)
                {
                    currentState = YOU_WIN;
                }
                else
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

                    // Yeni seviyedeki düşman sayısı ve aralık değişkenleri
                    int enemyCount = 5;
                    float startX = 120.f;
                    float spacing = 120.f;

                    if (level == 2)
                    {
                        enemyCount = 7;
                        startX = 60.f;
                        spacing = 100.f;
                    }

                    if (level == 3)
                    {
                        enemyCount = 9;
                        startX = 40.f;
                        spacing = 80.f;
                    }

                    // Yeni seviyenin düşmanlarını oluşturuyoruz
                    for (int i = 0; i < enemyCount; i++)
                    {
                        sf::RectangleShape enemy(sf::Vector2f(40.f, 22.f));

                        enemy.setFillColor(sf::Color::Blue);

                        enemy.setPosition(startX + i * spacing, 80.f);

                        enemies.push_back(enemy);

                        enemyBaseY.push_back(80.f);
                    }

                    // Yeni level yazısını güncelliyoruz
                    levelCompleteText.setString("LEVEL " + std::to_string(level) + " COMPLETE");
                }
            }


        // Eğer oyuncu vurulduysa süre saymaya başla
        if (playerGotShot)
        {
            gameOverTimer++;
        }

        // Yaklaşık 3 saniye sonra Game Over ekranına geç
        if (playerGotShot && gameOverTimer > 3900)
        {
            currentState = GAME_OVER;
        }

        // Skor yazısını güncelle
        scoreText.setString(
            "Score: " + std::to_string(score)
        );

        livesText.setString(
            "Lives: " + std::to_string(lives)
        );


        // =========================
        // EKRANI ÇİZME
        // =========================

        // Ekranı siyaha temizle
        window.clear();

        

        // =========================
        // CAN KUTUSU
        // =========================

        // Can sayısı için kutu
        sf::RectangleShape livesBox(sf::Vector2f(160.f, 40.f));

        // Kutu rengi
        livesBox.setFillColor(sf::Color(50, 50, 50));

        // Sol üst köşeye yerleştiriyoruz
        livesBox.setPosition(10.f, 10.f);


        // =========================
        // SKOR KUTUSU
        // =========================

        // Skor için kutu
        sf::RectangleShape scoreBox(sf::Vector2f(160.f, 40.f));

        scoreBox.setFillColor(sf::Color(50, 50, 50));

        scoreBox.setPosition(610.f, 10.f);
         
        // Can kutusunu çiz
        window.draw(livesBox);

        // Can yazısını çiz
        window.draw(livesText);

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