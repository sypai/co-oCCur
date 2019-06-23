/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "toolB.h"
#include <utility>

co_oCCur::ToolB::ToolB()
= default;

co_oCCur::ToolB::ToolB(std::string ModifiedAudioFile, std::string OriginalSubtitleFile)
{
    m_ModifiedAudioFile = std::move(ModifiedAudioFile);
    m_OriginalSubtitleFile = std::move(OriginalSubtitleFile);
}

co_oCCur::ToolB::~ToolB()
= default;

void co_oCCur::ToolB::createSubtitleString() {
    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    DEBUG << "A Temporary file 'temp.srt' created.";
//    createTempSRT(sub);

    DEBUG << "Enriched subtitle document created.";
    DEBUG << "rich_" << m_OriginalSubtitleFile;

    co_oCCurParser *parse;
    parse = new co_oCCurParser(sub);

    auto substring = parse->SpeechActivityDetection(m_SegmentWindow);

    m_SubtitleString = substring;

    int i = 0;
    for (int j = 0; j < sub.at(sub.size() - 1)->getEndTime(); j += 10)
    {
        std::cout << j << "ms : " << substring.at(i) << std::endl;
        i += 1;
    }
}

void co_oCCur::ToolB::createAudioString()
{
    std::vector<char> audioString;

    WaveFileData * file = new WaveFileData(m_ModifiedAudioFile);
    file->read();
    std::vector<int16_t> samples = file->getSamples();
    audioString = runVAD(samples, m_SegmentWindow);

    m_AudioString = audioString;
}

long int co_oCCur::ToolB::align()
{
    //TODO: Alignment of the two strings is the task
    // for CODING PHASE 3.
    createAudioString();
    createSubtitleString();

    long int delay = 4562;

    return delay;
}

void co_oCCur::ToolB::adjust()
{
    auto delta = align();

    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    DEBUG << "SYNCHRONIZATION DONE" ;
    DEBUG << "co-oCCur subtitle document created." ;
    DEBUG << "co-oCCur_" << m_OriginalSubtitleFile;

    co_oCCurEditor *edit;
    edit = new co_oCCurEditor(sub);
    edit->AdjustSRT(m_OriginalSubtitleFile, delta, true);
}






