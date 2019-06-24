/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/
#include "co_occur.h"

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

    Example: ./co-oCCur -tool A -org episode1.wav -mod episode1_W/OCommercial.wav -s input.srt
    Example: ./co-oCCur -tool B -mod episode1_W/OCommercial.wav -s input.srt)";

    std::cout << usage << std::endl;
}

int ParseOptions(int arg_c, char* arg_v[])
{
    co_oCCur::co_occur *sync;
    sync = new co_oCCur::co_occur();

    if (arg_c < 2)
    {
        PrintUsage();
        return 11;
    }

    for (int i=1; i<arg_c; i++)
    {
        std::string parameter(arg_v[i]);

        if ((parameter == "-tool" || parameter == "-t") && i + 1 < arg_c )
        {
            sync->setTool(arg_v[i+1]);
            i++;
        }

        else if ((parameter == "-orgAudio" || parameter == "-org") && ((sync->getToolName() == "A") && (i + 1 < arg_c)))
        {
            sync->setOriginalAudio(arg_v[i+1]);
            i++;
        }

        else if ((parameter == "-modAudio" || parameter == "-mod") && i + 1 < arg_c)
        {
            sync->setModifiedAudio(arg_v[i+1]);
            i++;
        }

        else if ((parameter == "-orgSRT" || parameter == "-s") && i + 1 < arg_c)
        {
            sync->setOriginalSubtitle(arg_v[i+1]);
            i++;
        }

        else
        {
            std::cout << "[ERROR] : Wrong input format." << std::endl;
            PrintUsage();
            return 11;
        }
    }

    sync->init();
    delete sync;
    return 0;
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


