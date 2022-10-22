#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief sort a given vector to an descending order
 *
 * @param v a vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortDesc(std::vector<int>& v)
{
    sort(v.begin(), v.end(), greater <>());
    return EXIT_SUCCESS;

  return EXIT_FAILURE;
}

