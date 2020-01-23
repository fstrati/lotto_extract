/*
 * combinations.h
 *
 *  Created on: Jan 21, 2020
 *      Author: fstrati
 */

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <errno.h>
#include <cstdlib>

#include "basic_types.h"
#include "utilities.h"
using namespace lotto_base;

#ifndef COMBINATIONS_H_
#define COMBINATIONS_H_

namespace lotto_extract {

typedef std::array<uint8_t,2>   key2;
typedef std::array<uint8_t,3>   key3;

class combinations
{
public:
	combinations() { ; }
	combinations(const combinations&) = delete;
	combinations(combinations&&) = delete;
	combinations& operator=(const combinations&) = delete;
	combinations& operator=(combinations&&) = delete;
    virtual ~combinations() = default;

public:
    static int32_t get_first_comb_2_of_90(key2& key);
    static int32_t get_next_comb_2_of_90(key2& key);

    static int32_t get_first_comb_3_of_90(key3& key);
    static int32_t get_next_comb_3_of_90(key3& key);

    static int32_t get_first_comb_2_of_5(key2& key);
    static int32_t get_next_comb_2_of_5(key2& key);

    static int32_t get_first_comb_3_of_5(key3& key);
    static int32_t get_next_comb_3_of_5(key3& key);

    static std::vector<key2> combinations_2_of_90;
    static std::vector<key3> combinations_3_of_90;
    static std::vector<key2> combinations_2_of_5;
    static std::vector<key3> combinations_3_of_5;

    static int32_t init_combinations(void);

    static std::vector<key2> get_ambi_combinations(const extraction_t ex);
    static std::vector<key3> get_terni_combinations(const extraction_t ex);

private:
    static uint8_t i_2_of_90;
    static uint8_t j_2_of_90;

    static uint8_t i_3_of_90;
    static uint8_t j_3_of_90;
    static uint8_t k_3_of_90;

    static uint8_t i_2_of_5;
    static uint8_t j_2_of_5;

    static uint8_t i_3_of_5;
    static uint8_t j_3_of_5;
    static uint8_t k_3_of_5;

};

} // namespace lotto_extract




#endif /* COMBINATIONS_H_ */

