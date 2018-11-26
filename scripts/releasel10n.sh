#!/bin/bash

if [ $(which lrelease-qt5) ]; then
        LRELEASE=`which lrelease-qt5`
        echo "Found lrelease at $LRELEASE"
fi

if [ ! -x $LUPDATE ]; then
        if [ $(which lrelease) ]; then
                LRELEASE=`which lrelease`
                echo "Found lrelease at $LRELEASE"
        else
                echo "Can not find executable for lrelease!"
                exit 1
        fi
fi

if [ ! -d translations ]; then
	echo "Directory translations not available!"
	echo "Maybe you have to run scripts/updatesl10n.sh at first."
	echo 1
fi

$LRELEASE -idbased translations/libfuoten.ts

for LANG in da de en_US en_GB fr nl sv
do
$LRELEASE -idbased translations/libfuoten_$LANG.ts
done
