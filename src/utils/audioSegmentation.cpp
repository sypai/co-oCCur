/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "audioSegmentation.h"

std::vector<char> runVAD(std::vector<int16_t>& samples, int SegmentWindow)
{
    std::vector<char> audioString;

    // Creates an instance to the VAD structure
    VadInst* vad = WebRtcVad_Create();

    if (!vad)
        throw std::runtime_error("Cannot instantiate the WebRTC VAD handle");

    // Initialize a VAD instance
    int ifError = WebRtcVad_Init(vad);

    if (ifError)
        throw std::runtime_error("Cannot initialize the WebRTC VAD handle");

    // Sets the VAD operating mode. A more aggressive (higher mode) VAD is more
    // restrictive in reporting speech. Put in other words the probability of being
    // speech when the VAD returns 1 is increased with increasing mode. As a
    // consequence also the missed detection rate goes up.
    int operating_mode = 3;
    ifError = WebRtcVad_set_mode(vad, operating_mode);

    if (ifError)
        throw std::runtime_error("Cannot set the operating mode of WebRTC VAD handle");

    const int16_t* audioData = samples.data();

    for (int i=0, ms=0; i<samples.size(); i+=160, ms+=SegmentWindow)
    {
        int isActive = WebRtcVad_Process(vad, 16000, audioData, 160);

        std::cout << isActive << ",";
        audioString.emplace_back(isActive);

        audioData = audioData + 160;
    }

    return audioString;
}