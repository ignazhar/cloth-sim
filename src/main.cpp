#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
// #include "configuration.hpp"
// #include "particle.hpp"
// #include "constraint.hpp"
#include "input_handler.hpp"

void Initialize(std::vector<Particle> &particles, std::vector<Constraint> &constraints)
{
    // Initialize particles
    for (int row = 0; row < conf::row_number; row ++)
    {
        for (int col = 0; col < conf::col_number; col ++)
        {
            float x = conf::start_x + col * conf::rest_distance;
            float y = conf::start_y + row * conf::rest_distance;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    // Initialize constraints
    auto GetIndex = [](int row, int col) -> int
    {
        return row * conf::col_number + col;
    };

    for (int row = 0; row < conf::row_number; row ++)
    {
        for (int col = 0; col < conf::col_number; col ++)
        {
            if (row + 1 < conf::row_number)
            {
                constraints.emplace_back(&particles[GetIndex(row, col)], &particles[GetIndex(row + 1, col)]);
            }
            if (col + 1 < conf::col_number)
            {
                constraints.emplace_back(&particles[GetIndex(row, col)], &particles[GetIndex(row, col + 1)]);
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(conf::window_x, conf::window_y), "Lets go");
    window.setFramerateLimit(conf::max_framelimit);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;
    
    Initialize(particles, constraints);    

    int last_handle = -conf::handle_interval;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
                window.close();

            // Handle mouse clicks
            // InputHandler::HandleMouseClick(event, particles, constraints);
            // if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::Button() == sf::Mouse::Left)
                // InputHandler::TearCloth((sf::Mouse::getPosition()).x, (sf::Mouse::getPosition()).y, particles, constraints);
            int time_ms = conf::clock.getElapsedTime().asMilliseconds();
            if (time_ms - last_handle > conf::handle_interval)
            {
                InputHandler::HandleMouse(window, particles, constraints);
                last_handle = time_ms;
            }
        }

        // Apply gravity and update particles
        for (Particle &particle : particles)
        {
            particle.ApplyForce(conf::gravity);
            particle.Update(conf::dt);
            particle.ConstraintToBoundsPoint(conf::window_x, conf::window_y);
            // std::cout << particle.position.x << ' ' << particle.position.y << '\n';
        }

        // Satisfy constraints
        for (int i = 0; i < 10; i ++)
        {   
            for (Constraint &constraint : constraints)
            {
                constraint.Satisfy();
            }
        }

        window.clear(sf::Color::Black);

        // Draw particles as balls
        // for (const Particle &particle : particles)
        // {
        //     sf::CircleShape circle(conf::particle_radius);
        //     circle.setFillColor(conf::particle_color);
        //     circle.setPosition(particle.position.x - conf::particle_radius, particle.position.y - conf::particle_radius);
        //     window.draw(circle);
        // }

        // Draw particles as points
        for (const Particle &particle : particles)
        {
            sf::Vertex point(particle.position, conf::particle_color);
            window.draw(&point, 1, sf::Points);
        }

        // Draw constraints as lines
        for (const Constraint &constraint : constraints)
        {
            if (!constraint.active)
                continue;
            
            sf::Vertex line[] =
            {
                sf::Vertex(constraint.p1->position, sf::Color::White),
                sf::Vertex(constraint.p2->position, sf::Color::White),
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
    return 0;
}
