cmake_minimum_required(VERSION 3.10)
project(co_oCCur C CXX)

set(CMAKE_CXX_STANDARD 14)

include(CMakePushCheckState)
include(CheckFunctionExists)

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

#
#### FFmpeg audio processor
#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
#
#find_package(FFmpeg REQUIRED)
#
#if(FFMPEG_LIBRARIES)
#    cmake_push_check_state(RESET)
#    set(CMAKE_REQUIRED_LIBRARIES ${FFMPEG_LIBRARIES} -lm)
#    check_function_exists(av_packet_unref HAVE_AV_PACKET_UNREF)
#    check_function_exists(av_frame_alloc HAVE_AV_FRAME_ALLOC)
#    check_function_exists(av_frame_free HAVE_AV_FRAME_FREE)
#    cmake_pop_check_state()
#endif()
#
##set(AUDIO_PROCESSOR_LIB "avresample")
##
##if(AUDIO_PROCESSOR_LIB STREQUAL "avresample")
##    if(FFMPEG_LIBAVRESAMPLE_FOUND)
##        set(USE_AVRESAMPLE ON)
##        set(AUDIO_PROCESSOR_LIBRARIES ${FFMPEG_LIBAVRESAMPLE_LIBRARIES})
##        set(AUDIO_PROCESSOR_INCLUDE_DIRS ${FFMPEG_LIBAVRESAMPLE_INCLUDE_DIRS})
##     endif()
##endif()
#
#set(AUDIO_PROCESSOR_LIB "swresample")
#
#if(AUDIO_PROCESSOR_LIB STREQUAL "swresample")
#    if(FFMPEG_LIBSWRESAMPLE_FOUND)
#        set(USE_SWRESAMPLE ON)
#        set(AUDIO_PROCESSOR_LIBRARIES ${FFMPEG_LIBSWRESAMPLE_LIBRARIES})
#        set(AUDIO_PROCESSOR_INCLUDE_DIRS ${FFMPEG_LIBSWRESAMPLE_INCLUDE_DIRS})
#    endif()
#endif()
#
#include_directories(
#        ${FFMPEG_LIBAVFORMAT_INCLUDE_DIRS}
#        ${FFMPEG_LIBAVCODEC_INCLUDE_DIRS}
#        ${FFMPEG_LIBAVUTIL_INCLUDE_DIRS}
#        ${AUDIO_PROCESSOR_INCLUDE_DIRS}
#)
#
#set(audioProcessingLIBS
#        ${FFMPEG_LIBAVFORMAT_LIBRARIES}
#        ${FFMPEG_LIBAVCODEC_LIBRARIES}
#        ${FFMPEG_LIBAVUTIL_LIBRARIES}
#        ${AUDIO_PROCESSOR_LIBRARIES}
#        ${CMAKE_THREAD_LIBS_INIT}
#        )
#
#add_library(ffmpegfiles ${audioProcessingLIBS})
#set_target_properties(ffmpegfiles PROPERTIES LINKER_LANGUAGE C)
#target_link_libraries(ffmpegfiles ${audioProcessingLIBS})

## chromaprint for audiofingerprinting
include_directories(libs/lib_ext/chromaprint)

set(kissfftFiles
        libs/lib_ext/chromaprint/vendor/kissfft/kiss_fft.h
        libs/lib_ext/chromaprint/vendor/kissfft/kiss_fft.c
        libs/lib_ext/chromaprint/vendor/kissfft/tools/kiss_fftr.h
        libs/lib_ext/chromaprint/vendor/kissfft/tools/kiss_fftr.c
        )

#set(audioFiles
#        libs/lib_ext/chromaprint/src/audio/audio_slicer.h
#        libs/lib_ext/chromaprint/src/audio/ffmpeg_audio_processor_swresample.h
#        libs/lib_ext/chromaprint/src/audio/ffmpeg_audio_processor.h
#        libs/lib_ext/chromaprint/src/audio/ffmpeg_audio_reader.h
#        )

set(avFiles
        libs/lib_ext/chromaprint/src/avresample/avcodec.h
        libs/lib_ext/chromaprint/src/avresample/resample2.c
        )

