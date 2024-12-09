// #include "particle.hpp"
#include <cmath>

class Constraint
{
public:
    Particle *p1, *p2;
    float initial_length;
    bool active;

    Constraint(Particle *p1, Particle *p2)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->initial_length = std::hypotf(p2->position.x - p1->position.x, p2->position.y - p1->position.y);
        this->active = true;
    }

    void Satisfy()
    {
        if (!active)
        {
            return;
        }

        sf::Vector2f delta = p2->position - p1->position;
        float current_length = std::hypotf(delta.x, delta.y);
        float difference = (current_length - initial_length) / current_length;
        sf::Vector2f correction = delta * difference * 0.5f;

        if (!p1->is_pinned) p1->position += correction;
        if (!p2->is_pinned) p2->position -= correction;
    }

    void Deactivate()
    {
        active = false;
    }
};