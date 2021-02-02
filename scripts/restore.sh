#!/bin/bash
source ./scripts/config
prerequisite tar
prerequisite mktemp
prerequisite curl
start
tempdir=$(mktemp --directory)
curl -s "$URL/upload.php?username=$user&password=$pass" \
     -o $tempdir/backup.tar.gz \
     -w "X_RESPONDE_CODE_X=%{response_code}" | grep "X_RESPONDE_CODE_X=200" > /dev/null 2>&1 || quit "The download has failed." "Have you ever backed up your work?" \
                                                                                                     "Are your credentials correct?" "Are you connected to Internet?" "Abort."
mkdir -p backup/
backupdir=$(mktemp --directory -p backup/)
cp $(find src include -name *.etu.?) $backupdir/
tar -xzf $tempdir/backup.tar.gz || quit "An error occured while extracting backup.tar.gz." "Abort."
rm -rf $tempdir/
end
print "Your project has been updated successfully. Old files have been backed up to the ./backup directory."
