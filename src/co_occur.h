/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_CO_OCCUR_H
#define CO_OCCUR_CO_OCCUR_H


#include <iostream>
#include <vector>
#include <string>

#include "../libs/lib_ext/CCAligner/logger.h"
#include "tools/toolA.h"
#include "tools/toolB.h"

namespace co_oCCur{

    class co_occur
    {
    private:
        std::string m_ToolName;
        std::string m_OriginalAudioFileName;
        std::string m_ModifiedAudioFileName;
        std::string m_OriginalSRTFileName;

    public:
        co_occur();
        std::string getToolName() const;
        void setTool(std::string ToolName);
        void setOriginalAudio(std::string OriginalAudioName);
        void setModifiedAudio(std::string ModifiedAudioName);
        void setOriginalSubtitle(std::string OriginalSRTName);
        void init();
        ~co_occur();
    };
}

#endif //CO_OCCUR_CO_OCCUR_H
