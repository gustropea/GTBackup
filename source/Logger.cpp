#include "Logger.h"

Logger::Logger(std::string pathToFile){
    std::ifstream file(pathToFile);
    if(!file){
        std::ofstream file (pathToFile);
        if(file.is_open()){
            file << "#GTBackup-Log" << std::endl;
            file << "#Date,time,Profile,BackupLocation" << std::endl;
            file.close();
        }
    }
    file.close();
    setPathToLogFile(pathToFile);
    setDateTime();
}

void Logger::setPathToLogFile(std::string pathToFile){
    pathToLogFile = pathToFile;
}

void Logger::setDateTime(){
    std::time_t now = std::time(0);
    tm *ltm = localtime(&now);
    std::string day = std::to_string(ltm->tm_mday);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string hour = std::to_string(1 + ltm->tm_hour);
    std::string minute = std::to_string(1 + ltm->tm_min);
    std::string sec = std::to_string(1 + ltm->tm_sec);

    date = month +"-"+day+"-"+year;
    time = hour+":"+minute+":"+sec;
}

void Logger::setProfileName(std::string profName){
    profileName = profName;
}

void Logger::setBackupLocation(std::string backLoc){
    backupLocation = backLoc;
}

std::string Logger::getDate(){
    return date;
}

std::string Logger::getTime(){
    return time;
}

std::string Logger::getProfileName(){
    return profileName;
}

std::string Logger::getPathToLogFile(){
    return pathToLogFile;
}

std::string Logger::getBackupLocation(){
    return backupLocation;
}

std::vector<std::string> Logger::splitLine(std::string line){
    std::vector<std::string> split;
    std::string temp = "";
    for(int i=0; i<line.length(); i++){
        if(line[i] == ','){
            split.push_back(temp);
            temp = "";
        }
        else if(i == line.length()-1){
            temp+=line[i];
            split.push_back(temp);
            temp ="";
        }
        else{
            temp+=line[i];
        }
    }
    return split;
}

void Logger::addEntry(std::string profName, std::string backLoc){
    setProfileName(profName);
    setBackupLocation(backLoc);
    write();
}

std::vector<std::string> Logger::getLast(){
    std::vector<std::string> result = read(false,false);
    std::size_t check = result[0].find("empty");
    if(check == std::string::npos){
        return splitLine(result[0]);
    }
    else{
        return result;
    }
}

std::vector<std::string> Logger::getLastInstance(std::string profName){
    setProfileName(profName);
    std::vector<std::string> result = read(true,false);
    std::size_t check = result[0].find("empty");
    if(check == std::string::npos){
        return splitLine(result[0]);
    }
    else{
        return result;
    }
}

std::vector<std::vector<std::string>> Logger::getLog(){
    std::vector<std::string> result = read(false, true);
    std::size_t check = result[0].find("empty");
    if(check == std::string::npos){
        std::vector<std::vector<std::string>> fullLog;
        for(int i=0; i<result.size(); i++){
            fullLog.push_back(splitLine(result[i]));
        }
        return fullLog;
    }else{
        std::vector<std::vector<std::string>> emptyLog;
        emptyLog.push_back(result);
        return emptyLog;
    }
}

std::vector<std::string> Logger::read(bool seek, bool dump){
    std::vector<std::string> result;
    std::vector<std::string> emptyVec;
    emptyVec.push_back("empty");
    std::ifstream file(getPathToLogFile());
    if (file.is_open()) {
      std::string line;
      while (getline(file, line)) {
          std::size_t comment = line.find("#");
          if(comment == std::string::npos){
              result.push_back(line);
          }
      }
      file.close();
    }
    if(dump==true){
        if(!result.empty()){
            return result;
        }
        else{
            return emptyVec;
        }
    }
    if(seek == false){
        if(!result.empty()){
            std::vector<std::string> last;
            int s = result.size();
            last.push_back(result[s-1]);
            return last;
        }
        else{
            return emptyVec;
        }
    }
    else{
        std::vector<std::string> seekResults;
        for(int i=0; i < result.size(); i++){
            std::size_t name = result[i].find(getProfileName());
            if(name != std::string::npos){
                seekResults.push_back(result[i]);
            }
        }
        if(!seekResults.empty()){
            std::vector<std::string> lastSeek;
            lastSeek.push_back(seekResults[seekResults.size()-1]);
            return lastSeek;
        }else{
            return emptyVec;
        }
    }
}

void Logger::write(){
    std::string entry = getDate()+","+getTime()+","+getProfileName()+","+getBackupLocation();
    int currentUid = getuid();
    setuid(0);
    std::ofstream file;
    file.open(getPathToLogFile(), std::ios::app);
    if(file.is_open()){
        file << entry << std::endl;
        file.close();
    }
    setuid(currentUid);
}
