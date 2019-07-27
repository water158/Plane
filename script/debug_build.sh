APPNAME="MGame"

# options

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
-h	this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# ... use paths relative to current directory
COCOS2DX_ROOT="${MTL_ENV}/libs/cocos2d-x-2.2.5"
APP_ROOT="$DIR/../"
APP_ANDROID_ROOT="$DIR/../proj.android_test"

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"

# make sure assets is exist
if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi

mkdir "$APP_ANDROID_ROOT"/assets

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

# run ndk-build
if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
    "$NDK_ROOT"/ndk-build -j4 -C "$APP_ANDROID_ROOT" $* \
    	"NDK_APPLICATION_MK=${APP_ANDROID_ROOT}/jni/ApplicationDebug.mk" \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source"
else
    echo "Using prebuilt externals"
    "$NDK_ROOT"/ndk-build -j4 -C "$APP_ANDROID_ROOT" $* \
        "NDK_APPLICATION_MK=${APP_ANDROID_ROOT}/jni/ApplicationDebug.mk" \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt:${COCOS2DX_ROOT}/external" \
        "NDK_OUT=${APP_ANDROID_ROOT}/../out/android/temp/debug/obj/" \
        "NDK_APP_DST_DIR=${APP_ANDROID_ROOT}/../out/android/temp/debug/native/armeabi"

    #"$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" $* \
    #    "NDK_APPLICATION_MK=${APP_ANDROID_ROOT}/jni/ApplicationDebug-v7a.mk" \
    #    "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt" \
    #    "NDK_OUT=${APP_ANDROID_ROOT}/../out/android/temp/debug/obj/" \
    #    "NDK_APP_DST_DIR=${APP_ANDROID_ROOT}/../out/android/temp/debug/native/armeabi-v7a"
fi
