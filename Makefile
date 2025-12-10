# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Targets
all: test_robot RobotWarz 

RobotBase.o: RobotBase.cpp RobotBase.h
	$(CXX) $(CXXFLAGS) -c -fPIC RobotBase.cpp 

test_robot: test_robot.cpp RobotBase.o
	$(CXX) $(CXXFLAGS) test_robot.cpp RobotBase.o -ldl -o test_robot

hell.o: hell.cpp hell.h RobotBase.o
	$(CXX) $(CXXFLAGS) -c hell.cpp 

RobotWarz.o: hell.o
	$(CXX) $(CXXFLAGS) -c RobotWarz.cpp

RobotWarz: RobotWarz.o hell.o
	$(CXX) $(CXXFLAGS) -g RobotWarz.o hell.o RobotBase.o -o RobotWarz


#死の願望: RobotBase.o 
#	$(CXX) $(CXXFLAGS) Robot_死の願望.cpp -ldl -o 死の願望

clean:
	rm -f *.o *.so RobotWarz test_robot
