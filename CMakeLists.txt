cmake_minimum_required(VERSION 3.10)
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
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/cross_correlation.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/division_operations.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/downsample_fast.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/energy.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/get_scaling_square.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/min_max_operations.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_48khz.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_by_2_internal.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_fractional.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_init.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_inl.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/vector_scaling_operations.c
        libs/lib_ext/webrtc/webrtc/common_audio/signal_processing/include/
        libs/lib_ext/webrtc/webrtc/common_audio/vad/vad_core.c
        libs/lib_ext/webrtc/webrtc/common_audio/vad/vad_filterbank.c
        libs/lib_ext/webrtc/webrtc/common_audio/vad/vad_gmm.c
        libs/lib_ext/webrtc/webrtc/common_audio/vad/vad_sp.c
        libs/lib_ext/webrtc/webrtc/common_audio/vad/webrtc_vad.c
        )
add_library(webrtc ${VADFiles})
set_target_properties(webrtc PROPERTIES FOLDER libs/lib_ext/webrtc)

## CCAligner for audio processing
include_directories(libs/lib_ext/CCAligner)
set(ReadAudio
        libs/lib_ext/CCAligner/logger.h
        libs/lib_ext/CCAligner/logger.cpp
        libs/lib_ext/CCAligner/commons.h
        libs/lib_ext/CCAligner/commons.cpp
        libs/lib_ext/CCAligner/params.h
        libs/lib_ext/CCAligner/params.cpp
        libs/lib_ext/CCAligner/read_wav_file.h
        libs/lib_ext/CCAligner/read_wav_file.cpp
        )
add_library(ccaligner ${ReadAudio})
set_target_properties(webrtc PROPERTIES FOLDER libs/lib_ext/CCAligner)

###### END INCLUDE libs

###### INCLUDE co-oCCur sources

include_directories(src/ src/utils/)

set(SourceFiles
        src/utils/base64Strings.h
        src/utils/base64Strings.cpp
        src/utils/audioSegmentation.h
        src/utils/audioSegmentation.cpp
        src/tools/toolA.cpp
        src/tools/toolA.h
        src/tools/toolB.cpp
        src/tools/toolB.h
        src/co_occur.h
        src/co_occur.cpp
        src/main.cpp)

add_executable(co_oCCur ${SourceFiles})
target_link_libraries(co_oCCur webrtc ccaligner ${EXTRA_FLAGS})