/*
 * parse_and_process.h
 *
 *  Created on: Jan 20, 2020
 *      Author: fstrati
 */

#ifndef PARSE_AND_PROCESS_H_
#define PARSE_AND_PROCESS_H_

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#define IN_CMD_RESTART_FROM_SCRATCH  ("restart")
#define IN_CMD_UPDATE                ("update")
#define IN_CMD_HELP                  ("help")
#define IN_CMD_COMPUTE               ("compute")

namespace lotto_extract {

int32_t parse_process_init(void);

int32_t parse_process_restart(std::vector<std::string> in_args, int argc, char *argv[]);

int32_t parse_process_update(std::vector<std::string> in_args, int argc, char *argv[]);

int32_t parse_process_compute(std::vector<std::string> in_args, int argc, char *argv[]);

}  // namespace lotto_extract

#endif /* PARSE_AND_PROCESS_H_ */


