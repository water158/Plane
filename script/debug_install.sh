#!/bin/bash

echo "[Uninstall]"
echo ""
adb uninstall com.ferrisgame.ambition
echo "[Install]"
echo ""
adb install out/android/apks/debug/ambition-debug.apk