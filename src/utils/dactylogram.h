/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_AUDIOFINGERPRINTER_H
#define CO_OCCUR_AUDIOFINGERPRINTER_H

#include "../../libs/lib_ext/CCAligner/read_wav_file.h"
#include "../../libs/lib_ext/chromaprint/src/chromaprint.h"

std::vector<int16_t> readAudio(std::string FileName);
void igniteChromaprint(std::string FileName);
//std::vector<std::string> getFP(std::string FileName);

#endif //CO_OCCUR_AUDIOFINGERPRINTER_H
