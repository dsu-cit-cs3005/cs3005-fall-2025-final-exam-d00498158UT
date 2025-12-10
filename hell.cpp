#include "hell.h"

// (Getters implmented directly in header, noted here for reference)
/*
int arena::getHeight() { return _arenasettings.at("height"); }
int arena::getWidth() { return _arenasettings.at("width"); }
int arena::getMounds() { return _arenasettings.at("mounds"); }
int arena::getPits() { return _arenasettings.at("pits"); }
int arena::getFlamers() { return _arenasettings.at("flamers"); }
int arena::getRounds() { return _arenasettings.at("rounds"); }
int arena::getBots() { return _robots.size(); }
*/

void arena::setArena(std::string key, int value) {
  key.erase(remove(key.begin(), key.end(), ' '), key.end());
  key.erase(remove(key.begin(), key.end(), '\n'), key.end());
  if (key == "" || value < 0) {
    return;
  }
  _arenasettings.at(key) = value;
}

bool arena::loadConfig() {
  std::string configl;
  std::ifstream confile("arena.conf");
  if (!confile.good()) {
    std::cerr << "Warning: Unable to read arena.conf file" << std::endl;
    return false;
  }
  while (getline(confile, configl, '=')) {
    std::transform(configl.begin(), configl.end(), configl.begin(), ::tolower);
    std::istringstream token(configl);
    std::string setting, option;
    std::getline(token, setting, '=');
    std::getline(token, option);
    int opt = std::stoi(option);
    setArena(setting, opt);
  }
  confile.close();
  return true;
};

void arena::send2Hell() {
  srand(time(0));
  int boardsize = getHeight() * getWidth();
  // init obstacle board vector, fill with obstacles
  std::vector<std::string> _arena;
  _arena.resize(boardsize);
  fill(_arena.begin(), _arena.end(), _ochars[0]);
  std::vector<std::string> _participants;
  _participants.resize(boardsize);
  fill(_participants.begin(), _participants.end(), _ochars[0] );
  _radar.resize(boardsize);
  for (int p = getPits(); p > 0; p--) {
    int wp = rand() % getWidth();
    int hp = rand() % getHeight();
    int pp = wp * hp;
    _arena.at(pp) = _ochars[1];
    _radar.at(pp) = {'P', wp, hp};
  }
  for (int m = getMounds(); m > 0; m--) {
    int wm = rand() % getWidth();
    int hm = rand() % getHeight();
    int mm = wm * hm;
    _arena.at(mm) = _ochars[2];
    _radar.at(mm) = {'M', wm, hm};
  }
  for (int f = getFlamers(); f > 0; f--) {
    int wf = rand() % getWidth();
    int hf = rand() % getHeight();
    int ff = wf * hf;
    _arena.at(ff) = _ochars[3];
    _radar.at(ff) = {'F', wf, hf};
  }
  int b;
  if (_robots.size() > 14) {
    b = 14;
  } else {
    b = _robots.size();
  }
  while (b > 0) {
    int wposit = rand() % getWidth();
    int hposit = rand() % getHeight();
    int posit = wposit * hposit;
    if (_arena.at(posit) == _ochars[0]) {
      _participants.at(posit) = _rchars[b];
      _radar.at(posit) = {'R', wposit, hposit};
      _robots.at(b)->move_to(wposit, hposit);
      b--;
    }
  }
}

void arena::showCarnage() {
  int he = getHeight();
  int wi = getWidth();
  for (int i = 0; i < he; i++) {
    std::cout << i << " ";
    for (int j = 0; j < wi; j++) {
      std::cout << j << " ";
      if (_arena.at(i * j).empty()) {
        std::cout << _arena.at(i * j) << " ";
      } else {
        std::cout << "🤖" << _participants.at(i * j);
      }
    }
    std::cout << std::endl;
  }
}

void arena::done() {
  int battle = 0;
  for (auto b : _robots) {
    if(b->get_health() > 0) {
      battle++;
    }
  }
  //auto check = [&](auto g) {
  //  if (g.get_health() > 0) {
  //    battle++;
  //  }
  //};
  //std::for_each(_robots.begin(), _robots.end(), check);
  if (battle == 1) {
    _end = true;
  } else {
    _end = false;
  }
}

std::vector<RadarObj> arena::radarScan(RobotBase *robot) {
  int radar_direction;
  robot->get_radar_direction(radar_direction);
  int current_row, current_col;
  robot->get_current_location(current_row, current_col);
  std::vector<RadarObj> results;
  switch (radar_direction) {
  case 0:
    results.push_back(_radar.at(current_col - 1 * current_row));
    results.push_back(_radar.at(current_col - 1 * current_row + 1));
    results.push_back(_radar.at(current_col * current_row + 1));
    results.push_back(_radar.at(current_col + 1 * current_row + 1));
    results.push_back(_radar.at(current_col + 1 * current_row));
    results.push_back(_radar.at(current_col + 1 * current_row - 1));
    results.push_back(_radar.at(current_col * current_row - 1));
    results.push_back(_radar.at(current_col - 1 * current_row - 1));
    break;
  case 1:
    for (int y = current_row - 1; y > 0; --y) {
      results.push_back(_radar.at(current_col * y));
      results.push_back(_radar.at(current_col - 1 * y));
      results.push_back(_radar.at(current_col + 1 * y));
    }
    break;
  case 3:
    for (int x = current_col + 1; x < getWidth(); ++x) {
      results.push_back(_radar.at(current_row * x));
      results.push_back(_radar.at(current_row - 1 * x));
      results.push_back(_radar.at(current_row + 1 * x));
    }
    break;
  case 5:
    for (int y = current_row + 1; y < getHeight(); ++y) {
      results.push_back(_radar.at(current_col * y));
      results.push_back(_radar.at(current_col - 1 * y));
      results.push_back(_radar.at(current_col + 1 * y));
    }
    break;
  case 7:
    for (int x = current_col - 1; x > 0; --x) {
      results.push_back(_radar.at(current_row * x));
      results.push_back(_radar.at(current_row - 1 * x));
      results.push_back(_radar.at(current_row + 1 * x));
    }
    break;
  }
  return results;
}

