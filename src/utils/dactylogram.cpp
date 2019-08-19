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

/*
double co_oCCur::Dactylogram::GetCurrentTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto usec = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return usec.count() / 1000000.0;
}

void co_oCCur::Dactylogram::read(const int16_t **data, size_t *size)
{
    std::vector<int16_t> tempSamples;

    tempSamples.reserve(44666);

    for (int i=0; i < 44666; i++)
    {
        if (i + m_temp_sample_num < m_Samples.size())
        {
            tempSamples.emplace_back(m_Samples[i + m_temp_sample_num]);
        }
    }

    *data = &tempSamples[0];
    *size = 44666;

    m_temp_sample_num += 44665;
}

void co_oCCur::Dactylogram::printResult(bool first, double timestamp, double duration)
{
    using namespace chromaprint;
    std::string tmp_fp;
    const char *fp;
    bool dealloc_fp = false;

    int size;
    if (!chromaprint_get_raw_fingerprint_size(m_ctx, &size))
    {
        std::cout << "[ERROR] : Could not get the fingerprinting size" << std::endl;
        exit(2);
    }

    if (size <= 0)
    {
        if (first)
        {
            std::cout << "[ERROR] : Empty fingerprint" << std::endl;
            exit(2);
        }
        return;
    }

    char *tmp_fp2;

    if (!chromaprint_get_fingerprint(m_ctx, &tmp_fp2))
    {
        std::cout << "[ERROR] : Could not get fingerprint" << std::endl;
        exit(2);
    }
    fp = tmp_fp2;
    dealloc_fp = true;

    SCOPE_EXIT(if (dealloc_fp)
    {
        chromaprint_dealloc((void *) fp);
    });

    std::cout << "{\"timestamp\": " << FIXED_FLOAT(timestamp);
    std::cout << ", \"duration\": " << duration << R"(, "fingerprint": ")" << fp << "\"}" << std::endl;

    fflush(stdout);
}*/

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
//    using namespace chromaprint;
//    chromaprint::FFmpegAudioReader reader;
//    reader.Open(m_AudioFileName);
    /**

    m_NumberOfSeconds = (double)m_NumberOfSamples / SampleRate;
    m_NumberOfMilliSeconds = (unsigned long int)(m_NumberOfSeconds * 1000);

    MaxDuration = m_NumberOfSeconds + 10;

    double ts = 0.0;
    ts = GetCurrentTimestamp();

//    if (!chromaprint_start(m_ctx, reader.GetSampleRate(), reader.GetChannels()))
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
        overlap = chromaprint_get_delay_ms(m_ctx) / 1000.0;
    }

    bool first_chunk = true;
    bool got_results = false;

    signed long int rem_Samples = m_NumberOfSamples;

    while (rem_Samples > 0)
//    while(!reader.IsFinished())
    {
        const int16_t *frame_data = nullptr;
        size_t frame_size = 0;

//        reader.Read(&frame_data, &frame_size);
        read(&frame_data, &frame_size);

        rem_Samples -= frame_size;
//        std::cout << rem_Samples << std::endl;
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
            if (!chromaprint_finish(m_ctx))
            {
                std::cout << "[ERROR] : Could not finish the fingerprinting process" << std::endl;
                exit(2);
            }

            const auto chunk_duration = (chunk_size - extra_chunk_limit) * 1.0 / SampleRate + overlap;
            printResult(first_chunk, ts, chunk_duration);
            got_results = true;

            ts = GetCurrentTimestamp();

            if (! chromaprint_clear_fingerprint(m_ctx))
            {
                std::cout << "[ERROR] : Could not initialize the fingerprinting process" << std::endl;
                exit(2);
            }

            ts -= overlap;

            if (first_chunk)
            {
                extra_chunk_limit = 0;
                first_chunk = false;
            }
        }

        frame_data += first_part_size * NumOfChannels;
        frame_size -= first_part_size;

        if (frame_size > 0)
        {
            if (!chromaprint_feed(m_ctx, frame_data, frame_size * NumOfChannels))
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
        printResult(first_chunk, ts, chunk_duration);
        got_results = true;
    }

    else if (first_chunk)
    {
        std::cout << "[ERROR] : Not enough audio data" << std::endl;
        exit(2);
    }
     */

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
//
//    int a = chromaprint_get_item_duration_ms(m_ctx);
//
//    DEBUG << a;

}
//void co_oCCur::Dactylogram::setFingerprints(std::vector<uint32_t> *array)
//{
////    for (int i = 0; i < m_NumberOfSeconds; i++)
////    {
////        int j = 0;
////        while(j < array[i].size())
////        m_AudioFingerprints[i].emplace_back(&array[i][j]);
////    }
//}
void co_oCCur::Dactylogram::collectFingerprints()
//void co_oCCur::Dactylogram::collectFingerprints(std::vector<std::vector<uint32_t>> * array)
{
    DEBUG << "Initializing audiofingerprinting generation.\n";
    igniteChromaprint();

    m_NumberOfSecondsPrecise = (double)m_NumberOfSamples / SampleRate;
    m_NumberOfSeconds = m_NumberOfSamples / SampleRate;
    m_NumberOfMilliSeconds = (long int)(m_NumberOfSecondsPrecise * 1000);

    DEBUG << "The audio is " << m_NumberOfSecondsPrecise << " seconds in length.";

    std::vector<uint32_t> AudioFingerprints;
//    auto array = new std::vector<uint32_t>[m_NumberOfSeconds];
//     std::vector<uint32_t> array[m_NumberOfSeconds];

    using namespace chromaprint;
    uint32_t *raw_fp_data = nullptr;
    int raw_fp_size = 0;

    if (!chromaprint_get_raw_fingerprint(m_ctx, &raw_fp_data, &raw_fp_size))
    {
        std::cout << "[ERROR] : Could not receive the fingerprints." << std::endl;
        exit(2);
    }

    SCOPE_EXIT(chromaprint_dealloc(raw_fp_data));

//    raw_fp_size = 10;
    AudioFingerprints.reserve(raw_fp_size);
    for (int i = 0; i < raw_fp_size; i++)
    {
        AudioFingerprints.emplace_back(raw_fp_data[i]);
    }

//    m_NumberOfSeconds =15;
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

    DEBUG << "Audiofingerprints successfully generated.";

//    for (int i = 0; i < m_NumberOfSeconds; i++)
//    {
//        std::cout << "\"timestamp\": " << i << ", \"fingerprint\": "<<"[";
//        for (unsigned int j : array[i])
//            std::cout << j << ",";
//        std::cout << "]" << std::endl;
//    }

}

std::vector<std::vector<uint32_t> > co_oCCur::Dactylogram::getFingerprints() const
{
    return m_AudioFingerprints;
}

double co_oCCur::Dactylogram::getAudioLength_secs() const
{
    return m_NumberOfSecondsPrecise;
}

long int co_oCCur::Dactylogram::getAudioLength_ms() const
{
    return m_NumberOfMilliSeconds;
}

//void igniteChromaprint(std::string FileName)
//{
//    ChromaprintContext *ctx;
//    ctx = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
//
//    const int sample_rate = 16000;
//    const int channels = 1;
//
//    if (chromaprint_start(ctx, sample_rate, channels))
//    {
//        auto samples = readAudio(std::move(FileName));
//
//
//
//        for(int i = 0 ; i < size; i++)
//        {
//            int16_t* data = &samples[i];
//            chromaprint_feed(ctx, data, 4);
//        }
//
//        chromaprint_finish(ctx);
//    }
//
//    char* fp;
//    chromaprint_get_fingerprint(ctx, &fp);
//
//    std::cout << fp << std::endl;
//
//    chromaprint_dealloc(fp);
//
//    chromaprint_free(ctx);
//}
//
//std::vector<std::string> getFP(std::string FileName)
//{
//
//}
//
