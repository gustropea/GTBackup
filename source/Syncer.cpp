#include "Syncer.h"

Syncer::Syncer(WINDOW* mw, WINDOW* tw, WINDOW* pw){
  setMainWindow(mw);
  setTaskWindow(tw);
  setProgressWindow(pw);
}

void Syncer::setMainWindow(WINDOW* mw){
  mainWin = mw;
}

void Syncer::setTaskWindow(WINDOW* tw){
  taskWin = tw;
}

void Syncer::setProgressWindow(WINDOW* pw){
  progressWin = pw;
}

void Syncer::setBackupLocation(std::string backLoc){
  backupLocation = backLoc;
}

void Syncer::setMaxProgress(int mp){
  maxProgress = mp;
}

void Syncer::setCurrentProgress(int cp){
  updateProgress();
  currentProgress = cp;
}

void Syncer::setCurrentTask(std::string ct){
  currentTask = ct;
  updateTask();
}

WINDOW* Syncer::getMainWindow(){
  return mainWin;
}

WINDOW* Syncer::getTaskWindow(){
  return taskWin;
}

WINDOW* Syncer::getProgressWindow(){
  return progressWin;
}

std::string Syncer::getBackupLocation(){
  return backupLocation;
}

int Syncer::getMaxProgress(){
  return maxProgress;
}

int Syncer::getCurrentProgress(){
  return currentProgress;
}

std::string Syncer::getCurrentTask(){
  return currentTask;
}

void Syncer::updateTask(){
  wclear(getTaskWindow());
  refresh();
  wrefresh(getTaskWindow());
  wprintw(getTaskWindow(), getCurrentTask().c_str());
  refresh();
  wrefresh(getTaskWindow());
}

void Syncer::updateProgress(){
  int inc = 1;
  int cp = getCurrentProgress();
  int mp = getMaxProgress();
  int margin = 2;
  int width = COLS-(2*margin);
  int unit = (width/mp);
  wmove(getProgressWindow(),0,(cp*unit)+margin);
  int updatedProgress = cp;
  if ((cp+inc) == mp){
    updatedProgress = width;
  }
  else{
    updatedProgress = (cp+inc)*unit;
  }
  for(int i=((cp*unit)+margin); i < updatedProgress+margin; i++){
    init_pair(1, COLOR_BLACK, COLOR_RED);
    wattron(getProgressWindow(),COLOR_PAIR(1));
    waddstr(getProgressWindow()," ");
    wattroff(getProgressWindow(),COLOR_PAIR(1));
  }
    refresh();
    wrefresh(getProgressWindow());
}

void Syncer::runner(std::vector<std::string> src, bool recursive, std::string username, std::string servername){
  for(int i=0; i < src.size(); i++){
    setCurrentTask("Task(s): copying "+src[i]+" to "+getBackupLocation()+"...");
    //Build command strings
    std::string backloc = getBackupLocation();
    std::string netsrc = username+"@"+servername+":"+src[i];
    char* args[]={"rsync", "-av", (char*)src[i].c_str(), (char*)backloc.c_str(), NULL};
    char* argsn[]={"rsync", "-av", (char*)netsrc.c_str(), (char*)backloc.c_str(), NULL};
    char* argsr[]={"rsync", "-arv", (char*)src[i].c_str(), (char*)backloc.c_str(), NULL};
    char* argsnr[]={"rsync", "-arv", (char*)netsrc.c_str(), (char*)backloc.c_str(), NULL};
    //Prepare for new proccess
    int buffSize = 1024;
    char buff[buffSize];
    int fd[2];
    int status;
    pipe(fd);
    if (pipe(fd) == -1) {
      exit(1);
    }
    pid_t pid=fork();
    if (pid == -1) {
      exit(1);
    }else if (pid == 0){
      // child                                                         
      while (dup2(fd[1], STDOUT_FILENO) == -1) {}
      close(fd[1]);
      close(fd[0]);
      if(username=="none" && servername=="none"){
          if(recursive == true){
              //non-network recursive
              execvp(argsr[0], argsr);
          }
          else{
              //non-network non-recursive
              execvp(args[0], args);
          }
      }
      else{
          if(recursive == true){
              //network recursive
              execvp(argsnr[0], argsnr);
          }
          else{
              //network non-recursive
              execvp(argsn[0], argsn);
          }
      }
    }
    close(fd[1]);
    while (1) {
      ssize_t bytes_read = read(fd[0], buff, buffSize-1);
      if (bytes_read == -1) {
          exit(1);
      }
      else if (bytes_read == 0) {
          break;
      }
      else {
          buff[bytes_read]='\n';
          buff[bytes_read]='\0';
          wprintw(getMainWindow(), buff);
          refresh();
          wrefresh(getMainWindow());
      }
    }
    close(fd[0]);
    wait(&status);
    setCurrentProgress(getCurrentProgress()+1);
  }
}

void Syncer::recursiveSync(std::vector<std::string> folders){
  runner(folders, true, "none", "none");
}

void Syncer::recursiveNetSync(std::string username, std::string servername, std::vector<std::string> folders){
  runner(folders, true, username, servername);
}

void Syncer::sync(std::vector<std::string> files){
  runner(files, false, "none", "none");
}

void Syncer::netSync(std::string username, std::string servername, std::vector<std::string>files){
  runner(files, false, username, servername);
}
