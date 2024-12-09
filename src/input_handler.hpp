#include <SFML/Graphics.hpp>
#include "particle.hpp"
#include "constraint.hpp"
#include "configuration.hpp"
#include <vector>
#include <iostream>

class InputHandler
{
public:
    static void HandleMouseClick(const sf::Event &event, std::vector<Particle> &particles, std::vector<Constraint> &constraints)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            float mouse_x = static_cast<float>(event.mouseButton.x);
            float mouse_y = static_cast<float>(event.mouseButton.y);
            TearCloth(mouse_x, mouse_y, particles, constraints);
        }
    }
    static void HandleMouse(const sf::Window &window, std::vector<Particle> &particles, std::vector<Constraint> &constraints)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f relative_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            float mouse_x = relative_position.x;
            float mouse_y = relative_position.y;
            DragCloth(mouse_x, mouse_y, particles, constraints);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2f relative_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            float mouse_x = relative_position.x;
            float mouse_y = relative_position.y;
            TearCloth(mouse_x, mouse_y, particles, constraints);
        }
    }
private:
    static float GetDistance(float mouse_x, float mouse_y, const Constraint &constraint)
    {
        float mid_x = (constraint.p1->position.x + constraint.p2->position.x) * 0.5f;
        float mid_y = (constraint.p1->position.y + constraint.p2->position.y) * 0.5f;
        
        float dx = mid_x - mouse_x;
        float dy = mid_y - mouse_y;

        return std::sqrt(dx * dx + dy * dy);
    }

    static Constraint *FindNearestConstraint(float mouse_x, float mouse_y, const std::vector<Constraint> &constraints)
    {
        Constraint *nearest = nullptr;
        float min_distance = conf::tear_click_tolerance;
        
        for (const Constraint &constraint : constraints)
        {
            float distance = GetDistance(mouse_x, mouse_y, constraint);

            if (distance < min_distance)
            {
                min_distance = distance;
                nearest = const_cast<Constraint*>(&constraint);
            }
        }

        return nearest;
    }

    static void TearCloth(float mouse_x, float mouse_y, const std::vector<Particle> &particles, std::vector<Constraint> &constraints)
    {
        for (Constraint &constraint : constraints)
        {
            float distance = GetDistance(mouse_x, mouse_y, constraint);

            if (distance <= conf::tear_max_radius)
            {
                constraint.Deactivate();
            }
        }
        // Constraint *nearest = FindNearestConstraint(mouse_x, mouse_y, constraints);
        // if (nearest)
        // {
        //     nearest->Deactivate();
        // }
    }



    static void DragCloth(float mouse_x, float mouse_y, std::vector<Particle> &particles, std::vector<Constraint> &constraints)
    {
        for (Particle &particle : particles)
        {
            float dx = particle.position.x - mouse_x;
            float dy = particle.position.y - mouse_y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance > conf::drag_max_distance)
                continue;
            

            float coeff = (conf::drag_max_distance - distance) / conf::drag_max_distance;

            float drag_force = conf::drag_max_force * coeff;

            sf::Vector2f normalized_vector = sf::Vector2f(mouse_x, mouse_y) - particle.position;
            // normalized_vector /= distance; 

            sf::Vector2f force_vector = normalized_vector * drag_force;

            // std::cout << "try : " << force_vector.x << ' ' << force_vector.y << '\n';
            // std::cout << sf::Time::asMilliseconds << '\n';
            // std::cout << conf::clock.getElapsedTime().asMilliseconds() << '\n';

            particle.ApplyForce(force_vector);
        }
    }
};