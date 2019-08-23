/*
* Author: Suyash Bajpai
* Email: 7suyashbajpai@gmail.com
* Github: https://github.com/sypai
*/

#include "NumCpp.hpp"

void run()
{
    auto a = nc::arange<double >(0, 5, 1);
    auto b = nc::filter::gaussianFilter1d(a, 0.6);

    for (int i : b) {
        std::cout << i << ", ";
    }
}

