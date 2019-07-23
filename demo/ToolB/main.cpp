/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "../../src/co_occur.h"

void PrintUsage()
{
    std::string usage;
    usage = R"(Syntax:
    ./ToolB -m /path/to/modified/wav/file -s /path/to/SRT

    Options:
        -mod OR -m   FILE: Modified Audio file
        -srt OR -s   FILE: Original Subtitle file

    Example: ./ToolB -mod episode1_W/OCommercial.wav -s input.srt)";

    std::cout << usage << std::endl;
}

int ParseOptions(int arg_c, char* arg_v[])
{
    co_oCCur::co_occur *sync;
    sync = new co_oCCur::co_occur();

    sync->setTool("B");

    if (arg_c < 2)
    {
        PrintUsage();
        return 11;
    }

    for (int i=2; i<arg_c; i++)
    {
        std::string parameter(arg_v[i]);

        if ((parameter == "-mod" || parameter == "-m") && i + 1 < arg_c)
        {
            sync->setModifiedAudio(arg_v[i+1]);
            i++;
        }

        else if ((parameter == "-srt" || parameter == "-s") && i + 1 < arg_c)
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

int main(int argc, char *argv[])
{
    ParseOptions(argc, argv);
    return 0;
}