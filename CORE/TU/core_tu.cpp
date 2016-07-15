/*
 * core_tu.cpp
 *
 *  Created on: 15 juil. 2016
 *      Author: blacksheep
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <bitset>
#include "CORE/INCLUDE/cpu.h"


int main(int argc, char** argv)
{
	std::uint16_t		w_i;
	Cpu					w_cpu;
	std::uint8_t		w_tab[3];
	std::uint8_t		w_id;
	std::string			w_idStr;

	w_tab[0] = 0x00;
	w_tab[1] = 0x00;
	w_tab[2] = 0x00;

	std::cout << "TESTING INSTR DECODER AND DISPLAYER" << std::endl;
	std::cout << "***********************************" << std::endl << std::endl;

	for (w_i = 0; w_i < 256; w_i++)
	{
		std::uint8_t	w_opcode = (w_i & 0xFF);

		w_tab[0] = w_opcode;

		w_id 	= w_cpu.showInstructionId(w_tab);
		w_idStr = w_cpu.showInstructionIdStr(w_tab);

		std::string		w_str = w_cpu.showInstruction(w_tab);

		std::cout 	<< std::bitset<8>(w_opcode) << " : "
					<< std::setfill(' ') << std::setw(5) << static_cast<std::uint16_t>(w_id)
					<< std::setfill(' ') << std::setw(7) << w_idStr
					<< " -> " << w_str << std::endl;
	}

}
