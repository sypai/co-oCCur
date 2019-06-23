/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#ifndef CO_OCCUR_DEBUG_H
#define CO_OCCUR_DEBUG_H

#ifdef NDEBUG
    #include <ostream>
#else
    #include <iostream>
#endif

namespace co_oCCur {

    #ifdef NDEBUG
        #define DEBUG(x)
    #else
        #define DEBUG(x) std::cerr << x << std::endl;
    #endif

} // namespace co_oCCur

#endif //CO_OCCUR_DEBUG_H
