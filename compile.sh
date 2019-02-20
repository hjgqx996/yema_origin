#!/bin/sh
if [ ! -n "$1" ]
then
    echo "no parameter use default version"
    export VERSION_FLAGS=-DAPP_VERSION="\"\\\"testversion\\\"\""
else
#    echo "the word you input is $1"
    export VERSION_FLAGS=-DAPP_VERSION="\"\\\"$1\\\"\""
fi
    make clean&&make
