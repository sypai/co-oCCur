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
    std::cout << "\nInserting the Audio Fingerprint annotations into the subtitle document..." << std::endl;

    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    DEBUG << "A Temporary file 'temp.srt' created." ;
    createTempSRT(sub);

    DEBUG << "Enriched subtitle document created." ;
//    DEBUG << m_OriginalSubtitleFile;

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

    std::cout << "\nGenerating Audio Fingerprint annotations...\n" << std::endl;

    double originalAudioLength = scan_original->getAudioLength_secs();
    long int originalAudioLength_ms = scan_original->getAudioLength_ms();

    long int FPTimestamp;
    std::vector<uint32_t> FP;

    //Fingerprint1
    FPTimestamp = originalAudioLength_ms / 4;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    int fp[FP.size()];
    for (int i = 0; i < FP.size(); i++)
    {
        fp[i] = FP[i];
    }
    std::string original(fp, fp + sizeof(fp) / sizeof(fp[0]));
    std::string fp_base64 = Base64Encode(original);

    m_Fingerprints_raw.emplace_back(FP);
    m_Fingerprints.emplace_back(fp_base64);
    m_FPTimestamps.emplace_back(FPTimestamp);

    std::cout << "Fingerprint anchors: " << std::endl;
    DEBUG << "{\"timestamp: \"" << FPTimestamp << ", \"fingerprint: \"" << "\"" << fp_base64 << "\"}";

    //Fingerprint2
    FPTimestamp = originalAudioLength_ms / 2;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    int fp2[FP.size()];
    for (int i = 0; i < FP.size(); i++)
    {
        fp2[i] = FP[i];
    }
    std::string original2(fp2, fp2 + sizeof(fp2) / sizeof(fp2[0]));
    std::string fp2_base64 = Base64Encode(original2);

    m_Fingerprints_raw.emplace_back(FP);
    m_Fingerprints.emplace_back(fp2_base64);
    m_FPTimestamps.emplace_back(FPTimestamp);

    DEBUG << "{\"timestamp: \"" << FPTimestamp << ", \"fingerprint: \"" << "\"" << fp2_base64 << "\"}";

    //Fingerprint3
    FPTimestamp = (3 * originalAudioLength_ms) / 4;
    FP = originalAudioFingerprints[FPTimestamp / 1000];

    int fp3[FP.size()];
    for (int i = 0; i < FP.size(); i++)
    {
        fp3[i] = FP[i];
    }
    std::string original3(fp3, fp3 + sizeof(fp3) / sizeof(fp3[0]));
    std::string fp3_base64 = Base64Encode(original3);

    m_Fingerprints_raw.emplace_back(FP);
    m_Fingerprints.emplace_back(fp3_base64);
    m_FPTimestamps.emplace_back(FPTimestamp);

    DEBUG << "{\"timestamp: " << FPTimestamp << R"(", "fingerprint: )" << fp3_base64 << "\"}";

    std::cout << "\nFingerprints Ready!!\n";

    //    for (int i=0; i<3; i++)
//    {
//        DEBUG << "FINGERPRINT: " << i ;
//        DEBUG << "TIMESTAMP: " << m_FPTimestamps[i] ;
//        DEBUG <<  m_Fingerprints[i] << "\n";
//    }
//
//    modifiedAudio->read();

}

long int co_oCCur::ToolA::seekAndCompare() {
    generateFingerprints();
    AFInserter();

    std::cout << "\nProcessing Modified Audio" << std::endl;
    std::vector<std::vector<uint32_t> > modifiedAudioFingerprints;

    co_oCCur::Dactylogram *scan_modified;
    scan_modified = new Dactylogram(m_ModifiedAudioFile);
    scan_modified->collectFingerprints();
//    scan_modified->collectFingerprints(&modifiedAudioFingerprints);
    modifiedAudioFingerprints = scan_modified->getFingerprints();

    std::cout << "\nAUDIO AND SUBTITLES ARE BEING SYNCHRONIZED..." << std::endl;

    long int delta = 0; // The constant temporal offset

    std::vector<std::string> match = {"NO_MATCH", "NO_MATCH", "NO_MATCH"};

    for (int anchor_no = 0; anchor_no < 3; anchor_no++) {
//        long int fpDuration = 1; // Default FP duration in seconds
//        long int reference = 0;

        long int seekOffset = m_FPTimestamps[anchor_no] / 1000;

        if(seekOffset < scan_modified->getAudioLength_ms())
            break;

        long int tempSeekOffset = seekOffset;
        int max_search_window = 60;
        int local_search_window = 1; //in seconds
        int attempts_rem = max_search_window;

        while (attempts_rem > 0) {
            auto testFP = modifiedAudioFingerprints[tempSeekOffset];

            if (attempts_rem == 60 && matches(anchor_no, testFP)) {
                return 0;
            } else if (matches(anchor_no, testFP)) {
                delta = (max_search_window - attempts_rem) * 1000;
            }

            tempSeekOffset -= local_search_window;
            attempts_rem--;
        }
    }
    return delta;
}

void co_oCCur::ToolA::adjust(long int delta)
{
    SubtitleParserFactory *spf;
    spf = new SubtitleParserFactory(m_OriginalSubtitleFile);

    SubtitleParser *parser;
    parser = spf->getParser();

    std::vector<SubtitleItem *> sub;
    sub = parser->getSubtitles();

    std::cout << "SYNCHRONIZATION DONE\n" ;
    DEBUG << "co-oCCur subtitle document created." ;

    co_oCCurEditor *edit;
    edit = new co_oCCurEditor(sub);

    edit->AdjustSRT(m_OriginalSubtitleFile, delta, false);
}

void co_oCCur::ToolA::sync()
{
    auto delta = seekAndCompare();
    adjust(delta);
}

