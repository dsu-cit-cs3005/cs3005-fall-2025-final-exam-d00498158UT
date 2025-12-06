#include "../RobotBase.h"
#include <algorithm>
#include <filesystem>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

//**Specifications**
//
//* The arena is a 2 dimensional array of 'cells.' row 0, col 0 is the upper
//left corner of the arena, and row n, column m is bottom right.
//* The arena size is configurable. Min size is 10 x 10.
//* The robots occupy a single location, a 'cell' - only one robot per cell.
//* Robots inherit from the RobotBase class and must implement the pure virtual
//functions. RobotBase.h and RobotBase.cpp will be provided to you.
//* The robot .cpp files are placed into the same directory as the arena, and
//when the game loads, the arena will compile the robots into .so (shared
//objects) using the arena's RobotBase.o
//* The arena will store all the robots in a vector of robots.
//* The 'game loop' loops through each robot in the vector and calls functions
//on the them to orchestrate the action.
//* The steps that the arena takes when it runs are as follows:
//    1. load a config file that specifies the parameters (arena size, number
//    and type of obstacles, max number of rounds, if you want to watch the game
//    live or not)
//    2. place obstacles in the arena (M - Mound, P - Pit or F - Flamethrower)
//    3. load the robots into a vector, assign a text character to represent the
//    robot (like & or @) and place them randomly in the arena. When placing a
//    robot, do not place it on an obstacle.
//    4. Loop through each robot in the vector:
//        1. print the number of the current round
//        2. print the state of the arena (ensure you indicate if a robot is
//        dead - they still stay in the arena)
//        3. check to see if there is a winner (is there only one living robot)
//        if so, end the game.
//        4. check to see if this current robot is dead, if so, skip it.
//        5. call this_robot->get_radar_direction - this returns a number
//        between 0 and 8.
//        6. The arena then uses the direction to scan the arena in the
//        direction indicated and produce a vector of RadarObj objects. (this is
//        a list of things the radar 'sees')
//        7. call this_robot->process_radar_results, passing it the RadarObj
//        vector.
//        8. call this_robot->get_shot_location
//        9. if the robot returns true, handle the shot with the row and column
//        specified. Calculate and call the take_damage() function on any robots
//        hit.
//        10. if the robot returns false, call this_robot->get_move_direction
//        and handle the move request (see Movement below). Do NOT call
//        get_move_direction if you handled a shot case. The robots only get one
//        action, move or shoot.
//        11. log the results - print them if watching live
//        12. if watching live, sleep for a second
//        13. increment the round number finish this loop.

class arena {
private:
  // Settings Map:
  // height, width, mounds, pits, flamers, rounds
  std::map<std::string, int> _arenasettings;
  // Main arena vector
  std::vector<char> _hell;
  // Main vector for Robot data
  typedef RobotBase ROBOT;
  std::vector<ROBOT> _robots;
  // Current round number
  int _round;
  // Winner?
  bool _end = false;

public:
  // Default constructor
  //arena() = default;
  // Overloaded constructor
  arena(int h = 10, int w = 10, int m = 1, int p = 1, int f = 1, int r = 10)
      : _arenasettings({{"height", h},
                        {"width", w},
                        {"mounds", m},
                        {"pits", p},
                        {"flamers", f},
                        {"rounds", r}}) {};

  // Return individual settings stored in map
  int getHeight() { return _arenasettings.at("height"); }
  int getWidth() { return _arenasettings.at("width"); }
  int getMounds() { return _arenasettings.at("mounds"); }
  int getPits() { return _arenasettings.at("pits"); }
  int getFlamers() { return _arenasettings.at("flamers"); }
  int getRounds() { return _arenasettings.at("rounds"); }
  // Return number of bots participating
  int getBots() { return _robots.size(); }

  // Set settings stored in _arenasettings map
  void setArena(std::string key, int value);

  // Function to load values from config, returns true if file sucessfully loads
  bool loadConfig();

  // Function to automatically compile robots into shared objects
  void assembleRobots();

  // Check that settings are valid, adjust invalid settings and throw warnings for each
  void validateSettings();

  // Prepare arena first with obstacles, then add robots with unique char.
  void send2Hell();

  // The main loop - the ultimate mashup of metal mayhem to meticulously mandate
  // that the mightiest machine is most mega.
  int mainLoop();

  // The winner is?
  void done();
};