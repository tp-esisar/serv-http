#!/bin/bash

TEST_FOLDER=../tests
TESTBIN=../bin/mainTest.e


if [ ! -e $TESTBIN ]
then
    echo "binaire de test manquant"
    exit 1
fi


function visualTest {
    echo "####test fichier \"$1\" avec element \"$2\"####"
    $TESTBIN $TEST_FOLDER/$1 $2
    echo "####fin test \"$1\" avec \"$2\"####"
    echo "_________________________________________________________________________________________"
}


function unitTest {
    RESULT=$($TESTBIN $TEST_FOLDER/$1 $2 "silence")
    if [ "$RESULT" == "$3" ]; then
        printf "unitTest file \"$1\" with \"$2\" OK\n"
    else
        printf "unitTest file \"$1\" with \"$2\" FAILED\n"
        printf "  got: \"$RESULT\"\n"
        printf "  expect: \"$3\"\n"
    fi
}


testfiles=(`ls $TEST_FOLDER`)

echo "fichiers de test:" ${testfiles[*]}
echo "-------------debut tests visuels sur fichier de test------------------"

echo "-------------------------fin tests visuels----------------------------"
echo
echo "-------------debut tests unitaires sur fichiers de test---------------"
#fichier de test get1

unitTest "get1" "start-line" "GET / HTTP/1.1"
unitTest "get1" "method" "GET" 
unitTest "get1" "request-target" "/"
unitTest "get1" "HTTP-version" "HTTP/1.1"

unitTest "get1" "header-field" "Connection: Keep-Alive
Host: www.google.com
Accept: */*
User-Agent: Wget/1.16 (linux-gnu)"
unitTest "get1" "field-name" "Connection
Host
Accept
User-Agent"
unitTest "get1" "field-value" "Keep-Alive
www.google.com
*/*
Wget/1.16 (linux-gnu)"
unitTest "get1" "Host-header" "Host: www.google.com"
unitTest "get1" "Host" "www.google.com"


echo "-------------------------fin tests unitaires--------------------------"