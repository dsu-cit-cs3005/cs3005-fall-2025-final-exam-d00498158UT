#include "../RobotBase.h"
#include "mmm.h"
//#include "RobotBase.h"
//#include "./mmm/mmm.h"

// Introducing... 死の願望 (Death Wish)
class 死の願望 : public mmm, public RobotBase {

public:
  死の願望() // Initialize Death Wish with 5 movement, 2 armor, flamethrower
      : RobotBase(5, 2, flamethrower) {}

  // Radar location for scanning in one of the 8 directions
  virtual void get_radar_direction(int &radar_direction) override {
    int current_row, current_col;
    get_current_location(current_row, current_col);

    // Decide radar direction
    radar_direction = (current_col > 0) ? 7 : 3; // Left or Right
  }

  // Processes radar results and updates known obstacles and target
  virtual void
  process_radar_results(const std::vector<RadarObj> &radar_results) override {
    clearTarget();
    for (const auto &obj : radar_results) {
      // Add static obstacles to the obstacle list
      addObstacle(obj);

      // Identify the first enemy found as the target
      if (obj.m_type == 'R' && getVictims()[0] != mmm::_cleared) {
        mmm::setTarget(obj.m_row, obj.m_col);
      }
    }
  }

  // Determines the next shot location
  virtual bool get_shot_location(int &shot_row, int &shot_col) override {
    if (getTarget() != _cleared) {
      shot_row = getTarget().first;
      shot_col = getTarget().second;
      clearTarget(); // Clear target after shooting
      return true;
    }
    return false;
  }

  // Determines the next movement direction
  void get_move_direction(int &move_direction, int &move_distance) override {
    int current_row, current_col;
    get_current_location(current_row, current_col);
    int move = get_move_speed(); // Max movement range for this robot

    // Step 1: Move left until column == 0
    if (current_col > 0) {
      move_direction = 7; // Left
      move_distance =
          std::min(move, current_col); // Clamp to avoid going out of bounds
      return;
    }

    // Step 2: Vertical movement once column == 0
    if (_movingd) {
      // Move down if not at the bottom
      if (current_row + move < m_board_row_max) {
        move_direction = 5; // Down
        move_distance = std::min(move, m_board_row_max - current_row - 1);
      } else {
        // Switch to moving up
        _movingd = false;
        move_direction = 1; // Up
        move_distance = 1;  // Take a single step up
      }
    } else {
      // Move up if not at the top
      if (current_row - move >= 0) {
        move_direction = 1; // Up
        move_distance = std::min(move, current_row);
      } else {
        // Switch to moving down
        _movingd = true;
        move_direction = 5; // Down
        move_distance = 1;  // Take a single step down
      }
    }
  }
};

// Factory function to create Death Wish
extern "C" RobotBase *create_robot() { return new 死の願望(); }