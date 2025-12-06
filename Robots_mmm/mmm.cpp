#include "mmm.h"

void mmm::setTarget(int x, int y) {
  if (x >= 0 && y >= 0) {
    std::pair pv = {x, y};
    if (!(std::find(getObstacles().begin(), getObstacles().end(), pv) !=
          getObstacles().end())) {
      std::cerr << "Woah! I thought " << x << y << " was a bot, lmao  ";
      return;
    }
    if (std::find(getVictims().begin(), getVictims().end(), pv) !=
        getVictims().end()) {
      _target = {x, y};
    } else {
      getVictims().push_back(pv);
      _target = {x, y};
    }
  } else {
    std::cerr << "Hurrrr duurrr i A suPeR SmArT KiLlEr rOboT TrYinG tO kiLl "
                 "SoMeThiNG tHaT DoeSNt ExiSt 󰧑 "
              << x << y;
    return;
  }
}

bool mmm::isObstacle(int row, int col) const {
  return std::any_of(_obstacles.begin(), _obstacles.end(),
                     [&](const RadarObj &obj) {
                       return obj.m_row == row && obj.m_col == col;
                     });
}

void mmm::addObstacle(const RadarObj &obj) {
  if (obj.m_type == 'M' || obj.m_type == 'P' ||
      obj.m_type == 'F' && !isObstacle(obj.m_row, obj.m_col)) {
    getObstacles().push_back(obj);
  }
}
