/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_TOOLA_H
#define CO_OCCUR_TOOLA_H

#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include <algorithm>

#include "../utils/align_the_fingerprints.h"

namespace co_oCCur {

    class ToolA
    {
    private:
        std::string m_OriginalAudioFile;
        std::string m_ModifiedAudioFile;
        std::string m_OriginalSubtitleFile;

    public:
        ToolA();
        ToolA(std::string OriginalAudioFile, std::string ModifiedAudioFile, std::string OriginalSubtitleFile);
        void sync();
        ~ToolA();
    };


} // namespace co_oCCur

#endif //CO_OCCUR_TOOLA_H
