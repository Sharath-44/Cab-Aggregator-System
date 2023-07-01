#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include<iomanip>
#include <thread>

// Structure to store destination coordinates
struct Coordinates {
    double x;
    double y;
};

// Class representing a Cab
class Cab {
public:
    double x;
    double y;
    bool available;

public:
    // Constructor
    Cab(double xCoord, double yCoord)
        : x(xCoord), y(yCoord), available(true) {}

    // Function to set cab's availability status
    void setAvailability(bool status) {
        available = status;
    }

    // Function to update cab's position randomly around the user's location
    void updatePosition(double userX, double userY) {
        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> disX(userX - 20, userX + 20);
        std::uniform_real_distribution<> disY(userY - 20, userY + 20);

        // Update the cab's position not by more than 20 units
        x = disX(gen);
        y = disY(gen);

        // Sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

// Class representing a User
class User {
public:
    double x;
    double y;

public:
    // Constructor
    User(double xCoord, double yCoord)
        : x(xCoord), y(yCoord) {}
};

// Function to display live table of cab positions
void displayCabPositions(const std::vector<Cab>& cabs) {
    std::cout << "Live Cab Positions:" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << std::setw(10) << std::left << "Driver"
              << std::setw(10) << std::left << "Car"
              << std::setw(10) << std::left << "Position" << std::endl;
    std::cout << std::setw(10) << std::left << "------"
              << std::setw(10) << std::left << "---"
              << std::setw(10) << std::left << "--------" << std::endl;

    // Display cab positions in the table
    for (const Cab& cab : cabs) {
        std::cout << std::setw(10) << std::left << "Cab"
                  << std::setw(10) << std::left << "Model"
                  << "(" << std::setw(2) << std::right << cab.x << ", " << std::setw(2) << std::right << cab.y << ")" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Create and store cab details
    std::vector<Cab> cabs;
    cabs.push_back(Cab(100, 100));
    cabs.push_back(Cab(200, 200));

    // Create user object
    User user(150, 150);

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Cab Booking Simulator");

    // Flag to determine if the ride request is active
    bool rideRequested = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (rideRequested) {
            // Clear the window
            window.clear(sf::Color::White);

            // Update cab positions randomly around the user's location
            for (Cab& cab : cabs) {
                cab.updatePosition(user.x, user.y);
            }

            // Draw cabs as squares
            for (const Cab& cab : cabs) {
                sf::RectangleShape cabShape(sf::Vector2f(20, 20));
                cabShape.setPosition(cab.x, cab.y);
                cabShape.setFillColor(sf::Color::Blue);
                window.draw(cabShape);
            }

            // Draw user as a circle
            sf::CircleShape userShape(10);
            userShape.setPosition(user.x, user.y);
            userShape.setFillColor(sf::Color::Green);
            window.draw(userShape);

            // Display the window
            window.display();
        } else {
            std::cout << "Enter '1' to request a ride: ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                rideRequested = true;
                std::cout << "Ride requested!" << std::endl;
                std::cout << "Waiting for available cabs..." << std::endl;

                // Display live table of cab positions
                displayCabPositions(cabs);
            }
        }
    }

    return 0;
}
