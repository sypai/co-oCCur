/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "toolA.h"

co_oCCur::ToolA::ToolA()
= default;

co_oCCur::ToolA::ToolA(std::string OriginalAudioFile, std::string ModifiedAudioFile, std::string OriginalSubtitleFile)
{
    m_OriginalAudioFile = std::move(OriginalAudioFile);
    m_ModifiedAudioFile = std::move(ModifiedAudioFile);
    m_OriginalSubtitleFile = std::move(OriginalSubtitleFile);
}

co_oCCur::ToolA::~ToolA()
= default;

void co_oCCur::ToolA::sync()
{
    co_oCCur::AlignFP *co;
//    co = new co_oCCur::AlignFP();
    co = new co_oCCur::AlignFP(m_OriginalAudioFile, m_ModifiedAudioFile, m_OriginalSubtitleFile);
    co->brum_brum();
    delete co;
}