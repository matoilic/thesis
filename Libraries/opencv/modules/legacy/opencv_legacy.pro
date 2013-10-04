include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_ml \
    -lopencv_flann \
    -lopencv_video \
    -lopencv_highgui

HEADERS += \
    include/opencv2/legacy/streams.hpp \
    include/opencv2/legacy/legacy.hpp \
    include/opencv2/legacy/compat.hpp \
    include/opencv2/legacy/blobtrack.hpp \
    src/facetemplate.h \
    src/facedetection.h \
    src/face.h \
    src/_vm.h \
    src/_vectrack.h \
    src/_matrix.h \
    src/_kdtree.hpp \
    src/_featuretree.h \
    src/_facedetection.h

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/video.cpp \
    src/vecfacetracking.cpp \
    src/trifocal.cpp \
    src/texture.cpp \
    src/testseq.cpp \
    src/subdiv2.cpp \
    src/stereogc.cpp \
    src/spilltree.cpp \
    src/snakes.cpp \
    src/segment.cpp \
    src/scanlines.cpp \
    src/pyrsegmentation.cpp \
    src/prewarp.cpp \
    src/precomp.cpp \
    src/planardetect.cpp \
    src/pgh.cpp \
    src/optflowlk.cpp \
    src/optflowhs.cpp \
    src/optflowbm.cpp \
    src/oneway.cpp \
    src/morphing.cpp \
    src/morphcontours.cpp \
    src/lsh.cpp \
    src/lmeds.cpp \
    src/lines.cpp \
    src/levmartrif.cpp \
    src/levmarprojbandle.cpp \
    src/levmar.cpp \
    src/lee.cpp \
    src/lcm.cpp \
    src/kdtree.cpp \
    src/image.cpp \
    src/hmmobs.cpp \
    src/hmm1d.cpp \
    src/hmm.cpp \
    src/findhandregion.cpp \
    src/findface.cpp \
    src/featuretree.cpp \
    src/features2d.cpp \
    src/facetemplate.cpp \
    src/facedetection.cpp \
    src/face.cpp \
    src/extendededges.cpp \
    src/epilines.cpp \
    src/enteringblobdetectionreal.cpp \
    src/enteringblobdetection.cpp \
    src/enmin.cpp \
    src/em.cpp \
    src/eigenobjects.cpp \
    src/dpstereo.cpp \
    src/dominants.cpp \
    src/decomppoly.cpp \
    src/createhandmask.cpp \
    src/corrimages.cpp \
    src/correspond.cpp \
    src/contourtree.cpp \
    src/condens.cpp \
    src/compat.cpp \
    src/clique.cpp \
    src/camshift.cpp \
    src/calonder.cpp \
    src/calibfilter.cpp \
    src/calcimagehomography.cpp \
    src/blobtrackpostproclist.cpp \
    src/blobtrackpostproclinear.cpp \
    src/blobtrackpostprockalman.cpp \
    src/blobtrackingmsfgs.cpp \
    src/blobtrackingmsfg.cpp \
    src/blobtrackinglist.cpp \
    src/blobtrackingccwithcr.cpp \
    src/blobtrackingcc.cpp \
    src/blobtrackingauto.cpp \
    src/blobtrackgenyml.cpp \
    src/blobtrackgen1.cpp \
    src/blobtrackanalysistrackdist.cpp \
    src/blobtrackanalysisior.cpp \
    src/blobtrackanalysishist.cpp \
    src/blobtrackanalysis.cpp \
    src/blobtrack.cpp \
    src/bgfg_estimation.cpp \
    src/bgfg_common.cpp \
    src/bgfg_codebook.cpp \
    src/bgfg_acmmm2003.cpp \
    src/auxutils.cpp \
    src/3dtracker.cpp \
    src/bgfg_gaussmix_legacy.cpp

