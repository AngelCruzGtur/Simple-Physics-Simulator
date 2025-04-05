#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib> 

using namespace std;

const double GRAVITY = 9.81;

/**
 * Function to calculate the drag force acting on an object.
 * 
 *  dragCoefficient The drag coefficient of the object.
 *  crossSectionalArea The cross-sectional area of the object in square meters.
 *  airDensity The density of air at the object's altitude in kg/m³.
 *  velocity The velocity of the object in m/s.
 *  The drag force in newtons.
 */
double calculateDragForce(double dragCoefficient, double crossSectionalArea, double airDensity, double velocity)
{
    return 0.5 * dragCoefficient * crossSectionalArea * airDensity * velocity * velocity;
}

/**
 * Function to calculate the gravitational force acting on an object.
 * 
 *  mass The mass of the object in kilograms.
 *  The gravitational force in newtons.
 */
double calculateGravitationalForce(double mass)
{
    return mass * GRAVITY;
}

/**
 * Function to calculate air density at a given altitude.
 * 
 *  altitude The altitude in meters above sea level.
 *  The air density at the given altitude in kg/m³.
 */
double calculateAirDensity(double altitude)
{
    const double seaLevelDensity = 1.225; // kg/m³ at sea level
    const double scaleHeight = 8500.0;    // Approximate scale height of Earth's atmosphere in meters
    return seaLevelDensity * exp(-altitude / scaleHeight);
}

/**
 * Function to dynamically adjust the drag coefficient based on wind speed.
 * 
 *  dragCoefficient The original drag coefficient.
 *  windSpeed The wind speed in m/s.
 *  The adjusted drag coefficient.
 */
double adjustDragCoefficient(double dragCoefficient, double windSpeed)
{
    if (abs(windSpeed) > 15.0)
    {
        return dragCoefficient * 1.5; // Increase drag coefficient for higher winds
    }
    return dragCoefficient; // No change for lower wind speeds
}

/**
 * Function to display the falling object's position and trail in the console.
 * 
 *  position The current position of the object.
 *  initialHeight The initial height from which the object is falling.
 *  trail A vector storing the position history of the object for trail effect.
 */
void displayFallingObject(double position, double initialHeight, vector<int> &trail)
{
    const int steps = 20;
    int currentStep = static_cast<int>(((initialHeight - position) / initialHeight) * steps);
    if (currentStep >= steps) currentStep = steps - 1;
    if (currentStep < 0) currentStep = 0;

    trail.push_back(currentStep);
    if (trail.size() > 5) trail.erase(trail.begin());

    for (int i = 0; i < steps; ++i)
    {
        if (i == currentStep)
            cout << "   \033[1;32m*\033[0m   <-- object\n";
        else if (find(trail.begin(), trail.end(), i) != trail.end())
            cout << "   \033[0;90mo\033[0m\n"; // faded trail
        else
            cout << "\n";
    }

    cout << "------ Ground ------\n";
}

/**
 * Function to simulate the motion of a falling object considering drag, gravity, and wind effects.
 * 
 *  mass The mass of the object in kilograms.
 *  dragCoefficient The drag coefficient of the object.
 *  crossSectionalArea The cross-sectional area of the object in square meters.
 *  airDensity The density of air at the object's altitude in kg/m³.
 *  initialVelocity The initial velocity of the object in m/s.
 *  initialHeight The initial height from which the object is falling in meters.
 *  windOption An integer to determine the type of wind effect (0: random, 1: no wind, 2: custom).
 *  customWindStrength The custom wind strength for type 2 wind effect.
 */
void simulateMotion(double mass, double dragCoefficient, double crossSectionalArea, double airDensity,
                    double initialVelocity, double initialHeight, int windOption, double customWindStrength = 0.0)
{
    double velocity = initialVelocity;
    double time = 0.0;
    double position = initialHeight;
    double gravityForce = calculateGravitationalForce(mass);
    vector<int> trail;

    const double timeStep = 0.05;
    double windDirection = 1.0;  // +1 for right, -1 for left
    double windStrength = 0.0;   // Starts with no wind

    while (position > 0 || abs(velocity) > 0.5)
    {
        double airDensityAtAltitude = calculateAirDensity(position);

        if (windOption == 0)
        {
            windStrength = sin(time * 0.5) * 0.5; // Oscillates between -0.5 and +0.5
        }
        else if (windOption == 1)
        {
            windStrength = 0.0; // No wind
        }
        else if (windOption == 2)
        {
            windStrength = customWindStrength; // Custom wind
        }

        if (windOption == 0 && static_cast<int>(time) % 3 == 0)
        {
            windDirection = (rand() % 2 == 0) ? 1.0 : -1.0; // Randomly switch direction
        }

        double wind = windStrength * windDirection;
        double adjustedDragCoefficient = adjustDragCoefficient(dragCoefficient, windStrength);
        double dragForce = calculateDragForce(adjustedDragCoefficient, crossSectionalArea, airDensityAtAltitude, abs(velocity));
        double netForce = gravityForce - dragForce + wind;
        double acceleration = netForce / mass;

        velocity += acceleration * timeStep;
        position -= velocity * timeStep;

        if (position < 0)
        {
            position = 0;
            if (abs(velocity) > 1.0)
                velocity = -velocity * 0.6; // lose some energy
            else
                velocity = 0; // settle
        }

        // Clear screen + move cursor to top-left
        cout << "\033[2J\033[H"; 
        displayFallingObject(position, initialHeight, trail);

        cout << "Time: "        << time << " s\t";
        cout << "Velocity: "    << velocity << " m/s\t";
        cout << "Height: "      << position << " m\t";
        cout << "Drag: "        << dragForce << " N\t";
        cout << "Wind: "        << wind << " N (Direction: " << (windDirection == 1 ? "Right" : "Left") << ")\n";
        cout << "Air Density: " << airDensityAtAltitude << " kg/m³\n";

        this_thread::sleep_for(chrono::milliseconds(50));
        time += timeStep;
    }

    cout << "\n\033[1;33mSimulation complete! Total time: " << time << " seconds.\033[0m\n";
}

int main()
{
    double dragCoefficient = 0.47;
    double crossSectionalArea = 0.01;
    double airDensity = 1.225;

    int numObjects;
    cout << "Enter the number of objects to simulate: ";
    cin >> numObjects;

    int windOption;
    cout << "\nChoose wind type:\n";
    cout << "0: Random Wind (Oscillates and switches direction)\n";
    cout << "1: No Wind\n";
    cout << "2: Custom Wind (Enter wind strength)\n";
    cin >> windOption;

    double customWindStrength = 0.0;
    if (windOption == 2)
    {
        cout << "Enter custom wind strength (positive for right, negative for left): ";
        cin >> customWindStrength;
    }

    cout << "\033[2J\033[H"; // Clear screen + move cursor to top-left
    cout << "\033[1;34mPhysics Object Simulation\033[0m\n";
    cout << "---------------------------\n";
    cout << fixed << setprecision(2);

    for (int i = 0; i < numObjects; i++)
    {
        double mass, initialVelocity, initialHeight;

        cout << "\nObject " << (i + 1) << ":\n";
        cout << "Enter the mass (kg): ";
        cin >> mass;

        cout << "Enter the initial velocity (m/s): ";
        cin >> initialVelocity;

        cout << "Enter the initial height (m): ";
        cin >> initialHeight;

        simulateMotion(mass, dragCoefficient, crossSectionalArea, airDensity, initialVelocity, initialHeight, windOption, customWindStrength);
    }

    return 0;
}
