TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt



INCLUDEPATH += /usr/local/include \
/usr/local/include/opencv4 \
/usr/local/include/opencv4/opencv2




LIBS += /usr/local/lib/libopencv_calib3d.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so\
        /usr/local/lib/libopencv_objdetect.so\
        /usr/local/lib/libopencv_photo.so \
        /usr/local/lib/libopencv_dnn.so \
        /usr/local/lib/libopencv_features2d.so \
        /usr/local/lib/libopencv_stitching.so \
        /usr/local/lib/libopencv_flann.so\
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_video.so\
        /usr/local/lib/libopencv_ml.so


SOURCES += \
        main.cpp

HEADERS += \
    string_process.h


