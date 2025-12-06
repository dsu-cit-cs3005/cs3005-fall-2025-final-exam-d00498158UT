# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Targets
all: test_robot 夜勤 昇る太陽 死の願望 

RobotBase.o: RobotBase.cpp RobotBase.h
	$(CXX) $(CXXFLAGS) -c RobotBase.cpp

test_robot: test_robot.cpp RobotBase.o
	$(CXX) $(CXXFLAGS) test_robot.cpp RobotBase.o -ldl -o test_robot

mmm.cpp: RobotBase.o mmm.cpp 
	$(CXX) $(CXXFLAGS) ./Robots_mmm/mmm.cpp ./Robots_mmm/mmm.h RobotBase.o

夜勤: RobotBase.o mmm.o 夜勤.o
	$(CXX) $(CXXFLAGS) ./Robots_mmm/夜勤.cpp ./Robots_mmm/mmm.o -ldl -o 夜勤

昇る太陽: RobotBase.o mmm.o 昇る太陽.o
	$(CXX) $(CXXFLAGS) ./Robots_mmm/昇る太陽.cpp ./Robots_mmm/mmm.o -ldl -o 昇る太陽

死の願望: RobotBase.o mmm.o 死の願望.o
	$(CXX) $(CXXFLAGS) ./Robots_mmm/死の願望.cpp ./Robots_mmm/mmm.o -ldl -o 死の願望

clean:
	rm -f *.o test_robot *.so
	rm -f ./Robots_mmm/*.o 夜勤 昇る太陽 死の願望 
