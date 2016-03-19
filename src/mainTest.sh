#!/bin/bash

TEST_FOLDER=../tests
TESTBIN=../bin/mainTest.e


if [ ! -e $TESTBIN ]
then
    echo "binaire de test manquant"
    exit 1
fi



testfiles=(`ls $TEST_FOLDER`)

echo "fichiers de test:" ${testfiles[*]}

for testfile in ${testfiles[*]}
do
    $TESTBIN $TEST_FOLDER/$testfile "methode"
done
