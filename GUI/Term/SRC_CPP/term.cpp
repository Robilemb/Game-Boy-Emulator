#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "GUI/Term/INCLUDE/term.h"

int main(int argc, char** argv)
{
	bool						bCont = true;
	std::string					sInput = "";
	std::string					sKey = "";
	std::istringstream			streamInput;
	std::vector<std::string>	vInput;

	Gameboy						w_gameboy;

	// AFFICHAGE DU TITRE
	// ******************
	std::cout << "GameBoy-Terminal" << std::endl;
	std::cout << "================" << std::endl;
	std::cout << std::endl;
	std::cout << "Tapez help pour avoir la liste des instructions."  << std::endl;

	// BOUCLE DE GESTION DES COMMANDES
	// *******************************
	while (bCont == true)
	{
		// AFFICHER UNE INVITE DE COMMANDE
		// *******************************
		std::cout << " > ";

		// LIRE UNE COMMANDE
		// *****************
		streamInput.clear();
		std::getline(std::cin, sInput);
		streamInput.str(sInput);

		// CREATION DU VECTEUR D'ENTREE DE PARAMETRES
		// ******************************************
		vInput.clear();
		while (getline(streamInput, sKey, ' '))
		{
			vInput.push_back(sKey);
		}

		// CHECK DES COMMANDES
		// *******************
		bCont = _analyse_input(vInput, w_gameboy);
	}

	return 0;
}


// ******************************************************************************************
// ******************************************************************************************
// ******************************************************************************************
// 									FONCTIONS DE COMMANDES
// ******************************************************************************************
// ******************************************************************************************
// ******************************************************************************************

// FONCTION D'ANALYSE DE LA COMMANDE
// *********************************
bool		_analyse_input(std::vector<std::string>& ai_vInput, Gameboy& ai_gb)
{
	bool	bCont = true;

	if (ai_vInput.size() > 0)
	{
		std::string			w_sKey = ai_vInput[0];

		if (w_sKey.compare("help") == 0)
		{
			bCont = _command_help(ai_vInput);
		}
		else if (w_sKey.compare("load") == 0)
		{
			bCont = _command_load(ai_vInput, ai_gb);
		}
		else if (w_sKey.compare("disp") == 0)
		{
			bCont = _command_disp(ai_vInput, ai_gb);
		}
		else if (w_sKey.compare("exec") == 0)
		{
			bCont = _command_exec(ai_vInput, ai_gb);
		}
		else if (w_sKey.compare("quit") == 0)
		{
			bCont = _command_quit(ai_vInput);
		}
	}

	return bCont;
}

// COMMANDE D'AFFICHAGE DE L'AIDE
// ******************************
bool		_command_help(std::vector<std::string>& ai_vInput)
{
	std::cout << "HELP" << std::endl;
	std::cout << " - load %str : Charge la rom %str." << std::endl;
	std::cout << " - disp rom 'start %hhhh stop %hhhh': Affiche la rom entre les bornes start et stop entree en hexa." << std::endl;
	std::cout << " - disp reg : Affiche les registres." << std::endl;
	std::cout << " - disp ins '%hhhh' : Affiche l'instruction. Sans arg = PC, avec arg = Adresse" << std::endl;
	std::cout << " - exec '%hhh' : Execute l'instruction. Sans arg = PC, avec arg = adresse." << std::endl;
	std::cout << " - quit : Terminer le programme." << std::endl;

	return true;
}

// COMMANDE DE CHARGEMENT DE LA ROM
// ********************************
bool		_command_load(std::vector<std::string>& ai_vInput, Gameboy& ai_gb)
{
	Status				w_status = E_ERROR;
	std::string			w_sRomName = "";

	if (ai_vInput.size() > 1)
	{
		w_sRomName = ai_vInput[1];

		w_status = ai_gb.loadROM(w_sRomName);
	}

	if (w_status == E_OK)
	{
		std::cout << "Chargement ROM '" << w_sRomName << "' [OK]" << std::endl;
	}
	else
	{
		std::cout << "Erreur chargement ROM '" << w_sRomName << "'" << std::endl;
	}

	return true;
}

