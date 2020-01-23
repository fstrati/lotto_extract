/*
 * history_extraction.h
 *
 *  Created on: Jan 20, 2020
 *      Author: fstrati
 */

#include <cstdint>
#include <string>
#include <vector>
#include <errno.h>
#include <cstdlib>
#include <map>

#include "basic_types.h"
#include "utilities.h"
using namespace lotto_base;

#ifndef HISTORY_EXTRACTION_H_
#define HISTORY_EXTRACTION_H_

namespace lotto_extract {

class extractions
{
public:
	extractions() { ; }
	extractions(const extractions&) = delete;
	extractions(extractions&&) = delete;
	extractions& operator=(const extractions&) = delete;
	extractions& operator=(extractions&&) = delete;
    virtual ~extractions() = default;

public:
	static std::vector<extraction_t> extraction_vec;
	static std::map<ruota_t,std::vector<extraction_t>> extractions_per_ruota;
	static void init_extractions(void);
};

} // namespace lotto_extract

#endif /* HISTORY_EXTRACTION_H_ */


