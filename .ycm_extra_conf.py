def Settings(**kwargs):
    return {
        "flags": ["-std=c++11", "-c", "-pipe", "-O2", "-Wall", "-W", "-D_REENTRANT", "-fPIC", "-DQT_DEPRECATED_WARNINGS", "-DQT_NO_DEBUG", "-DQT_WIDGETS_LIB", "-DQT_GUI_LIB", "-DQT_CORE_LIB", "-I.", "-isystem",  "/usr/include/x86_64-linux-gnu/qt5", "-isystem", "/usr/include/x86_64-linux-gnu/qt5/QtWidgets", "-isystem", "/usr/include/x86_64-linux-gnu/qt5/QtGui", "-isystem", "/usr/include/x86_64-linux-gnu/qt5/QtCore", "-I.", "-isystem", "/usr/include/libdrm",
            "-I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++", "-isystem", "/usr/local/include", "-I", "include"]
    }

