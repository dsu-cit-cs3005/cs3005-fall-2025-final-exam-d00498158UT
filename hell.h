#include "RadarObj.h"
#include "RobotBase.h"
#include <algorithm>
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

class arena {
private:
  // Obstacle/base layer board vector
  std::vector<std::string> _arena;
  // Robot vector board vector
  std::vector<std::string> _participants;
  // Main vector for Robot data
  std::vector<RobotBase> _robots;
  // Vectors of unique chars for Robots, Obstacles, etc.
  // Robots:
  std::vector<std::string> _rchars = {"🟥", "🟧", "🟨", "🟩", "🟦", "🟪", "🟫",
                                      "🔴", "🟠", "🟡", "🟢", "🔵", "🟣", "🟤"};
  // Obstacles: (Empty, Pit, Mound, Flame thrower)
  std::vector<std::string> _ochars = {"·", "🮽", "🭌", "🔥", "☠️"};

  // RadarObj main vector
  std::vector<RadarObj> _radar;

  // Settings Map:
  // height, width, mounds, pits, flamers, rounds
  std::map<std::string, int> _arenasettings;
  // Current round number
  int _round;
  // Winner?
  bool _end = false;

public:
  // Default constructor
  // arena() = default;
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
  int getBots() { return _robots.size(); }

  // Set settings stored in _arenasettings map
  void setArena(std::string key, int value);

  // Function to load values from config, returns true if file sucessfully loads
  bool loadConfig();

  // Check that settings are valid, adjust invalid settings and throw warnings
  // for each
  // void validateSettings();

  // Add assembled robots to robots vector
  void addRobot(RobotBase *robot) { _robots.push_back(*robot); }

  // Test Robots (taken from test_robot)
  // void testRobots(RobotBase *robot);

  // Init board class - add obstacles, then add robots. (Board class will make
  // bots unique)
  void send2Hell();

  // Print/show the arena
  void showCarnage();

  // The winner is?
  void done();

  // Radar scan function
  std::vector<RadarObj> radarScan(RobotBase *robot);

  // Damage calculation and legality function
  void botAttack(RobotBase *robot);

  // Robot movement handler
  void botMove(RobotBase *robot);

  // The main loop - the ultimate mashup of metal mayhem to meticulously mandate
  // that the mightiest machine is most mega.
  int mainLoop();
};
