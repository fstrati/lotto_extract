/*
 * rdbms.h
 *
 *  Created on: Jan 20, 2020
 *      Author: fstrati
 */

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <errno.h>
#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include <cassert>
#include "leveldb/db.h"

#include "basic_types.h"
#include "utilities.h"
using namespace lotto_base;

#include "combinations.h"

#ifndef RDBMS_H_
#define RDBMS_H_

namespace lotto_extract {

class rdbms
{
public:
	rdbms() { ; }
	rdbms(const rdbms&) = delete;
	rdbms(rdbms&&) = delete;
	rdbms& operator=(const rdbms&) = delete;
	rdbms& operator=(rdbms&&) = delete;
    virtual ~rdbms() = default;

public:
	static std::map<ruota_t,leveldb::DB*> db_ex_ambi;
	static std::map<ruota_t,leveldb::DB*> db_freq_ambi;
	static std::map<ruota_t,leveldb::DB*> db_ex_terni;
	static std::map<ruota_t,leveldb::DB*> db_freq_terni;

	static boost::filesystem::path get_name_db_ex_ambi_from_ruota(ruota_t ruota);
	static boost::filesystem::path get_name_db_freq_ambi_from_ruota(ruota_t ruota);
	static boost::filesystem::path get_name_db_ex_terni_from_ruota(ruota_t ruota);
	static boost::filesystem::path get_name_db_freq_terni_from_ruota(ruota_t ruota);

	static void init_db_maps(void);
	static void init_db_remove_all(void);
	static void init_db_open_create_if_missing_all(void);
	static void init_db_push_default_entries(void);

	static void close_dbs(void);

	static int32_t put_ambo_freq_date(leveldb::DB* db, const key2 key, const uint32_t freq, std::vector<date_t>& date);
	static int32_t get_ambo_freq_date(leveldb::DB* db, const key2 key, uint32_t& freq, std::vector<date_t>& date);
	static int32_t del_ambo_freq_date(leveldb::DB* db, const key2 key);

	static int32_t put_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex);
	static int32_t get_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex);
	static int32_t del_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq);

	static int32_t put_terno_freq_date(leveldb::DB* db, const key3 key, const uint32_t freq, std::vector<date_t>& date);
	static int32_t get_terno_freq_date(leveldb::DB* db, const key3 key, uint32_t& freq, std::vector<date_t>& date);
	static int32_t del_terno_freq_date(leveldb::DB* db, const key3 key);

	static int32_t put_freq_terno_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex);
	static int32_t get_freq_terno_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex);
	static int32_t del_freq_terno_date(leveldb::DB* db, const uint32_t key_freq);

};

} // namespace lotto_extract

#endif /* RDBMS_H_ */



