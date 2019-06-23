/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_TOOLB_H
#define CO_OCCUR_TOOLB_H

#include <string>
#include <vector>

#include "../../libs/lib_co-oCCur/SiftSRT/SiftSRT.h"
#include "../../libs/lib_ext/CCAligner/logger.h"
#include "../../libs/lib_ext/CCAligner/read_wav_file.h"

#include "../utils/audioSegmentation.h"

namespace co_oCCur {

    class ToolB
    {
    private:
        std::string m_ModifiedAudioFile;
        std::string m_OriginalSubtitleFile;

        int m_SegmentWindow = 10;
        std::vector<char> m_AudioString;
        std::vector<char> m_SubtitleString;

    public:
        ToolB();
        ToolB(std::string ModifiedAudioFile, std::string OriginalSubtitleFile);
        long int align();
        void adjust();
        ~ToolB();

    private:
        void createAudioString();
        void createSubtitleString();
    };

} // namespace co_oCCur

#endif //CO_OCCUR_TOOLB_H
