#include "Syncer.h"
#include "Profile.h"
#include "Logger.h"
#include <curses.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

//Change the path to the conf and log files here
#define PATH_TO_CONF_FILE "/etc/GTBackup/GTBackup.conf"
#define PATH_TO_LOG_FILE "/etc/GTBackup/log.txt"

void setupTitleWindow(WINDOW *win){
  wmove(win, 0, COLS/2);
  refresh();
  wprintw(win, "GT Backup");
  refresh();
  wrefresh(win);
}

void setupContainerWindow(WINDOW *win){
  box(win,0,0);
  refresh();
  wrefresh(win);
}

void setupMainWindow(WINDOW *win, std::string profName){
  refresh();
  wattron(win, COLOR_PAIR(2));
  wprintw(win,"Processing configuration file... \n");
  wattroff(win, COLOR_PAIR(2));
  refresh();
  wrefresh(win);
  sleep(1);
  if(profName == "default"){
    wattron(win, COLOR_PAIR(2));
    wprintw(win,"No profile specified, Loading default profile... \n");
    wattroff(win, COLOR_PAIR(2));
    refresh();
    wrefresh(win);
  }
  else{
    wattron(win,COLOR_PAIR(2));
    std::string str = "Loading "+profName+" profile...\n";
    wprintw(win, str.c_str());
    wattroff(win, COLOR_PAIR(2));
    refresh();
    wrefresh(win);
  }
  scrollok(win, TRUE);
  keypad(win, TRUE);
}

void setupTaskWindow(WINDOW *win){
  refresh();
  wprintw(win, "Task(s): Initializing...");
  refresh();
  wrefresh(win);
}

void setupProgressWindow(WINDOW *win){
  refresh();
  wmove(win,0,2);
  for(int i=0; i<COLS-4; i++){
    waddch(win,ACS_CKBOARD);
  }
  wmove(win,0,0);
  refresh();
  wrefresh(win);
}

void printHelp(){
    std::cout << "GTBackup" << std::endl;
    std::cout << std::endl;
    std::cout << "Program Usage:" << std::endl;
    std::cout << std::setw(40) << std::left << "\t -last:" << "Prints information on the last backup." << std::endl;
    std::cout << std::setw(40) << std::left << "\t -last profilename:" << "Prints the last backup of the specified profile." << std::endl;
    std::cout << std::setw(40) << std::left << "\t -log:" << "Prints a log of all backups." << std::endl;
    std::cout << std::setw(40) << std::left << "\t -help:" << "Prints this help screen." << std::endl;
    std::cout << std::setw(40) << std::left << "\t path-to-backuplocation:" << "backs up the system using the defualt profile to specified location." << std::endl;
    std::cout << std::setw(40) << std::left << "\t profilename path-to-backuplocation: " << "backs up system using the specified profile to the specified location." << std::endl;
    std::cout << std::endl;
    std::cout << "Note(s):" << std::endl;
    std::cout << "\t -The profiles can be found and edited in the configuration file." << std::endl;
    std::cout << "\t -The configuration file can be found in /etc/GTBackup." << std::endl;
    std::cout << std::endl;
}

void printLastEntry(){
    Logger log(PATH_TO_LOG_FILE);
    std::vector<std::string> last = log.getLast();
    std::size_t check = last[0].find("empty");
    if(check == std::string::npos){
        std::cout << "Last Backup: " << std::endl;
        std::cout << std::left << std::setfill(' ') << std::setw(20) << "\t"+last[0] << std::setw(20) << last[1] << std::setw(20) << last[2] << std::setw(20) << last[3] << std::endl;
    }
    else{
        std::cout << "Log is empty." << std::endl;
    }
}

