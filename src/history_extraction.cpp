/*
 * history_extraction.cpp
 *
 *  Created on: Jan 20, 2020
 *      Author: fstrati
 */

#include "history_extraction.h"

namespace lotto_extract {

std::vector<extraction_t> extractions::extraction_vec;

std::map<ruota_t,std::vector<extraction_t>> extractions::extractions_per_ruota;

void extractions::init_extractions(void)
{
	extractions::extraction_vec.clear();

	extractions::extractions_per_ruota[ruota_t::NAZIONALE] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::BARI] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::CAGLIARI] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::FIRENZE] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::GENOVA] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::MILANO] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::NAPOLI] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::PALERMO] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::ROMA] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::TORINO] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::VENEZIA] = extractions::extraction_vec;
	extractions::extractions_per_ruota[ruota_t::TUTTE] = extractions::extraction_vec;

}

} // namespace lotto_extract


