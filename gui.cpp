#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <map>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;

class Cab;
class User;

// Global variables
vector<Cab> cabs_; // Vector to store cab details

// Structure to store destination coordinates
struct Coordinates {
    double x;
    double y;
};

map<string, Coordinates> destinations = {
    {"mysore", {100, 200}},
    {"airport", {1000, 400}},
    {"market", {200, 400}},
    {"store", {50, 40}}
};

double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

string toLowercase(const string& str) {
    string lowercaseStr = str;
    transform(lowercaseStr.begin(), lowercaseStr.end(), lowercaseStr.begin(), ::tolower);
    return lowercaseStr;
}

class Cab {
public:
    string driverName;
    string carModel;
    string carNumber;
    double x;
    double y;
    bool available;

public:
    // Constructor
    Cab(const string& name, const string& model, const string& number, double xCoord, double yCoord)
        : driverName(name), carModel(model), carNumber(number), x(xCoord), y(yCoord), available(true) {}

    // Function to get cab's availability status
    bool isAvailable() {
        return available;
    }

    // Function to set cab's availability status
    void setAvailability(bool status) {
        available = status;
    }

    // Function to get cab's distance from user's location
    double getDistance(double userX, double userY) {
        return calculateDistance(x, y, userX, userY);
    }

    // Function to randomly accept or reject user request
    bool acceptRide() {
        // Random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        // Randomly decide to accept or reject ride
        return dis(gen) == 1;
    }

    // Function to update cab's position randomly around the user's location
    void updatePosition(double userX, double userY) {
        // Random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> disX(userX - 5, userX + 5);
        uniform_real_distribution<> disY(userY - 5, userY + 5);

        // Update the cab's position not by more than 5 units
        x = disX(gen);
        y = disY(gen);

        // Sleep for 1 second
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Friend function to access private members of Cab class
    friend void bookCab(User& user);
};

// Class representing a User
class User {
public:
    string name;
    double accountBalance;

public:
    // Constructor
    User(const string& userName, double balance)
        : name(userName), accountBalance(balance) {}

