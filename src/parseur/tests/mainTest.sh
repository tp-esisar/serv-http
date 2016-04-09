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

#unitTest "get1" "start-line" "GET / HTTP/1.1\r"
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

#fichier de test get2

#unitTest "get2" "start-line" "GET / HTTP/1.1\r"
unitTest "get2" "method" "GET" 
unitTest "get2" "request-target" "/"
unitTest "get2" "HTTP-version" "HTTP/1.1"
unitTest "get2" "Host-header" "Host: www.cvedetails.com"
unitTest "get2" "Host" "www.cvedetails.com"
unitTest "get2" "User-Agent" "Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
unitTest "get2" "User-Agent-header" "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0"
unitTest "get2" "Accept-header" "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
unitTest "get2" "Accept" "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
unitTest "get2" "Accept-Language-header" "Accept-Language: fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3"
unitTest "get2" "Accept-Language" "fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3"
unitTest "get2" "Accept-Encoding-header" "Accept-Encoding: gzip, deflate"
unitTest "get2" "Accept-Encoding" "gzip, deflate"
unitTest "get2" "Connection-header" "Connection: keep-alive"
unitTest "get2" "Connection" "keep-alive"

#fichier de test get3
# unitTest "get3" "Strange-Header-header" "Strange-Header: gzip, def
#  ate"

#fichier de test get4
# unitTest "get4" "Strange-Header" "gzip, def
#  a
#  te"

#fichier de test get8
unitTest "get8" "method" ""

unitTest "get11" "message-body" "One ring to rule them all "



echo "-------------------------fin tests unitaires--------------------------"
