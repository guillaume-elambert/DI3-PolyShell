#!/bin/bash
source ./scripts/config
if [[ $# -ne 1 ]]
then
	quit "Invalid number of arguments." "Abort."
fi
if ! [[ $1 = "projet-c" ]] && ! [[ $1 = "unix" ]]
then
	quit "Invalid submission. '$1' is not a valid value." "Abort."
fi
prerequisite curl
prerequisite tar
prerequisite mktemp
prerequisite diff
make --no-print-directory update
make --no-print-directory TARGET='release' rebuild || quit "Your code must compile." "Abort."
make --no-print-directory TARGET='debug'   rebuild || quit "Your code must compile." "Abort."
binome1=$(./bin/debug/PolyShell --names | cut --delimiter '&' --fields 1)
binome2=$(./bin/debug/PolyShell --names | cut --delimiter '&' --fields 2)
fn1=$(echo $binome1 | cut --delimiter '$' --fields 1)
ln1=$(echo $binome1 | cut --delimiter '$' --fields 2)
fn2=$(echo $binome2 | cut --delimiter '$' --fields 1)
ln2=$(echo $binome2 | cut --delimiter '$' --fields 2)
printf "partner 1 is FIRSTNAME = %-20s LASTNAME = %-20s\n" $fn1 $ln1
printf "partner 2 is FIRSTNAME = %-20s LASTNAME = %-20s\n" $fn2 $ln2
echo -n "Is it correct? yes/no=" && read yn
if [[ $yn = "yes" ]]
then
	print "You are about to submit your '$1' project." "It will be backed up beforehand (make save)."
	echo -n "Proceed? yes/no=" && read yn
	if [[ $yn = "yes" ]]
	then
		make --no-print-directory save
		start
		getdata mail email
		mail=$retval
		tempdir=$(mktemp --directory)
		tar -czf $tempdir/$1.tar.gz $(find src include -name *.etu.?) || quit "An error occured while building $1.tar.gz" "Abort."
		curl -s -F "operation=upload" -F "file=@$tempdir/$1.tar.gz" "$URL/upload.php?username=$user&password=$pass&mail=$mail" \
		     -o $tempdir/check.tar.gz \
		     -w "X_RESPONDE_CODE_X=%{response_code}" | grep "X_RESPONDE_CODE_X=200" > /dev/null 2>&1 || quit "The upload has failed." "Are your credentials correct?" \
                                                                                                                     "Are you connected to Internet?" "Abort."
		diff $tempdir/$1.tar.gz $tempdir/check.tar.gz > /dev/null 2>&1 || quit "An unexepected error occured." "Please try again." "Abort."
		rm -rf $tempdir/
		end
		print "Your '$1' project has been submitted successfully." "Check your mailbox!"
	else
		quit "Abort."
	fi
else
	quit "Please edit main.etu.c first." "Abort."
fi
