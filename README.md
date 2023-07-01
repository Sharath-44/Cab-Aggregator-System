
# Cab Booking Simulator

  

The Cab Booking Simulator is a command-line application written in C++. It simulates a cab booking service where users can book cabs to travel from their current location to a specified destination.

  

## Functionality

  

- Users can sign up and provide their name and initial account balance.

- Users can enter their current coordinates and choose a destination from the available options.

- The system finds the nearest available cab and displays its details, including driver name, car model, car number, and distance from the user.

- Users can confirm the booking and proceed with the ride.

- The fare is calculated based on the distance between the start and destination coordinates.

- If the cab accepts the ride, the user's account balance is updated, and the ride begins.

- The system simulates live cab positions and updates them every few seconds.

- After the ride is completed, the fare is displayed, and the updated account balance is shown.

- User data, consisting of the name and the account balance , is stored in a CSV file. New users are automatically added to the database and existing users have their account balance updated accordingly after a ride.

  

## Data Structures

  

-  `Cab`: Represents a cab with attributes like driver name, car model, car number, coordinates, and availability.

-  `User`: Represents a user with attributes like name and account balance.

-  `Coordinates`: A structure to store x and y coordinates for locations.

-  Additionally, the available destinations are stored as local environment variables whose values are their co-ordinates.

  

## Usage

  

1. Clone the repository: `git clone https://github.com/Sharath-44/Cab-Aggregator-System.git`

2. Compile the code: `g++ -o cab_booking_simulator CAB_AGGREGATOR_SYSTEM.cpp`

3. Run the application: `./cab_booking_simulator`

  

## More Changes

Incorporate a map-based GUI to display cab positions and user locations in real-time.