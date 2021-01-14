# GTBackup

GTBackup is a Unix command line utility written in C++. The purpose of GTBackup is to provide an easy to use  versatile tool for system backups. GTBackup is based on rsync and features a curses UI.

## Getting Started

### Defualt Installation:
Download and extract the zip file. from within the GTBackup directory run the command "sudo ./install.sh". This will build the program from source and place the executable in /usr/local/bin. The supporting documents will be copied to /etc/GTBackup. After the script has finished the program can be run from the command line using "GTBackup".

### Custom installation:
The defualt installation requires the GTBackup executable and the supporting documents to be owned by root. To install without root. Go the source directory. Open main.cpp and change the PATH_TO_CONF_FILE as well as PATH_TO_LOG_FILE definitions to the new location where you will place the supporting documents. Run the "make" command, and move the resulting executable to the new install location.

## Features

- Profiles allow for multiple configurations.
... This enables every user of the machine to have their own unique profile. Any user can perform a backup from their account or from an administrator account.

- Network support allows for remote backups.
... This gives users the ability to backup remote servers to their local machine or vice versa.

- Curses UI provides real time progress.
- Logging allows users to know how up to date their backups are.
- Fast backupS.
... Since GTBackup is based on rsync, only the changes between backups are copied. This makes for fast backups.


