#ifndef SYNCER_H
#define SYNCER_H

#include<string>
#include<vector>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<curses.h>


class Syncer{

 private:

  WINDOW* mainWin;
  WINDOW* taskWin;
  WINDOW* progressWin;
  std::string backupLocation;
  int maxProgress;
  int currentProgress = 0;
  std::string currentTask;
  
  void runner(std::vector<std::string> src, bool recursive, std::string username, std::string servername);
  void updateProgress();
  void updateTask();

 public:
  
  Syncer(WINDOW* mw, WINDOW* tw, WINDOW* pw);
  void setMainWindow(WINDOW* mw);
  void setTaskWindow(WINDOW* tw);
  void setProgressWindow(WINDOW* pw);
  void setBackupLocation(std::string backLoc);
  void setMaxProgress(int mp);
  void setCurrentProgress(int cp);
  void setCurrentTask(std::string ct);

  WINDOW* getMainWindow();
  WINDOW* getTaskWindow();
  WINDOW* getProgressWindow();
  std::string getBackupLocation();
  int getMaxProgress();
  int getCurrentProgress();
  std::string getCurrentTask(); 

  void recursiveSync(std::vector<std::string> folders);
  void recursiveNetSync(std::string username, std::string servername, std::vector<std::string> folders);
  void sync(std::vector<std::string> files);
  void netSync(std::string username, std::string servername, std::vector<std::string> files);


};

#endif
