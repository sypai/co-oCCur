/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_TOOLA_H
#define CO_OCCUR_TOOLA_H

#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include "../../libs/lib_co-oCCur/SiftSRT/SiftSRT.h"
#include "../../libs/lib_ext/CCAligner/read_wav_file.h"
#include "../utils/base64Strings.h"
//TODO: #include <dactylogram.h>

namespace co_oCCur {

    void createTempSRT(std::vector<SubtitleItem*> sub);

    class ToolA
    {
    private:
        std::string m_OriginalAudioFile;
        std::string m_ModifiedAudioFile;
        std::string m_OriginalSubtitleFile;

        std::vector<std::string> m_Fingerprints;
        std::vector<long int> m_FPTimestamps;

    public:
        ToolA();
        ToolA(std::string OriginalAudioFile, std::string ModifiedAudioFile, std::string OriginalSubtitleFile);
        void generateFingerprints();
        void AFInserter();
        void align();
        ~ToolA();

    private:
        long int seekAndCompare();
    };


} // namespace co_oCCur

#endif //CO_OCCUR_TOOLA_H
