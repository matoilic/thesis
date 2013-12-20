include (../QMake/Library.pri)
include (../QMake/Nvwa.pri)
include (../QMake/Opencv.pri)


TARGET = Ardoor

LIBS += \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lglfw \
    -lglew \
    -lopengl32

INCLUDEPATH += \
    $$ARDOOR_DIR/Libraries/glew/include \
    $$ARDOOR_DIR/Libraries/glfw/include

HEADERS += \
    Ardoor/PoseEstimation/PoseEstimator.hpp \
    Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp \
    Ardoor/CameraWindow/matgen.h \
    Ardoor/CameraWindow/glUtils.hpp \
    Ardoor/CameraWindow/CameraWindow.hpp \
    Ardoor/CameraCalib/CameraConfiguration.hpp \
    Ardoor/CameraCalib/CameraCalib.hpp \
    Ardoor/Capture/Capture.hpp \
    Ardoor/LineDetection/Types.hpp \
    Ardoor/LineDetection/PointQuadTree.hpp \
    Ardoor/LineDetection/LSDLineDetector.hpp \
    Ardoor/LineDetection/lsd.h \
    Ardoor/LineDetection/LineDetector.hpp \
    Ardoor/Ardoor.hpp \
    Ardoor/Graphics/OpenCvTexture.hpp

SOURCES += \
    Ardoor/PoseEstimation/PoseEstimator.cpp \
    Ardoor/PoseEstimation/ChessboardPoseEstimator.cpp \
    Ardoor/CameraWindow/matgen.cpp \
    Ardoor/CameraWindow/main.cpp \
    Ardoor/CameraWindow/glUtils.cpp \
    Ardoor/CameraWindow/CameraWindow.cpp \
    Ardoor/CameraCalib/CameraConfiguration.cpp \
    Ardoor/CameraCalib/CameraCalib.cpp \
    Ardoor/Capture/Capture.cpp \
    Ardoor/LineDetection/LSDLineDetector.cpp \
    Ardoor/LineDetection/lsd.c \
    Ardoor/Graphics/OpenCvTexture.cpp

