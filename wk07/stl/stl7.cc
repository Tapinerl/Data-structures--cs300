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
    const auto it = v.begin() + v.size() / 2;
    std::nth_element(v.begin(), it , v.end());
    auto median = *it;
    return median;

}

