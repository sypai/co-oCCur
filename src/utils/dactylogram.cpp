/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include <utility>
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

void co_oCCur::Dactylogram::read(const int16_t **data, size_t *size)
{
    std::vector<int16_t> tempSamples;

    for (int i=0; i < 44666; i++)
    {
        tempSamples[i] = m_Samples[i + m_temp_sample_num];
    }

    *data = &tempSamples[0];
    *size = 44666;
}

void co_oCCur::Dactylogram::igniteChromaprint()
{
    m_NumberOfSeconds = (double)m_NumberOfSamples / SampleRate;
    m_NumberOfMilliSeconds = (unsigned long int)(m_NumberOfSeconds * 1000);

    MaxDuration = m_NumberOfSeconds + 1000;

    auto size = m_Samples.size();

    if (!chromaprint_start(m_ctx, SampleRate, NumOfChannels))
    {
        std::cout << "[ERROR] : Could not initialize the fingerprinting process" << std::endl;
        exit(2);
    }

    size_t stream_size = 0;
    const size_t stream_limit = MaxDuration * SampleRate;

    size_t chunk_size = 0;
    const size_t chunk_limit = MaxChunkDuration * SampleRate;

    size_t extra_chunk_limit = 0;
    double overlap = 0.0;

    if (chunk_limit > 0)
    {
        extra_chunk_limit = chromaprint_get_delay(m_ctx);
        overlap = chromaprint_get_delay_ms(m_ctx);
    }

    bool first_chunk = true;
    bool got_results = false;

    while (m_temp_sample_num < m_NumberOfSamples)
    {
        const int16_t *frame_data = nullptr;
        size_t frame_size = 0;

        read(&frame_data, &frame_size);

        bool stream_done = false;

        if (stream_limit > 0)
        {
            const auto remaining = stream_limit - stream_size;
            if (frame_size > remaining)
            {
                frame_size = remaining;
                stream_done = true;
            }
        }
        stream_size += frame_size;

        if (frame_size == 0)
        {
            if (stream_done)
                break;
            else
                continue;
        }

        bool chunk_done = false;
        size_t first_part_size = frame_size;

        if (chunk_limit > 0)
        {
            const auto remaining = chunk_limit + extra_chunk_limit - chunk_size;
            if (first_part_size > remaining)
            {
                first_part_size = remaining;
                chunk_done = true;
            }
        }

        if (!chromaprint_feed(m_ctx, frame_data, first_part_size * NumOfChannels))
        {
            std::cout << "[ERROR] : Could not process the audio data" << std::endl;
            exit(2);
        }

        chunk_size += first_part_size;

        if (chunk_done)
        {
            if (! chromaprint_finish(m_ctx))
            {
                std::cout << "[ERROR] : Could not finish the fingerprinting process" << std::endl;
                exit(2);
            }

            const auto chunk_duration = (chunk_size - extra_chunk_limit) * 1.0 / SampleRate + overlap;
            got_results = true;


        }

        frame_data += first_part_size * NumOfChannels;
        frame_size -= first_part_size;

        if (frame_size > 0)
        {
            if (! chromaprint_feed(m_ctx, frame_data, frame_size * NumOfChannels))
            {
                std::cout << "[ERROR] : Could not process the audio data" << std::endl;
                exit(2);
            }
        }

        chunk_size += frame_size;

        if (stream_done)
        {
            break;
        }

    }

    if (!chromaprint_finish(m_ctx))
    {
        std::cout << "[ERROR] : Could not finish the fingerprinting process" << std::endl;
        exit(2);
    }

    if (chunk_size > 0)
    {
        const auto chunk_duration = (chunk_size - extra_chunk_limit) * 1.0 / SampleRate + overlap;
        got_results = true;
    }

    else if (first_chunk)
    {
        std::cout << "[ERROR] : Not enough audio data" << std::endl;
        exit(2);
    }
}

void igniteChromaprint(std::string FileName)
{
    ChromaprintContext *ctx;
    ctx = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);

    const int sample_rate = 16000;
    const int channels = 1;

    if (chromaprint_start(ctx, sample_rate, channels))
    {
        auto samples = readAudio(std::move(FileName));



        for(int i = 0 ; i < size; i++)
        {
            int16_t* data = &samples[i];
            chromaprint_feed(ctx, data, 4);
        }

        chromaprint_finish(ctx);
    }

    char* fp;
    chromaprint_get_fingerprint(ctx, &fp);

    std::cout << fp << std::endl;

    chromaprint_dealloc(fp);

    chromaprint_free(ctx);
}

std::vector<std::string> getFP(std::string FileName)
{

}
//