// COMMANDE D'AFFICHAGE D'INFORMATION (ROM / REGISTRES / INSTRUCTIONS / ...)
// *************************************************************************
bool		_command_disp(std::vector<std::string>& ai_vInput, Gameboy& ai_gb)
{
	std::uint8_t		w_disp = 0;
	std::uint16_t		w_start = 0;
	std::uint16_t		w_stop = 65535;

	if (ai_vInput.size() > 1)
	{
		for (std::uint8_t w_i = 1; w_i < ai_vInput.size(); w_i++)
		{
			std::string		w_cur = ai_vInput[w_i];

			if (w_cur.compare("rom") == 0)
			{
				w_disp = 0;
			}
			else if (w_cur.compare("reg") == 0)
			{
				w_disp = 1;
			}
			else if (w_cur.compare("ins") == 0)
			{
				w_disp = 2;

				if (++w_i < ai_vInput.size())
				{
					w_start = strtol((ai_vInput[w_i]).c_str(), NULL, 16);
				}
			}
			else if (w_cur.compare("start") == 0)
			{
				if (++w_i < ai_vInput.size())
				{
					w_start = strtol((ai_vInput[w_i]).c_str(), NULL, 16);
				}
			}
			else if (w_cur.compare("stop") == 0)
			{
				if (++w_i < ai_vInput.size())
				{
					w_stop = strtol((ai_vInput[w_i]).c_str(), NULL, 16);
				}
			}
		}
	}

	// GESTION DE L'AFFICHAGE
	// **********************
	if (w_disp == 0)
	{
		_display_rom(ai_gb, w_start, w_stop);
	}
	else if (w_disp == 1)
	{
		_display_reg(ai_gb);
	}
	else if (w_disp == 2)
	{
		_display_ins(ai_gb, w_start);
	}

	return true;
}

// COMMANDE D'EXECUTION D'UNE INSTRUCTION
// **************************************
bool		_command_exec(std::vector<std::string>& ai_vInput, Gameboy& ai_gb)
{
	std::uint16_t	w_pos = ai_gb.getCpu()->getRegisterPC();

	if (ai_vInput.size() > 1)
	{
		w_pos = strtol((ai_vInput[1]).c_str(), NULL, 16);
	}

	_exec_ins(ai_gb, w_pos);

	return true;
}

bool		_command_quit(std::vector<std::string>& ai_vInput)
{

	return false;
}

// ******************************************************************************************
// ******************************************************************************************
// ******************************************************************************************
// 									FONCTIONS D'AFFICHAGE
// ******************************************************************************************
// ******************************************************************************************
// ******************************************************************************************

