CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -lncurses

GTBackup: main.o Profile.o Syncer.o Logger.o
	$(CXX) $(CXXFLAGS) -o GTBackup main.o Profile.o Syncer.o Logger.o

main.o: main.cpp Profile.h Syncer.h Logger.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Profile.o: Profile.h

Syncer.o: Syncer.h

Logger.o: Logger.h

clean:
	rm -f *.o