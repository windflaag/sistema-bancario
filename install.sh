#!/bin/bash

mkdir -p $DESTDIR/usr/share/
mkdir -p $DESTDIR/usr/bin/

cp -r runtime $DESTDIR/usr/share/sistema-bancario
cp -r build/main $DESTDIR/usr/bin/sistema-bancario
