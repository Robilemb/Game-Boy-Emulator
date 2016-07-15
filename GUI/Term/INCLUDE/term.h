#ifndef __GUI_TERM_H__
#define __GUI_TERM_H__

#include <cstdint>
#include <vector>
#include "CORE/INCLUDE/gameboy.h"

// FONCTION PRINCIPALE DU PROGRAMME
// ********************************
int			main(int argc, char** argv);

// LISTE DES COMMANDES + FONCTION D'ANALYSE DES COMMANDES
// ******************************************************
bool		_analyse_input(std::vector<std::string>& ai_vInput, Gameboy& ai_gb);
bool		_command_help(std::vector<std::string>& ai_vInput);
bool		_command_load(std::vector<std::string>& ai_vInput, Gameboy& ai_gb);
bool		_command_disp(std::vector<std::string>& ai_vInput, Gameboy& ai_gb);
bool		_command_exec(std::vector<std::string>& ai_vInput, Gameboy& ai_gb);
bool		_command_quit(std::vector<std::string>& ai_vInput);

// LISTE DES FONCTIONS PERMISE
// ***************************
void		_display_rom(Gameboy& ai_gb, std::uint16_t ai_start = 0, std::uint16_t ai_stop = 65535);
void		_display_reg(Gameboy& ai_gb);
void		_display_ins(Gameboy& ai_gb, std::uint16_t ai_start = 0);
void		_exec_ins(Gameboy& ai_gb, std::uint16_t ai_start = 0);

#endif /* __GUI_TERM_H__ */
