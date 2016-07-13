#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include "GUI/Term/INCLUDE/term.h"

int main(int argc, char** argv)
{
	bool						bCont = true;
	std::string					sInput = "";
	std::string					sKey = "";
	std::istringstream			streamInput;
	std::vector<std::string>	vInput;

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

		vInput.clear();
		while (getline(streamInput, sKey, ' '))
		{
			vInput.push_back(sKey);
		}

		sKey = vInput[0];

		// AFFICHAGE DE LA COMMANDE
		// ************************
		std::cout << "Commande: " << sKey << std::endl;

		// CHECK DES COMMANDES
		// *******************
		if (sKey.compare("help") == 0)
		{
			std::cout << "HELP" << std::endl;
			std::cout << " - load %%s : Charge la rom %%s." << std::endl;
			std::cout << " - display rom : Affiche la rom." << std::endl;
			std::cout << " - display reg : Affiche les registres." << std::endl;
			// std::cout << " - quit : Terminer le programme." << std::endl;
			std::cout << " - quit : Terminer le programme." << std::endl;
		}

		if (sKey.compare("quit") == 0)
		{
			bCont = false;
		}
	}

	return 0;
}