void printLastInstance(std::string profName){
    Logger log(PATH_TO_LOG_FILE);
    std::vector<std::string> last = log.getLastInstance(profName);
    std::size_t check = last[0].find("empty");
    if(check == std::string::npos){
        std::cout << "Last Backup of " << profName << ": " << std::endl;
        std::cout << std::left << std::setfill(' ') << std::setw(20) << "\t"+last[0] << std::setw(20) << last[1] << std::setw(20) << last[2] << std::setw(20) << last[3] << std::endl;
    }
    else{
        std::cout << "There is no entry with that profile name." << std::endl;
    }
}

void printLog(){
    Logger log(PATH_TO_LOG_FILE);
    std::vector<std::vector<std::string>> fullLog = log.getLog();
    std::size_t check = fullLog[0][0].find("empty");
    if(check == std::string::npos){
        std::cout << "Backup Log" << std::endl;
        std::cout << std::left << std::setfill(' ') << std::setw(20) << "\t Day" << std::setw(20) << "Time" << std::setw(20) << "Profile" << std::setw(20) <<"Backup Location" << std::endl;        for(int i=0; i < fullLog.size(); i++){
            std::cout << std::left << std::setfill(' ') << std::setw(20) << "\t"+fullLog[i][0] << std::setw(20) << fullLog[i][1] << std::setw(20) << fullLog[i][2] << std::setw(20) << fullLog[i][3] << std::endl;
        }
    }
    else{
        std::cout << "Log is empty" << std::endl;
    }
}


int start(std::string profileName, std::string backupLocation){
    
    //start curses
    initscr();
    noecho();
    cbreak();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    clear();
    
    //setup windows
    WINDOW *titleWin = newwin(LINES*(0.05),COLS,0,0);
    setupTitleWindow(titleWin);

    WINDOW *containerWin = newwin((LINES*(0.85)),COLS,(LINES*(0.05)),0);
    setupContainerWindow(containerWin);

    WINDOW* mainWin = newwin(LINES*(0.85)-2,COLS-2,LINES*(0.05)+1,1);
    setupMainWindow(mainWin, profileName);

    WINDOW* taskWin = newwin(LINES*(0.05),COLS,LINES*(0.90),0);
    setupTaskWindow(taskWin);

    WINDOW* progressWin = newwin(LINES*(0.05),COLS,LINES*(0.95),0);
    setupProgressWindow(progressWin);
    
    sleep(2);
    
    Syncer backup(mainWin, taskWin, progressWin);
    backup.setBackupLocation(backupLocation);
    Profile prof(PATH_TO_CONF_FILE);
    
    int id = prof.loadProfile(profileName);
    std::vector<std::string> fs = prof.getFolders();
    for(int i = 0; i < fs.size(); i++){
        wprintw(mainWin, fs[i].c_str());
        wprintw(mainWin, "\n");
    }
    if(id != 0){
        wattron(mainWin, COLOR_PAIR(4));
        wprintw(mainWin, "Error: Failed to load profile.\n");
        wattroff(mainWin, COLOR_PAIR(4));
        if(id == 1){
            wattron(mainWin, COLOR_PAIR(4));
            wprintw(mainWin, "Error: Bad configuration file or syntax error.\n");
            wattroff(mainWin, COLOR_PAIR(4));
        }
        else if(id == 2){
            wattron(mainWin, COLOR_PAIR(4));
            wprintw(mainWin, "Error: Could not find specified profile name in configuration file.\n");
            wattroff(mainWin, COLOR_PAIR(4));
        }
        else{
            wattron(mainWin, COLOR_PAIR(4));
            wprintw(mainWin, "Error: An unknown error has occured.\n");
            wattroff(mainWin, COLOR_PAIR(4));
        }
    }
    else{
        backup.setMaxProgress(prof.getFolders().size()+prof.getFiles().size());
        wattron(mainWin, COLOR_PAIR(2));
        wprintw(mainWin, "Profile successfully loaded...\n");
        wattroff(mainWin, COLOR_PAIR(2));
        sleep(1);
        wattron(mainWin,COLOR_PAIR(2));
        wprintw(mainWin, "Starting backup...\n");
        wattroff(mainWin,COLOR_PAIR(2));
        if(prof.getNetworking() == true){
            if(prof.getFolders().size() > 0){
                backup.recursiveNetSync(prof.getUserName(), prof.getServerName(), prof.getFolders());
            }
            if(prof.getFiles().size() > 0){
                backup.netSync(prof.getUserName(), prof.getServerName(), prof.getFiles());
            }
        }
        else{
            if(prof.getFolders().size() > 0){
                backup.recursiveSync(prof.getFolders());
            }
            if(prof.getFiles().size() > 0){
                backup.sync(prof.getFiles());
            }
        }
        sleep(1);
        wattron(mainWin,COLOR_PAIR(2));
        wprintw(mainWin, "Logging backup...\n");
        wattroff(mainWin,COLOR_PAIR(2));
        Logger log(PATH_TO_LOG_FILE);
        log.addEntry(profileName, backupLocation);
    }
    
    sleep(1);
    
    wattron(mainWin,COLOR_PAIR(2));
    wprintw(mainWin,"Back up is complete. \n");
    wattroff(mainWin,COLOR_PAIR(2));
    wattron(mainWin,COLOR_PAIR(3));
    wprintw(mainWin,"Press any key to quit. \n");
    wattroff(mainWin,COLOR_PAIR(3));
    wclear(taskWin);
    wprintw(taskWin, "Task(s): Complete");
    refresh();
    wrefresh(mainWin);
    wrefresh(taskWin);
    getch();
    //exiting
    delwin(titleWin);
    delwin(containerWin);
    delwin(mainWin);
    delwin(taskWin);
    delwin(progressWin);
    endwin();
    return 0;
}

