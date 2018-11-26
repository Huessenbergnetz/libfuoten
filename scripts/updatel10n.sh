#!/bin/bash

if [ ! -d translations ]; then
    mkdir translations
fi

lupdate-qt5 -no-obsolete -pluralonly -source-language en -target-language en_GB -locations none Fuoten -ts translations/libfuoten_en_GB.ts
lupdate-qt5 -no-obsolete -pluralonly -source-language en -target-language en_US -locations none Fuoten -ts translations/libfuoten_en_US.ts
lupdate-qt5 -no-obsolete -source-language en -target-language en -locations none Fuoten -ts translations/libfuoten.ts
