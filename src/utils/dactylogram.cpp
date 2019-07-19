/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include <utility>
#include "dactylogram.h"

std::vector<int16_t> readAudio(std::string FileName)
{
    WaveFileData * file = new WaveFileData(std::move(FileName));
    file->read();
    std::vector<int16_t> samples = file->getSamples();
    return samples;
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

        auto size = samples.size();

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
