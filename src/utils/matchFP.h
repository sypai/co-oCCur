/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_MATCHFP_H
#define CO_OCCUR_MATCHFP_H

#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include "../../libs/lib_ext/chromaprint/src/fingerprint_matcher.h"
#include "../../libs/lib_ext/chromaprint/src/fingerprinter_configuration.h"
#include "../../libs/lib_ext/chromaprint/src/utils.h"
#include "../../libs/lib_ext/chromaprint/src/utils/gaussian_filter.h"
#include "../../libs/lib_ext/chromaprint/src/utils/gradient.h"
#include "dactylogram.h"

namespace co_oCCur {

    class FingerprinterConfiguration;

    struct Segment
    {
        size_t pos1;
        size_t pos2;
        size_t duration;
        double score;
        double left_score;
        double right_score;

        Segment(size_t pos1, size_t pos2, size_t duration, double score)
                : pos1(pos1), pos2(pos2), duration(duration), score(score), left_score(score), right_score(score) {}

        Segment(size_t pos1, size_t pos2, size_t duration, double score, double left_score, double right_score)
                : pos1(pos1), pos2(pos2), duration(duration), score(score), left_score(left_score), right_score(right_score) {}

        int public_score() const {
            //return std::max(0, int(100 - std::round(score * (100.0 / 14.0))));
            //return std::max(0, int(100 - std::round(score * (100.0 / 14.0))));
            return int(score * 100 + 0.5);
        }

        Segment merged(const Segment &other) {
//            assert(pos1 + duration == other.pos1);
//            assert(pos2 + duration == other.pos2);
            const auto new_duration = duration + other.duration;
            const auto new_score = (score * duration + other.score * other.duration) / new_duration;

            return Segment(pos1, pos2, new_duration, new_score, score, other.score);
        }
    };

    class matchFP
    {
    public:
//        matchFP(FingerprinterConfiguration *config);
        matchFP();

        // Anything above this is not considered a match.
        void set_match_threshold(double t) { m_match_threshold = t; }
        double match_threshold() const { return m_match_threshold; }
        static constexpr double kDefaultMatchThreshold = 10.0;

        bool Match(const std::vector<uint32_t> &fp1, const std::vector<uint32_t> &fp2);
        bool Match(const uint32_t fp1_data[], size_t fp1_size, const uint32_t fp2_data[], size_t fp2_size);

        double GetHashTime(size_t i) const;
        double GetHashDuration(size_t i) const;

        const std::vector<Segment> &segments() const { return m_segments; };

        bool match();

    private:
//        std::unique_ptr<FingerprinterConfiguration> m_config;
        std::vector<uint32_t> m_offsets;
        std::vector<uint32_t> m_histogram;
        std::vector<std::pair<uint32_t, uint32_t>> m_best_alignments;
        std::vector<Segment> m_segments;
        double m_match_threshold = kDefaultMatchThreshold;
    };

}

#endif //CO_OCCUR_MATCHFP_H
