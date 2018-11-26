#!/bin/bash

if [ $(which lupdate-qt5) ]; then
	LUPDATE=`which lupdate-qt5`
	echo "Found lupdate at $LUPDATE"
fi

if [ ! -x $LUPDATE ]; then
	if [ $(which lupdate) ]; then
		LUPDATE=`which lupdate`
		echo "Found lupdate at $LUPDATE"
	else
		echo "Can not find executable for lupdate!"
		exit 1
	fi
fi

if [ ! -d translations ]; then
    mkdir translations
    if [ ! -d translations ]; then
	echo "Failed to create translations directory!"
	exit 1
    fi
fi

$LUPDATE -no-obsolete -pluralonly -source-language en -target-language en_GB -locations none Fuoten -ts translations/libfuoten_en_GB.ts
$LUPDATE -no-obsolete -pluralonly -source-language en -target-language en_US -locations none Fuoten -ts translations/libfuoten_en_US.ts
$LUPDATE -no-obsolete -source-language en -target-language en -locations none Fuoten -ts translations/libfuoten.ts
