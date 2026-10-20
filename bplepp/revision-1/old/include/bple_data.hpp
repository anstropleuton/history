/*
 * BPLE Data
 * Note: BPLE only
 */
 
#ifndef BPLE_DATA_HPP
#define BPLE_DATA_HPP

#include <vector>

// Count of skins per type
const std::vector<int> skinsPerType = {
    1,   // Type: 0: Empry (still invalid)
    20,  // Type: 1
    1,   // Type: 2
    7,   // Type: 3
    6,   // Type: 4
    11,  // Type: 5
    134, // Type: 6
    11,  // Type: 7
    7,   // Type: 8
    8,   // Type: 9
    8,   // Type: 10
    8,   // Type: 11
    6,   // Type: 12
    10,  // Type: 13
    10,  // Type: 14
    5,   // Type: 15
    5,   // Type: 16
    4,   // Type: 17
    4,   // Type: 18
    10,  // Type: 19
    7,   // Type: 20
    7,   // Type: 21
    7,   // Type: 22
    4,   // Type: 23
    4,   // Type: 24
    6,   // Type: 25
    8,   // Type: 26
    8,   // Type: 27
    8,   // Type: 28
    8,   // Type: 29
    6,   // Type: 30
    4,   // Type: 31
    4,   // Type: 32
    5,   // Type: 33
    4,   // Type: 34
    4,   // Type: 35
    4,   // Type: 36
    5,   // Type: 37
    4,   // Type: 38
    11,  // Type: 39: One empty at index 7
    2,   // Type: 41
    5,   // Type: 42
    6,   // Type: 43
    4,   // Type: 44
    7,   // Type: 45
    4,   // Type: 46
    6,   // Type: 47
    48   // Type: 48
};

// Count of valid prop for each skins of all types
const std::vector<std::vector<int>> propsPerSkinType = {
    1,   // Type: 0: Empry (still invalid)
    20,  // Type: 1
    1,   // Type: 2
    7,   // Type: 3
    6,   // Type: 4
    11,  // Type: 5
    134, // Type: 6
    11,  // Type: 7
    7,   // Type: 8
    8,   // Type: 9
    8,   // Type: 10
    8,   // Type: 11
    6,   // Type: 12
    10,  // Type: 13
    10,  // Type: 14
    5,   // Type: 15
    5,   // Type: 16
    4,   // Type: 17
    4,   // Type: 18
    10,  // Type: 19
    7,   // Type: 20
    7,   // Type: 21
    7,   // Type: 22
    4,   // Type: 23
    4,   // Type: 24
    6,   // Type: 25
    8,   // Type: 26
    8,   // Type: 27
    8,   // Type: 28
    8,   // Type: 29
    6,   // Type: 30
    4,   // Type: 31
    4,   // Type: 32
    5,   // Type: 33
    4,   // Type: 34
    4,   // Type: 35
    4,   // Type: 36
    5,   // Type: 37
    4,   // Type: 38
    11,  // Type: 39: One empty at index 7
    2,   // Type: 41
    5,   // Type: 42
    6,   // Type: 43
    4,   // Type: 44
    7,   // Type: 45
    4,   // Type: 46
    6,   // Type: 47
    48   // Type: 48
};

#endif // BPLE_DATA_HPP
