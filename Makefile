# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Targets
all: test_robot RobotWarz 

RobotBase.o: RobotBase.cpp RobotBase.h
	$(CXX) $(CXXFLAGS) -c RobotBase.cpp

hell.o: RobotBase.o RadarObj.h
	$(CXX) $(CXXFLAGS) -c hell.cpp

RobotWarz: hell.o
	$(CXX) $(CXXFLAGS) -c RobotWarz

test_robot: test_robot.cpp RobotBase.o
	$(CXX) $(CXXFLAGS) test_robot.cpp RobotBase.o -ldl -o test_robot

#死の願望: RobotBase.o 
#	$(CXX) $(CXXFLAGS) Robot_死の願望.cpp -ldl -o 死の願望

clean:
	rm -f *.o *.so RobotWarz test_robot
