#include <SFML/Graphics.hpp>

// nice colors: blue(61, 183, 228), orange(255, 136, 73), green(105, 190, 40)

// Particle & Box Discrete Collision Dettection by tagberli

struct Box {
    sf::RectangleShape top_edge;
    sf::RectangleShape bottom_edge;
    sf::RectangleShape right_edge;
    sf::RectangleShape left_edge;

    Box(float height, float width, float thickness, float posx = 0, float posy = 0, sf::Color col = sf::Color::White) {
        top_edge.setSize(sf::Vector2f(width, thickness));
        top_edge.setPosition(posx, posy);

        bottom_edge.setSize(sf::Vector2f(width, thickness));
        bottom_edge.setPosition(posx, posy + height - thickness);

        left_edge.setSize(sf::Vector2f(thickness, height - 2 * thickness));
        left_edge.setPosition(posx, posy + thickness);

        right_edge.setSize(sf::Vector2f(thickness, height - 2 * thickness));
        right_edge.setPosition(posx + width - thickness, posy + thickness);

        top_edge.setFillColor(col);
        bottom_edge.setFillColor(col);
        right_edge.setFillColor(col);
        left_edge.setFillColor(col);

    }

    void display(sf::RenderWindow& window) {
        window.draw(top_edge);
        window.draw(bottom_edge);
        window.draw(left_edge);
        window.draw(right_edge);
    }
};

struct Particle {
    sf::CircleShape circle;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    
    Particle(float radius, sf::Vector2f pos, sf::Vector2f v, sf::Vector2f a, sf::Color col = sf::Color::White) {
        circle.setRadius(radius);
        circle.setPosition(pos);
        circle.setFillColor(col);
        velocity = v;
        acceleration = a;
        
    }

    void update(float delta_time) {
        velocity = velocity + acceleration * delta_time;
        circle.move(velocity * delta_time);
    }

    void display(sf::RenderWindow & window, float delta_time) {
        update(delta_time);
        window.draw(circle);
    }
};

void handleBoxCollision(Particle & p, Box b) {
    float plx = p.circle.getPosition().x;
    float prx = p.circle.getPosition().x + 2 * p.circle.getRadius();
    float pty = p.circle.getPosition().y;
    float pby = p.circle.getPosition().y + 2 * p.circle.getRadius();

    float blx = b.left_edge.getPosition().x + b.left_edge.getSize().x;
    float brx = b.right_edge.getPosition().x;
    float bty = b.top_edge.getPosition().y + b.top_edge.getSize().y;
    float bby = b.bottom_edge.getPosition().y;
    
    if (plx <= blx || prx >= brx) {
        p.velocity.x = -p.velocity.x;
    }
    if (pby >= bby || pty <= bty) {
        p.velocity.y = -p.velocity.y;
    }
}



int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Collision Example");


    Box myBox(1000, 1000, 10, 0, 0, sf::Color(61, 183, 228));
    sf::Vector2f pos0(500 - 10, 500 - 10);
    sf::Vector2f pos1(100, 100);
    Particle p1(10.0f, pos0, sf::Vector2f(100, 100), sf::Vector2f(0, 100), sf::Color(255, 136, 73));
    Particle p2(10.0f, pos0 + pos1, sf::Vector2f(-200, 0), sf::Vector2f(0, 200), sf::Color(105, 190, 40));


    // Main loop
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float delta_time = clock.restart().asSeconds();

        window.clear();
        p1.display(window, delta_time);
        handleBoxCollision(p1, myBox);

        p2.display(window, delta_time);
        handleBoxCollision(p2, myBox);

        myBox.display(window);
        window.display();
    }

    return 0;
}
