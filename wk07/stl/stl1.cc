#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief sort a given vector to an ascending order
 *
 * @param v vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortAsc(std::vector<int>& v)
{
    sort(v.rbegin(), v.rend(), greater <>());
    return EXIT_SUCCESS;

   return EXIT_FAILURE;
}

