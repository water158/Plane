#!/bin/bash

echo "[Uninstall]"
echo ""
adb uninstall com.ferrisgame.ambition
echo "[Install]"
echo ""
adb install out/android/apks/release/ambition_$1-release.apk