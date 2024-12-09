#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace conf {
    // Window configuration
    const int window_x = 1480;
    const int window_y = 960;
    const int max_framelimit = 60;
    const float dt = 1.0f / 20.0f;
    const int handle_interval = 1;

    // Particle configuration
    const float particle_radius = 1.0f;
    const float particle_mass = 1.0f;
    const sf::Color particle_color = sf::Color::Yellow;

    // Simulation configuration
    const sf::Vector2f gravity = {0.0f, 9.81f};
    const int row_number = 50;
    const int col_number = 100;
    const float rest_distance = 10.0f;
    const float start_x = window_x / 4.0f;
    const float start_y = window_y / 4.0f;

    // 
    const float tear_click_tolerance = 10.0f;
    const float tear_max_radius = 2.5f;
    const float drag_max_distance = 50.0f;
    const float drag_max_force = 50.0f;
    
    sf::Clock clock;
};


