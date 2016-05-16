$(TARGET)
TARGET := simulator
CFLAGS := -std=c++11 -O2 -Wall -pedantic -pthread
O_FILES:= main.o Battery.o House.o OurSensor.o Point.o Robot.o Score.o SimpleIniFileParser.o Simulator.o FuncForMain.o Table.o SharedAlgorithm.o forBfs.o AlgorithmRegistration.o AlgorithmRegistrar.o
 
default :
	make all

all : $(TARGET) _203246509_A.o _203246509_B.o _203246509_C.o scoreCreator.o 203246509_A_.so 203246509_B_.so 203246509_C_.so scoreCreator.so

$(TARGET): $(O_FILES)
	g++ -o $(TARGET) $(O_FILES) -std=c++11 -O2 -Wall -pedantic -pthread -L/usr/local/boost/boost_1_50_0/stage/lib -lboost_filesystem -lboost_system -ldl -rdynamic

main.o: main.cpp FuncForMain.h AlgorithmRegistrar.h Score.h
	g++ -c $(CFLAGS) $*.cpp -L/usr/local/boost/boost_1_50_0/stage/lib -lboost_filesystem -lboost_system -ldl -rdynamic

scoreCreator.o: 
	g++ -c -fPIC $(CFLAGS) $*.cpp
scoreCreator.so:
	g++ scoreCreator.o -shared -o score_formula.so

SharedAlgorithm.o: SharedAlgorithm.cpp SharedAlgorithm.h AbstractAlgorithm.h AbstractSensor.h OurSensor.h forBfs.h AlgorithmRegistration.h make_unique.h
	g++ -c $(CFLAGS) $*.cpp

_203246509_A.o: _203246509_A.cpp _203246509_A.h SharedAlgorithm.h 
	g++ -c -fPIC $(CFLAGS) $*.cpp

203246509_A_.so : _203246509_A.o
	g++ _203246509_A.o -shared -o 203246509_A_.so

_203246509_B.o: _203246509_B.cpp _203246509_B.h SharedAlgorithm.h 
	g++ -c -fPIC $(CFLAGS) $*.cpp

203246509_B_.so : _203246509_B.o
	g++ _203246509_B.o -shared -o 203246509_B_.so

_203246509_C.o: _203246509_C.cpp _203246509_C.h SharedAlgorithm.h 
	g++ -c -fPIC $(CFLAGS) $*.cpp

203246509_C_.so : _203246509_C.o
	g++ _203246509_C.o -shared -o 203246509_C_.so	

Table.o: Table.cpp Table.h Robot.h Simulator.h
	g++ -c $(CFLAGS) $*.cpp 
	
FuncForMain.o: FuncForMain.cpp FuncForMain.h House.h Simulator.h SimpleIniFileParser.h
	g++ -c $(CFLAGS) $*.cpp -L/usr/local/boost/boost_1_50_0/stage/lib -lboost_filesystem -lboost_system

Battery.o: Battery.cpp Battery.h
	g++ -c $(CFLAGS) $*.cpp 

House.o: House.cpp House.h SensorInformation.h
	g++ -c $(CFLAGS) $*.cpp 

OurSensor.o: OurSensor.cpp OurSensor.h AbstractSensor.h House.h
	g++ -c $(CFLAGS) $*.cpp 

Point.o: Point.cpp Point.h Direction.h
	g++ -c $(CFLAGS) $*.cpp 

Robot.o: Robot.cpp Robot.h House.h AbstractAlgorithm.h AbstractSensor.h OurSensor.h Battery.h Score.h
	g++ -c $(CFLAGS) $*.cpp 

Score.o: Score.cpp Score.h
	g++ -c $(CFLAGS) $*.cpp 

SimpleIniFileParser.o: SimpleIniFileParser.cpp SimpleIniFileParser.h
	g++ -c $(CFLAGS) $*.cpp 

Simulator.o: Simulator.cpp Simulator.h House.h Score.h AbstractAlgorithm.h AbstractSensor.h OurSensor.h Robot.h Battery.h Point.h
	g++ -c $(CFLAGS) $*.cpp 


forBfs.o: forBfs.cpp forBfs.h
	g++ -c $(CFLAGS) $*.cpp 

AlgorithmRegistration.o: AlgorithmRegistration.cpp AlgorithmRegistration.h AlgorithmRegistrar.h
	g++ -c $(CFLAGS) $*.cpp 

AlgorithmRegistrar.o: AlgorithmRegistrar.cpp AlgorithmRegistrar.h
	g++ -c $(CFLAGS) $*.cpp 




clean:
	-rm $(TARGET)  *.so *.o