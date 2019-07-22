/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_AUDIOFINGERPRINTER_H
#define CO_OCCUR_AUDIOFINGERPRINTER_H

#include <utility>
#include <chrono>
#include <iomanip>

#include "../../libs/lib_ext/CCAligner/read_wav_file.h"
#include "../../libs/lib_ext/chromaprint/src/chromaprint.h"
#include "../../libs/lib_ext/chromaprint/src/audio/ffmpeg_audio_reader.h"
#include "../../libs/lib_ext/chromaprint/src/utils/scope_exit.h"

// Number of channels in the audio
const static int NumOfChannels = 1;

// Sample rate of the audio
const static int SampleRate = 16000;

// Duration of the audio to be processed (in seconds)
static double MaxDuration;

// Split the audio into chunks of this duration
const static int MaxChunkDuration = 1;

namespace co_oCCur{

    #define FIXED_FLOAT(x) std::fixed << std::setprecision(2) << (x);

    class Dactylogram
    {
    private:

        std::string m_AudioFileName;
        std::vector<int16_t> m_Samples;
        unsigned long int m_NumberOfSamples;
        double m_NumberOfSeconds;
        unsigned long int m_NumberOfMilliSeconds;
        unsigned long int m_temp_sample_num;
        ChromaprintContext *m_ctx;

    public:
        Dactylogram();
        Dactylogram(std::string FileName);
        ~Dactylogram();
        void readAudio();
        void igniteChromaprint();

    private:
        double GetCurrentTimestamp();

        void read(const int16_t **data, size_t *size);
        void printResult(bool first, double timestamp, double duration);

    };
} // namespace co_oCCur

#endif //CO_OCCUR_AUDIOFINGERPRINTER_H
