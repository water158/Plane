APP_STL := gnustl_static
APP_CPPFLAGS += -frtti
APP_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
APP_CPPFLAGS += -DCOCOS2D_DEBUG=0
APP_CPPFLAGS += -DENCRYPTION_ENABLE=1
APP_CPPFLAGS += -Wno-error=format-security
APP_CPPFLAGS += -fexceptions
APP_CFLAGS += -Wno-error=format-security
APP_ABI := armeabi