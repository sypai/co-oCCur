cmake_minimum_required(VERSION 3.14)
project(co_oCCur)

set(CMAKE_CXX_STANDARD 14)

if(UNIX)
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread)
endif(UNIX)

###### INCLUDE libs

#### Include libs/lib_co-oCCur

## SiftSRT: Subtitle Parsing & Editing
include_directories(libs/lib_co-oCCur/SiftSRT)

#### Include libs/lib_ext

## webrtc for VAD
include_directories(libs/lib_ext/webrtc)
set(VADFiles
        libs/lib_ext/webrtc/common_audio/signal_processing/cross_correlation.c
        libs/lib_ext/webrtc/common_audio/signal_processing/division_operations.c
        libs/lib_ext/webrtc/common_audio/signal_processing/downsample_fast.c
        libs/lib_ext/webrtc/common_audio/signal_processing/energy.c
        libs/lib_ext/webrtc/common_audio/signal_processing/get_scaling_square.c
        libs/lib_ext/webrtc/common_audio/signal_processing/min_max_operations.c
        libs/lib_ext/webrtc/common_audio/signal_processing/resample_48khz.c
        libs/lib_ext/webrtc/common_audio/signal_processing/resample_by_2_internal.c
        libs/lib_ext/webrtc/common_audio/signal_processing/resample_fractional.c
        libs/lib_ext/webrtc/common_audio/signal_processing/spl_init.c
        libs/lib_ext/webrtc/common_audio/signal_processing/spl_inl.c
        libs/lib_ext/webrtc/common_audio/signal_processing/vector_scaling_operations.c
        libs/lib_ext/webrtc/common_audio/vad/vad_core.c
        libs/lib_ext/webrtc/common_audio/vad/vad_filterbank.c
        libs/lib_ext/webrtc/common_audio/vad/vad_gmm.c
        libs/lib_ext/webrtc/common_audio/vad/vad_sp.c
        libs/lib_ext/webrtc/common_audio/vad/webrtc_vad.c
        )
add_library(webrtc ${VADFiles})
set_target_properties(webrtc PROPERTIES FOLDER libs/lib_ext/webrtc)

## CCAligner for audio processing
include_directories(libs/lib_ext/CCAligner)
set(ReadAudio
        libs/lib_ext/CCAligner/src/lib_ccaligner/logger.h
        libs/lib_ext/CCAligner/src/lib_ccaligner/commons.h
        libs/lib_ext/CCAligner/src/lib_ccaligner/params.h
        libs/lib_ext/CCAligner/src/lib_ccaligner/read_wav_file.h
        libs/lib_ext/CCAligner/src/lib_ccaligner/read_wav_file.cpp
        )
add_library(ccaligner ${ReadAudio})
set_target_properties(webrtc PROPERTIES FOLDER libs/lib_ext/CCAligner)

###### END INCLUDE libs

###### INCLUDE co-oCCur sources

include_directories(src/ src/utils/)

set(SourceFiles
        src/utils/base64strings.h
        src/utils/base64strings.cpp
        src/utils/vad.h
        src/utils/vad.cpp
        src/toolA.cpp
        src/toolA.h
        src/toolB.cpp
        src/toolB.h
        src/co-occur.h
        src/co-occur.cpp
        )

add_executable(co_oCCur ${SourceFiles})
target_link_libraries(co_oCCur ccaligner webrtc ${EXTRA_FLAGS})