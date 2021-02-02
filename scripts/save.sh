#!/bin/bash
source ./scripts/config
prerequisite curl
prerequisite tar
prerequisite mktemp
prerequisite diff
start
getdata mail email
mail=$retval
tempdir=$(mktemp --directory)
tar -czf $tempdir/backup.tar.gz $(find src include -name *.etu.?) || quit "An error occured while building backup.tar.gz." "Abort."
curl -s -F "operation=upload" -F "file=@$tempdir/backup.tar.gz" "$URL/upload.php?username=$user&password=$pass&mail=$mail" \
     -o $tempdir/check.tar.gz \
     -w "X_RESPONDE_CODE_X=%{response_code}" | grep "X_RESPONDE_CODE_X=200" > /dev/null 2>&1 || quit "The upload has failed." "Are your credentials correct?" \
                                                                                                     "Are you connected to Internet?" "Abort."
diff $tempdir/backup.tar.gz $tempdir/check.tar.gz > /dev/null 2>&1 || quit "An unexepected error occured." "Please try again." "Abort."
rm -rf $tempdir/
end
print "Your project has been backed up successfully."
