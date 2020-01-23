/*
 * rdbms.cpp
 *
 *  Created on: Jan 20, 2020
 *      Author: fstrati
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <errno.h>
#include <cstdlib>
#include <algorithm>

#include "rdbms.h"

namespace lotto_extract {

std::map<ruota_t,leveldb::DB*> rdbms::db_ex_ambi;
std::map<ruota_t,leveldb::DB*> rdbms::db_freq_ambi;
std::map<ruota_t,leveldb::DB*> rdbms::db_ex_terni;
std::map<ruota_t,leveldb::DB*> rdbms::db_freq_terni;

boost::filesystem::path rdbms::get_name_db_ex_ambi_from_ruota(ruota_t ruota)
{
    boost::filesystem::path file_db(boost::filesystem::current_path());

    std::string ruota_str = convert_ruota_to_string(ruota);
	std::string prefix = "db_ex_ambi_";
	std::string postfix = ".db";

	std::string res = prefix + ruota_str + postfix;

    file_db /= boost::filesystem::path(res);

    return file_db;
}

boost::filesystem::path rdbms::get_name_db_freq_ambi_from_ruota(ruota_t ruota)
{
    boost::filesystem::path file_db(boost::filesystem::current_path());

    std::string ruota_str = convert_ruota_to_string(ruota);
	std::string prefix = "db_freq_ambi_";
	std::string postfix = ".db";

	std::string res = prefix + ruota_str + postfix;

    file_db /= boost::filesystem::path(res);

    return file_db;
}

boost::filesystem::path rdbms::get_name_db_ex_terni_from_ruota(ruota_t ruota)
{
    boost::filesystem::path file_db(boost::filesystem::current_path());

    std::string ruota_str = convert_ruota_to_string(ruota);
	std::string prefix = "db_ex_terni_";
	std::string postfix = ".db";

	std::string res = prefix + ruota_str + postfix;

    file_db /= boost::filesystem::path(res);

    return file_db;
}

boost::filesystem::path rdbms::get_name_db_freq_terni_from_ruota(ruota_t ruota)
{
    boost::filesystem::path file_db(boost::filesystem::current_path());

    std::string ruota_str = convert_ruota_to_string(ruota);
	std::string prefix = "db_freq_terni_";
	std::string postfix = ".db";

	std::string res = prefix + ruota_str + postfix;

    file_db /= boost::filesystem::path(res);

    return file_db;
}

void rdbms::init_db_maps(void)
{
	leveldb::DB* db = nullptr;

	rdbms::db_ex_ambi[ruota_t::NAZIONALE] = db;
	rdbms::db_ex_ambi[ruota_t::BARI] = db;
	rdbms::db_ex_ambi[ruota_t::CAGLIARI] = db;
	rdbms::db_ex_ambi[ruota_t::FIRENZE] = db;
	rdbms::db_ex_ambi[ruota_t::GENOVA] = db;
	rdbms::db_ex_ambi[ruota_t::MILANO] = db;
	rdbms::db_ex_ambi[ruota_t::NAPOLI] = db;
	rdbms::db_ex_ambi[ruota_t::PALERMO] = db;
	rdbms::db_ex_ambi[ruota_t::ROMA] = db;
	rdbms::db_ex_ambi[ruota_t::TORINO] = db;
	rdbms::db_ex_ambi[ruota_t::VENEZIA] = db;
	rdbms::db_ex_ambi[ruota_t::TUTTE] = db;

	rdbms::db_freq_ambi[ruota_t::NAZIONALE] = db;
	rdbms::db_freq_ambi[ruota_t::BARI] = db;
	rdbms::db_freq_ambi[ruota_t::CAGLIARI] = db;
	rdbms::db_freq_ambi[ruota_t::FIRENZE] = db;
	rdbms::db_freq_ambi[ruota_t::GENOVA] = db;
	rdbms::db_freq_ambi[ruota_t::MILANO] = db;
	rdbms::db_freq_ambi[ruota_t::NAPOLI] = db;
	rdbms::db_freq_ambi[ruota_t::PALERMO] = db;
	rdbms::db_freq_ambi[ruota_t::ROMA] = db;
	rdbms::db_freq_ambi[ruota_t::TORINO] = db;
	rdbms::db_freq_ambi[ruota_t::VENEZIA] = db;
	rdbms::db_freq_ambi[ruota_t::TUTTE] = db;

	rdbms::db_ex_terni[ruota_t::NAZIONALE] = db;
	rdbms::db_ex_terni[ruota_t::BARI] = db;
	rdbms::db_ex_terni[ruota_t::CAGLIARI] = db;
	rdbms::db_ex_terni[ruota_t::FIRENZE] = db;
	rdbms::db_ex_terni[ruota_t::GENOVA] = db;
	rdbms::db_ex_terni[ruota_t::MILANO] = db;
	rdbms::db_ex_terni[ruota_t::NAPOLI] = db;
	rdbms::db_ex_terni[ruota_t::PALERMO] = db;
	rdbms::db_ex_terni[ruota_t::ROMA] = db;
	rdbms::db_ex_terni[ruota_t::TORINO] = db;
	rdbms::db_ex_terni[ruota_t::VENEZIA] = db;
	rdbms::db_ex_terni[ruota_t::TUTTE] = db;

	rdbms::db_freq_terni[ruota_t::NAZIONALE] = db;
	rdbms::db_freq_terni[ruota_t::BARI] = db;
	rdbms::db_freq_terni[ruota_t::CAGLIARI] = db;
	rdbms::db_freq_terni[ruota_t::FIRENZE] = db;
	rdbms::db_freq_terni[ruota_t::GENOVA] = db;
	rdbms::db_freq_terni[ruota_t::MILANO] = db;
	rdbms::db_freq_terni[ruota_t::NAPOLI] = db;
	rdbms::db_freq_terni[ruota_t::PALERMO] = db;
	rdbms::db_freq_terni[ruota_t::ROMA] = db;
	rdbms::db_freq_terni[ruota_t::TORINO] = db;
	rdbms::db_freq_terni[ruota_t::VENEZIA] = db;
	rdbms::db_freq_terni[ruota_t::TUTTE] = db;
}

void rdbms::init_db_remove_all(void)
{
	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		boost::filesystem::path p = rdbms::get_name_db_ex_ambi_from_ruota(mapdb.first);
		boost::filesystem::remove_all(p);
		boost::filesystem::remove(p);
	}
	for(auto& mapdb : rdbms::db_freq_ambi)
	{
		boost::filesystem::path p = rdbms::get_name_db_freq_ambi_from_ruota(mapdb.first);
		boost::filesystem::remove_all(p);
		boost::filesystem::remove(p);
	}
	for(auto& mapdb : rdbms::db_ex_terni)
	{
		boost::filesystem::path p = rdbms::get_name_db_ex_terni_from_ruota(mapdb.first);
		boost::filesystem::remove_all(p);
		boost::filesystem::remove(p);
	}
	for(auto& mapdb : rdbms::db_freq_terni)
	{
		boost::filesystem::path p = rdbms::get_name_db_freq_terni_from_ruota(mapdb.first);
		boost::filesystem::remove_all(p);
		boost::filesystem::remove(p);
	}
}

void rdbms::init_db_open_create_if_missing_all(void)
{
	leveldb::Options options;
	options.create_if_missing = true;
	options.compression = leveldb::kNoCompression;
	options.max_open_files = 32;

	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		boost::filesystem::path p = rdbms::get_name_db_ex_ambi_from_ruota(mapdb.first);
		leveldb::Status status = leveldb::DB::Open(options, std::string(p.c_str()), &(mapdb.second));
		if(!status.ok())
		{
			std::fprintf(stderr,"Error: could not create db %s.\n",p.c_str());
			std::fprintf(stderr,"Error: %s.\n",status.ToString().c_str());
		}
	}
	for(auto& mapdb : rdbms::db_freq_ambi)
	{
		boost::filesystem::path p = rdbms::get_name_db_freq_ambi_from_ruota(mapdb.first);
		leveldb::Status status = leveldb::DB::Open(options, std::string(p.c_str()), &(mapdb.second));
		if(!status.ok())
		{
			std::fprintf(stderr,"Error: could not create db %s.\n",p.c_str());
			std::fprintf(stderr,"Error: %s.\n",status.ToString().c_str());
		}
	}
	for(auto& mapdb : rdbms::db_ex_terni)
	{
		boost::filesystem::path p = rdbms::get_name_db_ex_terni_from_ruota(mapdb.first);
		leveldb::Status status = leveldb::DB::Open(options, std::string(p.c_str()), &(mapdb.second));
		if(!status.ok())
		{
			std::fprintf(stderr,"Error: could not create db %s.\n",p.c_str());
			std::fprintf(stderr,"Error: %s.\n",status.ToString().c_str());
		}
	}
	for(auto& mapdb : rdbms::db_freq_terni)
	{
		boost::filesystem::path p = rdbms::get_name_db_freq_terni_from_ruota(mapdb.first);
		leveldb::Status status = leveldb::DB::Open(options, std::string(p.c_str()), &(mapdb.second));
		if(!status.ok())
		{
			std::fprintf(stderr,"Error: could not create db %s.\n",p.c_str());
			std::fprintf(stderr,"Error: %s.\n",status.ToString().c_str());
		}
	}
}

void rdbms::close_dbs(void)
{
	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		delete mapdb.second;
	}
	for(auto& mapdb : rdbms::db_freq_ambi)
	{
		delete mapdb.second;
	}
	for(auto& mapdb : rdbms::db_ex_terni)
	{
		delete mapdb.second;
	}
	for(auto& mapdb : rdbms::db_freq_terni)
	{
		delete mapdb.second;
	}
}

void rdbms::init_db_push_default_entries(void)
{
	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		for(const auto& key : combinations::combinations_2_of_90 )
		{
			const uint32_t freq = 0;
			std::vector<date_t> date;
			date.clear();
			int32_t rv = rdbms::put_ambo_freq_date(mapdb.second, key, freq, date);
			if( rv )
			{
				std::fprintf(stderr,"Error: init_db_push_default_entries.\n");
			}
		}
	}

	for(auto& mapdb : rdbms::db_ex_terni)
	{
		for(const auto& key : combinations::combinations_3_of_90 )
		{
			const uint32_t freq = 0;
			std::vector<date_t> date;
			date.clear();
			int32_t rv = rdbms::put_terno_freq_date(mapdb.second, key, freq, date);
			if( rv )
			{
				std::fprintf(stderr,"Error: init_db_push_default_entries.\n");
			}
		}
	}
}

int32_t rdbms::put_ambo_freq_date(leveldb::DB* db, const key2 key, const uint32_t freq, std::vector<date_t>& date)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: put_ambo_freq_date error: db is null.\n");
		return (-1);
	}

	sort_dates(date);

	leveldb::WriteOptions options;

	std::vector<uint32_t> slice_put;
	slice_put.push_back(freq);
	for( const auto& d : date )
	{
		slice_put.push_back(d.raw);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());
	const leveldb::Slice slice_in((const char*) slice_put.data(), slice_put.size() * sizeof(uint32_t));

    leveldb::Status status  = db->Put(options, slice_key, slice_in);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: put_ambo_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key2 key = [%02u,%02u].\n",key[0],key[1]);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::get_ambo_freq_date(leveldb::DB* db, const key2 key, uint32_t& freq, std::vector<date_t>& date)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: get_ambo_freq_date error: db is null.\n");
		return (-1);
	}

	leveldb::ReadOptions options;
	options.verify_checksums = true;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());
	std::string slice_out;
	slice_out.clear();

    leveldb::Status status  = db->Get(options, slice_key, &slice_out);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: get_ambo_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key2 key = [%02u,%02u].\n",key[0],key[1]);
    	return (-1);
    }

    if( 0 != slice_out.size() && slice_out.size()%sizeof(uint32_t) !=  0 )
    {
		std::fprintf(stderr,"Error: get_ambo_freq_date size error: returned size is %lu.\n",slice_out.size());
    	return (-1);
    }

    bool is_first = true;
    for( size_t i = 0; i < slice_out.size(); i += sizeof(uint32_t) )
    {
    	uint32_t dato = 0;
    	std::memcpy((void*) &dato, (void*) &(slice_out[i]), sizeof(uint32_t));
    	if( is_first )
    	{
    		freq = dato;
    		is_first = false;
    	}
    	else
    	{
    		date_t data;
    		data.raw = dato;
    		date.push_back(data);
    	}
    }

    return (0);
}

int32_t rdbms::del_ambo_freq_date(leveldb::DB* db, const key2 key)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: del_ambo_freq_date error: db is null.\n");
		return (-1);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());

	leveldb::WriteOptions options;
    leveldb::Status status  = db->Delete(options, slice_key);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: del_ambo_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key2 key = [%02u,%02u].\n",key[0],key[1]);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::put_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: put_freq_ambo_date error: db is null.\n");
		return (-1);
	}

	sort_extractions(ex);

	leveldb::WriteOptions options;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));
	const leveldb::Slice slice_in((const char*) ex.data(), ex.size() * sizeof(uint64_t));

    leveldb::Status status  = db->Put(options, slice_key, slice_in);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: put_freq_ambo_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::get_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: get_freq_ambo_date error: db is null.\n");
		return (-1);
	}

	leveldb::ReadOptions options;
	options.verify_checksums = true;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));
	std::string slice_out;
	slice_out.clear();

    leveldb::Status status  = db->Get(options, slice_key, &slice_out);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: get_freq_ambo_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
    	return (-1);
    }

    if( 0 != slice_out.size() && slice_out.size()%sizeof(uint64_t) !=  0 )
    {
		std::fprintf(stderr,"Error: get_freq_ambo_date size error: returned size is %lu.\n",slice_out.size());
    	return (-1);
    }

    for( size_t i = 0; i < slice_out.size(); i += sizeof(uint64_t) )
    {
    	uint64_t dato = 0;
    	std::memcpy((void*) &dato, (void*) &(slice_out[i]), sizeof(uint64_t));
    	extraction_t data;
		data.raw = dato;
		ex.push_back(data);
    }

    return (0);
}

int32_t rdbms::del_freq_ambo_date(leveldb::DB* db, const uint32_t key_freq)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: del_freq_ambo_date error: db is null.\n");
		return (-1);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));

	leveldb::WriteOptions options;
    leveldb::Status status  = db->Delete(options, slice_key);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: del_freq_ambo_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::put_terno_freq_date(leveldb::DB* db, const key3 key, const uint32_t freq, std::vector<date_t>& date)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: put_terno_freq_date error: db is null.\n");
		return (-1);
	}

	sort_dates(date);

	leveldb::WriteOptions options;

	std::vector<uint32_t> slice_put;
	slice_put.push_back(freq);
	for( const auto& d : date )
	{
		slice_put.push_back(d.raw);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());
	const leveldb::Slice slice_in((const char*) slice_put.data(), slice_put.size() * sizeof(uint32_t));

    leveldb::Status status  = db->Put(options, slice_key, slice_in);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: put_terno_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key3 key = [%02u,%02u,%02u].\n",key[0],key[1],key[2]);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::get_terno_freq_date(leveldb::DB* db, const key3 key, uint32_t& freq, std::vector<date_t>& date)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: get_terno_freq_date error: db is null.\n");
		return (-1);
	}

	leveldb::ReadOptions options;
	options.verify_checksums = true;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());
	std::string slice_out;
	slice_out.clear();

    leveldb::Status status  = db->Get(options, slice_key, &slice_out);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: get_terno_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key3 key = [%02u,%02u,%02u].\n",key[0],key[1],key[2]);
    	return (-1);
    }

    if( 0 != slice_out.size() && slice_out.size()%sizeof(uint32_t) !=  0 )
    {
		std::fprintf(stderr,"Error: get_terno_freq_date size error: returned size is %lu.\n",slice_out.size());
    	return (-1);
    }

    bool is_first = true;
    for( size_t i = 0; i < slice_out.size(); i += sizeof(uint32_t) )
    {
    	uint32_t dato = 0;
    	std::memcpy((void*) &dato, (void*) &(slice_out[i]), sizeof(uint32_t));
    	if( is_first )
    	{
    		freq = dato;
    		is_first = false;
    	}
    	else
    	{
    		date_t data;
    		data.raw = dato;
    		date.push_back(data);
    	}
    }

    return (0);
}

int32_t rdbms::del_terno_freq_date(leveldb::DB* db, const key3 key)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: del_terno_freq_date error: db is null.\n");
		return (-1);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) key.data(), key.size());

	leveldb::WriteOptions options;
    leveldb::Status status  = db->Delete(options, slice_key);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: del_terno_freq_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key3 key = [%02u,%02u,%02u].\n",key[0],key[1],key[2]);
    	return (-1);
    }

    return (0);
}


int32_t rdbms::put_freq_terno_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: put_freq_terno_date error: db is null.\n");
		return (-1);
	}

	sort_extractions(ex);

	leveldb::WriteOptions options;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));
	const leveldb::Slice slice_in((const char*) ex.data(), ex.size() * sizeof(uint64_t));

    leveldb::Status status  = db->Put(options, slice_key, slice_in);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: put_freq_terno_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
    	return (-1);
    }

    return (0);
}

int32_t rdbms::get_freq_terno_date(leveldb::DB* db, const uint32_t key_freq, std::vector<extraction_t>& ex)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: get_freq_terno_date error: db is null.\n");
		return (-1);
	}

	leveldb::ReadOptions options;
	options.verify_checksums = true;

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));
	std::string slice_out;
	slice_out.clear();

    leveldb::Status status  = db->Get(options, slice_key, &slice_out);

    if( !status.ok() )
    {
		std::fprintf(stderr,"Error: get_freq_terno_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
    	return (-1);
    }

    if( 0 != slice_out.size() && slice_out.size()%sizeof(uint64_t) !=  0 )
    {
		std::fprintf(stderr,"Error: get_freq_terno_date size error: returned size is %lu.\n",slice_out.size());
    	return (-1);
    }

    for( size_t i = 0; i < slice_out.size(); i += sizeof(uint64_t) )
    {
    	uint64_t dato = 0;
    	std::memcpy((void*) &dato, (void*) &(slice_out[i]), sizeof(uint64_t));
    	extraction_t data;
		data.raw = dato;
		ex.push_back(data);
    }

    return (0);
}

int32_t rdbms::del_freq_terno_date(leveldb::DB* db, const uint32_t key_freq)
{
	if( nullptr == db )
	{
		std::fprintf(stderr,"Error: del_freq_terno_date error: db is null.\n");
		return (-1);
	}

	// Slice(const char* d, size_t n)
	const leveldb::Slice slice_key((const char*) &key_freq, sizeof(uint32_t));

	leveldb::WriteOptions options;
	leveldb::Status status  = db->Delete(options, slice_key);

	if( !status.ok() )
	{
		std::fprintf(stderr,"Error: del_freq_terno_date error: %s.\n",status.ToString().c_str());
		std::fprintf(stderr,"Error: key_freq = [%u].\n",key_freq);
		return (-1);
	}

	return (0);
}

} // namespace lotto_extract

