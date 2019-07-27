#!/bin/bash

echo "[Check Debug Path]"
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

if [ ! -d 'out/android/apks/debug' ];
then
	echo "out/android/apks/debug not exsit, make out/android/apks/debug"
	mkdir out/android/apks/debug
fi

echo "[Clear Debug APK]"
echo ""
rm -rf out/android/apks/debug/*.apk

echo "[Packaging Debug APK]"
echo ""
ant -buildfile proj.android_test/build.xml debug