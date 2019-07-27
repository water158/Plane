#!/bin/bash

echo "[Check release Path]"
echo ""

if [ ! -d 'out/' ];
then
	echo "out/ not exsit, make out/"
	mkdir out
fi

if [ ! -d 'out/android' ];
then
	echo "out/android not exsit, make out/android"
	mkdir out/android
fi

if [ ! -d 'out/android/apks' ];
then
	echo "out/android/apks not exsit, make out/android/apks"
	mkdir out/android/apks
fi

if [ ! -d 'out/android/apks/release' ];
then
	echo "out/android/apks/release not exsit, make out/android/apks/release"
	mkdir out/android/apks/release
fi

echo "[Clear release APK]"
echo ""
rm -rf out/android/apks/release/*.apk

echo "[Packaging release APK]"
echo ""
ant -buildfile proj.android_$1/build.xml release