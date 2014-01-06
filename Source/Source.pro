include (../QMake/Ardoor.pri)
include (../QMake/Opencv.pri)
#include (../QMake/Nvwa.pri)

TARGET = Ardoor

TEMPLATE = lib
CONFIG += qt shared dll
#CONFIG += qt staticlib
QT += core gui opengl

DEFINES += ARDOOR_LIBRARY ARDOOR_DLL

LIBS += \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \

android: LIBS += -lGLESv2

HEADERS += \
    Ardoor/PoseEstimation/PoseEstimator.hpp \
    Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp \
    #Ardoor/CameraWindow/matgen.h \
    #Ardoor/CameraWindow/glUtils.hpp \
    #Ardoor/CameraWindow/CameraWindow.hpp \
    Ardoor/CameraCalib/CameraConfiguration.hpp \
    Ardoor/CameraCalib/CameraCalib.hpp \
    Ardoor/Capture/Capture.hpp \
    Ardoor/Ardoor.hpp \
    Ardoor/Rendering/RenderingContext.hpp \
    Ardoor/Rendering/Geometry.hpp \
    Ardoor/Rendering/CubeGeometry.hpp \
    Ardoor/Rendering/CoordinateAxisGeometry.hpp \
    Ardoor/Rendering/ChessboardRenderingContext.hpp \
    Ardoor/Context/ArdoorContext.hpp \
    Ardoor/Rendering/TexturedRectangleGeometry.hpp \
    Ardoor/PoseEstimation/Types.hpp \
    Ardoor/Rendering/OpenGLWindow.hpp \
    Ardoor/Capture/FrameProcessor.hpp \
    Ardoor/Capture/ImageHandler.hpp \
    Ardoor/Rendering/ArRenderingContext.hpp \
    Ardoor/Rendering/GLUtils.hpp \
    Ardoor/DoorDetection/DoorDetector.hpp \
    Ardoor/DoorDetection/LinePoint.hpp \
    Ardoor/DoorDetection/LineSegment.hpp \
    Ardoor/DoorDetection/LSWMS.hpp \
    Ardoor/DoorDetection/SegmentDistance.hpp \
    Ardoor/PoseEstimation/DoorPoseEstimator.hpp \
    Ardoor/Rendering/DoorRenderingContext.hpp \
    Ardoor/Projection/ProjectionUtil.hpp

SOURCES += \
    Ardoor/PoseEstimation/PoseEstimator.cpp \
    Ardoor/PoseEstimation/ChessboardPoseEstimator.cpp \
    #Ardoor/CameraWindow/matgen.cpp \
    #Ardoor/CameraWindow/main.cpp \
    #Ardoor/CameraWindow/glUtils.cpp \
    #Ardoor/CameraWindow/CameraWindow.cpp \
    Ardoor/CameraCalib/CameraConfiguration.cpp \
    Ardoor/CameraCalib/CameraCalib.cpp \
    Ardoor/Capture/Capture.cpp \
    Ardoor/Rendering/Geometry.cpp \
    Ardoor/Rendering/CubeGeometry.cpp \
    Ardoor/Rendering/CoordinateAxisGeometry.cpp \
    Ardoor/Rendering/ChessboardRenderingContext.cpp \
    Ardoor/Context/ArdoorContext.cpp \
    Ardoor/Rendering/TexturedRectangleGeometry.cpp \
    Ardoor/Rendering/OpenGLWindow.cpp \
    Ardoor/Capture/FrameProcessor.cpp \
    Ardoor/Rendering/ArRenderingContext.cpp \
    Ardoor/Rendering/GLUtils.cpp \
    Ardoor/DoorDetection/LSWMS.cpp \
    Ardoor/DoorDetection/LineSegment.cpp \
    Ardoor/DoorDetection/LinePoint.cpp \
    Ardoor/DoorDetection/DoorDetector.cpp \
    Ardoor/PoseEstimation/DoorPoseEstimator.cpp \
    Ardoor/Rendering/DoorRenderingContext.cpp \
    Ardoor/Projection/ProjectionUtil.cpp

RESOURCES += \
    ../Data/Shader/shaders.qrc
