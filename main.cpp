#include <iostream>
#include <SFML/Graphics.hpp>

struct octaveParam
{
    octaveParam(int amp, float per) : amplitude(amp), period(per) {}
    int amplitude;
    float period;
};

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
    if (x < 0)
        x0--;

    int p0 = noise(x0, seed) % amplitude;
    int p1 = noise(x0 + 1, seed) % amplitude;

    float t = x / float(period) - float(x0);

    return float(p0) - smooth(t) * float(p0 - p1);
}

void update(sf::VertexArray &v, int x, int seed, std::vector<octaveParam> &parameters)
{
    v.clear();
    for (int i(0); i < 960; i++)
    {
        float h(0);
        for(int j(0); j< parameters.size(); j++)
            h += octave(i + x, parameters[j].amplitude, parameters[j].period, seed + j);

        h += 90;
        v.append(sf::Vertex(sf::Vector2f(i, h), sf::Color::Blue));
    }
}

int main()
{
    std::cout << "Espace: redessiner le bruit" << std::endl;
    std::cout << "Gauche et Droite pour se deplacer" << std::endl;

    sf::RenderWindow mWindow(sf::VideoMode(16 * 60, 9 * 60), "Noise");

    sf::VertexArray line(sf::LineStrip);
    std::vector<octaveParam> parameters;
    parameters.emplace_back(300,200);
    parameters.emplace_back(100,50);
    parameters.emplace_back(10,10);

    int x(0);
    int seed(rand());
    update(line, x, seed,parameters);

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
                sf::Clock clock;
                update(line, x, seed,parameters);
                std::cout << "Noise drawn in : " << clock.getElapsedTime().asMicroseconds();
                std::cout << " microseconds" << std::endl;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right)
            {
                x += 250;
                update(line, x, seed, parameters);
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left)
            {
                x -= 250;
                update(line, x, seed, parameters);
            }
            if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R))
            {
                unsigned int o;
                parameters.clear();

                std::cout<<"Octave count:";
                std::cin>>o;

                for(int i(0); i<o; i++)
                {
                    int a;
                    float p;
                    std::cout<<"Octave "<<i+1<<std::endl;
                    std::cout<<"Amplitude :";
                    std::cin>>a;
                    std::cout<<"Period :";
                    std::cin>>p;
                    parameters.emplace_back(a,p);
                    update(line, x, seed,parameters);
                }
            }

        }
        mWindow.clear(sf::Color(25, 25, 25));
        mWindow.draw(line);
        mWindow.display();
        sf::sleep(sf::seconds(1.f / 60));
    }
}