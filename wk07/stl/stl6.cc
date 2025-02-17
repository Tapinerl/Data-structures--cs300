#include <iterator>
#include <map>
#include <algorithm>

#include "test.hh" // NOT_FOUND constant

using namespace std;


/**
 * @brief From a map find the first element, whose value is at least given limit
 *        regardless of the key of the element. Return only the value or
 *        NOT_FOUND if none of the values match the the search criteria.
 * @param m map that is scanned trough
 * @param given
 * @return int
 */
int findAtLeastGiven(std::map<std::string, int>& m, int given)
{
    auto it = std::find_if(std::begin(m), std::end(m), [given](const auto& mo) {return mo.second == given; });
    auto x = (it !=std::end(m));
    return x;
    if (x == 0){
        return -1;
    }
}

