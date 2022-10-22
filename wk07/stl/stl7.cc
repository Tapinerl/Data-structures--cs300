#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh" // NOT_FOUND constant

using namespace std;


/**
 * @brief Find the median value of a given vector, whose elements are in random
 *        order. Return NOT_FOUND if the size of the vector is zero.
 *
 * @param v a random vector
 * @return int
 */
int findMedian(std::vector<int>& v)
{
    if (v.size() % 2 == 0) {
        const auto it1 = v.begin() + v.size() / 2 - 1;
        const auto it2 = v.begin() + v.size() / 2;

        std::nth_element(v.begin(), it1 , v.end());
        const auto e1 = *it1;

        std::nth_element(v.begin(), it2 , v.end());
        const auto e2 = *it2;

        return (e1 + e2) / 2;

    } else {
        const auto it = v.begin() + v.size() / 2;
        std::nth_element(v.begin(), it , v.end());
        return *it;
    }

}

