/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include "utils/dactylogram.h"
#include "src/utils/gaussian_filter.h"
#include "src/utils/gradient.h"
#include "NumCpp.hpp"

//std::vector<int> left_shift(int num, std::vector<int>& inArray)
//{
//    std::vector<int> b;
//    for(int i=31; i>=5; i-=2)
//    {
//        b.emplace_back(std::pow(2, i));
//    }
//    return array1;
//}

double index_to_s(int i)
{
    return i * (4096 / 3) / 11025.0;
}

void printVector(std::vector<int> InVector)
{
    if(!InVector.empty())
    {
        std::cout << "[";
        for(int i : InVector) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

void printVector(std::vector<float> InVector)
{
    if(!InVector.empty())
    {
        std::cout << "[";
        for(float i : InVector) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
}
void printVector(std::vector<double > InVector)
{
    if(!InVector.empty())
    {
        std::cout << "[";
        for(double i : InVector) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

void printMap(std::map <uint32_t, std::vector<int> > InMap)
{
    if (!InMap.empty())
    {
        auto itr = InMap.begin();

        while (itr != InMap.end()) {
            std::cout << itr->first << ": [";

            for (int i : itr->second) {
                std::cout << i << ", ";
            }
            std::cout << "]" << std::endl;

            itr++;
        }
    }

    else
    {
        std::cout << "Nothing in here!" <<std::endl;
    }
}

void printMap(std::map <int32_t, int> InMap)
{
    if (!InMap.empty())
    {
        auto itr = InMap.begin();

        while(itr != InMap.end())
        {
            std::cout << itr->first << ": " ;
            std::cout << itr->second << std::endl;

            itr++;
        }
    }

    else
    {
        std::cout << "Nothing in here!" <<std::endl;
    }
}

int mn(std::map <int32_t, int> InMap)
{
    if (!InMap.empty())
    {
        auto itr = InMap.begin();
        return itr->first;
    }

    return INT32_MAX;
}

int mx(std::map <int32_t, int> InMap)
{
    if (!InMap.empty())
    {
        auto itr = InMap.rbegin();
        return itr->first;
    }

    return INT32_MIN;
}

double findMax(std::vector<double> a)
{
    return *std::max_element(a.begin(), a.end());
}

std::vector<int> arange(int lb, int ub)
{
    std::vector<int> arangeVector;
//    std::cout << arangeVector.max_size();
    arangeVector.clear();
    arangeVector.reserve(-lb + ub +1);
    for (int i = lb; i < ub; i++) {
        arangeVector.emplace_back(i);
    }

    return arangeVector;
}

void align_fingerprints()
{
    const uint32_t mask = 2863311520;

    std::map <uint32_t, std::vector<int> > abs_offsets;
    std::map <int32_t, int > rel_offsets;

    std::vector<uint32_t > fp1, fp2;
//    co_oCCur::Dactylogram *org;
//    org = new co_oCCur::Dactylogram("/home/sypai/co-oCCur/demo/TestFiles/test1/original.wav");
//    org->collectFingerprints();
//    fp2 = org->getAllFingerprints();

    co_oCCur::Dactylogram *mod;
    mod = new co_oCCur::Dactylogram("/home/sypai/co-oCCur/demo/TestFiles/test1/modified.wav");
    mod->collectFingerprints();
    fp1 = mod->getAllFingerprints();

//    std::vector<uint32_t > fp1 = {2352396484,2368907460,2368973005,2406591693,2398139599,2398130655,2396107247,2387842031,2387792879,2253583343,2521916397,2522112873,2538874729,2505390905,2488744731,2488695050,2625624378};
//    std::vector<uint32_t > fp2 = {1383518282,1390858442,1387643131,1387561193,1404346856,1354035560,1349833064,1345705320,1077332841,1076296281,3493259850,3497323082,3547630154,3530988362,3530985546,3535179787,3535175705};

    // seg1
//    fp2 = {2116461623, 4096179735, 4096182791, 1961806407, 1958693575,
//           1958562503, 1958484581, 2008821605, 2128358519, 1583096887,
//           1578378275, 1578972258, 1579028594, 1549145283, 1555175233,
//           1555170816, 1284525568, 3432041984, 3431004960, 3430967648,
//           2357210464, 2357225440, 2357352099, 2357384866, 2223359650,
//           2277805734, 2262600374, 2186779014, 2455152775, 2451024005,
//           2450827429, 2450820213, 2453054477, 2435244045, 3492175935,
//           3492152357, 3492742181, 4033748772, 4042133108, 4038938325,
//           4053549718, 4086612726, 4089770990, 4077187390, 3540316426,
//           3507839242, 1343496459, 1947544856, 1947609896, 1960196648,
//           2090732264, 2090797977, 2107116427, 1589133194,  519832458,
//           443498890,  444002730,  456647082,  420536554, 1477501294,
//           1209065774, 1213263654, 1213325090, 1232338738, 1525872402,
//           1519571222, 3398861062, 3397812502, 3398210871, 3419188581,
//           3360205292, 3691559356, 3692792204, 3696922012, 3722085820,
//           3750663092, 3754865589, 3745890981, 3741631207, 3742659367,
//           3744952071, 3711314183, 1568024901, 1575369205, 1558584805,
//           1551172085, 1551106517, 1546736069, 1565708741, 1565630791,
//           1607509014, 1607511094, 1599453206, 1599519766, 1566495766,
//           1568293182, 1568146207, 1568158299, 1576743641, 1442460409,
//           1421743721, 1421807593, 1421614585, 1421092329, 1425285208,
//           1165230088, 1176764680, 1176912664, 1445384984, 1449584440,
//           1458024248, 1457826584, 1453627931, 1470402106, 1437896442,
//           1576896186, 1564447642, 1564463290, 3711512746, 3709608106,
//           3713867946, 3703972010, 3703979950, 3695581918, 3445496518,
//           3386781190, 3382454023, 3418153220, 3418140964, 2344132900,
//           2327360292, 2330493732, 2330493477, 2322314018, 2318050082,
//           2318049570, 2334818338, 3409675463, 3376065669, 3625626756,
//           3692726660, 3692743044, 3695103876, 3695107972, 3713925045,
//           3713841125, 3747407333, 4284347869, 4284348373, 4284348405,
//           4284343269, 4284347365, 4292826087, 4275917813, 4267536861,
//           4283789789, 3747051005, 4250328437, 4244101495, 4243978583,
//           4244094215, 4235574535, 2104857879, 2138411063, 2117578807,
//           4265013301, 4130828305, 1983328465, 1983784113, 2000559521,
//           1979649697, 1958416049, 1958350467, 1421547139, 3568837267,
//           3567788897, 3572245793, 3563859248, 2221680896, 2237934336,
//           2246453776, 2410560048, 2410548736, 2410037760, 2377433969,
//           2377437665, 2364834210, 2354348962, 2216075906, 3289835138,
//           3305432738, 3338950130, 1191595202, 1461087302, 1461120006,
//           1463807030,  327138326,  326995206,  871926806,  855147543,
//           846761991,  842965063,  842965191,  859738309,  826180740,
//           1898875028, 1899415988, 1668795044, 3804036836, 3804029540,
//           3804029492, 4093403660, 4051657231, 4030603822, 4029156910,
//           4033366318, 4058531878, 4087924758, 4071278598, 4075478021,
//           4067095620, 4063158340};

//seg2

    fp2 = {2502946804, 2486038004, 2490232052, 2624584853, 2625035413,
           2641810823, 2671162759, 2669127126, 2652366326, 2652233830,
           2658459751, 2541019509, 2541052244, 2541642708, 3078518493,
           3078577917, 3086864365, 3086798191, 3070090366, 2524967950,
           2541215758, 2507628558, 2424254750, 2424182046, 2424184606,
           2424249959, 2432634599, 2432519911, 2447169439, 2480731023,
           2459742607, 2531242439, 2531235287, 2539452263, 2501703463,
           2503829285, 2638163772, 2637638956, 2641706284, 3221050412,
           3221180468, 3181367300, 2643325252, 2626462661, 2626463415,
           2630676134, 2622357158, 2618280870, 2634993078, 2669563014,
           2669569223, 2673767621, 3749594325, 3757823220, 3623609844,
           4160480708, 4160354245, 4160346071, 3623475190, 3623483110,
           4160284262, 4151867238, 4147672950, 4114184022, 3577443798,
           3581637078, 3573240310, 3573235927, 3571007701, 3571283396,
           3562857940, 3579707380, 3613384692, 3604897620, 3598612292,
           2524919572, 2256484148, 2273123636, 2239569172, 2247900436,
           2243765556, 2243912996, 2243907893, 2243805557, 2375860583,
           2380038503, 2405204342, 2371855702, 2635965814, 3709646198,
           3709701462, 3708456262, 3708464606, 4245335542, 4280266231,
           4280133063, 4280133063, 4279742935, 4283937255, 4282905063,
           4282741206, 4265834326, 3192094582, 3201531686, 3197591398,
           3063238502, 2528462678, 2528928086, 2547872215, 2497659333,
           2491220420, 3560505812, 3560649200, 3564839780, 3564791652,
           3573251957, 3588841429, 3614169543, 3597659590, 3597598150,
           3615348182, 3615282662, 3581727215, 3512521727, 4083152877,
           4083152621, 4152354797, 4135561213, 4135298525, 4135301471,
           4152066335, 3044639007, 3044648767, 3044665135, 3052992303,
           3053051693, 3053180221, 3053180285, 3078190429, 3078186333,
           3074123599, 3061479263, 3061470830, 3061433966, 2792937342,
           2807625034, 2774070731, 4116305353, 4099659209, 4099667421,
           3025790956, 3025795052, 3027982317, 3027989885, 3035321661,
           3051963709, 3085501757, 3085514527, 2549818886, 2516264470,
           2512070262, 2512140130, 2231113058, 3321627682, 3313303602,
           3344830466, 3344855043, 3346982145, 1207876865, 1207864128,
           1199475536, 1194166624, 1177394545, 1178576209, 1178596417,
           1195208723, 1195324467, 1195324451, 1161763875, 1165958435,
           1157574499, 1151280707, 1151170241, 1151203009, 1151186625,
           1174118353, 1174113777, 1198099937, 1198112225, 1181330929,
           1182379475, 1190784082, 1199270994, 1199258658, 1161640994,
           1144855586, 1279210786, 1283356178, 1285509634, 1301935619,
           1301917185, 1301913153, 1341885265, 1333365089, 1311358433,
           1311352305, 1312416241, 1316581585, 1316689107, 1333398771,
           1294665954, 1277761890, 1281964322, 1290501634, 1286338050,
           1286201859, 3434718787, 3433674323, 3454766963, 3446247395,
           3475611619, 3475633906};

////comm4
//
//    fp2 ={558349038,  554154670,  554154686,  554023646,  587579390,
//          1661384111, 1661384107, 1656927661, 1652733357, 1652667788,
//          1657126284, 3804544396, 3796155828, 3796155636, 3808730324,
//          3875982805, 2770266485, 2774461285, 2785005349, 2818559797,
//          2818557719, 2818392839, 2809942343, 2809946471, 2776457575,
//          2776580454, 2759740758, 2759723334, 2768206166, 2902288758,
//          2902423895, 2901368263, 2909623751, 2909590983, 2909664229,
//          2952782821, 2952714225, 2934888257, 2952775491, 2809976643,
//          2772198754, 2755421538, 2889839970, 2893956466, 2759806450,
//          2780810738, 2814369233, 2797600192, 2796477760, 2792332608,
//          2788005120, 2787964192, 2789009252, 2810045796, 2810041701,
//          2808958453, 2808956405, 2809091575, 2809099767, 2809091575,
//          2951697919, 2951699935, 2951568862, 2917953022, 2918018550,
//          2901303254, 2901296454, 2909750598, 2909585783, 2909593959,
//          2784879973, 2784417108, 2784417028, 2776037184, 2771842624,
//          2772223712, 2776388325, 2776388581, 2783795199, 2783792606,
//          2783825406, 2783967742, 2783967710, 2817514846, 2808927503,
//          2805782287, 2805851933, 2805917485, 2805978405, 2810004773,
//          2792179047, 2817458678, 2817393138, 2783850898, 2758750711,
//          2758623223, 2767133653, 3840891733, 3836603220, 3853511504,
//          3853516609, 3857502465, 3891056915, 2775351330, 2776605798,
//          2776542562, 2810117459, 2809060673, 2804862272, 2805777776,
//          2805786080, 2775868900, 2784319460, 2780290804, 2780290805,
//          2780167926, 2817847030, 2810042326, 2805777750, 2804728950,
//          2804798710, 2771281111, 2775477461, 2708372692, 2716818900,
//          2716800500, 2716669296, 2716668721, 2716693302, 2716803862,
//          3790419734, 3783008054, 3850132278, 3883882774, 3883825414,
//          3883859270, 3883719878, 3883455734, 3889747174, 3852120294,
//          3835343350, 2761593793, 2784605120, 2771968976, 2906186208,
//          2906383840, 2906379744, 2805944292, 2809089013, 2817510261,
//          3874355509, 3870157077, 3874356501, 2792158533, 3865895237,
//          3882869093, 3849242087, 3849242583, 3849281495, 3849156055,
//          3849150963, 3849134579, 3857527159, 3857518967, 3840741759,
//          3840874335, 3840244735, 3840273407, 3836078841, 3836599257,
//          3841777609, 3850166217, 3849094601, 3882783081, 2809045097,
//          2809061421, 2809131357, 2804773213, 3882836975, 3849217007,
//          3849192423, 3975087062, 3975086534, 3966506327, 3965989927,
//          1751460903, 2045189151, 2040929551, 2074557710, 2057781006,
//          1520909838, 1538735631, 1504978461, 1502668413, 1501602536,
//          1501864680, 3682903016, 4219511800, 4202803676, 4202926319,
//          4202869998, 3192042982, 3204943334, 3204943334, 3171323862,
//          2903961158, 2903965250, 2773896755, 2776588083, 3858722579,
//          3858709778, 4118615314, 4101838098, 4101968210, 4100870482,
//          4109324658, 4109333875, 4126889795, 4122726209, 3082420177,
//          3082416113, 3086614481};
    for(auto i = 0; i < fp1.size(); i++){
        abs_offsets[fp1.at(i) & mask].push_back(i+1);
    }
    for(auto i = 0; i < fp2.size(); i++){
        abs_offsets[fp2.at(i) & mask].push_back(-(i+1));
    }

    std::cout << "abs_offsets" << std::endl;
    printMap(abs_offsets);

    auto itr = abs_offsets.begin();
    while (itr != abs_offsets.end())
    {
        auto hash_abs_offsets = itr->second;
        size_t len_hash_abs_offsets = hash_abs_offsets.size();

        if (len_hash_abs_offsets <= 1){
            itr++;
            continue;
        }

        for(int i = 0; i < len_hash_abs_offsets; i++){
            int o1 = hash_abs_offsets[i];

            if(o1 <= 0){
                continue;
            }
            for (int j = 0; j < len_hash_abs_offsets; j++){
                int o2 = hash_abs_offsets[j];
                if(o2 >= 0){
                    continue;
                }

                rel_offsets[o1+o2] += 1;
            }
        }
        itr++;
    }

    std::cout << "rel_offsets" << std::endl;
    printMap(rel_offsets);

    int min = mn(rel_offsets);
    int max = mx(rel_offsets);

//    int min = -31714;
//    int max = 19694;

    auto offsets = arange(min, max+1);

    std::vector<double > raw_counts;
    raw_counts.assign((1 + max - min), 0.0);

    auto itr1 = rel_offsets.begin();

    while(itr1 != rel_offsets.end())
    {
        raw_counts[itr1->first - min] = itr1->second;
        itr1++;
    }

    std::cout << "offsets" << std::endl;
    printVector(offsets);

//    std::vector<float> orig_bit_counts = raw_counts;
    std::vector<double > counts;
//    chromaprint::GaussianFilter(raw_counts, counts, 2, 2);

//    std::vector<float> gradient;
//    chromaprint::Gradient(counts.begin(), counts.end(), gradient.begin());

//    printMap(rel_offsets);


    auto a = nc::NdArray<double >(raw_counts);
    auto b = nc::filter::gaussianFilter1d(a, 0.6);

    for (auto i : b) {
        counts.emplace_back(i);
    }

    std::cout << "counts" << std::endl;
    printVector(counts);

//    auto threshold = nc::max(b);
//    std::cout << threshold;

    std::vector<float> gradient(counts.size());
    chromaprint::Gradient(counts.begin(), counts.end(), gradient.begin());

    for (size_t i = 0; i < counts.size(); i++) {
        gradient[i] = std::abs(gradient[i]);
    }

    std::vector<size_t> gradient_peaks;
    for (size_t i = 0; i < counts.size(); i++) {
        const auto gi = gradient[i];
        if (i > 0 && i < counts.size() - 1 && gi > 0.5 && gi >= gradient[i - 1] && gi >= gradient[i + 1]) {
            if (gradient_peaks.empty() || gradient_peaks.back() + 1 < i) {
                gradient_peaks.push_back(i);
            }
        }
    }
//    gradient_peaks.push_back(counts.size());

//    std::cout << "[";
//    for(auto i : gradient_peaks) {
//        std::cout << i << ", ";
//    }
//    std::cout << "]" << std::endl;

    auto threshold = findMax(counts) * 0.1;
    std::vector<int> peaks;
    peaks.clear();
    peaks.reserve(counts.size());

    for(int i : gradient_peaks)
    {
        if(counts[i] > threshold)
        {
            peaks.emplace_back(i);
        }
    }

    std::cout << "peaks" << std::endl;
    printVector(peaks);


    std::sort(peaks.begin(), peaks.end(), std::greater<int>());

    if(peaks.size() > 10) {
        for (int i = 0; i < 10; i++) {
            auto ind = peaks[i];

            auto offsets_peaks = offsets[ind];

            auto counts_peaks = counts[ind];

            auto secs = index_to_s(offsets_peaks);

            std::cout << "\n" << i <<") OFFSETS_PEAKS: " << offsets_peaks;
            std::cout << ", COUNTS_PEAKS: " << counts_peaks;
            std::cout << ", SECS: " << secs << std::endl;
        }
    } else{
        for (int i = 0; i < peaks.size(); i++) {
            auto ind = peaks[i];

            auto offsets_peaks = offsets[ind];

            auto counts_peaks = counts[ind];

            auto secs = index_to_s(offsets_peaks);

            std::cout << "\n" << i <<") OFFSETS_PEAKS: " << offsets_peaks;
            std::cout << ", COUNTS_PEAKS: " << counts_peaks;
            std::cout << ", SECS: " << secs << std::endl;
        }
    }

//    delete org;
    delete mod;

    float secs = 2.6;
    double factor = (4096.0 / 3) / 11025.0;
    uint32_t hash = (secs - 2.6) / factor;

}

void segment()
{

}






























//    auto odds = nc::arange<int>(1, 32, 2);
//    auto evens = nc::arange<int>(0, 32, 2);
//
//    auto a = nc::append(evens, odds);
//    void *b = std::reverse(std::begin(a), std::end(a));

//    auto bits = nc::arange<int>(31, 3, -2);
//
//    auto c = nc::left_shift(1, bits);

//std::vector<int> a;
//for(int i = 31; i>=5; i-=2)
//{
//a.emplace_back(i);
//}
//
//std::vector<unsigned long long> b;
//b.emplace_back(2147483648);
//for(auto i=30; i>=5; i-=2)
//{
//b.emplace_back(std::pow(2, i));
//}
//
//std::cout << "Gp" <<std::endl;
////    auto b = left_shift(1, a);
//for (int i : b) {
//std::cout << i << ", ";
//}


//    auto offsets = nc::arange(min, max + 1);
//    auto raw_counts = nc::zeros(1 + max - min,);






