#include "RobotBase.h"
#include <algorithm>
#include <bitset>
#include <vector>

// Introducing... 死の願望 (Death Wish)
class 死の願望 : public RobotBase {

private:
  // Default cleared pair
  static constexpr std::pair<int, int> _cleared = {-1, -1};
  // Permanent obstacle list
  std::vector<RadarObj> _obstacles;
  // Current target for the turn
  std::pair<int, int> _target;
  // Engage target? (set to, and will remain, true for 死の願望)
  bool _engage = true;
  // "Sanity" variable (Unused by 死の願望)
  // int _sanity; // Is there any varience in sanity from a robot with a death
  // wish?

public:
  // Tracks if vertical movement direction
  bool _movingv;
  // Set target data vector
  void setTarget(int x, int y) { _target = {x, y}; };
  // Return current target
  std::pair<int, int> getTarget() { return _target; }
  // Clears the target when no enemy is found
  void clearTarget() { _target = _cleared; }
  // Return _obstacles data member
  std::vector<RadarObj> getObstacles() { return _obstacles; }
  // Helper function to determine if a cell is an obstacle
  bool isObstacle(int row, int col) const;
  // Helper function to add an obstacle to the list if it's not already there
  void addObstacle(const RadarObj &obj) { _obstacles.push_back(obj); };
  // Return _sanity data member
  // int insane() { return _sanity; }

  死の願望() // Initialize Death Wish with 5 movement, 2 armor, hammer
      : RobotBase(5, 2, hammer) {}

  virtual void get_radar_direction(int &radar_direction) override {
    // Using a bitset to represent different directions for simplified radar
    // direction syntax
    std::bitset<8> pRDirs(std::string("11111111"));
    std::vector<int> fpRDirs;
    int current_row, current_col;
    get_current_location(current_row, current_col);
    // Decide radar direction
    if (current_col == 0) {
      pRDirs.set(7, false);
      pRDirs.set(6, false);
      pRDirs.set(5, false);
    }
    if (current_col == m_board_col_max) {
      pRDirs.set(1, false);
      pRDirs.set(2, false);
      pRDirs.set(3, false);
    }
    if (current_row == 0) {
      pRDirs.set(7, false);
      pRDirs.set(0, false);
      pRDirs.set(1, false);
    }
    if (current_row == m_board_row_max) {
      pRDirs.set(3, false);
      pRDirs.set(4, false);
      pRDirs.set(5, false);
    }
    for (size_t i = 0; i < pRDirs.size(); ++i) {
      if (pRDirs[i]) {
        fpRDirs.push_back(i);
      }
    }
    srand(time(0));
    radar_direction = rand() % fpRDirs.size();
    fpRDirs.clear();
  }

  // Processes radar results and updates known obstacles and target
  virtual void
  process_radar_results(const std::vector<RadarObj> &radar_results) override {
    for (const auto &obj : radar_results) {
      // Add static obstacles to the obstacle list
      addObstacle(obj);
      if (obj.m_type == 'R' && getTarget() == _cleared) {
        setTarget(obj.m_row, obj.m_col);
      }
    }
  }

  // Determines the next shot location
  virtual bool get_shot_location(int &shot_row, int &shot_col) override {
    if (getTarget() != _cleared && _engage) {
      shot_row = getTarget().first;
      shot_col = getTarget().second;
      clearTarget(); // Clear target after shooting
      return true;
    }
    return false;
  }

  // Determines the next movement direction
  void get_move_direction(int &move_direction, int &move_distance) override {
    srand(time(0));
    _movingv = std::rand() % 2;
    int current_row, current_col;
    get_current_location(current_row, current_col);
    int move = get_move_speed(); // Max movement range for this robot
    auto one = [&]() {
      move_direction = 1;
      move_distance = std::min(move, getTarget().second + 1);
    };
    auto three = [&]() {
      move_direction = 3;
      move_distance = std::min(move, getTarget().first - 1);
    };
    auto five = [&]() {
      move_direction = 5;
      move_distance = std::min(move, getTarget().second - 1);
    };
    auto seven = [&]() {
      move_direction = 7;
      move_distance = std::min(move, getTarget().first + 1);
    };
    if (getTarget() != _cleared) {
      if (getTarget().first == current_col) {
        _movingv = true;
      } else if (getTarget().second == current_row) {
        _movingv = false;
      }
      if (getTarget().first > current_col && !_movingv) {
        three();
      } else if (!_movingv) {
        seven();
      }
      if (getTarget().second > current_row && _movingv) {
        five();
      } else if (_movingv) {
        one();
      }
    } else {
      setTarget(rand() % m_board_col_max, rand() % m_board_row_max);
      int bored = rand() % 3;
      switch (bored) {
      case 0:
        one();
        clearTarget();
        break;
      case 1:
        three();
        clearTarget();
        break;
      case 2:
        five();
        clearTarget();
        break;
      case 3:
        seven();
        clearTarget();
        break;
      }
    }
  }
};
// Factory function to create Death Wish
extern "C" RobotBase *create_robot() { return new 死の願望(); }