int main(int argc, char* argv[]){
    std::string backupLocation;
    std::string profileName;
    std::ifstream file(PATH_TO_CONF_FILE);
    if(!file){
        //conf file is not installe
        std::cout << "Error: The configuration file was not found in /etc/GTBackup." << std::endl;
        exit(1);
    }
    else{
        file.close();
        if(argc <= 1){
            std::cout << "Error: Too few arguements." << std::endl;
            printHelp();
            exit(1);
        }
        else if(argc > 1){
            if(argc == 2){
                if(strcmp(argv[1],"-last")==0){
                    printLastEntry();
                }
                else if(strcmp(argv[1], "-help")==0){
                    printHelp();
                }
                else if(strcmp(argv[1], "-log") == 0){
                    printLog();
                }
                else{
                    char* results = strstr(argv[1], "/");
                    if (results != NULL){
                        std::ifstream file(argv[1]);
                        if(!file){
                            //Backup location does not exist
                            std::cout << "Error: The specified backup location does not exist." << std::endl;
                            exit(1);
                        }
                        else{
                            //Backup location exists
                            profileName = "default";
                            if(argv[1] != NULL){
                                backupLocation = argv[1];
                            }
                        }
                        file.close();
                        start(profileName, backupLocation);
                    }
                    else{
                        printHelp();
                        exit(1);
                    }
                }
            }
            else if(argc == 3){
                if(strcmp(argv[1], "-last")==0){
                    if(argv[2] != NULL){
                        profileName = argv[2];
                    }
                    printLastInstance(profileName);
                }
                else{
                    if (argv[1] != NULL){
                        profileName = argv[1];
                    }
                    char* results = strstr(argv[2], "/");
                    if (results != NULL){
                        std::ifstream file(argv[2]);
                        if(!file){
                            //Backup location does not exist
                            std::cout << "Error: The specified backup location does not exist." << std::endl;
                            exit(1);
                        }
                        else{
                            //Backup location exists
                            if(argv[2] != NULL){
                                backupLocation = argv[2];
                            }
                        }
                        file.close();
                        start(profileName, backupLocation);
                    }
                    else{
                        printHelp();
                        exit(1);
                    }
                }
            }
            else if(argc > 3) {
                std::cout << "Error: Too many arguements." << std::endl;
                printHelp();
                exit(1);
            }
        }
    }

  return 0;
  
}
