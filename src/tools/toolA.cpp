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
    WaveFileData* originalAudio = new WaveFileData(m_OriginalAudioFile);
//    originalAudio->read();

    std::cout << "\nProcessing Modified Audio" << std::endl;
    WaveFileData* modifiedAudio = new WaveFileData(m_ModifiedAudioFile);
//    modifiedAudio->read();

    DEBUG << "\n";
    std::cout << "Generating Audio Fingerprint annotations" << std::endl;

    // TODO: This function will leverage Dactylogram library for fingerprint generation
    // Which is the primary deliverable of Coding Phase 2

    // fingerprint 1
    int fp1[] = {
            1, 0, 1, 207, 17, 181, 36, 18, 19, 37, 65, 15, 31, 197, 149, 161, 63, 33, 22,
            60, 141, 27, 202, 35, 184, 47, 254, 227, 135, 135, 11, 58, 139, 208, 65, 127,
            52, 167, 241, 31, 99, 182, 25, 159, 96, 70, 71, 160, 251, 168, 75, 132, 185,
            112, 230, 193, 133, 252, 42, 126, 66, 91, 121, 60, 135, 79, 24, 185, 210, 28,
            199, 133, 255, 240, 113, 101, 67, 199, 23, 225, 181, 160, 121, 140, 67, 123,
            161, 229, 184, 137, 30, 205, 135, 119, 70, 94, 252, 71, 120, 150
    };

    long int fp1Time = 43660;
    std::string original(fp1, fp1 + sizeof(fp1) / sizeof(fp1[0]));
    std::string fp1_base64 = Base64Encode(original);

    m_Fingerprints.emplace_back(fp1_base64);
    m_FPTimestamps.emplace_back(fp1Time);

    // fingerprint 2
    int fp2[] = {
            9, 0, 9, 203, 93, 989, 36, 98, 99, 33, 65, 95, 39, 993, 949, 969, 63, 33, 22,
            60, 949, 23, 202, 35, 984, 43, 254, 223, 935, 935, 9, 58, 939, 208, 65, 923,
            52, 963, 249, 39, 99, 982, 25, 959, 96, 30, 39, 960, 259, 968, 35, 932, 985,
            92, 230, 993, 933, 252, 42, 926, 66, 99, 929, 60, 935, 39, 24, 985, 290, 28,
            999, 933, 255, 240, 93, 909, 63, 999, 23, 225, 989, 960, 929, 940, 63, 923,
            969, 229, 984, 933, 30, 205, 935, 99, 30, 94, 252, 39, 920, 950
    };

    long int fp2Time = 3660;
    std::string original2(fp2, fp2 + sizeof(fp2) / sizeof(fp2[0]));
    std::string fp2_base64 = Base64Encode(original2);

    m_Fingerprints.emplace_back(fp2_base64);
    m_FPTimestamps.emplace_back(fp2Time);

    // fingerprint 3
    int fp3[] = {
            9, 0, 9, 403, 95, 181, 36, 18, 19, 35, 65, 15, 31, 195, 149, 161, 63, 33, 4,
            60, 141, 45, 404, 35, 184, 45, 454, 45, 135, 135, 11, 58, 139, 408, 65, 145,
            54, 165, 441, 31, 99, 184, 45, 159, 96, 50, 51, 160, 451, 168, 55, 134, 185,
            114, 430, 193, 133, 454, 44, 146, 66, 91, 141, 60, 135, 59, 44, 185, 410, 48,
            199, 133, 455, 440, 113, 101, 65, 199, 43, 45, 181, 160, 141, 140, 65, 143,
            161, 49, 184, 135, 30, 405, 135, 119, 50, 94, 454, 51, 140, 150
    };

    long int fp3Time = 434660;
    std::string original3(fp3, fp3 + sizeof(fp3) / sizeof(fp3[0]));
    std::string fp3_base64 = Base64Encode(original3);

    m_Fingerprints.emplace_back(fp3_base64);
    m_FPTimestamps.emplace_back(fp3Time);

    DEBUG << "Fingerprints Ready!!\n";

    for (int i=0; i<3; i++)
    {
        DEBUG << "FINGERPRINT: " << i ;
        DEBUG << "TIMESTAMP: " << m_FPTimestamps[i] ;
        DEBUG <<  m_Fingerprints[i] << "\n";
    }

}

long int co_oCCur::ToolA::seekAndCompare()
{
    generateFingerprints();
    AFInserter();

    std::cout << "\nAUDIO AND SUBTITLES ARE BEING SYNCHRONIZED.." << std::endl;

    long int delta;

    std::vector<std::string> match = {"NO_MATCH", "NO_MATCH", "NO_MATCH"};

    for (int anchor_no=1; anchor_no < 4 ; anchor_no++)
    {
        int fpDuration = 1; // Default FP duration in seconds
        long int seekOffset =  m_FPTimestamps[anchor_no];

        for(int attempt = 1 ; attempt < 3 ; attempt++)
        {
            auto testFP = snapFP(modifiedAudioFile, seekOffset, fpDuration);

            if(testFP.matches(m_Fingerprints[anchor_no]))
            {
                delta = testFP.matchOffset();

                if( delta != 0 )
                {
                    for (int i = anchor_no + 1; i < 4; i++)
                        m_FPTimestamps[i] = delta;


                }
            }
        }
    }

    long int delta = 3432; // The constant temporal offset

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

    DEBUG << "SYNCHRONIZATION DONE" ;
    DEBUG << "co-oCCur subtitle document created." ;
    DEBUG << "co-oCCur_" << m_OriginalSubtitleFile;

    co_oCCurEditor *edit;
    edit = new co_oCCurEditor(sub);

    edit->AdjustSRT(m_OriginalSubtitleFile, delta, true);
}

void co_oCCur::ToolA::sync()
{
    generateFingerprints();
    AFInserter();
    seekAndCompare();
    auto delta = seekAndCompare();
    adjust(delta);
}

