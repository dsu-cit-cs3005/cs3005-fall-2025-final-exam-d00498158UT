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
}

void arena::assembleRobots() {
  // Compile the file into a shared library - put this in a loop that
  // traverses an array of all the robot.cpp files...
  for (const auto &entry : std::filesystem::directory_iterator(".")) {
    std::string fname = entry.path().filename().string();
    if (fname.find("Robot_")) {
      std::string compile_cmd = "g++ -shared -fPIC -o " + shared_lib + " " +
                                fname + " RobotBase.o -I. -std=c++20";
      std::cout << "Compiling " << fname << " to " << shared_lib << "...\n";
      int compile_result = std::system(compile_cmd.c_str());

      if (compile_result != 0) {
        std::cerr << "Robot " << fname << " was too scared to compile 😂"
                  << std::endl
                  << "(command: " << compile_cmd << ")" << std::endl;
        continue;
      }
    }

    // to load the shared objects:
    void *handle = dlopen(shared_lib.c_str(), RTLD_LAZY);
    if (!handle) {
      std::cerr << "Failed to load " << shared_lib << ":" << dlerror()
                << std::endl;
      continue;
    }
    // to Locate the factory function to create the robot (note: RobotBase has a typedef for RobotFactory... go look at it!)
  RobotFactory create_robot = (RobotFactory)dlsym(handle, "create_robot");
  if (!create_robot) {
    std::cerr << "Failed to find create_robot in " << shared_lib << ": "
              << dlerror() << std::endl;
    dlclose(handle);
    continue;
  }
  // To Instantiate the robot, you call the function that you LOADED from the
  // shared library: you'll want to ensure that this function worked and then
  // add this robot to a vector of robots. set it up how you like...
  RobotBase *robot = create_robot();
  }
}

void arena::validateSettings() {
  if (!arena::loadConfig()) {
    while (
        (arena::getHeight() * arena::getWidth()) +
            ((arena::getHeight() * static_cast<int>(arena::getWidth())) / 5) <=
        arena::getBots()) {
      arena::setArena("width", getWidth() + 1);
      arena::setArena("height", getWidth() + 1);
      return;
    }
  }
  while ((arena::getHeight() * arena::getWidth()) +
             ((arena::getHeight() * static_cast<int>(arena::getWidth())) / 5) <=
         arena::getBots()) {
    arena::setArena("width", getWidth() + 1);
    arena::setArena("height", getWidth() + 1);
  }
  if (arena::getHeight() * arena::getWidth() < 100) {
    std::cerr << "Warning: Arena size too small. Setting adjusting...";
    if (arena::getHeight() < 10) {
      arena::setArena("height", 10);
    }
    if (arena::getWidth() < 10) {
      arena::setArena("width", 10);
    }
    while (
        (arena::getHeight() * arena::getWidth()) +
            ((arena::getHeight() * static_cast<int>(arena::getWidth())) / 5) <=
        arena::getBots()) {
      arena::setArena("width", getWidth() + 1);
      arena::setArena("height", getWidth() + 1);
    }
  }
  while (arena::getMounds() + arena::getPits() + arena::getFlamers() +
             arena::getBots() >=
         arena::getHeight() * arena::getWidth()) {
    std::cerr << "Warning: Too many obstacles for Arena, reducing...";
    if (arena::getMounds() <= 1) {
      break;
    } else {
      setArena("mounds", arena::getMounds() - 1);
    }
    if (arena::getPits() <= 1) {
      break;
    } else {
      setArena("pits", arena::getPits() - 1);
    }
    if (arena::getFlamers() <= 1) {
      break;
    } else {
      setArena("flamers", arena::getPits() - 1);
    }
  }
}

void arena::send2Hell() {}

int arena::mainLoop() {}

int main() {
  // Initialize arena object.
  arena hell = arena();
  // Load+validate settings for arena
  hell.arena::validateSettings();
  // Apply settings for arena and add Robots w/random marks
  hell.arena::send2Hell();
  // Begin/end
  return hell.arena::mainLoop();
}