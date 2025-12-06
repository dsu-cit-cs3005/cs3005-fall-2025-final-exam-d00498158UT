#pragma once
#include "../RobotBase.h"
#include <algorithm>
#include <ostream>
#include <vector>


class mmm {
private:
  // Tracks loosers that will be eliminated (tracks x,y of other robots)
  std::vector<std::pair<int, int>> _victims;
  // Permanent obstacle list
  std::vector<RadarObj> _obstacles;
  // Current target for the turn
  std::pair<int, int> _target;
  // Engage target?
  bool _engage;
  // "Sanity" variable
  int _sanity;

public:
  // Default cleared pair
  std::pair<int,int> _cleared = {-1, -1};

  // Tracks if vertical movement direction
  bool _movingd = true;

  // Return the location of the victims to the masters of metal meyhem (return
  // _victims vector data member)
  std::vector<std::pair<int, int>> getVictims() { return _victims; }

  // Set target data vector
  void setTarget(int x, int y);

  // Return current target
  std::pair<int,int> getTarget() {return _target;}

  // Clears the target when no enemy is found
  void clearTarget() { _target = _cleared; }

  // Return _obstacles data member
  std::vector<RadarObj> getObstacles() { return _obstacles; }

  // Helper function to determine if a cell is an obstacle
  bool isObstacle(int row, int col) const;

  // Helper function to add an obstacle to the list if it's not already there
  void addObstacle(const RadarObj &obj);

  // Return _sanity data member
  int insane() { return _sanity; }
};