#!/bin/bash
source ./scripts/config
prerequisite wget
prerequisite mktemp
prerequisite tar
tmpdir=$(mktemp --directory)
wget -q $URL/pro.$architecture.tar.gz -O $tmpdir/pro.tar.gz || quit "An error occured while downloading pro.tar.gz." "Abort."
wget -q $URL/etu.tar.gz               -O $tmpdir/etu.tar.gz || quit "An error occured while downloading etu.tar.gz." "Abort."
tar -xz  -f $tmpdir/pro.tar.gz                 > /dev/null 2>&1 || quit "An error occured while extracting pro.tar.gz." "Abort."
tar -xz --skip-old-files -f $tmpdir/etu.tar.gz > /dev/null 2>&1 || quit "An error occured while extracting etu.tar.gz." "Abort."
rm -rf $tmpdir/
chmod +x ./scripts/*.sh
print "Your project has been updated successfully."
print "Please read README.txt carefully."
