/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_AUDIOFINGERPRINTER_H
#define CO_OCCUR_AUDIOFINGERPRINTER_H

#include "../../libs/lib_ext/CCAligner/read_wav_file.h"
#include "../../libs/lib_ext/chromaprint/src/chromaprint.h"

namespace co_oCCur{

    class Dactylogram
    {
    private:
        // Number of channels in the audio
        const static int NumOfChannels = 1;

        // Sample rate of the audio
        const static int SampleRate = 16000;

        // Duration of the audio to be processed (in seconds)
        static double MaxDuration;

        // Split the audio into chunks of this duration
        const static int MaxChunkDuration = 1;

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

    private:
        void readAudio();
        void read(const int16_t **data, size_t *size);
        void igniteChromaprint();
    };
} // namespace co_oCCur

#endif //CO_OCCUR_AUDIOFINGERPRINTER_H