    // Function to book a cab
    void bookCab(double startX, double startY, const string& destinationName) {
        cout << "\nBooking a cab for " << name << endl;
        cout << "\nInitial Account Balance: " << accountBalance << endl;

        // Find the nearest available cab
        double minDistance = -1;
        Cab* nearestCab = nullptr;
        for (Cab& cab : cabs_) {
            if (cab.isAvailable()) {
                double distance = cab.getDistance(startX, startY);
                if (minDistance == -1 || distance < minDistance) {
                    minDistance = distance;
                    nearestCab = &cab;
                }
            }
        }

        // If a cab is available, ask for the user's confirmation
        if (nearestCab != nullptr) {
            cout << "Nearest Cab Details:" << endl;
            cout << "Driver Name: " << nearestCab->driverName << endl;
            cout << "Car Model: " << nearestCab->carModel << endl;
            cout << "Car Number: " << nearestCab->carNumber << endl;
            cout << "Distance from user: " << minDistance << endl;

            // Wait for some time to simulate the cab's response time
            this_thread::sleep_for(chrono::seconds(2));

            // Check if the cab accepts the ride
            if (nearestCab->acceptRide()) {
                nearestCab->setAvailability(false);

                // Get the destination coordinates from the map
                string lowercaseDestination = toLowercase(destinationName);
                Coordinates destinationCoords = destinations[lowercaseDestination];

                // Calculate fare based on distance. for the first 0.5 units, fare is 40. for every additional unit, fare is 100
                double fare = 40 + (calculateDistance(startX, startY, destinationCoords.x, destinationCoords.y) - 0.5) + 500;

                // double fare = calculateDistance(startX, startY, destinationCoords.x, destinationCoords.y) * 10;

                // Update user's account balance
                accountBalance -= fare;

                cout << "\nCab accepted the ride!" << endl;
                cout << "Destination: " << destinationName << endl;
                this_thread::sleep_for(chrono::seconds(1));

                cout << "Fare: " << fare << endl;
                cout << "Updated Account Balance: " << accountBalance << endl;
            } else {
                cout << "\n\nCab rejected the ride, trying again." << endl;
                // Remove the rejected cab from consideration
                nearestCab->setAvailability(false);
                // Try again with the next cab
                bookCab(startX, startY, destinationName);
            }
        } else {
            cout << "\n\nNo cabs available. Please try again later." << endl;
        }
    }
};
// function to display cabs as green squares and the user as a red circle using sfml, keep updating the positions of the cabs randomly for 10 seconds
void displayCabsAndUser(double userX, double userY) {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Cab Booking System");

    // Create a circle for the user
    sf::CircleShape userCircle(10);
    userCircle.setFillColor(sf::Color::Red);
    userCircle.setPosition(userX * 100, userY * 100);

    // Create a rectangle for each cab
    vector<sf::RectangleShape> cabRectangles;
    for (const Cab& cab : cabs_) {
        sf::RectangleShape cabRectangle(sf::Vector2f(20, 20));
        cabRectangle.setFillColor(sf::Color::Green);
        cabRectangle.setPosition(cab.x * 100, cab.y * 100);
        cabRectangles.push_back(cabRectangle);
    }

    // Create a clock to keep track of time
    sf::Clock clock;

    // Keep the window open till 10 seconds have passed
    while (clock.getElapsedTime().asSeconds() < 10) {
        // Check if the user has closed the window
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        if (clock.getElapsedTime().asSeconds() > 2) {
            for (sf::RectangleShape& cabRectangle : cabRectangles) {
                cabRectangle.setPosition(cabRectangle.getPosition().x + rand() % 3 - 4, cabRectangle.getPosition().y + rand() % 3 +4);
            }
            clock.restart();
        }
        // Clear the window
        window.clear();

        // Draw the user and cabs
        window.draw(userCircle);
        for (sf::RectangleShape& cabRectangle : cabRectangles) {
            window.draw(cabRectangle);
        }

        // Display the window
        window.display();
    }
}


// Function to display live table of cab positions 
void displayCabPositions() {
    cout << "Live Cab Positions:" << endl;
    cout << "-------------------" << endl;
    cout << setw(10) << left << "Driver"
         << setw(10) << left << "Car"
         << setw(10) << left << "Position" << endl;
    cout << setw(10) << left << "------"
         << setw(10) << left << "---"
         << setw(10) << left << "--------" << endl;



    // Display cab positions in the table
    for (const Cab& cab : cabs_) {
        cout << setw(10) << left << cab.driverName
             << setw(10) << left << cab.carModel
             << "(" << setw(2) << right << cab.x << ", " << setw(2) << right << cab.y << ")" << endl;
    }
    cout << endl;
}



int main() {
    // Create and store cab details
    cabs_.push_back(Cab("Samuel", "Sedan", "ABC123", 1, 1));
    cabs_.push_back(Cab("Sriram", "Nano", "DEF456", 2, 2));
    cabs_.push_back(Cab("Murali", "SUV", "GHI789", 3, 3));
    cabs_.push_back(Cab("Anirudh", "Sedan", "JKL012", 4, 4));
    cabs_.push_back(Cab("Shreyas", "Sedan", "MNO345", 5, 5));

    // User sign-up
    string userName;
    double accountBalance;
    cout << "Sign up for the Cab Booking Simulator\n\n" << endl;
    cout << "Enter your name: ";
    //handle invalid input for name it should not be a number
    while (!(cin >> userName && userName.length() > 0 ) && !isdigit(userName[0]) ) {
        cout << "Invalid input. Please enter a valid string Username: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    fflush(stdin);
    cout << "Enter your account balance: ";
    while (!(cin >> accountBalance && accountBalance > 500)) {
        cout << "Invalid input. Please enter a valid amount/value for your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Create user object
    User user(userName, accountBalance);

    // Get user's starting coordinates
    double startX, startY;
    cout << "Enter your current coordinates (x, y): ";
    //handle invalid input
    while (!(cin >> startX >> startY)) {
        cout << "Invalid input. Please enter a valid coordinate pair (x, y): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    //print the destinations available
    cout << "Destinations available: " << endl;
    for (auto& destination : destinations) {
        cout << destination.first << endl;
    }

    // Get destination name
    string destinationName;
    cout << "Enter your destination name: ";
    //if the destination is not in the map, ask the user to enter a valid destination name check if fare exceeds balance and if it does, display a message and try again
    while (!(cin >> destinationName) || destinations.find(toLowercase(destinationName)) == destinations.end()) {
        cout << "Invalid input. Please enter a valid destination name: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    //check if the fare exceeds the balance
    double fare = 40 + (calculateDistance(startX, startY, destinations[destinationName].x, destinations[destinationName].y) - 0.5) + 500;
    while (fare > accountBalance) {
        cout << "Your account balance is insufficient to book a cab to this destination\n. Please enter a greater balance amount: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> accountBalance;
        fare = 40 + (calculateDistance(startX, startY, destinations[destinationName].x, destinations[destinationName].y) - 0.5) + 100;
        //update account balance
        user.accountBalance = accountBalance;
    }

    // Display user details and ride information
    cout << endl;
    cout << "User Details:" << endl;
    cout << "-------------" << endl;
    cout << "Name: " << user.name << endl;
    cout << "Account Balance: " << user.accountBalance << endl;
    cout << endl;
    cout << "Ride Information:" << endl;
    cout << "----------------" << endl;
    cout << "Start Location: (" << startX << ", " << startY << ")" << endl;
    cout << "Destination: " << destinationName << endl;
    cout << "Fare Calculation: 40 Rs. for the first 0.5 + 100 Rs./unit" << endl;
    cout << endl;

    time_t startTime = time(nullptr);
    while (time(nullptr) - startTime < 10) {
        this_thread::sleep_for(chrono::seconds(2));
        for (Cab& cab : cabs_) {
            cab.updatePosition(startX, startY);
        }

        system("clear"); 
        displayCabPositions();
        displayCabsAndUser(startX, startY);
    }
    user.bookCab(startX, startY, destinationName);
    return 0;
}

