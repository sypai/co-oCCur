/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "dactylogram.h"

co_oCCur::Dactylogram::Dactylogram()
= default;

co_oCCur::Dactylogram::Dactylogram(std::string FileName)
{
    m_AudioFileName = std::move(FileName);
    m_ctx = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
    m_temp_sample_num = 0;
}

co_oCCur::Dactylogram::~Dactylogram()
= default;

void co_oCCur::Dactylogram::readAudio()
{
    WaveFileData * file = new WaveFileData(m_AudioFileName);
    file->read();
    m_Samples = file->getSamples();
    m_NumberOfSamples = file->m_TotalSamples;

    delete file;
}

void co_oCCur::Dactylogram::igniteChromaprint()
{
    readAudio();

    if(!chromaprint_start(m_ctx, SampleRate, NumOfChannels))
    {
        std::cout << "[ERROR] : Could not initialize the fingerprinting process." << std::endl;
        exit(2);
    }

    if(!chromaprint_feed(m_ctx, &m_Samples[0], m_NumberOfSamples))
    {
        std::cout << "[ERROR] : Could not process the audio data." << std::endl;
        exit(2);
    }

    if(!chromaprint_finish(m_ctx))
    {
        std::cout << "[ERROR] : Could not finish the fingerprinting process." << std::endl;
        exit(2);
    }

}

void co_oCCur::Dactylogram::collectFingerprints()
{
    DEBUG0 << "Initializing audiofingerprinting generation.\n";
    igniteChromaprint();

    m_NumberOfSecondsPrecise = (double)m_NumberOfSamples / SampleRate;
    m_NumberOfSeconds = m_NumberOfSamples / SampleRate;
    m_NumberOfMilliSeconds = (long int)(m_NumberOfSecondsPrecise * 1000);

    DEBUG0 << "The audio is " << m_NumberOfSecondsPrecise << " seconds in length.";

    std::vector<uint32_t> AudioFingerprints;

    using namespace chromaprint;

    uint32_t *raw_fp_data = nullptr;
    int raw_fp_size = 0;

    if (!chromaprint_get_raw_fingerprint(m_ctx, &raw_fp_data, &raw_fp_size))
    {
        std::cout << "[ERROR] : Could not receive the fingerprints." << std::endl;
        exit(2);
    }

    SCOPE_EXIT(chromaprint_dealloc(raw_fp_data));

    AudioFingerprints.reserve(raw_fp_size);
    for (int i = 0; i < raw_fp_size; i++)
    {
        AudioFingerprints.emplace_back(raw_fp_data[i]);
    }

    m_AllFingerprints = AudioFingerprints;
    m_AudioFingerprints.reserve(m_NumberOfSeconds);
    int j = 0;
    for (int i = 1; i < m_NumberOfSeconds; i++)
    {
        std::vector<uint32_t > array ;
        int count = 0;
        while(j < raw_fp_size)
        {
            if (i % 13 != 0)
            {
                if(count < 8)
                {
                    array.emplace_back(AudioFingerprints[j]);
                    j++;
                    count++;
                    continue;
                }
                else
                {
                    m_AudioFingerprints.emplace_back(array);
                    break;
                }
            }

            else
            {
                if(count < 9)
                {
                    array.emplace_back(AudioFingerprints[j]);
                    j++;
                    count++;
                    continue;
                }
                else
                {
                    m_AudioFingerprints.emplace_back(array);
                    break;
                }
            }
        }
    }
    DEBUG0 << "Audiofingerprints successfully generated.";
}

std::vector<std::vector<uint32_t> > co_oCCur::Dactylogram::getFingerprints() const
{
    return m_AudioFingerprints;
}

std::vector<uint32_t> co_oCCur::Dactylogram::getAllFingerprints() const
{
    return m_AllFingerprints;
}

double co_oCCur::Dactylogram::getAudioLength_secs() const
{
    return m_NumberOfSecondsPrecise;
}

long int co_oCCur::Dactylogram::getAudioLength_ms() const
{
    return m_NumberOfMilliSeconds;
}