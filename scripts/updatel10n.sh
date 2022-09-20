#!/bin/bash

# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

LUPDATE=`which lupdate-qt5`
if [ ! -x $LUPDATE ]
then
    LUPDATE=`which lupdate`
fi

if [ -x $LUPDATE ]
then
    echo "Found lupdate at $LUPDATE"
else
    echo "Can neither find lupdate-qt nor lupdate"
    exit 1
fi

if [ ! -d translations ]; then
    mkdir translations
    if [ ! -d translations ]; then
        echo "Failed to create translations directory!"
        exit 1
    fi
fi

$LUPDATE -no-obsolete -source-language en -target-language en -locations none Fuoten -ts translations/libfuoten.ts
