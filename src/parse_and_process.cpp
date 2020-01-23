/*
 * parse_and_process.cpp
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
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include "basic_types.h"
#include "utilities.h"
using namespace lotto_base;

#include "history_extraction.h"
#include "rdbms.h"
#include "combinations.h"

#include "parse_and_process.h"

namespace lotto_extract {

static int32_t compute_all_ambi_per_ruota(void);
static int32_t compute_all_terni_per_ruota(void);
static int32_t compute_all_ambi_freq_per_ruota(void);
static int32_t compute_all_terni_freq_per_ruota(void);

static void spool_freq_ambi_first_number(uint32_t number_to_print);
static void spool_freq_terni_first_number(uint32_t number_to_print);
static void spool_delay_ambi_first_number(uint32_t number_to_print);
static void spool_delay_terni_first_number(uint32_t number_to_print);

int32_t parse_process_compute(std::vector<std::string> in_args, int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	if( 4 != in_args.size() )
	{
		return (-1);
	}

	rdbms::init_db_maps();
	rdbms::init_db_open_create_if_missing_all();

	try
	{
		if( in_args[2] == std::string("frequency") )
		{
			uint32_t number_to_print = (uint32_t) std::atoi(in_args[3].c_str());
			spool_freq_ambi_first_number(number_to_print);
			spool_freq_terni_first_number(number_to_print);
		}
		else if( in_args[2] == std::string("delay") )
		{
			uint32_t number_to_print = (uint32_t) std::atoi(in_args[3].c_str());
			spool_delay_ambi_first_number(number_to_print);
			spool_delay_terni_first_number(number_to_print);
		}
		else
		{
			std::fprintf(stderr,"Error: unknown command.\n");
			return (-1);
		}
	}
	catch(const std::exception& excp)
	{
		std::fprintf(stderr,"Error: exception:\n");
		std::fprintf(stderr,"\te.what = %s\n",excp.what());
	}
	catch(...)
	{
		std::fprintf(stderr,"Error: unknown exception.\n");
	}

	rdbms::close_dbs();

	return (0);
}

int32_t parse_process_restart(std::vector<std::string> in_args, int argc, char *argv[])
{
	(void) in_args;
	(void) argc;
	(void) argv;

	rdbms::init_db_maps();
	rdbms::init_db_remove_all();
	rdbms::init_db_open_create_if_missing_all();
	rdbms::init_db_push_default_entries();

	// compute...
	int32_t rv = 0;

	try
	{
		rv = compute_all_ambi_per_ruota();
		if( rv )
		{
			std::fprintf(stderr,"Error: abort.\n");
			rdbms::close_dbs();
			return (rv);
		}

		rv = compute_all_terni_per_ruota();
		if( rv )
		{
			std::fprintf(stderr,"Error: abort.\n");
			rdbms::close_dbs();
			return (rv);
		}

		rv = compute_all_ambi_freq_per_ruota();
		if( rv )
		{
			std::fprintf(stderr,"Error: abort.\n");
			rdbms::close_dbs();
			return (rv);
		}

		rv = compute_all_terni_freq_per_ruota();
		if( rv )
		{
			std::fprintf(stderr,"Error: abort.\n");
			rdbms::close_dbs();
			return (rv);
		}
	}
	catch(const std::exception& excp)
	{
		std::fprintf(stderr,"Error: exception:\n");
		std::fprintf(stderr,"\te.what = %s\n",excp.what());
	}
	catch(...)
	{
		std::fprintf(stderr,"Error: unknown exception.\n");
	}

	rdbms::close_dbs();

	return (rv);
}

int32_t parse_process_update(std::vector<std::string> in_args, int argc, char *argv[])
{
	(void) in_args;
	(void) argc;
	(void) argv;

	return -1;
}

static void debug_ex_struct(void);
static int32_t validate_extraction(extraction_t ex);

int32_t parse_process_init(void)
{
	debug_ex_struct();

	extractions::init_extractions();

    // check valid path
    boost::filesystem::path file_db(boost::filesystem::current_path());
    file_db /= boost::filesystem::path(std::string("estrazioni.db"));

    if( !(boost::filesystem::exists(file_db) && boost::filesystem::is_regular_file(file_db)) )
    {
		std::fprintf(stderr,"Error: file %s does not exist or is not a regular file.\n",file_db.c_str());
		return (-1);
    }

    // open the file
	std::FILE *read_ptr = std::fopen(file_db.c_str(),"rb");
	size_t current_ex_kounter = 0;

	if( NULL != read_ptr )
	{
    	uint64_t current_ex = 0;
		uint8_t value_64bit[8] = { 0 };
		do
    	{
    		current_ex = 0;

    		std::memset(value_64bit, 0, sizeof(value_64bit));
    		size_t ret = std::fread(value_64bit,sizeof(value_64bit),1,read_ptr);
    		if( ret < 1 )
    		{
				break;
    		}

    		current_ex  = (uint64_t) ( (((uint64_t) value_64bit[0]) << 56) | \
    				                   (((uint64_t) value_64bit[1]) << 48) | \
    						           (((uint64_t) value_64bit[2]) << 40) | \
    								   (((uint64_t) value_64bit[3]) << 32) | \
    								   (((uint64_t) value_64bit[4]) << 24) | \
    								   (((uint64_t) value_64bit[5]) << 16) | \
    								   (((uint64_t) value_64bit[6]) <<  8) | \
    								   (((uint64_t) value_64bit[7]) <<  0) );

    		current_ex_kounter++;

    		extraction_t ex;
    		ex.raw = current_ex;

    		if( validate_extraction(ex) )
    		{
    			std::fprintf(stderr,"Error: at position 0x%08lX.\n",(current_ex_kounter-1));
    			continue;
    		}

    		extractions::extraction_vec.push_back(ex);

    		extractions::extractions_per_ruota[((ruota_t) ex.bits.ruota)].push_back(ex);
    		if( ruota_t::NAZIONALE != ((ruota_t) ex.bits.ruota) )
    		{
    			extractions::extractions_per_ruota[ruota_t::TUTTE].push_back(ex);
    		}

    	} while(true);
    	std::fclose(read_ptr);
	}
	else
	{
		std::fprintf(stderr,"Error: could not open file %s.\n",file_db.c_str());
		return (-1);
	}
	std::fprintf(stderr,"Info: read %lu entries from file %s.\n",current_ex_kounter,file_db.c_str());

	if(combinations::init_combinations())
		return (-1);

	system_pause();

	return 0;
}

static void debug_ex_struct(void)
{
	extraction_t ex;

	ex.raw = 0;
	ex.bits.year = 0xFFFF;

	if( ex.bits.year == (ex.raw & 0xFFFF) >> 0 )
	{
		std::fprintf(stderr,"Info: ex.bits.year == (ex.raw & 0xFFFF) >> 0.\n");
	}
	if( ex.bits.year == (ex.raw & 0xFFFF000000000000) >> 48 )
	{
		std::fprintf(stderr,"Info: ex.bits.year == (ex.raw & 0xFFFF000000000000) >> 48.\n");
	}

	ex.raw = 0;
	ex.bits.month = 0xF;

	if( ex.bits.month == (ex.raw & 0xF0000) >> 16 )
	{
		std::fprintf(stderr,"Info: ex.bits.month == (ex.raw & 0xF0000) >> 16.\n");
	}
	if( ex.bits.month == (ex.raw & 0x0000F00000000000) >> 44 )
	{
		std::fprintf(stderr,"Info: ex.bits.month == (ex.raw & 0x0000F00000000000) >> 44.\n");
	}
}

static int32_t validate_extraction(extraction_t ex)
{
	int32_t ret = 0;

	if( ex.bits.year < 1871 )
	{
		std::fprintf(stderr,"Error: invalid extraction for year, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.month < 1 || ex.bits.month > 12 )
	{
		std::fprintf(stderr,"Error: invalid extraction for month, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.day < 1 || ex.bits.day > 31 )
	{
		std::fprintf(stderr,"Error: invalid extraction for day, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.a < 1 || ex.bits.a > 90 )
	{
		std::fprintf(stderr,"Error: invalid extraction for a, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.b < 1 || ex.bits.b > 90 )
	{
		std::fprintf(stderr,"Error: invalid extraction for b, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.c < 1 || ex.bits.c > 90 )
	{
		std::fprintf(stderr,"Error: invalid extraction for c, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.d < 1 || ex.bits.d > 90 )
	{
		std::fprintf(stderr,"Error: invalid extraction for d, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.e < 1 || ex.bits.e > 90 )
	{
		std::fprintf(stderr,"Error: invalid extraction for e, see what follows: \n");
		ret = -1;
	}
	if( ex.bits.ruota < ((uint64_t) ruota_t::NAZIONALE) || ex.bits.ruota >= ((uint64_t) ruota_t::UNKNOWN) )
	{
		std::fprintf(stderr,"Error: invalid extraction for ruota, see what follows: \n");
		ret = -1;
	}

	std::vector<uint8_t> extracted;
	extracted.push_back(((uint8_t) ex.bits.a));
	extracted.push_back(((uint8_t) ex.bits.b));
	extracted.push_back(((uint8_t) ex.bits.c));
	extracted.push_back(((uint8_t) ex.bits.d));
	extracted.push_back(((uint8_t) ex.bits.e));
	std::sort(extracted.begin(),extracted.end());

	if(    extracted[0] >= extracted[1] \
		|| extracted[1] >= extracted[2] \
		|| extracted[2] >= extracted[3] \
		|| extracted[3] >= extracted[4] \
		)
	{
		std::fprintf(stderr,"Error: invalid extraction numbers repeated, see what follows: \n");
		ret = -1;
	}

	if(ret)
	{
		std::fprintf(stderr,"Error Found:\n");
		std::fprintf(stderr,"   e.bits.year: %d\n",(uint16_t) ex.bits.year);
		std::fprintf(stderr,"   e.bits.month: %d\n",(uint8_t) ex.bits.month);
		std::fprintf(stderr,"   e.bits.day: %d\n",(uint8_t) ex.bits.day);
		std::fprintf(stderr,"   e.bits.a: %d\n",(uint8_t) ex.bits.a);
		std::fprintf(stderr,"   e.bits.b: %d\n",(uint8_t) ex.bits.b);
		std::fprintf(stderr,"   e.bits.c: %d\n",(uint8_t) ex.bits.c);
		std::fprintf(stderr,"   e.bits.d: %d\n",(uint8_t) ex.bits.d);
		std::fprintf(stderr,"   e.bits.e: %d\n",(uint8_t) ex.bits.e);
		std::fprintf(stderr,"   e.bits.ruota: %d\n",(uint8_t) ex.bits.ruota);
	}

	return (ret);
}

static int32_t compute_all_ambi_per_ruota(void)
{
	size_t prev_kounter = 0; size_t kounter = 0;

	std::fprintf(stderr,"START compute_all_ambi_per_ruota ...\n");

	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		std::vector<extraction_t>& local_extraction_vec = extractions::extractions_per_ruota[mapdb.first];

		for( const auto& ex : local_extraction_vec )
		{
			std::vector<key2> local_extraction_vec_ambi_keys = combinations::get_ambi_combinations(ex);
			for( const auto& key : local_extraction_vec_ambi_keys )
			{
				std::vector<date_t> date;
				uint32_t freq;
				if( rdbms::get_ambo_freq_date(mapdb.second, key, freq, date) )
				{
					std::fprintf(stderr,"Error: compute_all_ambi_per_ruota get_ambo_freq_date error get.\n");
					system_pause();
					continue;
				}
				if( rdbms::del_ambo_freq_date(mapdb.second, key) )
				{
					std::fprintf(stderr,"Error: compute_all_ambi_per_ruota get_ambo_freq_date error delete.\n");
					system_pause();
				}
				date_t new_date;
				new_date.bits.year = ex.bits.year;
				new_date.bits.month = ex.bits.month;
				new_date.bits.day = ex.bits.day;
				date.push_back(new_date);
				freq++;
				if( rdbms::put_ambo_freq_date(mapdb.second, key, freq, date) )
				{
					std::fprintf(stderr,"Error: compute_all_ambi_per_ruota put_ambo_freq_date error put.\n");
					system_pause();
					continue;
				}
				kounter++;
				if( kounter >= prev_kounter+100000 )
				{
					prev_kounter = kounter;
					std::fprintf(stderr,"Info: compute_all_ambi_per_ruota kounter = %lu.\n",kounter);
				}
			}
		}
	}

	std::fprintf(stderr,"END compute_all_ambi_per_ruota! Success!.\n");

	return (0);
}

static int32_t compute_all_terni_per_ruota(void)
{
	size_t prev_kounter = 0; size_t kounter = 0;

	std::fprintf(stderr,"START compute_all_terni_per_ruota ...\n");

	for(auto& mapdb : rdbms::db_ex_terni)
	{
		std::vector<extraction_t>& local_extraction_vec = extractions::extractions_per_ruota[mapdb.first];

		for( const auto& ex : local_extraction_vec )
		{
			std::vector<key3> local_extraction_vec_ambi_keys = combinations::get_terni_combinations(ex);
			for( const auto& key : local_extraction_vec_ambi_keys )
			{
				std::vector<date_t> date;
				uint32_t freq;
				if( rdbms::get_terno_freq_date(mapdb.second, key, freq, date) )
				{
					std::fprintf(stderr,"Error: compute_all_terni_per_ruota get_terno_freq_date error get.\n");
					system_pause();
					continue;
				}
				if( rdbms::del_terno_freq_date(mapdb.second, key) )
				{
					std::fprintf(stderr,"Error: compute_all_terni_per_ruota get_ambo_freq_date error delete.\n");
					system_pause();
				}
				date_t new_date;
				new_date.bits.year = ex.bits.year;
				new_date.bits.month = ex.bits.month;
				new_date.bits.day = ex.bits.day;
				date.push_back(new_date);
				freq++;
				if( rdbms::put_terno_freq_date(mapdb.second, key, freq, date) )
				{
					std::fprintf(stderr,"Error: compute_all_terni_per_ruota put_terno_freq_date error put.\n");
					system_pause();
					continue;
				}
				kounter++;
				if( kounter >= prev_kounter+100000 )
				{
					prev_kounter = kounter;
					std::fprintf(stderr,"Info: compute_all_terni_per_ruota kounter = %lu.\n",kounter);
				}
			}
		}
	}

	std::fprintf(stderr,"END compute_all_terni_per_ruota! Success!\n");

	return (0);
}

static int32_t compute_all_ambi_freq_per_ruota(void)
{
	size_t prev_kounter = 0; size_t kounter = 0;
	leveldb::ReadOptions options;
	options.verify_checksums = true;

	std::fprintf(stderr,"START compute_all_ambi_freq_per_ruota...\n");

	for(auto& mapdb : rdbms::db_ex_ambi)
	{
		std::fprintf(stderr,"Info: compute_all_ambi_freq_per_ruota db %s.\n", \
				             rdbms::get_name_db_ex_ambi_from_ruota(mapdb.first).c_str());

		leveldb::Iterator* it = mapdb.second->NewIterator(options);
		for(it->SeekToFirst(); it->Valid(); it->Next())
		{
			std::vector<date_t> dates;
			dates.clear();
			dates.shrink_to_fit();
			uint32_t freq = 0;

			key2 ambo;
			ambo[0] = (uint8_t) (it->key().data())[0];
			ambo[1] = (uint8_t) (it->key().data())[1];

			if( it->value().size() < sizeof(uint32_t) )
			{
				std::fprintf(stderr,"Error: compute_all_ambi_freq_per_ruota, it->value().size() = %lu.\n",it->value().size());
				continue;
			}
			// std::fprintf(stderr,"Info: compute_all_ambi_freq_per_ruota, it->value().size() = 0X%016lX.\n",it->value().size());

			bool is_first = true;
			for(size_t i = 0; i < it->value().size(); i += sizeof(uint32_t))
			{
				uint32_t dato;
				std::memcpy((void*) &dato, ((uint8_t*) it->value().data()) + i, sizeof(uint32_t));
				if(is_first)
				{
					freq = dato;
					is_first = false;
				}
				else
				{
					date_t date;
					date.raw = dato;
					dates.push_back(date);
				}
			}
			if( 0 == dates.size() )
			{
				date_t date;
				date.raw = 0;
				dates.push_back(date);
			}
			// std::fprintf(stderr,"Info: compute_all_ambi_freq_per_ruota, freq = %u.\n",freq);

			dates.shrink_to_fit();
			std::vector<extraction_t> ex_vec;
			ex_vec.clear();
			ex_vec.shrink_to_fit();
			for(const auto& d : dates)
			{
				extraction_t ex;
				ex.raw = 0;
				ex.bits.a = (uint8_t) ambo[0];
				ex.bits.b = (uint8_t) ambo[1];
				ex.bits.year = d.bits.year;
				ex.bits.month = d.bits.month;
				ex.bits.day = d.bits.day;
				ex.bits.ruota = (uint8_t) mapdb.first;
				ex_vec.push_back(ex);
			}
			ex_vec.shrink_to_fit();

			leveldb::DB* db_target = rdbms::db_freq_ambi[mapdb.first];
			if( nullptr != db_target )
			{
				std::vector<extraction_t> ex_vec_out;
				ex_vec_out.clear();
				ex_vec_out.shrink_to_fit();
				if( rdbms::get_freq_ambo_date(db_target, freq, ex_vec_out) )
				{
					std::fprintf(stderr,"Info: error from get_freq_ambo_date.\n");
				}
				else
				{
					ex_vec_out.shrink_to_fit();
					rdbms::del_freq_ambo_date(db_target, freq);
				}
				for( const auto& ee : ex_vec )
				{
					ex_vec_out.push_back(ee);
				}
				ex_vec_out.shrink_to_fit();
				if( rdbms::put_freq_ambo_date(db_target, freq, ex_vec_out) )
				{
					std::fprintf(stderr,"Info: error from put_freq_ambo_date.\n");
				}
				kounter++;
				if( kounter >= prev_kounter+10000 )
				{
					prev_kounter = kounter;
					std::fprintf(stderr,"Info: compute_all_ambi_freq_per_ruota kounter = %lu.\n",kounter);
				}
			}
		}
		// Check for any errors found during the scan
		if( !it->status().ok() )
		{
			std::fprintf(stderr,"Info: compute_all_ambi_freq_per_ruota, errors from it scan = %s.\n",it->status().ToString().c_str());
		}
		delete it;
	}

	std::fprintf(stderr,"END compute_all_ambi_freq_per_ruota! Success!\n");

	return (0);
}

static int32_t compute_all_terni_freq_per_ruota(void)
{
	size_t prev_kounter = 0; size_t kounter = 0;
	leveldb::ReadOptions options;
	options.verify_checksums = true;

	std::fprintf(stderr,"START compute_all_terni_freq_per_ruota...\n");

	for(auto& mapdb : rdbms::db_ex_terni)
	{
		std::fprintf(stderr,"Info: compute_all_terni_freq_per_ruota db %s.\n", \
				             rdbms::get_name_db_ex_terni_from_ruota(mapdb.first).c_str());

		leveldb::Iterator* it = mapdb.second->NewIterator(options);
		for (it->SeekToFirst(); it->Valid(); it->Next())
		{
			std::vector<date_t> dates;
			uint32_t freq = 0;

			key3 terno;
			terno[0] = (uint8_t) (it->key().data())[0];
			terno[1] = (uint8_t) (it->key().data())[1];
			terno[2] = (uint8_t) (it->key().data())[2];

			if( it->value().size() < sizeof(uint32_t) )
			{
				std::fprintf(stderr,"Error: compute_all_terni_freq_per_ruota, it->value().size() = %lu.\n",it->value().size());
				continue;
			}

			bool is_first = true;
			for(size_t i = 0; i < it->value().size(); i += sizeof(uint32_t))
			{
				uint32_t dato;
				std::memcpy((void*) &dato, ((uint8_t*) it->value().data()) + i, sizeof(uint32_t));
				if(is_first)
				{
					freq = dato;
					is_first = false;
				}
				else
				{
					date_t date;
					date.raw = dato;
					dates.push_back(date);
				}
			}
			if( 0 == dates.size() )
			{
				date_t date;
				date.raw = 0;
				dates.push_back(date);
			}
			std::vector<extraction_t> ex_vec;
			for(const auto& d : dates)
			{
				extraction_t ex;
				ex.raw = 0;
				ex.bits.a = (uint8_t) terno[0];
				ex.bits.b = (uint8_t) terno[1];
				ex.bits.c = (uint8_t) terno[2];
				ex.bits.year = d.bits.year;
				ex.bits.month = d.bits.month;
				ex.bits.day = d.bits.day;
				ex.bits.ruota = (uint8_t) mapdb.first;
				ex_vec.push_back(ex);
			}

			leveldb::DB* db_target = rdbms::db_freq_terni[mapdb.first];
			if( nullptr != db_target )
			{
				std::vector<extraction_t> ex_vec_out;
				if( rdbms::get_freq_terno_date(db_target, freq, ex_vec_out) )
				{
					std::fprintf(stderr,"Info: error from get_freq_terno_date.\n");
				}
				else
				{
					rdbms::del_freq_terno_date(db_target, freq);
				}
				for( const auto& ee : ex_vec )
				{
					ex_vec_out.push_back(ee);
				}
				if( rdbms::put_freq_terno_date(db_target, freq, ex_vec_out) )
				{
					std::fprintf(stderr,"Info: error from put_freq_terno_date.\n");
				}
				kounter++;
				if( kounter >= prev_kounter+10000 )
				{
					prev_kounter = kounter;
					std::fprintf(stderr,"Info: compute_all_terni_freq_per_ruota kounter = %lu.\n",kounter);
				}
			}
		}
		// Check for any errors found during the scan
		if( !it->status().ok() )
		{
			std::fprintf(stderr,"Info: compute_all_terni_freq_per_ruota, errors from it scan = %s.\n",it->status().ToString().c_str());
		}
		delete it;
	}

	std::fprintf(stderr,"END compute_all_terni_freq_per_ruota! Success!.\n");

	return (0);
}

static void spool_freq_ambi_first_number(uint32_t number_to_print)
{
	std::fprintf(stderr,"START spool_freq_ambi_first_number...\n");

	for(auto& mapdb : rdbms::db_freq_ambi)
	{
	    boost::filesystem::path file_out(boost::filesystem::current_path());

	    std::string ruota_str = convert_ruota_to_string(mapdb.first);
		std::string prefix = "ambi_freq_first_";
		std::string postfix = ".txt";

		std::string res = prefix + ruota_str + postfix;

		file_out /= boost::filesystem::path(res);

		std::FILE* out_file = std::fopen(file_out.c_str(),"w");

		uint32_t kounter = 0;
		uint32_t frequency_bin = 0;
		bool do_exit = false;
		do
		{
			std::vector<extraction_t> ex_vec;
			ex_vec.clear();
			if( rdbms::get_freq_ambo_date(mapdb.second, frequency_bin, ex_vec) )
			{ // skip
				frequency_bin++;
				continue;
			}
			ex_vec.shrink_to_fit();
			sort_extractions_by_ambo_then_date(ex_vec);
			for( const auto& ee : ex_vec )
			{
				std::fprintf(out_file,"Freq: %d, ambo [%02d,%02d], data %04d-%02d-%02d Ruota: %s\n",frequency_bin,
						               (uint8_t) ee.bits.a,(uint8_t) ee.bits.b,\
									   (uint16_t) ee.bits.year,(uint8_t) ee.bits.month,(uint8_t) ee.bits.day, \
									   convert_ruota_to_string((ruota_t) ee.bits.ruota).c_str());
				kounter++;
				if( kounter > number_to_print )
				{
					do_exit = true;
					break;
				}
			}
			frequency_bin++;
		}
		while(!do_exit && (frequency_bin <= 10000));

		std::fprintf(out_file,"\n\n\n");
		std::fclose(out_file);
	}
	std::fprintf(stderr,"END spool_freq_ambi_first_number!\n");
}

static void spool_freq_terni_first_number(uint32_t number_to_print)
{
	std::fprintf(stderr,"START spool_freq_terni_first_number...\n");

	for(auto& mapdb : rdbms::db_freq_terni)
	{
	    boost::filesystem::path file_out(boost::filesystem::current_path());

	    std::string ruota_str = convert_ruota_to_string(mapdb.first);
		std::string prefix = "terni_freq_first_";
		std::string postfix = ".txt";

		std::string res = prefix + ruota_str + postfix;

		file_out /= boost::filesystem::path(res);

		std::FILE* out_file = std::fopen(file_out.c_str(),"w");

		uint32_t kounter = 0;
		uint32_t frequency_bin = 0;
		bool do_exit = false;
		do
		{
			std::vector<extraction_t> ex_vec;
			ex_vec.clear();
			if( rdbms::get_freq_terno_date(mapdb.second, frequency_bin, ex_vec) )
			{ // skip
				frequency_bin++;
				continue;
			}
			ex_vec.shrink_to_fit();
			// !!! strange hard fault from what follows, commented out...
			// sort_extractions_by_terno_then_date(ex_vec);
			for( const auto& ee : ex_vec )
			{
				std::fprintf(out_file,"Freq: %d, terno [%02d,%02d,%02d], data %04d-%02d-%02d Ruota: %s\n",frequency_bin,
						               (uint8_t) ee.bits.a,(uint8_t) ee.bits.b,(uint8_t) ee.bits.c,\
									   (uint16_t) ee.bits.year,(uint8_t) ee.bits.month,(uint8_t) ee.bits.day, \
									   convert_ruota_to_string((ruota_t) ee.bits.ruota).c_str());
				kounter++;
				if( kounter > number_to_print )
				{
					do_exit = true;
					break;
				}
			}
			frequency_bin++;
		}
		while(!do_exit && (frequency_bin <= 10000));

		std::fprintf(out_file,"\n\n\n");
		std::fclose(out_file);
	}
	std::fprintf(stderr,"END spool_freq_terni_first_number!\n");
}

static void spool_delay_ambi_first_number(uint32_t number_to_print)
{
	std::fprintf(stderr,"START spool_delay_ambi_first_number...\n");

	for(auto& mapdb : rdbms::db_ex_ambi)
	{
	    boost::filesystem::path file_out(boost::filesystem::current_path());

	    std::string ruota_str = convert_ruota_to_string(mapdb.first);
		std::string prefix = "ambi_delay_first_";
		std::string postfix = ".txt";

		std::string res = prefix + ruota_str + postfix;

		file_out /= boost::filesystem::path(res);

		std::FILE* out_file = std::fopen(file_out.c_str(),"w");

		uint32_t kounter = 0;

		std::vector<extraction_t> ex_vec;
		ex_vec.clear();
		for( const auto& key : combinations::combinations_2_of_90 )
		{
			extraction_t ee;
			ee.raw = 0;
			uint32_t freq = 0;
			std::vector<date_t> dates;
			dates.clear();
			if( rdbms::get_ambo_freq_date(mapdb.second, key, freq, dates) )
			{ // skip
				continue;
			}
			date_t last_date;
			last_date.raw = 0;
			if( dates.size() > 0 )
			{
				last_date.raw = dates[dates.size()-1].raw;
			}
			ee.bits.a = key[0];
			ee.bits.b = key[1];
			ee.bits.year = last_date.bits.year;
			ee.bits.month = last_date.bits.month;
			ee.bits.day = last_date.bits.day;
			ee.bits.ruota = mapdb.first;
			ex_vec.push_back(ee);
		}
		ex_vec.shrink_to_fit();
		sort_extractions(ex_vec);

		for( const auto& eee : ex_vec )
		{
			std::fprintf(out_file,"ambo [%02d,%02d], data %04d-%02d-%02d Ruota: %s\n", \
					               (uint8_t) eee.bits.a,(uint8_t) eee.bits.b,\
								   (uint16_t) eee.bits.year,(uint8_t) eee.bits.month,(uint8_t) eee.bits.day, \
								   convert_ruota_to_string((ruota_t) eee.bits.ruota).c_str());
			kounter++;
			if( kounter > number_to_print )
			{
				std::fprintf(out_file,"\n\n\n");
				break;
			}
		}

		std::fclose(out_file);
	}

	std::fprintf(stderr,"END spool_delay_ambi_first_number!\n");
}

static void spool_delay_terni_first_number(uint32_t number_to_print)
{
	std::fprintf(stderr,"START spool_delay_terni_first_number...\n");

	for(auto& mapdb : rdbms::db_ex_terni)
	{
	    boost::filesystem::path file_out(boost::filesystem::current_path());

	    std::string ruota_str = convert_ruota_to_string(mapdb.first);
		std::string prefix = "terni_delay_first_";
		std::string postfix = ".txt";

		std::string res = prefix + ruota_str + postfix;

		file_out /= boost::filesystem::path(res);

		std::FILE* out_file = std::fopen(file_out.c_str(),"w");

		uint32_t kounter = 0;

		std::vector<extraction_t> ex_vec;
		ex_vec.clear();
		for( const auto& key : combinations::combinations_3_of_90 )
		{
			extraction_t ee;
			ee.raw = 0;
			uint32_t freq = 0;
			std::vector<date_t> dates;
			dates.clear();
			if( rdbms::get_terno_freq_date(mapdb.second, key, freq, dates) )
			{ // skip
				continue;
			}
			date_t last_date;
			last_date.raw = 0;
			if( dates.size() > 0 )
			{
				last_date.raw = dates[dates.size()-1].raw;
			}
			ee.bits.a = key[0];
			ee.bits.b = key[1];
			ee.bits.c = key[2];
			ee.bits.year = last_date.bits.year;
			ee.bits.month = last_date.bits.month;
			ee.bits.day = last_date.bits.day;
			ee.bits.ruota = mapdb.first;
			ex_vec.push_back(ee);
		}
		ex_vec.shrink_to_fit();
		sort_extractions(ex_vec);

		for( const auto& eee : ex_vec )
		{
			std::fprintf(out_file,"terno [%02d,%02d,%02d], data %04d-%02d-%02d Ruota: %s\n", \
					               (uint8_t) eee.bits.a,(uint8_t) eee.bits.b,(uint8_t) eee.bits.c,\
								   (uint16_t) eee.bits.year,(uint8_t) eee.bits.month,(uint8_t) eee.bits.day, \
								   convert_ruota_to_string((ruota_t) eee.bits.ruota).c_str());
			kounter++;
			if( kounter > number_to_print )
			{
				std::fprintf(out_file,"\n\n\n");
				break;
			}
		}

		std::fclose(out_file);
	}

	std::fprintf(stderr,"END spool_delay_terni_first_number!\n");
}

}  // namespace lotto_extract


