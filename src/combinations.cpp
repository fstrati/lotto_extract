/*
 * combinations.cpp
 *
 *  Created on: Jan 21, 2020
 *      Author: fstrati
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "combinations.h"

namespace lotto_extract {

uint8_t combinations::i_2_of_90 = 1;
uint8_t combinations::j_2_of_90 = 2;

uint8_t combinations::i_3_of_90 = 1;
uint8_t combinations::j_3_of_90 = 2;
uint8_t combinations::k_3_of_90 = 3;

uint8_t combinations::i_2_of_5 = 1;
uint8_t combinations::j_2_of_5 = 2;

uint8_t combinations::i_3_of_5 = 1;
uint8_t combinations::j_3_of_5 = 2;
uint8_t combinations::k_3_of_5 = 3;

std::vector<key2> combinations::combinations_2_of_90;
std::vector<key3> combinations::combinations_3_of_90;
std::vector<key2> combinations::combinations_2_of_5;
std::vector<key3> combinations::combinations_3_of_5;

int32_t combinations::get_first_comb_2_of_90(key2& key)
{
	combinations::i_2_of_90 = 1;
	combinations::j_2_of_90 = 2;
	key[0] = combinations::i_2_of_90;
	key[1] = combinations::j_2_of_90;
	return (0);
}

int32_t combinations::get_next_comb_2_of_90(key2& key)
{
	combinations::j_2_of_90++;
	if( combinations::j_2_of_90 > 90 )
	{
		combinations::i_2_of_90++;
		if( combinations::i_2_of_90 > 89 )
		{
			return (-1);
		}
		else
		{
			combinations::j_2_of_90 = (uint8_t) combinations::i_2_of_90 + ((uint8_t) 0x01);
		}
	}

	key[0] = combinations::i_2_of_90;
	key[1] = combinations::j_2_of_90;

	return (0);
}

int32_t combinations::get_first_comb_3_of_90(key3& key)
{
	combinations::i_3_of_90 = 1;
	combinations::j_3_of_90 = 2;
	combinations::k_3_of_90 = 3;
	key[0] = combinations::i_3_of_90;
	key[1] = combinations::j_3_of_90;
	key[2] = combinations::k_3_of_90;
	return (0);
}

int32_t combinations::get_next_comb_3_of_90(key3& key)
{
	combinations::k_3_of_90++;
	if( combinations::k_3_of_90 > 90 )
	{
		combinations::j_3_of_90++;
		if( combinations::j_3_of_90 > 89 )
		{
			combinations::i_3_of_90++;
			if( combinations::i_3_of_90 > 88 )
			{
				return (-1);
			}
			else
			{
				combinations::j_3_of_90 = (uint8_t) combinations::i_3_of_90 + ((uint8_t) 0x01);
				combinations::k_3_of_90 = (uint8_t) combinations::j_3_of_90 + ((uint8_t) 0x01);
			}
		}
		else
		{
			combinations::k_3_of_90 = (uint8_t) combinations::j_3_of_90 + ((uint8_t) 0x01);
		}
	}

	key[0] = combinations::i_3_of_90;
	key[1] = combinations::j_3_of_90;
	key[2] = combinations::k_3_of_90;

	return (0);
}

int32_t combinations::get_first_comb_2_of_5(key2& key)
{
	combinations::i_2_of_5 = 1;
	combinations::j_2_of_5 = 2;
	key[0] = combinations::i_2_of_5;
	key[1] = combinations::j_2_of_5;
	return (0);
}

int32_t combinations::get_next_comb_2_of_5(key2& key)
{
	combinations::j_2_of_5++;
	if( combinations::j_2_of_5 > 5 )
	{
		combinations::i_2_of_5++;
		if( combinations::i_2_of_5 > 4 )
		{
			return (-1);
		}
		else
		{
			combinations::j_2_of_5 = (uint8_t) combinations::i_2_of_5 + ((uint8_t) 0x01);
		}
	}

	key[0] = combinations::i_2_of_5;
	key[1] = combinations::j_2_of_5;

	return (0);
}

int32_t combinations::get_first_comb_3_of_5(key3& key)
{
	combinations::i_3_of_5 = 1;
	combinations::j_3_of_5 = 2;
	combinations::k_3_of_5 = 3;
	key[0] = combinations::i_3_of_5;
	key[1] = combinations::j_3_of_5;
	key[2] = combinations::k_3_of_5;
	return (0);
}

int32_t combinations::get_next_comb_3_of_5(key3& key)
{
	combinations::k_3_of_5++;
	if( combinations::k_3_of_5 > 5 )
	{
		combinations::j_3_of_5++;
		if( combinations::j_3_of_5 > 4 )
		{
			combinations::i_3_of_5++;
			if( combinations::i_3_of_5 > 3 )
			{
				return (-1);
			}
			else
			{
				combinations::j_3_of_5 = (uint8_t) combinations::i_3_of_5 + ((uint8_t) 0x01);
				combinations::k_3_of_5 = (uint8_t) combinations::j_3_of_5 + ((uint8_t) 0x01);
			}
		}
		else
		{
			combinations::k_3_of_5 = (uint8_t) combinations::j_3_of_5 + ((uint8_t) 0x01);
		}
	}

	key[0] = combinations::i_3_of_5;
	key[1] = combinations::j_3_of_5;
	key[2] = combinations::k_3_of_5;

	return (0);
}

int32_t combinations::init_combinations(void)
{
	combinations::combinations_2_of_90.clear();
	combinations::combinations_3_of_90.clear();
	combinations::combinations_2_of_5.clear();
	combinations::combinations_3_of_5.clear();

	key2 key_two;

	combinations::get_first_comb_2_of_90(key_two);
	combinations::combinations_2_of_90.push_back(key_two);
	while( 0 == combinations::get_next_comb_2_of_90(key_two) )
	{
		combinations::combinations_2_of_90.push_back(key_two);
	}
	if( 4005 != combinations::combinations_2_of_90.size() )
	{
		fprintf(stderr, "Error: combinations_2_of_90.size() = %lu.\n", combinations::combinations_2_of_90.size());
		return (-1);
	}
#if 0
	// verify
	for( auto& comb : combinations::combinations_2_of_90 )
	{
		fprintf(stderr, "Debug: ambo [%02u,%02u].\n", comb[0], comb[1]);
	}
	system_pause();
#endif

	combinations::get_first_comb_2_of_5(key_two);
	combinations::combinations_2_of_5.push_back(key_two);
	while( 0 == combinations::get_next_comb_2_of_5(key_two) )
	{
		combinations::combinations_2_of_5.push_back(key_two);
	}
	if( 10 != combinations::combinations_2_of_5.size() )
	{
		fprintf(stderr, "Error: combinations_2_of_5.size() = %lu.\n", combinations::combinations_2_of_5.size());
		return (-1);
	}
#if 0
	// verify
	for( auto& comb : combinations::combinations_2_of_5 )
	{
		fprintf(stderr, "Debug: ambo [%02u,%02u].\n", comb[0], comb[1]);
	}
	system_pause();
#endif

	key3 key_three;

	combinations::get_first_comb_3_of_90(key_three);
	combinations::combinations_3_of_90.push_back(key_three);
	while( 0 == combinations::get_next_comb_3_of_90(key_three) )
	{
		combinations::combinations_3_of_90.push_back(key_three);
	}
	if( 117480 != combinations::combinations_3_of_90.size() )
	{
		fprintf(stderr, "Error: combinations_3_of_90.size() = %lu.\n", combinations::combinations_3_of_90.size());
		return (-1);
	}
#if 0
	// verify
	for( auto& comb : combinations::combinations_3_of_90 )
	{
		fprintf(stderr, "Debug: terno [%02u,%02u,%02u].\n", comb[0], comb[1], comb[2]);
	}
	system_pause();
#endif

	combinations::get_first_comb_3_of_5(key_three);
	combinations::combinations_3_of_5.push_back(key_three);
	while( 0 == combinations::get_next_comb_3_of_5(key_three) )
	{
		combinations::combinations_3_of_5.push_back(key_three);
	}
	if( 10 != combinations::combinations_3_of_5.size() )
	{
		fprintf(stderr, "Error: combinations_3_of_5.size() = %lu.\n", combinations::combinations_3_of_5.size());
		return (-1);
	}
#if 0
	// verify
	for( auto& comb : combinations::combinations_3_of_5 )
	{
		fprintf(stderr, "Debug: terno [%02u,%02u,%02u].\n", comb[0], comb[1], comb[2]);
	}
	system_pause();
#endif

    return (0);
}

std::vector<key2> combinations::get_ambi_combinations(const extraction_t ex)
{
	std::vector<key2> res;

	std::vector<uint8_t> extracted;
	extracted.push_back(((uint8_t) ex.bits.a));
	extracted.push_back(((uint8_t) ex.bits.b));
	extracted.push_back(((uint8_t) ex.bits.c));
	extracted.push_back(((uint8_t) ex.bits.d));
	extracted.push_back(((uint8_t) ex.bits.e));

	std::sort(extracted.begin(),extracted.end());

	key2 key_result;
	for( const auto& key_comb : combinations::combinations_2_of_5 )
	{
		key_result[0] = extracted[(size_t) (key_comb[0]-1)];
		key_result[1] = extracted[(size_t) (key_comb[1]-1)];
		res.push_back(key_result);
	}

	return res;
}

std::vector<key3> combinations::get_terni_combinations(const extraction_t ex)
{
	std::vector<key3> res;

	std::vector<uint8_t> extracted;
	extracted.push_back(((uint8_t) ex.bits.a));
	extracted.push_back(((uint8_t) ex.bits.b));
	extracted.push_back(((uint8_t) ex.bits.c));
	extracted.push_back(((uint8_t) ex.bits.d));
	extracted.push_back(((uint8_t) ex.bits.e));

	std::sort(extracted.begin(),extracted.end());

	key3 key_result;
	for( const auto& key_comb : combinations::combinations_3_of_5 )
	{
		key_result[0] = extracted[(size_t) (key_comb[0]-1)];
		key_result[1] = extracted[(size_t) (key_comb[1]-1)];
		key_result[2] = extracted[(size_t) (key_comb[2]-1)];
		res.push_back(key_result);
	}

	return res;
}

} // namespace lotto_extract







