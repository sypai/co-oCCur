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

void co_oCCur::ToolB::createTempSRT(const std::vector<SubtitleItem*>& sub)
{
    std::ofstream myfile;
    myfile.open("temp.srt");

    for (SubtitleItem* element : sub)
    {
        int subNo = element->getSubtitleNumber();

        long int startTime = element->getStartTime();
        long int endTime = element->getEndTime();
        int duration = endTime - startTime;

        std::string payload = element->getText();

        myfile << "BEGIN" << std::endl;
        myfile << subNo << std::endl;
        myfile << "start= " << startTime  << std::endl;
        myfile << "end= " << endTime << std::endl;
        myfile << "duration= " << duration << std::endl;
        myfile << std::endl;
        myfile << "payload= " << payload << std::endl;
        myfile << "fingerprint= " << "NO" << std::endl;
        myfile << "END"<< std::endl;
        myfile << std::endl;
    }

    myfile.close();
}

int co_oCCur::ToolB::createSubtitleString() {

    std::cout << "Trying to create Subtitle String..." << std::endl;

    DEBUG0 << "Opening .srt file: " << m_OriginalSubtitleFile;
    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    createTempSRT(sub);

    co_oCCurParser *parse;
    parse = new co_oCCurParser(sub);

    if (parse->getTotalSubtitles() < 1)
    {
        DEBUG0 << "Selected SRT file has no subtitles or the file doesn't exist.";
        return 0;
    }

    DEBUG0 << "A Temporary file 'temp.srt' created.";
    DEBUG0 << "Subtitles are being segmented into a " << m_SegmentWindow <<"ms window";
    auto substring = parse->SpeechActivityDetection(m_SegmentWindow);

    m_SubtitleString = substring;
    DEBUG0 << "Subtitle String created.\n";

    std::cout << "Subtitle String: " << std::endl;
    int i = 0;

    for (int j = 0; j < sub.at(sub.size() - 1)->getEndTime(); j += m_SegmentWindow)
    {
//        std::cout << j << "ms : " << substring.at(i) << std::endl;
        std::cout << m_SubtitleString[i] << ",";
        i += 1;
    }

    return 1;
}

void co_oCCur::ToolB::createAudioString()
{
    std::cout << "\n\nTrying to create Audio String..." << std::endl;

    std::vector<char> audioString;

    WaveFileData * file = new WaveFileData(m_ModifiedAudioFile);
    file->read();
    std::vector<int16_t> samples = file->getSamples();

    std::cout << "\nAudio String: " << std::endl;
    audioString = runVAD(samples, m_SegmentWindow);

    DEBUG0 << "\nAudio String Created\n";

    m_AudioString = audioString;

//    for(int i=0, ms =0;i<samples.size(); ms+=m_SegmentWindow)
//    {
//        std::cout<< m_AudioString[i];
//        i+=160;
//    }
}

long int co_oCCur::ToolB::align()
{
    std::cout << "AUDIO AND SUBTITLES ARE BEING SYNCHRONIZED.." << std::endl;

    //TODO: Alignment of the two strings is the task
    // for CODING PHASE 3.

    long int delay = 4562;

    return delay;
}

void co_oCCur::ToolB::adjust(long int delta)
{
    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    DEBUG0 << "SYNCHRONIZATION DONE" ;
    DEBUG0 << "co-oCCur subtitle document created." ;

    co_oCCurEditor *edit;
    edit = new co_oCCurEditor(sub);
    edit->AdjustSRT(m_OriginalSubtitleFile, delta, true);
}

int co_oCCur::ToolB::sync()
{
    if(!createSubtitleString())
    {
        std::cout << "Choose a valid SRT file";
        return 0;
    }

    createAudioString();
    auto delta = align();
    adjust(delta);
}






