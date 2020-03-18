#ifndef PROFILE_H
#define PROFILE_H

#include<string>
#include <fstream>
#include <vector>

class Profile{

 private:

  std::string pathToFile;
  std::string name;
  bool networking;
  std::string username;
  std::string servername;
  std::vector<std::string> folders;
  std::vector<std::string> files;
  
  void setPathToFile(std::string pathToConf);
  std::string getPathToFile();
  bool fileOk();
  bool validateName(std::string prof_name);

 public:
    
  Profile(std::string pathToConf);
  void setName(std::string prof_name);
  void setNetworking(bool isNet);
  void setUserName(std::string user);
  void setServerName(std::string server);
  void addFolder(std::string f);
  void addFile(std::string f);
  
  std::string getName();
  bool getNetworking();
  std::string getUserName();
  std::string getServerName();
  std::vector<std::string> getFolders();
  std::vector<std::string> getFiles();

  int loadProfile(std::string prof_name);

};


#endif
