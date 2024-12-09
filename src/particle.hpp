#include <SFML/Graphics.hpp>

class Particle 
{
public:
    sf::Vector2f position;
    sf::Vector2f previous_position;
    sf::Vector2f acceleration;
    bool is_pinned;

    Particle(float x, float y, bool pinned = false)
    {
        position = previous_position = sf::Vector2f(x, y);
        acceleration = sf::Vector2f(0.0f, 0.0f);
        is_pinned = pinned;
    }

    void ApplyForce(const sf::Vector2f &force)
    {
        if (!is_pinned)
        {
            acceleration += force;
        }
    }

    void Update(const float dt)
    {
        if (!is_pinned)
        {
            // Verlet integration: solving for x(t+dt) in O(dt^4)
            sf::Vector2f save_position = position;
            position = 2.0f * position - previous_position + acceleration * dt * dt;
            previous_position = save_position;
            // Reset after update
            acceleration = sf::Vector2f(0.0f, 0.0f);
        }
    }

    void ConstraintToBoundsCircle(float width, float height, float radius)
    {
        if (position.x < radius) position.x = radius;
        if (position.y < radius) position.y = radius;
        if (position.x > width  - radius) position.x = width  - radius;
        if (position.y > height - radius) position.y = height - radius;
    }
    
    void ConstraintToBoundsPoint(float width, float height)
    {
        position.x = std::max(position.x, 0.0f);
        position.y = std::max(position.y, 0.0f);
        position.x = std::min(position.x, width);
        position.y = std::min(position.y, height);
    }
};