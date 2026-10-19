
// EAB_V1.cpp : This file contains the 'main' function. Program execution begins and ends there//

#include <iostream>
//#include "stdafx.h"
#include <stdexcept>
#include  <fstream>
#include <vector>
#include <direct.h>
#include <cstdlib>
#include "EABRandom.h"
#include "EABModel.h"
#include "EABGrid.h"
#include <string>

int main(int argc, char* argv[])
{

		//Initialise the landscape
	try
	{
		// Run "eab 12345" for a repeatable run; with no number the seed is random and printed.
		unsigned long seed = (argc > 1) ? std::strtoul(argv[1], nullptr, 10) : 0;
		RNG_Init(seed);

		int iniMode = 1; //0 = uniform trees no landscape structure; 1= read from files; 2= uniform but has plantations and blocks
		double RandIf = 0.0; // The probability that a single bug arrives at a randomly chosen host cell

		//Control parameters
		double kill_rate = 0.94; //Not used in this model but there for future
		const int ConstMaxInvYr(9); // The number of years

		//Set up some write files
		char path_buffer[_MAX_PATH];
		char Npath_buffer[_MAX_PATH];
		_getcwd(path_buffer, _MAX_PATH);
		strcpy_s(Npath_buffer, path_buffer);


		InitialiseModel(); //For running EAB model - remove if want to just run ADB base layers

		_getcwd(path_buffer, _MAX_PATH);
		strcpy_s(Npath_buffer, path_buffer);
		strcat_s(Npath_buffer, "\\OutFiles\\MyErrors.txt");
		std::ofstream of(Npath_buffer);
		of.close();

		_getcwd(path_buffer, _MAX_PATH);
		strcpy_s(Npath_buffer, path_buffer);
		char ECBStr[50];
		strcpy_s(ECBStr, "\\OutFiles\\DataForOptJuneRun1.txt"); //Chnage OutFile name depending on model run
		strcat_s(Npath_buffer, ECBStr);
		std::ofstream OpOutF(Npath_buffer);
		OpOutF.close();

		Grid::ReSetProbDetection(); // For running EAB model - remove if want to just run ADB base layers
		int iflag(0);
		Grid::SetLandBasic(iflag); // For running EAB model - remove if want to just run ADB base layers (in code below)
		
		DetectionFullRunW_Target(RandIf, kill_rate, ConstMaxInvYr); // For running EAB model - remove if want to just run ADB base layers

		///////// Add for generating ADB base layers without running EAB /////////

		//for (int nits = 0; nits < 1; ++nits) {
		//	int iflag(0);
		//	Grid::SetLandBasic(iflag);
			//WriteCrops(nits);
		//}
		/////////////////////////////////////////////////////

	}
	catch (std::logic_error& E)
	{
		char Mess[500];
		strcpy_s(Mess, E.what());
		std::cout << Mess;


	}
	RNG_Free();
	std::cout << "Hello World!\n";
}

	