// FONCTION D'AFFICHAGE DE LA ROM
// ******************************
void		_display_rom(Gameboy& ai_gb, std::uint16_t ai_start, std::uint16_t ai_stop)
{
	std::uint32_t		w_start = 0;
	std::uint32_t		w_stop = 0;
	std::uint32_t		w_romSize = 0;

	// ON RECUPERE LA TAILLE DE LA ROM
	// *******************************
	w_romSize = ai_gb.getRomSize();
	std::cout << "Rom Size : " << w_romSize << std::endl;

	// ON CALCULE LES INDEX DE DEBUT ET DE FIN
	// ***************************************
	w_start = ai_start - (ai_start % 32);
	if (w_start > 65535)
	{
		w_start = 0;
	}

	w_stop = ai_stop + (31 - (ai_stop % 32));
	if (w_stop > 65535)
	{
		w_stop = 65535;
	}
	if (w_stop > w_romSize)
	{
		w_stop = w_romSize;
	}

	std::cout << "Start = 0x" << std::hex << ai_start << " => 0x" << w_start << std::endl;
	std::cout << "Stop = 0x" << std::hex << ai_stop << " => 0x" << w_stop << std::endl;

	// AFFICHAGE DE LA ZONE VOULUE
	// ***************************
	for (std::uint32_t w_i = w_start; w_i <= w_stop; w_i++)
	{
		// TOUS LES MULTIPLES DE 32, ON AFFICHE L'ADRESSE
		// **********************************************
		if ((w_i % 32) == 0)
		{
			std::cout << std::setfill('0') << std::setw(5) << std::hex << w_i << "     ";
		}

		if (w_i >= ai_start && w_i <= ai_stop)
		{
			std::cout << "\033[1;31m";
		}

		// ON AFFICHE LA DATA ROM DE L'EMPLACEMENT MEMOIRE VOULU
		// *****************************************************
		std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getMemVal(w_i)) << " ";

		if (w_i >= ai_start && w_i <= ai_stop)
		{
			std::cout << "\033[0m";
		}

		// EN FIN DE LIGNE DE 32, ON REVIENS A LA LIGNE
		// ********************************************
		if ((w_i % 32) == 31)
		{
			std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}

// FONCTION D'AFFICHAGE DES REGISTRES
// **********************************
void		_display_reg(Gameboy& ai_gb)
{
	std::cout << std::setfill(' ');
	std::cout << "+--------------------------------------------------+" << std::endl;
	std::cout << "|" << std::setw(24) << "REGISTERS" << std::setw(26) << " " << "|" << std::endl;
	std::cout << "+--------------------------------------------------+" << std::endl;
	std::cout << "|" << std::setfill('0')
					 << "   A = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterA()) << " "
			  	     << "   B = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterB()) << " "
			  	     << "   C = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterC()) << " "
			  	     << "   D = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterD()) << " "
			  	     << std::setfill(' ') << std::setw(2) << " " << "|" << std::endl;
	std::cout << "|" << std::setfill('0')
					 << "   E = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterE()) << " "
			  	     << "   F = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterF()) << " "
			  	     << "   H = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterH()) << " "
			  	     << "   L = 0x" << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterL()) << " "
			  	     << std::setfill(' ') << std::setw(2) << " " << "|" << std::endl;
	std::cout << "+--------------------------------------------------+" << std::endl;
	std::cout << "|" << std::setfill('0')
					 << "   AF = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterAF()) << " "
			  	     << "     BC = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterBC()) << " "
			  	     << "    DE = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterDE()) << " "
			  	     << std::setfill(' ') << std::setw(2) << " " << "|" << std::endl;
	std::cout << "|" << std::setfill('0')
					 << "   PC = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterPC()) << " "
			  	     << "     HL = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterHL()) << " "
			  	     << "    SP = 0x" << std::setw(4) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getRegisterSP()) << " "
			  	     << std::setfill(' ') << std::setw(2) << " " << "|" << std::endl;
	std::cout << "+--------------------------------------------------+" << std::endl;
	std::cout << "|" << std::setfill(' ')
					 << "   Z = " << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getFlagZ()) << "   "
			  	     << "   N = " << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getFlagN()) << "   "
			  	     << "   H = " << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getFlagH()) << "   "
			  	     << "   C = " << std::setw(2) << std::hex << static_cast<unsigned short>(ai_gb.getCpu()->getFlagC()) << "   "
			  	     << std::setfill(' ') << std::setw(2) << " " << "|" << std::endl;
	std::cout << "+--------------------------------------------------+" << std::endl;
}

// FONCTION D'AFFICHAGE DES INSTRUCTIONS
// *************************************
void		_display_ins(Gameboy& ai_gb, std::uint16_t ai_start)
{
	std::uint16_t		w_pos = ai_start;
	std::string			w_str = "";

	if (ai_start == 0)
	{
		w_pos = ai_gb.getCpu()->getRegisterPC();
	}

	w_str = ai_gb.showInstr(w_pos);

	std::cout << std::setfill('0') << std::setw(5) << std::hex << w_pos << "     " << w_str << std::endl;
}

// FONCTION D'EXECUTION DE L'INSTRUCTION
// *************************************
void		_exec_ins(Gameboy& ai_gb, std::uint16_t ai_start)
{
	std::uint16_t		w_pos = ai_start;
	std::string			w_str = "";

	if (ai_start == 0)
	{
		w_pos = ai_gb.getCpu()->getRegisterPC();
	}

	ai_gb.execInstr(w_pos);
}
