#ifndef Logger_H
#define Logger_H

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

class Logger{
	private:
    
        std::string date;
        std::string time;
        std::string pathToLogFile;
        std::string profileName;
        std::string backupLocation;
    
        void setPathToLogFile(std::string pathToFile);
        void setDateTime();
        void setProfileName(std::string profName);
        void setBackupLocation(std::string backLoc);
        
        std::string getDate();
        std::string getTime();
        std::string getProfileName();
        std::string getBackupLocation();

        std::vector<std::string> splitLine(std::string line);
        std::string getPathToLogFile();
        void write();
        std::vector<std::string> read(bool seek, bool dump);
    
    public:
        
        Logger(std::string pathToFile);
        void addEntry(std::string profName, std::string backLoc);
        std::vector<std::string> getLast();
        std::vector<std::string> getLastInstance(std::string profName);
        std::vector<std::vector<std::string>> getLog();
};

#endif
