#!/bin/bash

LRELEASE=`which lrelease-qt5`
if [ ! -x $LRELEASE ]
then
    LRELEASE=`which lrelease`
fi

if [ -x $LRELEASE ]
then
    echo "Found lrelease at $LRELEASE"
else
    echo "Can neither find lrelease nor lrelease-qt5"
    exit 1
fi

if [ ! -d translations ]; then
    echo "Directory translations not available!"
    echo "Maybe you have to run scripts/updatel10n.sh at first."
    echo 1
fi

$LRELEASE -idbased translations/libfuoten.ts

for LANG in da de en_US en_GB fr nl sv zh_CN
do
$LRELEASE -idbased translations/libfuoten_$LANG.ts
done
