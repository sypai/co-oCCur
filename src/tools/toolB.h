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

namespace co_oCCur {

    class ToolB
    {
    private:
        std::string m_ModifiedAudioFile;
        std::string m_OriginalSubtitleFile;

        std::vector<char> m_AudioString;
        std::vector<long int> m_SubtitleString;

    public:
        ToolB();
        ToolB(std::string ModifiedAudioFile, std::string OriginalSubtitleFile);
        ~ToolB();

    private:
        long int alignStrings();
    };

} // namespace co_oCCur

#endif //CO_OCCUR_TOOLB_H
