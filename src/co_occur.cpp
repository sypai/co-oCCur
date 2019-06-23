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

        handle->adjust();

        delete handle;
    }

    else if (m_ToolName == "B")
    {
        std::cout << "[TOOL] Using Tool B for synchronization" << std::endl;

        co_oCCur::ToolB* handle;
        handle = new co_oCCur::ToolB(m_ModifiedAudioFileName, m_OriginalSRTFileName);

        handle->adjust();

        delete handle;
    }

    else
    {
        std::cout << "[ERROR] : Wrong input format. Select A or B" << std::endl;
        exit(2);
    }
}

co_oCCur::co_occur::~co_occur()
= default;

void CLIHeader()
{
    std::cout << "co_oCCur: Subtitle Synchronization tool, Suyash Bajpai." << std::endl;
    std::cout << "Parent Organization: CCExtractor Development" << std::endl;
    std::cout << "Mentor: Carlos Fernandez Sans, Google Summer of Code 2019." << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
}

void PrintUsage()
{
    std::string usage;
    usage = R"(Syntax:
    ./co-oCCur -tool "A | B" -orgAudio /path/to/original/wav/file -modAudio /path/to/modified/wav/file -s /path/to/SRT

    Options:
        -tool OR -t         NAME:  A or B (Set the Tool to be used)
        -orgAudio OR -org   FILE: Original Audio file
        -modAudio OR -mod   FILE: Modified Audio file
        -orgSRT OR -s       FILE: Original Subtitle file

    Example: ./co-oCCur -tool A -org episode1.wav -mod episode1_W/OCommercial -s input.srt
    Example: ./co-oCCur -tool B -mod episode1_W/OCommercial.wav -s input.srt)";

    std::cout << usage << std::endl;
}

void ParseOptions(int arg_c, char* arg_v[])
{
    co_oCCur::co_occur *sync;
    sync = new co_oCCur::co_occur();

    if (arg_c < 2)
    {
        PrintUsage();
        exit(1);
    }

    for (int i=1; i<arg_c; i++)
    {
        std::string parameter(arg_v[i]);

        if ((parameter == "-tool" || parameter == "-t") && i + 1 < arg_c )
        {
            sync->setTool(arg_v[i+1]);
        }

        else if ((parameter == "-orgAudio" || parameter == "-org") && ((sync->getToolName() == "A") && (i + 1 < arg_c)))
        {
            sync->setOriginalAudio(arg_v[i+1]);
        }

        else if ((parameter == "-modAudio" || parameter == "-mod") && i + 1 < arg_c)
        {
            sync->setModifiedAudio(arg_v[i+1]);
        }

        else if ((parameter == "-orgSRT" || parameter == "-s") && i + 1 < arg_c)
        {
            sync->setOriginalSubtitle(arg_v[i+1]);
        }

//        else
//        {
//            std::cout << "[ERROR] : Wrong input format." << std::endl;
//            PrintUsage();
//        }
    }

    sync->init();
    delete sync;
}

void CLIFooter()
{
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "Issues? Open a ticket here" << std::endl;
    std::cout << "https://github.com/sypai/co-oCCur/issues" << std::endl;
}

int main(int argc, char *argv[])
{
    CLIHeader();
    ParseOptions(argc, argv);
    CLIFooter();
    return 0;
}
