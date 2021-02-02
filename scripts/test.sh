#!/bin/bash
echo lancement script  # ------------------------------- begin
# ------------------------------------------------------------
mkdir -p /tmp/polyshell.tests/
find . -name **.etu.c >  /tmp/polyshell.tests/etu.files.txt
find . -name **.etu.c >> /tmp/polyshell.tests/etu.files.txt
rm /tmp/polyshell.tests/does.not.exist.1 2>  /tmp/polyshell.tests/errors.txt
rm /tmp/polyshell.tests/does.not.exist.1 2>> /tmp/polyshell.tests/errors.txt
rm /tmp/polyshell.tests/does.not.exist.2  >> /tmp/polyshell.tests/errors.txt 2>&1
touch /tmp/polyshell.tests/e.1
touch /tmp/polyshell.tests/e.2
rm --verbose /tmp/polyshell.tests/e.1 /tmp/polyshell.tests/e.2 /tmp/polyshell.tests/ne 2>> /tmp/polyshell.tests/errors.txt | grep 1 > /tmp/polyshell.tests/e.1.txt
grep 1 < /tmp/polyshell.tests/e.1.txt | grep 1 > /tmp/polyshell.tests/e.2.txt
find . -name **.c | grep -v .etu.c | tee -a /tmp/polyshell.tests/etu.files.txt
echo test/* | xargs -n 1 | sort > /tmp/polyshell.tests/wildcard.txt
echo include/*/imp*/*.h | xargs -n 1 | sort >> /tmp/polyshell.tests/wildcard.txt
cd /tmp/polyshell.tests/
touch e.3
touch e.4
echo toto > e.3
echo tata > e.4
# ------------------------------------------------------------
exit  # -------------------------------------------------- end
