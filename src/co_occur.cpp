/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "co_occur.h"

co_oCCur::co_occur::co_occur()
= default;

void co_oCCur::co_occur::setTool(std::string ToolName)
{
    m_ToolName = std::move(ToolName);
}

void co_oCCur::co_occur::setOriginalAudio(std::string OriginalAudioName)
{
    m_OriginalAudioFileName = std::move(OriginalAudioName);
}

void co_oCCur::co_occur::setModifiedAudio(std::string ModifiedAudioName)
{
    m_ModifiedAudioFileName = std::move(ModifiedAudioName);
}

void co_oCCur::co_occur::setOriginalSubtitle(std::string OriginalSRTName)
{
    m_OriginalSRTFileName = std::move(OriginalSRTName);
}

std::string co_oCCur::co_occur::getToolName() const
{
    return m_ToolName;
}

void co_oCCur::co_occur::init()
{
    if (m_ToolName == "A")
    {
        std::cout << "[TOOL] Using Tool A for synchronization" << std::endl;

        co_oCCur::ToolA* handle;
        handle = new co_oCCur::ToolA(m_OriginalAudioFileName, m_ModifiedAudioFileName, m_OriginalSRTFileName);

        handle->sync();

        delete handle;
    }

    else if (m_ToolName == "B")
    {
        std::cout << "[TOOL] Using Tool B for synchronization" << std::endl;

        co_oCCur::ToolB* handle;
        handle = new co_oCCur::ToolB(m_ModifiedAudioFileName, m_OriginalSRTFileName);

        handle->sync();

        delete handle;
    }

    else
    {
        std::cout << "[ERROR] : Wrong input format. Select a TOOL, A or B" << std::endl;
        exit(2);
    }
}

co_oCCur::co_occur::~co_occur()
= default;
