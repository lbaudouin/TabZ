#!/bin/bash
rm ui_mainwindow.h
qmake-qt4 -spec win32-x-g++ -o win32/
cd win32
make -j 3
cd ..
