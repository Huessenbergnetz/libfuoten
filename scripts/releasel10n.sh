#!/bin/bash

lrelease-qt5 -idbased translations/libfuoten.ts

for LANG in da de en_US en_GB fr nl sv
do
lrelease-qt5 -idbased translations/libfuoten_$LANG.ts
done
