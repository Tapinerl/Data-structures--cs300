#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief doubles even numbers in the vector, removes uneven numbers.
 *
 * @param vec vector to be handled
 */
void doubleEvenRemoveUneven(std::vector<int>& vec) {

    using Iter = std::vector<int>::iterator;

    Iter begin = vec.begin();
    Iter end = vec.end();
    for ( Iter it = begin; it < end; it++){
        if ( *it % 2 == 0 ){
            vec.insert(it, *it);
            it++;

        }
        else {
            vec.erase(it);
            it--;
        }
    }
}

