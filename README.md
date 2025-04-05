# Physics Object Simulation

This C++ program simulates the motion of a falling object under the influence of gravity, drag, and wind effects. The program allows the user to input various parameters, including object mass, initial velocity, and height. It calculates and displays the position, velocity, drag force, and wind effects in real-time while the object falls. 

## Features
- Simulates the effect of gravity on a falling object.
- Accounts for drag force acting on the object based on its drag coefficient, cross-sectional area, and air density.
- Includes wind effects with multiple options (random, no wind, or custom wind strength).
- Provides a visual trail of the object’s fall in the terminal with a step-by-step display.
- Displays real-time information such as position, velocity, drag force, wind, and air density.

## Requirements
To run the program, you need a C++ compiler (e.g., GCC, Clang, or Visual Studio) that supports C++11 or later. The program has been tested on a Linux environment but should work on any platform with minor adjustments.

## How to Run

1. Clone this repository to your local machine.
2. Compile the program:
    - Use your C++ compiler (e.g., GCC, Clang) to compile the source code.
3. Run the program after compiling.

## User Input

When running the program, you will be asked to provide the following:
1. The number of objects to simulate.
2. Wind options:
   - `0`: Random wind (oscillates and switches direction).
   - `1`: No wind.
   - `2`: Custom wind strength (positive for right, negative for left).
3. For each object:
   - The mass (in kilograms).
   - The initial velocity (in meters per second).
   - The initial height (in meters).

## Example Output

```text
Enter the number of objects to simulate: 1

Choose wind type:
0: Random Wind (Oscillates and switches direction)
1: No Wind
2: Custom Wind (Enter wind strength)
1

Object 1:
Enter the mass (kg): 1.5
Enter the initial velocity (m/s): 0
Enter the initial height (m): 100

Physics Object Simulation
---------------------------
Time: 0.00 s   Velocity: 0.00 m/s   Height: 100.00 m   Drag: 0.00 N   Wind: 0.00 N (Direction: Right)
Air Density: 1.225 kg/m³

... (Simulation running)

Simulation complete! Total time: 4.67 seconds.
