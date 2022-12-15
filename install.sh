#!/bin/bash

mkdir -p $DESTDIR/usr/share/
mkdir -p $DESTDIR/usr/bin/
mkdir -p $DESTDIR/var/log/sistema-bancario

cp -r runtime $DESTDIR/usr/share/sistema-bancario
cp -r build/main $DESTDIR/usr/bin/sistema-bancario