set(chromaprintFiles
        libs/lib_ext/chromaprint/src/audio_consumer.h
        libs/lib_ext/chromaprint/src/audio_processor.cpp
        libs/lib_ext/chromaprint/src/audio_processor.h
        libs/lib_ext/chromaprint/src/chroma.h
        libs/lib_ext/chromaprint/src/chroma.cpp
        libs/lib_ext/chromaprint/src/chroma_filter.h
        libs/lib_ext/chromaprint/src/chroma_filter.cpp
        libs/lib_ext/chromaprint/src/chroma_normalizer.h
        libs/lib_ext/chromaprint/src/chroma_resampler.h
        libs/lib_ext/chromaprint/src/chroma_resampler.cpp
        libs/lib_ext/chromaprint/src/classifier.h
        libs/lib_ext/chromaprint/src/debug.h
        libs/lib_ext/chromaprint/src/feature_vector_consumer.h
#        libs/lib_ext/chromaprint/src/fft_lib_avfft.h
#        libs/lib_ext/chromaprint/src/fft_lib_avfft.cpp
        libs/lib_ext/chromaprint/src/fft_lib_kissfft.h
        libs/lib_ext/chromaprint/src/fft_lib_kissfft.cpp
        libs/lib_ext/chromaprint/src/fft_lib.h
        libs/lib_ext/chromaprint/src/fft_frame.h
        libs/lib_ext/chromaprint/src/fft_frame_consumer.h
        libs/lib_ext/chromaprint/src/fft.h
        libs/lib_ext/chromaprint/src/fft.cpp
#        libs/lib_ext/chromaprint/src/fft_test.cpp
        libs/lib_ext/chromaprint/src/filter.h
        libs/lib_ext/chromaprint/src/filter_utils.h
        libs/lib_ext/chromaprint/src/fingerprint_calculator.h
        libs/lib_ext/chromaprint/src/fingerprint_calculator.cpp
        libs/lib_ext/chromaprint/src/fingerprint_compressor.h
        libs/lib_ext/chromaprint/src/fingerprint_compressor.cpp
        libs/lib_ext/chromaprint/src/fingerprint_decompressor.h
        libs/lib_ext/chromaprint/src/fingerprint_decompressor.cpp
        libs/lib_ext/chromaprint/src/fingerprint_matcher.h
        libs/lib_ext/chromaprint/src/fingerprint_matcher.cpp
        libs/lib_ext/chromaprint/src/fingerprinter.h
        libs/lib_ext/chromaprint/src/fingerprinter.cpp
        libs/lib_ext/chromaprint/src/fingerprinter_configuration.h
        libs/lib_ext/chromaprint/src/fingerprinter_configuration.cpp
        libs/lib_ext/chromaprint/src/image.h
        libs/lib_ext/chromaprint/src/image_builder.h
        libs/lib_ext/chromaprint/src/image_builder.cpp
        libs/lib_ext/chromaprint/src/moving_average.h
        libs/lib_ext/chromaprint/src/quantizer.h
        libs/lib_ext/chromaprint/src/silence_remover.h
        libs/lib_ext/chromaprint/src/silence_remover.cpp
        libs/lib_ext/chromaprint/src/simhash.h
        libs/lib_ext/chromaprint/src/simhash.cpp
        libs/lib_ext/chromaprint/src/spectrum.h
        libs/lib_ext/chromaprint/src/spectrum.cpp
        libs/lib_ext/chromaprint/src/utils.h
        libs/lib_ext/chromaprint/src/chromaprint.h
        libs/lib_ext/chromaprint/src/chromaprint.cpp
        )

set(chromaprintUtils
        libs/lib_ext/chromaprint/src/utils/base64.h
        libs/lib_ext/chromaprint/src/utils/base64.cpp
        libs/lib_ext/chromaprint/src/utils/gaussian_filter.h
        libs/lib_ext/chromaprint/src/utils/gradient.h
        libs/lib_ext/chromaprint/src/utils/pack_int3_array.h
        libs/lib_ext/chromaprint/src/utils/pack_int5_array.h
        libs/lib_ext/chromaprint/src/utils/unpack_int3_array.h
        libs/lib_ext/chromaprint/src/utils/unpack_int5_array.h
        libs/lib_ext/chromaprint/src/utils/rolling_integral_image.h
        libs/lib_ext/chromaprint/src/utils/scope_exit.h
        )

#add_library(chromaprint ${avFiles} ${audioFiles} ${kissfftFiles} ${chromaprintUtils} ${chromaprintFiles})
add_library(chromaprint ${avFiles} ${kissfftFiles} ${chromaprintUtils} ${chromaprintFiles})
set_target_properties(chromaprint PROPERTIES FOLDER libs/lib_ext/chromaprint)


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
set_target_properties(ccaligner PROPERTIES FOLDER libs/lib_ext/CCAligner)

###### END INCLUDE libs

###### INCLUDE co-oCCur sources

include_directories(src/ src/utils/)

set(SourceFiles
        src/utils/dactylogram.h
        src/utils/dactylogram.cpp
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
        src/main.cpp
        )

add_executable(co_oCCur ${SourceFiles})
target_link_libraries(co_oCCur webrtc chromaprint ccaligner ${EXTRA_FLAGS})
#target_link_libraries(co_oCCur webrtc ffmpegfiles chromaprint ccaligner ${EXTRA_FLAGS} zlib.a -lz)