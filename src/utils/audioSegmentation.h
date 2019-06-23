/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_VAD_H
#define CO_OCCUR_VAD_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "../../libs/lib_ext/webrtc/webrtc/common_audio/vad/include/webrtc_vad.h"
#include "../../libs/lib_ext/CCAligner/read_wav_file.h"

std::vector<char> runVAD(std::vector<int16_t>& samples, int SegmentWindow);

#endif //CO_OCCUR_VAD_H

