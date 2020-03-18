#!/bin/bash
echo "Building GTBackup..."
cd source
make
make clean
wait
cd ..
echo "Build Complete..."
echo "Installing..."
sudo cp source/GTBackup /usr/local/bin/GTBackup
sudo mkdir /etc/GTBackup
sudo cp supportingDocs/GTBackup.conf /etc/GTBackup/GTBackup.conf
sudo cp supportingDocs/log.txt /etc/GTBackup/log.txt
sudo cp supportingDocs/GTBackup /usr/local/man/man8/GTBackup.1
sudo gzip /usr/local/man/man8/GTBackup.1
echo "Setting permisions..."
sudo chown root /usr/local/bin/GTBackup
sudo chown root /etc/GTBackup/GTBackup.conf
sudo chown root /etc/GTBackup/log.txt
sudo chmod =xs /usr/local/bin/GTBackup
sudo chmod =rw /etc/GTBackup/GTBackup.conf
sudo chmod =rw /etc/GTBackup/log.txt
source ~/.bash_profile
echo "GTBackup successfully installed"
