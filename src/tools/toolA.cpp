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

void co_oCCur::ToolA::createTempSRT(const std::vector<SubtitleItem*>& sub)
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

bool co_oCCur::ToolA::matches(int anchor, std::vector<uint32_t> testFP)
{
    int matchCount = 0;
    for (int i = 0; i < 8; i++)
    {
        if (m_Fingerprints_raw[anchor][i] == testFP[i])
            matchCount++;
    }

    return (matchCount > 6);
}

void co_oCCur::ToolA::AFInserter()
{
    std::cout << "Inserting the Audio Fingerprint annotations into the subtitle document..." << std::endl;

    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    DEBUG << "A Temporary file 'temp.srt' created." ;
    createTempSRT(sub);

    DEBUG << "Enriched subtitle document created." ;
    DEBUG << "rich_" << m_OriginalSubtitleFile;

    co_oCCurEditor *edit;
    edit = new co_oCCurEditor(sub);
    edit->EnrichSRT("temp.srt", m_Fingerprints, m_FPTimestamps);
}

void co_oCCur::ToolA::generateFingerprints()
{
    std::cout << "Processing Original Audio" << std::endl;

    std::vector<std::vector<uint32_t> > originalAudioFingerprints;

    co_oCCur::Dactylogram *scan_original;
    scan_original = new Dactylogram(m_OriginalAudioFile);
    scan_original->collectFingerprints();
//    scan_original->collectFingerprints(&originalAudioFingerprints);

    originalAudioFingerprints = scan_original->getFingerprints();

    std::cout << "\nGenerating Audio Fingerprint annotations" << std::endl;

    double originalAudioLength = scan_original->getAudioLength_secs();
    long int originalAudioLength_ms = scan_original->getAudioLength_ms();

    long int FPTimestamp;
    std::vector<uint32_t> FP;

    //Fingerprint1
    FPTimestamp = originalAudioLength_ms / 4;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    m_Fingerprints_raw.emplace_back(FP);
    m_FPTimestamps.emplace_back(FPTimestamp);

    //Fingerprint2
    FPTimestamp = originalAudioLength_ms / 2;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    m_Fingerprints_raw.emplace_back(FP);
    m_FPTimestamps.emplace_back(FPTimestamp);

    //Fingerprint3
    FPTimestamp = (3 * originalAudioLength_ms) / 4;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    m_Fingerprints_raw.emplace_back(FP);
    m_FPTimestamps.emplace_back(FPTimestamp);

    DEBUG << "Fingerprints Ready!!\n";

    //    int fp3[] = {
//            9, 0, 9, 403, 95, 181, 36, 18, 19, 35, 65, 15, 31, 195, 149, 161, 63, 33, 4,
//            60, 141, 45, 404, 35, 184, 45, 454, 45, 135, 135, 11, 58, 139, 408, 65, 145,
//            54, 165, 441, 31, 99, 184, 45, 159, 96, 50, 51, 160, 451, 168, 55, 134, 185,
//            114, 430, 193, 133, 454, 44, 146, 66, 91, 141, 60, 135, 59, 44, 185, 410, 48,
//            199, 133, 455, 440, 113, 101, 65, 199, 43, 45, 181, 160, 141, 140, 65, 143,
//            161, 49, 184, 135, 30, 405, 135, 119, 50, 94, 454, 51, 140, 150
//    };
//
//    long int fp3Time = 434660;
//    std::string original3(fp3, fp3 + sizeof(fp3) / sizeof(fp3[0]));
//    std::string fp3_base64 = Base64Encode(original3);
//    for (int i=0; i<3; i++)
//    {
//        DEBUG << "FINGERPRINT: " << i ;
//        DEBUG << "TIMESTAMP: " << m_FPTimestamps[i] ;
//        DEBUG <<  m_Fingerprints[i] << "\n";
//    }
//
//    modifiedAudio->read();

}

long int co_oCCur::ToolA::seekAndCompare()
{
    generateFingerprints();
    AFInserter();

    std::cout << "\nProcessing Modified Audio" << std::endl;
    std::vector<std::vector<uint32_t> > modifiedAudioFingerprints;

    co_oCCur::Dactylogram *scan_modified;
    scan_modified = new Dactylogram(m_ModifiedAudioFile);
    scan_modified->collectFingerprints();
//    scan_modified->collectFingerprints(&modifiedAudioFingerprints);
    modifiedAudioFingerprints = scan_modified->getFingerprints();


    std::cout << "\nAUDIO AND SUBTITLES ARE BEING SYNCHRONIZED.." << std::endl;

    long int delta=0; // The constant temporal offset

    std::vector<std::string> match = {"NO_MATCH", "NO_MATCH", "NO_MATCH"};

    for (int anchor_no=1; anchor_no < 4 ; anchor_no++)
    {
//        long int fpDuration = 1; // Default FP duration in seconds
//        long int reference = 0;

        long int seekOffset =  m_FPTimestamps[anchor_no] / 1000;
        long int tempSeekOffset = seekOffset;
        int max_search_window = 20;
        int local_search_window = 1; //in seconds
        int attempt = 1;

        while (max_search_window)
        {
            auto testFP = modifiedAudioFingerprints[tempSeekOffset];

            if(matches(anchor_no, testFP))
            {
                delta = (attempt + 1) * 1000;
                return delta;
            }

            tempSeekOffset -= local_search_window;

            max_search_window--;
        }

//        for(int attempt = 1 ; attempt < 3 ; attempt++)
//        {
//            auto testFP = modifiedAudioFingerprints[seekOffset / 1000];
//
//            if(matches(anchor_no, testFP))
//            {
//                delta = testFP.matchOffset();
//
//                if (delta != 0) {
//                    for (int i = anchor_no + 1; i < 4; i++) {
//                        m_FPTimestamps[i] += delta;
//                    }
//                    match[anchor_no] = attempt == 1 ? "MATCH" : "LOCAL_MATCH";
//                    break;
//                }
//            }
//
//            else
//            {
//                fpDuration = 5 ; // Local search interval in seconds
//                seekOffset = std::max((seekOffset - fpDuration / 2), reference);
//            }
//        }
    }

    return -1;
}

void co_oCCur::ToolA::adjust(long int delta)
{

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

    edit->AdjustSRT(m_OriginalSubtitleFile, delta, false);
}

void co_oCCur::ToolA::sync()
{
    generateFingerprints();
    AFInserter();
    seekAndCompare();
    auto delta = seekAndCompare();
    adjust(delta);
}