void arena::botAttack(RobotBase *robot) {
  int shot_row, shot_col, current_row, current_col;
  if (robot->get_shot_location(shot_row, shot_col)) {
    (void)shot_row;
    (void)shot_col;
    (void)current_row;
    (void)current_col;
    // is shot_row and shot_col valid given the weapon?
    // if so, deal damage to robot at index. If not, do nothing
  }
}

void arena::botMove(RobotBase *robot) {
  int current_row, current_col, direction;
  int distance = robot->get_move_speed();
  robot->get_current_location(current_row, current_col);
  robot->get_move_direction(direction, distance);
  switch (direction) {
  case 1:
    for (int m = current_row; m > distance; --m) {
      if (_arena.at(current_row * m) == _ochars[0]) {
        robot->move_to(current_row, m);
      } else if (_arena.at(current_row * m) == _ochars[1]) {
        robot->move_to(current_row, m);
        robot->disable_movement();
      } else if (_arena.at(current_row * m) == _ochars[2] ||
                 _arena.at(current_row * m) == _ochars[4]) {
        continue;
      } else if (_arena.at(current_row * m) == _ochars[3]) {
        robot->move_to(current_row, m);
        robot->take_damage(
            static_cast<int>(rand() % (50 - 30 + 1) + 30 / robot->get_armor()));
        robot->reduce_armor(1);
      }
    }
    break;
  case 3:
    for (int m = current_col; m > distance; ++m) {
      if (_arena.at(current_col * m) == _ochars[0]) {
        robot->move_to(current_col, m);
      } else if (_arena.at(current_col * m) == _ochars[1]) {
        robot->move_to(current_col, m);
        robot->disable_movement();
      } else if (_arena.at(current_col * m) == _ochars[2] ||
                 _arena.at(current_col * m) == _ochars[4]) {
        continue;
      } else if (_arena.at(current_col * m) == _ochars[3]) {
        robot->move_to(current_col, m);
        robot->take_damage(
            static_cast<int>(rand() % (50 - 30 + 1) + 30 / robot->get_armor()));
        robot->reduce_armor(1);
      }
    }
    break;
  case 5:
    for (int m = current_row; m > distance; ++m) {
      if (_arena.at(current_row * m) == _ochars[0]) {
        robot->move_to(current_row, m);
      } else if (_arena.at(current_row * m) == _ochars[1]) {
        robot->move_to(current_row, m);
        robot->disable_movement();
      } else if (_arena.at(current_row * m) == _ochars[2] ||
                 _arena.at(current_row * m) == _ochars[4]) {
        continue;
      } else if (_arena.at(current_row * m) == _ochars[3]) {
        robot->move_to(current_row, m);
        robot->take_damage(
            static_cast<int>(rand() % (50 - 30 + 1) + 30 / robot->get_armor()));
        robot->reduce_armor(1);
      }
    }
    break;
  case 7:
    for (int m = current_col; m > distance; --m) {
      if (_arena.at(current_col * m) == _ochars[0]) {
        robot->move_to(current_col, m);
      } else if (_arena.at(current_col * m) == _ochars[1]) {
        robot->move_to(current_col, m);
        robot->disable_movement();
      } else if (_arena.at(current_col * m) == _ochars[2] ||
                 _arena.at(current_col * m) == _ochars[4]) {
        continue;
      } else if (_arena.at(current_col * m) == _ochars[3]) {
        robot->move_to(current_col, m);
        robot->take_damage(
            static_cast<int>(rand() % (50 - 30 + 1) + 30 / robot->get_armor()));
        robot->reduce_armor(1);
      }
    }
    break;
  }
}

int arena::mainLoop() {
  while (!_end) {
    RadarObj radarobj;
    showCarnage();
    for (auto a : _robots) {
      radarScan(a);
    }
//    // Step 1: Radar direction and obj construction
//    std::for_each(_robots.begin(), _robots.end(),
//                  [&](RobotBase *robot) { radarScan(robot); });
//    // Step 2: Handle attack and inflict damage
//    std::for_each(_robots.begin(), _robots.end(),
//                  [&](RobotBase *robot) { botAttack(robot); });
//    // Step 3: Handle robot movement
//    std::for_each(_robots.begin(), _robots.end(),
//                  [&](RobotBase *robot) { botMove(robot); });
    next();
    done();
  }
  return 0;
}