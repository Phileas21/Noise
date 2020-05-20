#include <iostream>
#include <SFML/Graphics.hpp>

float smooth(float x)
{
    return 6 * x * x * x * x * x - 15 * x * x * x * x + 10 * x * x * x;
}

uint32_t noise(uint64_t x, uint32_t seed)
{
    x ^= seed << 21;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    x *= UINT64_C(0x2545F4914F6CDD1D);
    x = x >> 32;
    return static_cast<uint32_t>(x);
}

float octave(float x, int amplitude, float period, int seed)
{
    int x0 = static_cast<int>( x / period );
    if(x < 0)
        x0--;

    int p0 = noise(x0, seed) % amplitude;
    int p1 = noise(x0 + 1, seed) % amplitude;

    float t = x / float(period) - float(x0);

    return float(p0) - smooth(t) * float(p0 - p1);

}

void update(sf::VertexArray &v, int x, int seed)
{
    sf::Clock clock;

    v.clear();
    for (int i(0); i < 960; i++)
    {
        float h = octave(i + x, 300, 200, seed) + octave(i + x, 100, 50, seed) + octave(i + x, 10, 10, seed);
        h += 90;
        v.append(sf::Vertex(sf::Vector2f(i, h), sf::Color::Blue));
    }
    std::cout << "Noise drawn in : " << clock.getElapsedTime().asMicroseconds();
    std::cout << " microseconds" << std::endl;
}


int main()
{
    std::cout << "Espace: redessiner le bruit" << std::endl;
    std::cout << "Gauche et Droite pour se deplacer" << std::endl;

    sf::RenderWindow mWindow(sf::VideoMode(16 * 60, 9 * 60), "Noise");

    sf::VertexArray line(sf::LineStrip);

    int x(0);
    int seed(rand());
    update(line, x, seed);

    while (mWindow.isOpen())
    {
        sf::Event event{};
        while (mWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                seed = rand();
                x = 0;
                update(line, x, seed);
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right)
            {
                x += 100;
                update(line, x, seed);
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left)
            {
                x -= 100;
                update(line, x, seed);
            }

        }
        mWindow.clear(sf::Color(25, 25, 25));
        mWindow.draw(line);
        mWindow.display();
        sf::sleep(sf::seconds(1.f / 60));
    }
}

/*
     sf::Clock clock;
    clock.restart();
    unsigned int seed = rand();
    int x(0);
    while (x * period < 1000)
    {
        double p0, p1;
        p0 = noise(x, seed) % 500;
        p1 = noise(x + 1, seed) % 500;


        for (int i(0); i < period; i++)
        {
            float t = float(i) / float(period);
            auto h = p0 - smooth(t) * (p0 - p1);
            sf::Vector2f position(x * period + i, h);
            v.append(sf::Vertex(position, sf::Color(0, 0, 255)));
        }
        x++;
    }
    std::cout << "Noise drawn in : " << clock.getElapsedTime().asMicroseconds();
    std::cout << " microseconds" << std::endl;
}
 */