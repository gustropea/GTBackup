#include "Profile.h"

Profile::Profile(std::string pathToConf){
    setPathToFile(pathToConf);
}

void Profile::setPathToFile(std::string pathToConf){
    pathToFile = pathToConf;
}

std::string Profile::getPathToFile(){
    return pathToFile;
}

void Profile::setName(std::string n){
  name=n;
}

void Profile::setNetworking(bool isNet){
  networking = isNet;
}

void Profile::setUserName(std::string user){
  username = user;
}

void Profile::setServerName(std::string server){
  servername = server;
}

void Profile::addFolder(std::string f){
  folders.push_back(f);
}

void Profile::addFile(std::string f){
  files.push_back(f);
}

std::string Profile::getName(){
  return name;
}

bool Profile::getNetworking(){
  return networking;
}

std::string Profile::getUserName(){
  return username;
}

std::string Profile::getServerName(){
  return servername;
}

std::vector<std::string> Profile::getFolders(){
  return folders;
}

std::vector<std::string> Profile::getFiles(){
  return files;
}

//Scan conf file for syntax errors.
 bool Profile::fileOk(){
   bool checksOut = true;
   std::ifstream file(getPathToFile());
   if (file.is_open()) {
     std::string line;
     //Switch on when in a profile block.
     int inBlock = 0;
     while (getline(file, line)) {
       std::size_t comment = line.find("#");
       //If the current line is not blank and not commented out continue.
       if (line.length() > 0 && comment == std::string::npos){
	std::size_t space = line.find(" ");
	std::size_t colon = line.find(":");
	std::size_t prof = line.find("profile");
	std::size_t f = line.find("files");
	//If profile keyword is in current line flip switch on.
	if (prof!=std::string::npos && inBlock == 0){
	  inBlock = 1;
	}
	if(inBlock == 1){
	  if(colon!=std::string::npos){
	    if (space!=std::string::npos){
	      //There is an illegal space in the profile block.
	      checksOut = false;
	    }
	  }
	  else{
	    //There is a missing colon in the profile block.
	    checksOut = false;
	  }
	}
	//if files keyword is in current line flip switch off.
	if (f!=std::string::npos && inBlock == 1){
	  inBlock = 0;
	}
      }
    }
    file.close();
  }
  return checksOut;
}

//Scan conf file for specific profile name.
bool Profile::validateName(std::string prof_name){
  bool namePresent = false;
  std::ifstream file(getPathToFile());
  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      std::size_t comment = line.find("#");
      //If current line is not blank and not commented out continue.
      if (line.length() > 0 && comment == std::string::npos){
	std::size_t p = line.find("profile");
	std::size_t n = line.find(prof_name);
	//If the keyword profile and specified name have been found.
	if (p!=std::string::npos && n!=std::string::npos){
	  namePresent = true;
	  break;
	}
      }
    }
    file.close();
  }
  return namePresent;
}

int Profile::loadProfile(std::string prof_name){
  if(fileOk() == false){
    //Return syntax error.
    return 1;
  }
  else{
    //Return no name match found.
    if(validateName(prof_name)==false){
      return 2;
    }
    else{
      std::ifstream file(getPathToFile());
      if (file.is_open()) {
	std::string line;
	//In profile block switch.
	int inBlock = 0;
	//Networking enabled switch.
	int isNetworking = 0;
	while (getline(file, line)) {
	  std::size_t comment = line.find("#");
	  //If current line is not blank and not commented out continue.
	  if (line.length() > 0 && comment == std::string::npos){
	    //check for keywords.
	    std::size_t p = line.find("profile");
	    std::size_t n = line.find(prof_name);
	    std::size_t net = line.find("network");
	    std::size_t user = line.find("username");
	    std::size_t server = line.find("servername");
	    std::size_t fo = line.find("folders");
	    std::size_t fi = line.find("files");
	    //If profile keyword and profile name are in line flip switch on.
	    if (p!=std::string::npos && n!=std::string::npos){
	      inBlock = 1;
	      setName(prof_name);
	    }
	    //check to see if networking is enabled and set switch.
	    if (net!=std::string::npos && inBlock == 1){
	      std::size_t colon = line.find(":"); 
	      std::string temp = line.substr(colon+1, (line.length())-(colon+1));
	      if (temp.compare("yes") == 0){
		isNetworking = 1;
		setNetworking(true);
	      }
	      else{
		setNetworking(false);
	      }
	    }
	    //If networking is enabled, set username and servername.
	    if (user!=std::string::npos && inBlock == 1 && isNetworking == 1){
	      std::size_t colon = line.find(":");
	      std::string temp = line.substr(colon+1, (line.length())-(colon+1));
	      setUserName(temp);
	    }
	    if (server!=std::string::npos && inBlock == 1 && isNetworking == 1){
	      std::size_t colon = line.find(":");
	      std::string temp = line.substr(colon+1, (line.length())-(colon+1));
	      setServerName(temp);
	    }
	    //If folders is not an empty list go through and add them.
	    if (fo!=std::string::npos && inBlock == 1){
	      std::size_t result = line.find("[]");
	      if(result==std::string::npos){
		std::size_t b1 = line.find("[");
		std::size_t b2 = line.find("]");
		std::string s = line.substr(b1+1, (b2)-(b1+1));
		std::string temp = "";
		for(int i=0; i<s.length(); i++){
		  if(s[i] == ','){
		    addFolder(temp);
		    temp = "";
		  }
		  else if(i == s.length()-1){
		    temp+=s[i];
		    addFolder(temp);
		    temp ="";
		  }
		  else{
		    temp+=s[i];
		  }
		}
	      }
	    }
	    //If files is not an empty list go through and add them.
	    if (fi!=std::string::npos && inBlock == 1){
	      std::size_t result = line.find("[]");
	      if (result==std::string::npos){
		std::size_t b1 = line.find("[");
		std::size_t b2 = line.find("]");
		std::string s = line.substr(b1+1, (b2)-(b1+1));
		std::string temp = "";
		for(int i=0; i<s.length(); i++){
		  if(s[i] == ','){
		    addFile(temp);
		    temp = "";
		  }
		  else if(i == s.length()-1){
		    temp+=s[i];
		    addFile(temp);
		    temp="";
		  }
		  else{
		    temp+=s[i];
		  }
		}
		inBlock = 0;
		break;
	      }
	      else{
		inBlock = 0;
		break;
	      }
	    }
	  }
	}   
	file.close();
	//Return success.
	return 0;
      }
    }
  }
}
