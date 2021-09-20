// Mazen Ibrahim
//295924
#include "ring.h"

template <typename Key, typename Info>
void removeAllElements(Ring <Key, Info>& ring) {
    while (ring.any() != ring.none()) {
        auto iter = ring.any();
        ring.remove(iter);
    }
}

template <typename Key, typename Info>
Ring <Key, Info> split(Ring <Key, Info>& ring,
    const Key& sKey, int sInst,
    const Key& eKey, int eInst,
    bool direction)
{
    Ring<Key, Info> result = ring;
    auto iter_i = result.any();
    auto iter_j = result.any();
    result.find(sKey, sInst, iter_i, direction);
    result.find(eKey, eInst, iter_j, direction);
    result.remove(iter_i, iter_j);
    return result;
}

int main(void) {
    Ring<int, int> ringA;
    Ring<int, int> ringB;

    ringA.insert(1, 1);
    ringA.insert(2, 3);
    ringA.insert(3, 1);
    ringA.insert(2, 2);
    ringA.insert(2, 1);
    ringA.insert(5, 1);

    ringB = ringA;

    std::cout << "Ring A:" << std::endl << ringA << std::endl;
    std::cout << "Ring B:" << std::endl << ringB << std::endl;

    ringB = split(ringA, 3, 1, 2, 3, true);

    std::cout << "Ring A:" << std::endl << ringA << std::endl;
    std::cout << "Ring B:" << std::endl << ringB << std::endl;

    removeAllElements(ringA);

    return 0;
}