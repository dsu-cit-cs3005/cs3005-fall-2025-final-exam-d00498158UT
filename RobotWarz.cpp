#include "hell.h"

int main() {
  // Initialize arena object.
  arena hell = arena();

  // Compile the file into a shared library - put this in a loop that
  // traverses an array of all the robot.cpp files...
  for (const auto &entry : std::filesystem::directory_iterator(".")) {
    std::filesystem::path name = entry.path();
    std::string fname = name.string();

    if (fname.find("Robot_") != std::string::npos &&
        fname.substr(fname.size() - 4) == ".cpp") {
      const std::string shared_lib = fname.substr(0, fname.size() - 4) + ".so";
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

      // to load the shared objects:
      void *handle = dlopen(shared_lib.c_str(), RTLD_LAZY);
      if (!handle) {
        std::cerr << "Failed to load " << shared_lib << ":" << dlerror()
                  << std::endl;
        continue;
      }
      // to Locate the factory function to create the robot (note: RobotBase has
      // a typedef for RobotFactory... go look at it!)
      RobotFactory create_robot = (RobotFactory)dlsym(handle, "create_robot");
      if (!create_robot) {
        std::cerr << "Failed to find create_robot in " << shared_lib << ": "
                  << dlerror() << std::endl;
        dlclose(handle);
        continue;
      }
      // To Instantiate the robot, you call the function that you LOADED from
      // the shared library: you'll want to ensure that this function worked and
      // then add this robot to a vector of robots. set it up how you like...
      RobotBase *robot = create_robot();
      if (robot) {
      hell.arena::addRobot(*robot);
      }
    }
  }

  // Load+validate settings for arena
  // hell.arena::validateSettings();
  // Load robots
  void *handle;

  // hell.arena::testRobots(robot);
  //  Apply settings for arena and add Robots w/random marks
  hell.arena::send2Hell();
  // Begin/end
  return hell.arena::mainLoop();
}