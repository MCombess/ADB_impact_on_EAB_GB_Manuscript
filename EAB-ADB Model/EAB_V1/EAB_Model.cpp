#include "EABGrid.h"
#include "EABModel.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include <direct.h>
#include <list>


//Model scenarios for EAB-ADB interaction without control//

//If we're looking at No ADB model, then turn off the switches (indicated in code) for ADB mortality, and for EAB attraction to specific ADB categories (i.e. ADB has no impact)

//Pop dynamics EAB params
// Time
double P_LarvalDays = 56; // 8 weeks of development according to Duan et al 2013
// Larvae
double P_LarvToAdultStart = 0.25; // Grid::GetPropOverWinterLarvaeStart();//LarvaeToAdultInOneYear(Lyons, 2015)

double P_Cannibalism = 0.0001986334; //  0.00025; // 0.0075 Death rate due to cannibalism per m ^ 2 of phloem surface area(commented ones are obtained from the transformations in PPT)
// (Duan et al 2013 fig2)  sin ^ 2(0.000578) = 0.0000003341 Green ash; sin ^ 2(0.000404) = 0.000000163216 Tropical ash
//double P_Starvation = 0.0036;
double P_Starvation_HI = 0.0036;

// (Duan et al 2013 fig1 and fig2) sin ^ 2(0.000220) = 0.00000004834 Green ash, sin ^ 2(0.000105) = 0.00000001102 Tropical ash
double LarvalDeath1_H = exp(-P_Starvation_HI * P_LarvalDays);
double LarvalDeath1_I = exp(-P_Starvation_HI * P_LarvalDays);
double LarvalDeath1_S = exp(-P_Starvation_HI * P_LarvalDays);
double LarvalDeath1_D = exp(-P_Starvation_HI * P_LarvalDays);

double LarvalDeath2 = (P_Cannibalism / P_Starvation_HI) * (1 - exp(-P_Starvation_HI * P_LarvalDays));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////// If we're looking at adjusting larval mortality depending on ADB status, substitute the above P_Starvation_HI, LarvalDeath1_H, LarvalDeath2 for this:     /////////////////////////////

//double P_Starvation_H = 0.0036; // ADD
//double P_Starvation_I = 0.0036 / 1.17; // ADD - change to 1.66 for larval mortality scenario 2
//double P_Starvation_S = 0.0036 / 1.33; // ADD - change to 2.33 for larval mortality scenario 2
//double P_Starvation_D = 0.0036 / 1.5; // ADD - change to 3.0 for larval mortality scenario 2

//double LarvalDeath1_H = exp(-P_Starvation_H * P_LarvalDays); //ADD
//double LarvalDeath1_I = exp(-P_Starvation_I * P_LarvalDays); //ADD
//double LarvalDeath1_S = exp(-P_Starvation_S * P_LarvalDays); //ADD
//double LarvalDeath1_D = exp(-P_Starvation_D * P_LarvalDays); //ADD

//double LarvalDeath2_H = (P_Cannibalism / P_Starvation_H) * (1 - exp(-P_Starvation_H * P_LarvalDays)); //ADD
//double LarvalDeath2_I = (P_Cannibalism / P_Starvation_I) * (1 - exp(-P_Starvation_I * P_LarvalDays)); //ADD
//double LarvalDeath2_S = (P_Cannibalism / P_Starvation_S) * (1 - exp(-P_Starvation_S * P_LarvalDays)); //ADD
//double LarvalDeath2_D = (P_Cannibalism / P_Starvation_D) * (1 - exp(-P_Starvation_D * P_LarvalDays)); //ADD

//And then where we have LarvalDeath2 in the Generation function, replace this per ADB category for LarvalDeath2_H, LarvalDeath2_I, LarvalDeath2_S, LarvalDeath2_D
//as was already done for the LarvalDeath1_H, LarvalDeath1_I, LarvalDeath1_S, LarvalDeath1_D

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Adults
double P_AdultDeath = 0.17 / 0.279; //ProbDeathB4Reproduction 0.17
double P_NumberEggs = 26.58; //- 35 LarvaeOffspring - female depostis eggs in bark cracks in small groups of two or 3 eggs(Mcquarrie et al 2015)
double P_LayingEgg = 0.0001; // This is the probability of laying an egg range [0.0005 - 0.00004]

double treekappa = 0.0138939; // this is now per emergence hole per m2 //0.25; // 0.25; //tree death rate once infected Put this to 1 to avoid tree death


void InitialiseModel( )
{
	
	Grid::ClearData();
	FlightP::ClearData();
	//Initialises some outputfiles that we shall write our results in. 
	
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd( path_buffer, _MAX_PATH );
	strcpy_s(Npath_buffer, path_buffer);
	strcat_s(Npath_buffer, "\\OutFiles");
	int  dirMade=_mkdir(Npath_buffer);
	
	SetLand(0); //0 = read from files		
	
	
	

	//In SetLand - we have initialised some trees in a cell as one year infected so we need to pop the EAB on that

	for (int icount = 0; icount < Grid::GetNumRows(); icount++)
	{
		for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
		{
			if ((Grid::GetCrop(icount, jcount, 4) + Grid::GetCrop(icount, jcount, 5) + Grid::GetCrop(icount, jcount, 6) + Grid::GetCrop(icount, jcount, 7)) > 0.0) ////Status of trees/////
			{


				Grid::SetNumG(icount, jcount, 10.0); //Sets total number of adults in a cell
				Grid::SetOWLav(icount, jcount, 0);
				Grid::SetInvasion(icount, jcount, -1);

				Grid::SetPLarvH(icount, jcount, P_LarvToAdultStart); //Sets proportion of overwintering larvae, we do this for the different ash dieback status
				Grid::SetPLarvI(icount, jcount, P_LarvToAdultStart);
				Grid::SetPLarvS(icount, jcount, P_LarvToAdultStart);
				Grid::SetPLarvD(icount, jcount, P_LarvToAdultStart);
				Grid::SetOWLavmc_H(icount, jcount, 0.0); //Stores vector of larvae on trees of different ash dieback status
				Grid::SetOWLavmc_I(icount, jcount,0.0);
				Grid::SetOWLavmc_S(icount, jcount, 0.0);
				Grid::SetOWLavmc_D(icount, jcount, 0.0);

			}
			else
			{
				Grid::SetNumG(icount, jcount, 0.0); //Sets total number of adults in a cell
				Grid::SetOWLav(icount, jcount, 0);
				Grid::SetInvasion(icount, jcount, -1);

				Grid::SetPLarvH(icount, jcount, P_LarvToAdultStart);
				Grid::SetPLarvI(icount, jcount, P_LarvToAdultStart);
				Grid::SetPLarvS(icount, jcount, P_LarvToAdultStart);
				Grid::SetPLarvD(icount, jcount, P_LarvToAdultStart);
				Grid::SetOWLavmc_H(icount, jcount, 0.0);
				Grid::SetOWLavmc_I(icount, jcount, 0.0);
				Grid::SetOWLavmc_S(icount, jcount, 0.0);
				Grid::SetOWLavmc_D(icount, jcount, 0.0);

			}

		}
	}

	
	FlightP::Initialise(); // This initialises the expotential decay for flight
	
	



}

void DeleteModel()
{
	Grid::ClearData();
	FlightP::ClearData();
	

}

void SetLand(int readin)
{
	int iflag(0);
	Grid::SetLand(readin, iflag);
	if (iflag==1)
		throw std::logic_error("Error in Planta assignment");
	else if (iflag==2)
		throw std::logic_error("Error creating Planta landscape");
	else if (iflag==3)
		throw std::logic_error("Error creating Block assignment");
	
}

//Eggs are laid on tips of growing shoots on and between unfurling leaves. Females lay 300 to 800 eggs during their lifetime. 
//Nymphs pass through five instars. The total life cycle requires from 15 to 47 days, depending on environmental factors such as temperature and season. 
//The adults may live for more than a month. There is no diapause, but populations are typically low in the winter or during dry periods. 
//There are 9 to 10 generations a year, with up to 16 observed under observation in field cages. 


//Note this is an annual time step not monthly as the name suggests!
void ModelMonth(int imth, double randInf, double kill_rate)
{

	//Progress ADB before EAB, because ADB not dependent on EAB, but EAB is dependent on ADB//
	for (int icount = 0; icount < Grid::GetNumRows(); icount++)
	{
		for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
		{

			Tree_Generation(icount, jcount, imth); //ADB

		}
	}

	//This runs the bugs life cycle
	for (int icount=0; icount<Grid::GetNumRows(); icount++)
	{
		for (int jcount=0; jcount<Grid::GetNumCols(); jcount++)
		{
			Generation(icount, jcount, imth, kill_rate) ; //grows bugs and calculates the number of infected ones based on tree health
		}
	}

//	WriteBugs(imth); // this function writes a map of infected bugs for total see Tbugs

	Flight(); //Bugs distribute
	

	for (int icount = 0; icount < Grid::GetNumRows(); icount++)
	{
		for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
		{
			int FellingType(0);
			double propFelled(0);
			Grid::GetTreeFellingStats(icount, jcount, FellingType, propFelled);
			if (FellingType != 0)
				int junk = 0;
			AdjustEABStatusPostFlight(icount, jcount, FellingType, propFelled);
		}
	}

				
}

void Generation(int irow, int jcol, int imth, double kill_rate) // Model the population dynamics over a year
{
	//Initialise all variables used below//
	//double TAdultmc[4] = { 0, 0, 0, 0 };
	double TAdult = 0.0;
	double P_LarvToAdult(0);
	double P_LarvToAdultH(0);
	double P_LarvToAdultI(0);
	double P_LarvToAdultS(0);
	double P_LarvToAdultD(0);
	double TLarvae = 0.0;
	double TLarvaemcH = 0.0;
	double TLarvaemcI = 0.0;
	double TLarvaemcS = 0.0;
	double TLarvaemcD = 0.0;
	double numTrees = Grid::GetTreeDensity(irow, jcol);

	//Get the adults from last year 
	TAdult = Grid::GetNumG(irow, jcol); // Number of adults in cell

	//Get the larvae from last year
	Grid::GetOWLav(irow, jcol, TLarvae); // Density larvae per tree NOTE DIFFERENCE IN UNITS BETWEEN ADULTS AND LARVAE!
	Grid::GetOWLavmc_H(irow, jcol, TLarvaemcH);
	Grid::GetOWLavmc_I(irow, jcol, TLarvaemcI);
	Grid::GetOWLavmc_S(irow, jcol, TLarvaemcS);
	Grid::GetOWLavmc_D(irow, jcol, TLarvaemcD);
	Grid::GetPLarvH(irow, jcol, P_LarvToAdultH);
	Grid::GetPLarvI(irow, jcol, P_LarvToAdultI);
	Grid::GetPLarvS(irow, jcol, P_LarvToAdultS);
	Grid::GetPLarvD(irow, jcol, P_LarvToAdultD);

	//Get the ratio of eggs laid on a tree dependent on adb status (i.e. attraction probability). Three categories: asymptomatic+recently dead(H+D), lightly Stressed(I), heavily stressed(S)//

	//The attraction changes with the density of larvae on hasymptomatic and recently dead
	//so if we have adults but no larvae (i.e. just arrived in a cell) set the larval density per m2 phloem for healthy and recently dead trees assuming we have 1 larvae i.e. beetle just arrived
	//this gives us the maximum value from Mercador et al., 2011 paper - Figure 1 so we don't overshoot the maximum value from the paper

	//First get the number of ADB healthy and recently dead trees//
	double nadbHD = Grid::GetTreeDensity(irow, jcol) * (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 11));

	double trees = Grid::GetTreeDensity(irow, jcol);

	if (trees > 0.0) {
		double allprops = (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 5) +
			Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 9) + Grid::GetCrop(irow, jcol, 10) + Grid::GetCrop(irow, jcol, 11));

		int stop = 0;
		if (allprops < 0.0) {
			int go = 0;
	}
	
	}

	//Fetch the larvae per m2 phloem on a tree for asymptomatic and recently dead//
	double Ldens = Grid::Getlarvdens(irow, jcol);
	//Set manually as mentioned above if adults have arrived in a cell, but no larvae yet (i.e. set 1 larvae and then express this as m2 per phloem in a cell): //this gives us the maximum value from Mercador et al., 2011 paper - Figure 1 so we don't overshoot the maximum value from the paper

	if (Ldens < 1.0) {
		Ldens = 1.0;
	}
				//Calculating the differences in attraction between groups//

	//Now calculate beetle attraction to trees based on ash dieback status (Mercador et al., 2011 paper - Figure 1)//
	 //The ratio of beetles attracted to lightly stressed 
	double Iattraction =  (20.468 * pow(Ldens, -0.559)); //Adjust the baseline attraction accordingly here (half basline = *0.5; quarter basline = *0.25)
	//The ratio of beetles attracted to heavily stressed
	double Sattraction =  ((20.468 * pow(Ldens, -0.559))) / 2.0; //Adjust the baseline attraction accordingly here (half basline = *0.5; quarter basline = *0.25)

	if (isinf(Iattraction) || isnan(Iattraction)) { Iattraction = 0.0; }
	if (isinf(Sattraction) || isnan(Sattraction)) { Sattraction = 0.0; }

	if (Sattraction < 1.0) { Sattraction = 1.0; }
	if (Iattraction < 1.0) { Iattraction = 1.0; }

	//Calculating the proportion in each category//
	double tempTreeStates[12];
	for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
	{
		tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount);
	}

	double Hprop = tempTreeStates[4] + tempTreeStates[8];
	double Iprop = tempTreeStates[5] + tempTreeStates[9];
	double Sprop = tempTreeStates[6] + tempTreeStates[10];
	double Dprop = tempTreeStates[7] + tempTreeStates[11];

	double totaleablay = numTrees * exp(-P_LayingEgg * TAdult);

	double noeablay = (totaleablay * Hprop) + (totaleablay * Iprop) + (totaleablay * Sprop) + (totaleablay * Dprop);

	double HnoEAB = (totaleablay * Hprop);
	double InoEAB = (totaleablay * Iprop);
	double SnoEAB = (totaleablay * Sprop);
	double DnoEAB = (totaleablay * Dprop);

	//Now the total infected//
	double Hinf = (numTrees*Hprop)-HnoEAB;
	double Iinf = (numTrees * Iprop) - InoEAB;
	double Sinf = (numTrees * Sprop) - SnoEAB;
	double Dinf = (numTrees * Dprop) - DnoEAB;

	double totalinf = Hinf + Iinf + Sinf + Dinf;
	double nSinf = Sinf*Sattraction;
	double nIinf = Iinf * Iattraction;

	double af = totalinf / (Hinf + nIinf + nSinf + Dinf);

	double nnSinf = nSinf*af;
	double nnIinf = nIinf * af;
	double nnDinf = Dinf * af;
	double nnHinf = Hinf * af;


	//Now we get the proportion of egg laying on each ADB category//
	double Attprops[4] = { (nnHinf / (nnHinf + nnIinf + nnSinf + nnDinf)),
		(nnIinf / (nnHinf + nnIinf + nnSinf + nnDinf)),
		(nnSinf / (nnHinf + nnIinf + nnSinf + nnDinf)),
		(nnDinf / (nnHinf + nnIinf + nnSinf + nnDinf)) };

	int junknew = 1;

	if (isnan(Attprops[0])) { Attprops[0] = 0.0; }
	if (isnan(Attprops[1])) { Attprops[1] = 0.0; }
	if (isnan(Attprops[2])) { Attprops[2] = 0.0; }
	if (isnan(Attprops[3])) { Attprops[3] = 0.0; }
	if (isinf(Attprops[0])) { Attprops[0] = 0.0; }
	if (isinf(Attprops[1])) { Attprops[1] = 0.0; }
	if (isinf(Attprops[2])) { Attprops[2] = 0.0; }
	if (isinf(Attprops[3])) { Attprops[3] = 0.0; }


	if (Attprops[0] < 0.0) { Attprops[0] = 0.0; }
	if (Attprops[0] > 1.0) { Attprops[0] = 1.0; }
	if (Attprops[1] < 0.0) { Attprops[1] = 0.0; }
	if (Attprops[1] > 1.0) { Attprops[1] = 1.0; }
	if (Attprops[2] < 0.0) { Attprops[2] = 0.0; }
	if (Attprops[2] > 1.0) { Attprops[2] = 1.0; }
	if (Attprops[3] < 0.0) { Attprops[3] = 0.0; }
	if (Attprops[3] > 1.0) { Attprops[3] = 1.0; }


	if ((TAdult + TLarvae) > 0.0)
	{

		Grid::GetPLarv(irow, jcol, P_LarvToAdult);
		//Increase in bugs per time step
		double spray = 0.0; // If they buy into control strategy and its the month when control is applied then spray

		double tempTreeStates[12];
		for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
		{
			tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount);
		}

		double testattprops = Attprops[0]+ Attprops[1]+ Attprops[2]+Attprops[3];

		int junkere = 1;

		double Insecticide = spray * kill_rate; // control applied to current time step, maybe needeed later to kill bugs

		//Get trees with no EAB// 
		double propENoEAB[4] = { tempTreeStates[0], tempTreeStates[1], tempTreeStates[2], tempTreeStates[3] };


		//Get trees with longer term EAB// 
		double propEInfGTYr[4] = { tempTreeStates[8], tempTreeStates[9], tempTreeStates[10], tempTreeStates[11] }; // GT a year of EAB

	
		//All trees infested by EAB//
		double propIfest[4] = { tempTreeStates[4] + tempTreeStates[8], tempTreeStates[5] + tempTreeStates[9], tempTreeStates[6] + tempTreeStates[10], tempTreeStates[7] + tempTreeStates[11] };
		
		
		/////////////////         Used relpropIfest if we are using the NO ADDITIONAL ATTRACTION to any ADB and substituted in for Attprops when calculating AdultsM2Phloem      //////////////////
		
		//double relpropIfest[4] = { (tempTreeStates[4] + tempTreeStates[8]) / (tempTreeStates[4] + tempTreeStates[8] + tempTreeStates[5] + tempTreeStates[9] + tempTreeStates[6] + tempTreeStates[10] + tempTreeStates[7] + tempTreeStates[11]),
//(tempTreeStates[5] + tempTreeStates[9]) / (tempTreeStates[4] + tempTreeStates[8] + tempTreeStates[5] + tempTreeStates[9] + tempTreeStates[6] + tempTreeStates[10] + tempTreeStates[7] + tempTreeStates[11]),
//(tempTreeStates[6] + tempTreeStates[10]) / (tempTreeStates[4] + tempTreeStates[8] + tempTreeStates[5] + tempTreeStates[9] + tempTreeStates[6] + tempTreeStates[10] + tempTreeStates[7] + tempTreeStates[11]),
//(tempTreeStates[7] + tempTreeStates[11]) / (tempTreeStates[4] + tempTreeStates[8] + tempTreeStates[5] + tempTreeStates[9] + tempTreeStates[6] + tempTreeStates[10] + tempTreeStates[7] + tempTreeStates[11]),
	//	};

		//Now calculate this expressed as the number of trees//
		double NumTreesHealthy[4] = { Grid::GetTreeDensity(irow, jcol) * propENoEAB[0], Grid::GetTreeDensity(irow, jcol) * propENoEAB[1], Grid::GetTreeDensity(irow, jcol) * propENoEAB[2], Grid::GetTreeDensity(irow, jcol) * propENoEAB[3] };
		double NumTreesInfested[4] = { Grid::GetTreeDensity(irow, jcol) * propIfest[0], Grid::GetTreeDensity(irow, jcol) * propIfest[1], Grid::GetTreeDensity(irow, jcol) * propIfest[2], Grid::GetTreeDensity(irow, jcol) * propIfest[3] };
		double NumTreesInfGTYr[4] = { Grid::GetTreeDensity(irow, jcol) * propEInfGTYr[0], Grid::GetTreeDensity(irow, jcol) * propEInfGTYr[1], Grid::GetTreeDensity(irow, jcol) * propEInfGTYr[2], Grid::GetTreeDensity(irow, jcol) * propEInfGTYr[3] };
		double phloemPerTree = Grid::GetPhloemPerTree();

		//Multiply TAdult by proportion of overall egg laying occuring on trees to estimate the adults per m2 phloem//
		double AdultsM2Phloem[4] = { (TAdult * Attprops[0]) / (NumTreesInfested[0] * phloemPerTree), (TAdult * Attprops[1]) / (NumTreesInfested[1] * phloemPerTree), (TAdult * Attprops[2]) / (NumTreesInfested[2] * phloemPerTree), (TAdult * Attprops[3]) / (NumTreesInfested[3] * phloemPerTree)};
		if (isnan(AdultsM2Phloem[0])) { AdultsM2Phloem[0] = 0.0; } if (isnan(AdultsM2Phloem[1])) { AdultsM2Phloem[1] = 0.0; } if (isnan(AdultsM2Phloem[2])) { AdultsM2Phloem[2] = 0.0; } if (isnan(AdultsM2Phloem[3])) { AdultsM2Phloem[3] = 0.0; }
		if (isinf(AdultsM2Phloem[0])) { AdultsM2Phloem[0] = 0.0; } if (isinf(AdultsM2Phloem[1])) { AdultsM2Phloem[1] = 0.0; } if (isinf(AdultsM2Phloem[2])) { AdultsM2Phloem[2] = 0.0; } if (isinf(AdultsM2Phloem[3])) { AdultsM2Phloem[3] = 0.0; }

		//Adults that emerge from trees infested for a while.
		double AdultsM2Phloem_2[4] = { (((1 - P_LarvToAdultH) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_H) * AdultsM2Phloem[0] + LarvalDeath1_H * TLarvaemcH) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[0]) + (LarvalDeath2 * TLarvaemcH)),
			(((1 - P_LarvToAdultI) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_I)* AdultsM2Phloem[1] + LarvalDeath1_I * TLarvaemcI) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[1]) + (LarvalDeath2 * TLarvaemcI)),
			(((1 - P_LarvToAdultS) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_S)) * AdultsM2Phloem[2] + (LarvalDeath1_S) * TLarvaemcS) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[2]) + (LarvalDeath2 * TLarvaemcS)),
			(((1 - P_LarvToAdultD) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_D))* AdultsM2Phloem[3] + (LarvalDeath1_D) * TLarvaemcD) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[3]) + (LarvalDeath2 * TLarvaemcD)),
		};
		if (isnan(AdultsM2Phloem_2[0])) { AdultsM2Phloem_2[0] = 0.0; } if (isnan(AdultsM2Phloem_2[1])) { AdultsM2Phloem_2[1] = 0.0; } if (isnan(AdultsM2Phloem_2[2])) { AdultsM2Phloem_2[2] = 0.0; } if (isnan(AdultsM2Phloem_2[3])) { AdultsM2Phloem_2[3] = 0.0; }
		if (isinf(AdultsM2Phloem_2[0])) { AdultsM2Phloem_2[0] = 0.0; } if (isinf(AdultsM2Phloem_2[1])) { AdultsM2Phloem_2[1] = 0.0; } if (isinf(AdultsM2Phloem_2[2])) { AdultsM2Phloem_2[2] = 0.0; } if (isinf(AdultsM2Phloem_2[3])) { AdultsM2Phloem_2[3] = 0.0; }

		//Adults that emerge from trees infested only last year
		double AdultsM2Phloem_1[4] = { (((1 - P_LarvToAdultH) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_H) * AdultsM2Phloem[0]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[0])),
		(((1 - P_LarvToAdultI) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_I) * AdultsM2Phloem[1]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[1])),
		(((1 - P_LarvToAdultS) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_S)) * AdultsM2Phloem[2]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[2])),
		(((1 - P_LarvToAdultD) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_D)) * AdultsM2Phloem[3]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[3]))
		};
		if (isnan(AdultsM2Phloem_1[0])) { AdultsM2Phloem_1[0] = 0.0; } if (isnan(AdultsM2Phloem_1[1])) { AdultsM2Phloem_1[1] = 0.0; } if (isnan(AdultsM2Phloem_1[2])) { AdultsM2Phloem_1[2] = 0.0; } if (isnan(AdultsM2Phloem_1[3])) { AdultsM2Phloem_1[3] = 0.0; }
		if (isinf(AdultsM2Phloem_1[0])) { AdultsM2Phloem_1[0] = 0.0; } if (isinf(AdultsM2Phloem_1[1])) { AdultsM2Phloem_1[1] = 0.0; } if (isinf(AdultsM2Phloem_1[2])) { AdultsM2Phloem_1[2] = 0.0; } if (isinf(AdultsM2Phloem_1[3])) { AdultsM2Phloem_1[3] = 0.0; }


		//This is Larvae per m2 phloem for each ADB disease category
		double TLarvaemc[4] = { ((P_LarvToAdultH) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_H * AdultsM2Phloem[0]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[0]) + LarvalDeath2 * TLarvaemcH),
			((P_LarvToAdultI) * (1 - P_AdultDeath) * P_NumberEggs * LarvalDeath1_I * AdultsM2Phloem[1]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[1]) + LarvalDeath2 * TLarvaemcI),
			((P_LarvToAdultS) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_S) * AdultsM2Phloem[2]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[2]) + LarvalDeath2 * TLarvaemcS),
			((P_LarvToAdultD) * (1 - P_AdultDeath) * P_NumberEggs * (LarvalDeath1_D) * AdultsM2Phloem[3]) / (1 + (LarvalDeath2 * (1 - P_AdultDeath) * P_NumberEggs * AdultsM2Phloem[3]) + LarvalDeath2 * TLarvaemcD)
		};
		if (isnan(TLarvaemc[0])) { TLarvaemc[0] = 0.0; } if (isnan(TLarvaemc[1])) { TLarvaemc[1] = 0.0; } if (isnan(TLarvaemc[2])) { TLarvaemc[2] = 0.0; } if (isnan(TLarvaemc[3])) { TLarvaemc[3] = 0.0; }
		if (isinf(TLarvaemc[0])) { TLarvaemc[0] = 0.0; } if (isinf(TLarvaemc[1])) { TLarvaemc[1] = 0.0; } if (isinf(TLarvaemc[2])) { TLarvaemc[2] = 0.0; } if (isinf(TLarvaemc[3])) { TLarvaemc[3] = 0.0; }

		Grid::SetOWLavmc_H(irow, jcol, TLarvaemc[0]); //Set the larvae per m2 in each ADB category//
		Grid::SetOWLavmc_I(irow, jcol, TLarvaemc[1]);
		Grid::SetOWLavmc_S(irow, jcol, TLarvaemc[2]);
		Grid::SetOWLavmc_D(irow, jcol, TLarvaemc[3]);


		//Then the total number of adults emerging from this cell is 
		//Delta change in trees(NumTreesInfested - NumTreesInfsLT 
		double Delta[4] = { NumTreesInfested[0] - NumTreesInfGTYr[0], NumTreesInfested[1] - NumTreesInfGTYr[1], NumTreesInfested[2] - NumTreesInfGTYr[2], NumTreesInfested[3] - NumTreesInfGTYr[3] }; // Total number infested - number infested for a long time

		if (Delta[0] + Delta[1] + Delta[2] + Delta[3] < 0)
		{
			if (Delta[0] + Delta[1] + Delta[2] + Delta[3] > -0.00000000001)
			{
				Delta[0] = 0; Delta[1] = 0; Delta[2] = 0; Delta[3] = 0;
			}
			else
				throw std::logic_error("Error in ModelMonth");

		}
		//Calculating the number of adults and proportion of larvae to adult for each ADB category//
		double TAdultmc[4] = { 0.0, 0.0, 0.0, 0.0 };
		double P_LarvToAdultG[4] = { P_LarvToAdultH, P_LarvToAdultI, P_LarvToAdultS, P_LarvToAdultD };
		for (int icount = 0; icount < 4; ++icount) {
			TAdultmc[icount] = (Delta[icount] * AdultsM2Phloem_1[icount] + NumTreesInfGTYr[icount] * AdultsM2Phloem_2[icount]) * phloemPerTree;

			if ((P_LarvToAdultG[icount] <= 0.05))
			{
				P_LarvToAdultG[icount] = 0.05;
			}
			else if ((P_LarvToAdultG[icount] > 0.95))
			{
				P_LarvToAdultG[icount] = 0.95;
			}

			P_LarvToAdultG[icount] = -0.194 * log(TLarvaemc[icount] / P_LarvToAdultG[icount]) + 0.9862;

			if ((P_LarvToAdultG[icount] <= 0.05))
			{
				P_LarvToAdultG[icount] = 0.05;
			}
			else if ((P_LarvToAdultG[icount] > 0.95))
			{
				P_LarvToAdultG[icount] = 0.95;
			}
			;
		}

		TAdult = TAdultmc[0] + TAdultmc[1] + TAdultmc[2] + TAdultmc[3];

		//Caculate the overall larvae in a cell//
		double TLarvaetotal = (TLarvaemc[0]*(NumTreesInfested[0] * phloemPerTree)) + (TLarvaemc[1] * (NumTreesInfested[1] * phloemPerTree)) + (TLarvaemc[2] * (NumTreesInfested[2] * phloemPerTree)) + (TLarvaemc[3] * (NumTreesInfested[3] * phloemPerTree));

		//Now express this as larval density per m2 phloem, irrespective of disease category//
		double infPhloem = (NumTreesInfested[0] + NumTreesInfested[1] + NumTreesInfested[2] + NumTreesInfested[3]) * phloemPerTree;

		double TLarvaetotalnew = 0.0;

		if (infPhloem > 0.0) {

			TLarvaetotalnew = TLarvaetotal / infPhloem;

		}
		//Setting the proportion of larvae to adult by ADB disease category//
		Grid::SetPLarvH(irow, jcol, P_LarvToAdultG[0]);
		Grid::SetPLarvI(irow, jcol, P_LarvToAdultG[1]);
		Grid::SetPLarvS(irow, jcol, P_LarvToAdultG[2]);
		Grid::SetPLarvD(irow, jcol, P_LarvToAdultG[3]);

		//Setting average proportion of larvae to adult//
		
		double p_F = 0.0;
		if (TLarvaetotal > 0) {
			//Total larve in each category//
			double T_L[4] = { TLarvaemc[0] * (NumTreesInfested[0] * phloemPerTree), TLarvaemc[1] * (NumTreesInfested[1] * phloemPerTree), TLarvaemc[2] * (NumTreesInfested[2] * phloemPerTree),
				TLarvaemc[3] * (NumTreesInfested[3] * phloemPerTree) };

			//Next, we already have overwintering proportions on each category P_LarvToAdultG[0-3]//
			//Calculate the proportion of Larvae in each category relative to the total larvae (TLarvaetotal)

			double p_T_L[4] = { T_L[0] / TLarvaetotal, T_L[1] / TLarvaetotal , T_L[2] / TLarvaetotal , T_L[3] / TLarvaetotal };

			double p_OW[4] = { P_LarvToAdultG[0] * p_T_L[0],  P_LarvToAdultG[1] * p_T_L[1], P_LarvToAdultG[2] * p_T_L[2],
			P_LarvToAdultG[3] * p_T_L[3] };
			p_F = p_OW[0] + p_OW[1] + p_OW[2] + p_OW[3];

		}

		if (p_F > 0.95) { p_F = 0.95; }
		if (p_F <0.05) { p_F = 0.05; }

		//Now sum for the weighted average//
		Grid::SetPLarv(irow, jcol, p_F);

		//Setting total number of adults in a cell
		Grid::SetNumG(irow, jcol, TAdult);

		//And now the overall larval density per m2 phloem
		Grid::SetOWLav(irow, jcol, TLarvaetotalnew);

		//Now the emergence density per m2 phloem of beetles for each ADB category//
		double emergH = 0.0;
		if (NumTreesInfested[0] * phloemPerTree > 0) {
			emergH = TAdultmc[0] / (NumTreesInfested[0] * phloemPerTree);
		}

		double emergI = 0.0;
		if (NumTreesInfested[1] * phloemPerTree > 0) {
			emergI = TAdultmc[1] / (NumTreesInfested[1] * phloemPerTree);
		}

		double emergS = 0.0;
		if (NumTreesInfested[2] * phloemPerTree > 0) {
			emergS = TAdultmc[2] / (NumTreesInfested[2] * phloemPerTree);
		}

		double emergD = 0.0;
		if (NumTreesInfested[3] * phloemPerTree > 0) {
			emergD = TAdultmc[3] / (NumTreesInfested[3] * phloemPerTree);
		}

		Grid::SetEmergenceDensity_H(irow, jcol, emergH);
		Grid::SetEmergenceDensity_I(irow, jcol, emergI);
		Grid::SetEmergenceDensity_S(irow, jcol, emergS);
		Grid::SetEmergenceDensity_D(irow, jcol, emergD);

		//Set the larval density on asymptomatic and recently dead ADB trees in a cell to use as a control for relative EAB on stressed trees
		//as shown in Mercador et al., 2011a paper.

		double larvdensHD = 0.0;
		if (NumTreesInfested[0] + NumTreesInfested[3] > 0) {
			double ini_L_HD_ps[2] = { NumTreesInfested[0] / (NumTreesInfested[0] + NumTreesInfested[3]),  NumTreesInfested[3] / (NumTreesInfested[0] + NumTreesInfested[3]) };

			larvdensHD = (ini_L_HD_ps[0] * TLarvaemc[0]) + (ini_L_HD_ps[1] * TLarvaemc[3]);
		}

		Grid::Setlarvdens(irow, jcol, larvdensHD);

	}
	else
	{
		Grid::SetNumG(irow, jcol, TAdult);
		Grid::SetOWLav(irow, jcol, TLarvae);
	}

	//ofm.close();
}


void Tree_Generation(int irow, int jcol, int iyr) //progesses tree ADB severity in a time step
{
	double numTrees = Grid::GetTreeDensity(irow, jcol); //number of trees in each cell
	if (numTrees > Grid::GetInitialTreeDensity(irow, jcol) + 0.0000001) {
		throw std::logic_error("Creating trees!!!");
	}
	
	//Only progress comparmental model if we have trees and ADB in a cell//
	if (numTrees > 0.0 && Grid::Getadbarrival(irow, jcol) > -1) {

		double tempTreeStates[12]; //start conditions for each cell
		for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
		{
			tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount); // These are proportions
		}

		//Fetch variables for each cell//
		//ADB arrival working with assumption that cells where disease was detected in 2013, ADB symtpoms were
		//present throughout these cells (i.e. had been present a number of years - only detected when symptoms clear), this then spreads at a rate of
		//50 km per year from these cells.

		int adbyear = Grid::Getadbarrival(irow, jcol); //will return the year of disease arrival from the cell
		int lastIdx = 13 - adbyear; //Index where EABGrid.cpp finished (change 13 to 18 if delaying ADB arrival by 5 years)
		double Ninit = Grid::GetInitialTreeDensity(irow, jcol); //Getting number of ash before ADB
		double share = Grid::GetMatShare(irow, jcol); //Getting the current mature ash cell share (matprop is the original)
		int adbsoil = Grid::Getadbsoil(irow, jcol); // soil drainage
		double adbeusoil = Grid::Getadbeusoil(irow, jcol); //estimate soil water content
		double nonwoodp = Grid::Getnonwoodp(irow, jcol); //proportion of trees in non-wood setting
		double inimatgamma = 0.0205; //mature gamma values (0.009 for minimum, 0.0205 for median, 0.032 for max)
		double inijuvgamma = inimatgamma * (0.1 / 0.032); //Calculate juvenile mortality
		double adbeta = 0.5; //Beta value
		double nonwoodeff = 1.0 - (0.19 / 0.41); //This is the difference in crown dieback between Hedgerow and Woodland trees
		double matprop = 0.29; //define the proportion of mature ash in GB woodlands
		double juvprop = 0.71; //define the proportion of juvenile ash in GB woodlands
		double matstresseff = 0.724; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
		double juvstresseff = 0.712; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
		double ResPropstart = Grid::GetResProp(irow, jcol); //Getting the resistant proportion in a cell
		double mortratio = 3.0; //Difference in rate severly stressed trees die vs lightly stressed trees - will adjust with q values//
		double precipmortklesse = (1.0 / (1.0 + exp(-(0.5 - 6.7)))); //Mortality given a unit precipitation increase
		double noprecipmortklesse = (1.0 / (1.0 + exp(-(-6.7)))); //Mortality at minimum precipitation
		double precippercentchange = ((precipmortklesse / noprecipmortklesse) - 1) / (312.0 + 1305.575); //Percent change in mortality per unit increase in rainfall

		double precippmat = (inimatgamma * (1.0 + (precippercentchange * Grid::Getprecipp(irow, jcol)))) - inimatgamma; //i.e. the additional gamma value per cell//
		double precippjuv = (inijuvgamma * (1.0 + (precippercentchange * Grid::Getprecipp(irow, jcol)))) - inijuvgamma; //i.e. the additional gamma value per cell//

		//Initiliasing variables//
		double toberemovedmat = 0.0;
		double toberemovedjuv = 0.0;

		double soilp = 1.0;
		if (adbsoil > 4 && adbeusoil > 0.3605875) { //wet sites
			soilp = 1.9505; //soil moisture adjusted transition rate multiplier for wet sites (1.857 for minimum, 1.9505 for median, 2.04 for max)
		}
		else { soilp = 1.0; }

		double soilpmat = (inimatgamma * soilp) - inimatgamma; //i.e. the additional gamma value per cell//
		double soilpjuv = (inijuvgamma * soilp) - inijuvgamma; //i.e. the additional gamma value per cell//

		//Adding the effect of soil and precipitation to the base ADB gamma value
		double matgammaa = inimatgamma + soilpmat + precippmat;
		double juvgammaa = inijuvgamma + soilpjuv + precippjuv;

		//Now adjusting the gamma value dependent on the proportion of ADB in a woodland
		double matgamma = matgammaa - (matgammaa * nonwoodp * nonwoodeff);
		double juvgamma = juvgammaa - (juvgammaa * nonwoodp * nonwoodeff);

		//Calculating the rate at which trees move to the stressed category//
		double matalpha = matgamma / matstresseff;
		double juvalpha = juvgamma / juvstresseff;

		//Check out ADB by age aligns with the allADB and we've not lost information along the way//
		for (int kk = 0; kk < 4; ++kk) {

			double allTogether = tempTreeStates[kk] + tempTreeStates[kk + 4] + tempTreeStates[kk + 8];
			double agecats = share * Grid::GetCropmat(irow, jcol, kk) + (1.0-share)*Grid::GetCropjuv(irow,jcol,kk);
			if (allTogether - agecats > 0.00001 || allTogether - agecats < -0.00001) {
				throw std::logic_error("ADB by age different from ADB when ages grouped together");
			}
		}
		
		
		
		
		for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
		{
			Grid::SetCrop(irow, jcol, icount, tempTreeStates[icount]); // These are proportions
		}

		//Before running the model, remove trees that have been dead for three years//

		 //change below to if (iyr < 0) if we don't want to remove trees//
		//if (iyr < 0) { TURN ON FOR NO ADB MODEL
		if (iyr > 0) { //Set the condition to iyr above 0 because we've already removed trees in the EABGrid.cpp - PUT THIS LINE IN IF WE WANT TO REMOVE TREES - TURN OFF FOR NO ADB MODEL

			
			int k = iyr + lastIdx - 3;
			double diedm = Grid::Getnremovedmat(irow, jcol, k) - Grid::Getnremovedmat(irow, jcol, k - 1);
			double diedj = Grid::Getnremovedjuv(irow,jcol,k) - Grid::Getnremovedjuv(irow, jcol, k - 1);
			
			double Nbefore = Grid::GetTreeDensity(irow, jcol);
			if (share > 0.0) {
				toberemovedmat = diedm * (matprop * Ninit) / (share * Nbefore);
			}
			
			else {
				toberemovedmat = 0.0;
			}

			if (share < 1.0) {
				toberemovedjuv = diedj * (juvprop * Ninit) / ((1.0 - share) * Nbefore);
			}

			else {
				toberemovedjuv = 0.0;
			}

			if (toberemovedmat < 0.0) { toberemovedmat = 0.0; } if (toberemovedjuv < 0.0) { toberemovedjuv = 0.0; }
			if (isinf(toberemovedmat) || isnan(toberemovedmat)) { toberemovedmat = 0.0; } if (isinf(toberemovedjuv) || isnan(toberemovedjuv)) { toberemovedjuv = 0.0; }

			if (toberemovedmat > Grid::GetCropmat(irow, jcol, 3)) { toberemovedmat = Grid::GetCropmat(irow, jcol, 3); }
			if (toberemovedjuv > Grid::GetCropjuv(irow, jcol, 3)) { toberemovedjuv = Grid::GetCropjuv(irow, jcol, 3); }

			//Remove dead for three years from mature trees//
			double ninvestDm = Grid::GetCropmat(irow, jcol, 3);
			if (toberemovedmat > 0.0) {
				ninvestDm = Grid::GetCropmat(irow, jcol, 3) - toberemovedmat;
			}
			if (ninvestDm < 0.0) { ninvestDm = 0.0; }

			double newpropmat = (Grid::GetCropmat(irow, jcol, 0)) + (Grid::GetCropmat(irow, jcol, 1)) + (Grid::GetCropmat(irow, jcol, 2)) + ninvestDm;

			//Adjust the other categories of ash dieback disease now we have removed trees//
			if (toberemovedmat > 0.0 && newpropmat > 0.0) {
				double newHmat = (Grid::GetCropmat(irow, jcol, 0)) / newpropmat;
				double newImat = (Grid::GetCropmat(irow, jcol, 1)) / newpropmat;
				double newSmat = (Grid::GetCropmat(irow, jcol, 2)) / newpropmat;
				double newDmat = ninvestDm / newpropmat;

				Grid::SetCropmat(irow, jcol, 0, newHmat);
				Grid::SetCropmat(irow, jcol, 1, newImat);
				Grid::SetCropmat(irow, jcol, 2, newSmat);
				Grid::SetCropmat(irow, jcol, 3, newDmat);
			}
			else {
				Grid::SetCropmat(irow, jcol, 0, Grid::GetCropmat(irow, jcol, 0));
				Grid::SetCropmat(irow, jcol, 1, Grid::GetCropmat(irow, jcol, 1));
				Grid::SetCropmat(irow, jcol, 2, Grid::GetCropmat(irow, jcol, 2));
				Grid::SetCropmat(irow, jcol, 3, ninvestDm);
			}

			//Now repeat the process for juvenile trees//
			double ninvestDj = Grid::GetCropjuv(irow, jcol, 3);
			if (toberemovedjuv > 0.0) {
				ninvestDj = Grid::GetCropjuv(irow, jcol, 3) - toberemovedjuv;
			}
			if (ninvestDj < 0.0) { ninvestDj = 0.0; }

			double newpropjuv = (Grid::GetCropjuv(irow, jcol, 0)) + (Grid::GetCropjuv(irow, jcol, 1)) + (Grid::GetCropjuv(irow, jcol, 2)) + ninvestDj;

			if (ninvestDj < 0.0) {
				double junk = 0.0;
			}

			if (toberemovedjuv > 0.0 && newpropjuv > 0.0) {
				double newHjuv = (Grid::GetCropjuv(irow, jcol, 0)) / newpropjuv;
				double newIjuv = (Grid::GetCropjuv(irow, jcol, 1)) / newpropjuv;
				double newSjuv = (Grid::GetCropjuv(irow, jcol, 2)) / newpropjuv;
				double newDjuv = ninvestDj / newpropjuv;

				Grid::SetCropjuv(irow, jcol, 0, newHjuv);
				Grid::SetCropjuv(irow, jcol, 1, newIjuv);
				Grid::SetCropjuv(irow, jcol, 2, newSjuv);
				Grid::SetCropjuv(irow, jcol, 3, newDjuv);
			}
			else {
				Grid::SetCropjuv(irow, jcol, 0, (Grid::GetCropjuv(irow, jcol, 0)));
				Grid::SetCropjuv(irow, jcol, 1, (Grid::GetCropjuv(irow, jcol, 1)));
				Grid::SetCropjuv(irow, jcol, 2, (Grid::GetCropjuv(irow, jcol, 2)));
				Grid::SetCropjuv(irow, jcol, 3, ninvestDj);
			}

			//Now remove dead for more than three years from all trees (e.g. not by age)//
			//double totalremoved = (toberemovedjuv * juvprop) + (toberemovedmat * matprop);
			double totalremoved = (toberemovedjuv * (1.0 - share)) + (toberemovedmat * share);

			//Check for positive and negatives (i.e. overshot it)
			if (totalremoved > 1.000000001 || totalremoved < -0.000000001) {
				int junk = 0.0;
				throw std::logic_error("error in removing trees regardless of age");

			}

			if (totalremoved > 1.0) { totalremoved = 1.0; }
			if (totalremoved < 0.0) { totalremoved = 0.0; }
			if (isinf(totalremoved) || isnan(totalremoved)) { totalremoved = 0.0; }

			//Check value possible//
			double deadPool = Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11);
			
			if (totalremoved > deadPool + 0.000000001) {
				throw std::logic_error("removing more trees than are dead");
			}
			if (totalremoved > deadPool) { totalremoved = deadPool; }

			//Now update the share of trees in each age category//
			double matLeft = share * (1.0 - toberemovedmat);
			double juvLeft = (1.0 - share) * (1.0 - toberemovedjuv);
			if (matLeft + juvLeft > 0.0) { share = matLeft / (matLeft + juvLeft); }
			Grid::SetMatShare(irow, jcol, share);

			//I need to divide up the total removed by EAB category [0] is no EAB, [1] is new EAB, [2] is old EAB//
			double totalremovedbyEAB[3] = { 0.0, 0.0, 0.0 };
			if (deadPool > 0.0) {
				totalremovedbyEAB[0] = totalremoved * Grid::GetCrop(irow,jcol,3)/deadPool;
				totalremovedbyEAB[1] = totalremoved * Grid::GetCrop(irow, jcol, 7) / deadPool;
				totalremovedbyEAB[2] = totalremoved * Grid::GetCrop(irow, jcol, 11) / deadPool;
			}


			//Now adjust for those that have died//
			double nallDnoEAB = Grid::GetCrop(irow, jcol, 3);
			if (totalremovedbyEAB[0] > 0.0) {
				nallDnoEAB = Grid::GetCrop(irow, jcol, 3) - totalremovedbyEAB[0];
			}
			if (nallDnoEAB < 0.0) { nallDnoEAB = 0.0; }

			double nallDnewEAB = Grid::GetCrop(irow, jcol, 7);
			if (totalremovedbyEAB[1] > 0.0) {
				nallDnewEAB = Grid::GetCrop(irow, jcol, 7) - totalremovedbyEAB[1];
			}
			if (nallDnewEAB < 0.0) { nallDnewEAB = 0.0; }

			double nallDoldEAB = Grid::GetCrop(irow, jcol, 11);
			if (totalremovedbyEAB[2] > 0.0) {
				nallDoldEAB = Grid::GetCrop(irow, jcol, 11) - totalremovedbyEAB[2];
			}
			if (nallDoldEAB < 0.0) { nallDoldEAB = 0.0; }


			//Adjusting the proportions now we have removed trees//
			double newpropall = Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 2) +
				nallDnoEAB + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 6) +
				nallDnewEAB + Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 9) + Grid::GetCrop(irow, jcol, 10) +
				nallDoldEAB;

			if (totalremoved > 0.0) {

				Grid::SetCrop(irow, jcol, 0, (Grid::GetCrop(irow, jcol, 0) / newpropall));
				Grid::SetCrop(irow, jcol, 1, (Grid::GetCrop(irow, jcol, 1) / newpropall));
				Grid::SetCrop(irow, jcol, 2, (Grid::GetCrop(irow, jcol, 2) / newpropall));
				Grid::SetCrop(irow, jcol, 3, (nallDnoEAB / newpropall));
				Grid::SetCrop(irow, jcol, 4, (Grid::GetCrop(irow, jcol, 4) / newpropall));
				Grid::SetCrop(irow, jcol, 5, (Grid::GetCrop(irow, jcol, 5) / newpropall));
				Grid::SetCrop(irow, jcol, 6, (Grid::GetCrop(irow, jcol, 6) / newpropall));
				Grid::SetCrop(irow, jcol, 7, (nallDnewEAB / newpropall));
				Grid::SetCrop(irow, jcol, 8, (Grid::GetCrop(irow, jcol, 8) / newpropall));
				Grid::SetCrop(irow, jcol, 9, (Grid::GetCrop(irow, jcol, 9) / newpropall));
				Grid::SetCrop(irow, jcol, 10, (Grid::GetCrop(irow, jcol, 10) / newpropall));
				Grid::SetCrop(irow, jcol, 11, (nallDoldEAB / newpropall));
			}
			else {
				Grid::SetCrop(irow, jcol, 0, Grid::GetCrop(irow, jcol, 0));
				Grid::SetCrop(irow, jcol, 1, Grid::GetCrop(irow, jcol, 1));
				Grid::SetCrop(irow, jcol, 2, Grid::GetCrop(irow, jcol, 2));
				Grid::SetCrop(irow, jcol, 3, Grid::GetCrop(irow, jcol, 3));
				Grid::SetCrop(irow, jcol, 4, Grid::GetCrop(irow, jcol, 4));
				Grid::SetCrop(irow, jcol, 5, Grid::GetCrop(irow, jcol, 5));
				Grid::SetCrop(irow, jcol, 6, Grid::GetCrop(irow, jcol, 6));
				Grid::SetCrop(irow, jcol, 7, Grid::GetCrop(irow, jcol, 7));
				Grid::SetCrop(irow, jcol, 8, Grid::GetCrop(irow, jcol, 8));
				Grid::SetCrop(irow, jcol, 9, Grid::GetCrop(irow, jcol, 9));
				Grid::SetCrop(irow, jcol, 10, Grid::GetCrop(irow, jcol, 10));
				Grid::SetCrop(irow, jcol, 11, Grid::GetCrop(irow, jcol, 11));
			}

			Grid::SetTreeDensity(irow, jcol, Grid::GetTreeDensity(irow, jcol) * (1 - totalremoved));
		}

		//Adjusting the proportion of resistant trees now//
		double Ttrees = Grid::GetTreeDensity(irow, jcol);

		double Nmat = share * Ttrees;
		double Njuv = (1.0 - share) * Ttrees;

		double ResPropm = 0.0, ResPropj = 0.0;
		if (Nmat > 0.0) { ResPropm = matprop * ResPropstart * Ninit / Nmat; }
		if (Njuv > 0.0) { ResPropj = juvprop * ResPropstart * Ninit / Njuv; }

		if (ResPropm > 1.0) { ResPropm = 1.0; }
		if (ResPropj > 1.0) { ResPropj = 1.0; }

		//Check resistant code is behaving//
		double resCount = ResPropstart * Ninit;
		double Hcount = (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8)) * Ttrees;
		if (resCount > Hcount + 0.000001) {
			throw std::logic_error("Resistant trees more than healthy");
		}

		if (ResPropstart <= 0.0) {
			throw std::logic_error("Resistant trees gone");
		}

		//We will calculate the proportions of the mature and juvenile trees in each of the 12 disease-EAB categories//
		//I.e. divide up proportion of trees in each ADB category, by the proportion that are in the respective EAB category
		//for each disease category // Three states of EAB - none (1st col), present not detected (2nd col), present (longer) (3rd col)	


		double H_treesm[3] = { Grid::GetCropmat(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 0) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))), //Get mature adb asymptomatic trees and divide up by EAB category//
				Grid::GetCropmat(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 4) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))),
			Grid::GetCropmat(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 8) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))) };
		if (isnan(H_treesm[0]) || isinf(H_treesm[0])) { H_treesm[0] = 0.0; }
		if (isnan(H_treesm[1]) || isinf(H_treesm[1])) { H_treesm[1] = 0.0; }
		if (isnan(H_treesm[2]) || isinf(H_treesm[2])) { H_treesm[2] = 0.0; }

		double I_treesm[3] = { Grid::GetCropmat(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 1) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))), //Get mature adb asymptomatic trees and divide up by EAB category//
			Grid::GetCropmat(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 5) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))),
		Grid::GetCropmat(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 9) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))) };
		if (isnan(I_treesm[0]) || isinf(I_treesm[0])) { I_treesm[0] = 0.0; }
		if (isnan(I_treesm[1]) || isinf(I_treesm[1])) { I_treesm[1] = 0.0; }
		if (isnan(I_treesm[2]) || isinf(I_treesm[2])) { I_treesm[2] = 0.0; }


		double S_treesm[3] = { Grid::GetCropmat(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 2) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))), //Get mature adb asymptomatic trees and divide up by EAB category//
			Grid::GetCropmat(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 6) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))),
		Grid::GetCropmat(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 10) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))) };
		if (isnan(S_treesm[0]) || isinf(S_treesm[0])) { S_treesm[0] = 0.0; }
		if (isnan(S_treesm[1]) || isinf(S_treesm[1])) { S_treesm[1] = 0.0; }
		if (isnan(S_treesm[2]) || isinf(S_treesm[2])) { S_treesm[2] = 0.0; }


		double D_treesm[3] = { Grid::GetCropmat(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 3) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))), //Get mature adb dead trees and divide up by EAB category//
			Grid::GetCropmat(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 7) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))),
		Grid::GetCropmat(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 11) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))) };

		if (isnan(D_treesm[0]) || isinf(D_treesm[0])) { D_treesm[0] = 0.0; }
		if (isnan(D_treesm[1]) || isinf(D_treesm[1])) { D_treesm[1] = 0.0; }
		if (isnan(D_treesm[2]) || isinf(D_treesm[2])) { D_treesm[2] = 0.0; }

		//NOW REPEAT THE ABOVE FOR JUVENILE TREES!!!//

		double H_treesj[3] = { Grid::GetCropjuv(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 0) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))), //Get juvenile adb asymptomatic trees and divide up by EAB category//
			Grid::GetCropjuv(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 4) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))),
		Grid::GetCropjuv(irow, jcol, 0) * (Grid::GetCrop(irow, jcol, 8) / (Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8))) };

		if (isnan(H_treesj[0]) || isinf(H_treesj[0])) { H_treesj[0] = 0.0; }
		if (isnan(H_treesj[1]) || isinf(H_treesj[1])) { H_treesj[1] = 0.0; }
		if (isnan(H_treesj[2]) || isinf(H_treesj[2])) { H_treesj[2] = 0.0; }

		double I_treesj[3] = { Grid::GetCropjuv(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 1) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))), //Get mature adb asymptomatic trees and divide up by EAB category//
			Grid::GetCropjuv(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 5) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))),
		Grid::GetCropjuv(irow, jcol, 1) * (Grid::GetCrop(irow, jcol, 9) / (Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9))) };

		if (isnan(I_treesj[0]) || isinf(I_treesj[0])) { I_treesj[0] = 0.0; }
		if (isnan(I_treesj[1]) || isinf(I_treesj[1])) { I_treesj[1] = 0.0; }
		if (isnan(I_treesj[2]) || isinf(I_treesj[2])) { I_treesj[2] = 0.0; }

		double S_treesj[3] = { Grid::GetCropjuv(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 2) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))), //Get mature adb asymptomatic trees and divide up by EAB category//
			Grid::GetCropjuv(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 6) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))),
		Grid::GetCropjuv(irow, jcol, 2) * (Grid::GetCrop(irow, jcol, 10) / (Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10))) };

		if (isnan(S_treesj[0]) || isinf(S_treesj[0])) { S_treesj[0] = 0.0; }
		if (isnan(S_treesj[1]) || isinf(S_treesj[1])) { S_treesj[1] = 0.0; }
		if (isnan(S_treesj[2]) || isinf(S_treesj[2])) { S_treesj[2] = 0.0; }

		double D_treesj[3] = { Grid::GetCropjuv(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 3) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))), //Get mature adb dead trees and divide up by EAB category//
			Grid::GetCropjuv(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 7) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))),
		Grid::GetCropjuv(irow, jcol, 3) * (Grid::GetCrop(irow, jcol, 11) / (Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11))) };

		if (isnan(D_treesj[0]) || isinf(D_treesj[0])) { D_treesj[0] = 0.0; }
		if (isnan(D_treesj[1]) || isinf(D_treesj[1])) { D_treesj[1] = 0.0; }
		if (isnan(D_treesj[2]) || isinf(D_treesj[2])) { D_treesj[2] = 0.0; }

		double NH_treesm[3], NI_treesm[3], NS_treesm[3], ND_treesm[3]; //Mature tree categories for comapartmental model
		double NH_treesj[3], NI_treesj[3], NS_treesj[3], ND_treesj[3]; //Juvenile tree categories for comapartmental model

		double newDeadm = 0.0; //proportion of mature that die this year
		double newDeadj = 0.0; //proportion of juvenile that die this year

		//We also need to adjust for the larvae moving between ADB status trees//
		double fH_I = 0.0; //Healthy to lightly stressed
		double fI_S = 0.0; //Lightly stressed to heavily stressed
		double fI_D = 0.0;  //Lightly stressed to dead
		double fS_D = 0.0; //Heavily stressed to dead

		//Need totals to split resistance across EAB categories, and calculate the different gamma for S and I trees//
		double totHm = H_treesm[0] + H_treesm[1] + H_treesm[2];
		double totHj = H_treesj[0] + H_treesj[1] + H_treesj[2];

		double totIm = I_treesm[0] + I_treesm[1] + I_treesm[2];
		double totIj = I_treesj[0] + I_treesj[1] + I_treesj[2];

		double totSm = S_treesm[0] + S_treesm[1] + S_treesm[2];
		double totSj = S_treesj[0] + S_treesj[1] + S_treesj[2];

		double gI_m = matgamma, gS_m = matgamma;
		double den_m = totIm + (mortratio * totSm);
		if (den_m > 0.0) { 
			gI_m = matgamma * (totIm + totSm) / den_m; 
			gS_m = mortratio * gI_m;
		}
		if (gS_m > 1.0) { gS_m = 1.0; }

		double gI_j = juvgamma, gS_j = juvgamma;
		double den_j = totIj + (mortratio * totSj);
		if (den_j > 0.0) {
			gI_j = juvgamma * (totIj + totSj) / den_j;
			gS_j = mortratio * gI_j;
		}
		if (gS_j > 1.0) { gS_j = 1.0; }


		for (int icount = 0; icount < 3; icount++)
		{

			double availHm = H_treesm[icount];
			if (totHm > 0.0) {
				availHm -= ResPropm * H_treesm[icount] / totHm;
			}
			if (availHm < 0.0) { availHm = 0.0; }

			double availHj = H_treesj[icount];
			if (totHj > 0.0) {
				availHj -= ResPropj * H_treesj[icount] / totHj;
			}
			if (availHj < 0.0) { availHj = 0.0; }
			

			//Compartmental model for mature and juvenile trees//
			NH_treesm[icount] = H_treesm[icount] + (-adbeta * availHm);
			NI_treesm[icount] = I_treesm[icount] + (adbeta * availHm) - (matalpha * I_treesm[icount]) - (gI_m * I_treesm[icount]);
			NS_treesm[icount] = S_treesm[icount] + (matalpha * I_treesm[icount]) - (gS_m * S_treesm[icount]);
			ND_treesm[icount] = D_treesm[icount] + (gI_m * I_treesm[icount]) + (gS_m * S_treesm[icount]);

			NH_treesj[icount] = H_treesj[icount] + (-adbeta * availHj);
			NI_treesj[icount] = I_treesj[icount] + (adbeta * availHj) - (juvalpha * I_treesj[icount]) - (gI_j * I_treesj[icount]);
			NS_treesj[icount] = S_treesj[icount] + (juvalpha * I_treesj[icount]) - (gS_j * S_treesj[icount]);
			ND_treesj[icount] = D_treesj[icount] + (gI_j * I_treesj[icount]) + (gS_j * S_treesj[icount]);


			//TO DO: Keeping track of tree transitions to track larvae transitioning between ADB statuses (no larvae in 0)//
			if (icount > 0) {
				fH_I += share * (adbeta * availHm) + (1.0 - share) * (adbeta * availHj);
				fI_S += share * (matalpha * I_treesm[icount]) + (1.0 - share) * (juvalpha * I_treesj[icount]);
				fI_D += share * (gI_m * I_treesm[icount]) + (1.0 - share) * (gI_j * I_treesj[icount]);
				fS_D += share * (gS_m * S_treesm[icount]) + (1.0 - share) * (gS_j * S_treesj[icount]);

			}

			newDeadm += (gI_m * I_treesm[icount]) + (gS_m * S_treesm[icount]);
			newDeadj += (gI_j * I_treesj[icount]) + (gS_j * S_treesj[icount]);

			//Pull back if overshot it//
			if (NH_treesm[icount] < 0.0) { NH_treesm[icount] = 0.0; }
			if (NI_treesm[icount] > 1.0) { NI_treesm[icount] = 1.0; }
			else if (NI_treesm[icount] < 0.0) { NI_treesm[icount] = 0.0; }
			if (NS_treesm[icount] > 1.0) { NS_treesm[icount] = 1.0; }
			else if (NS_treesm[icount] < 0.0) { NS_treesm[icount] = 0.0; }
			if (ND_treesm[icount] > 1.0) { ND_treesm[icount] = 1.0; }
			else if (ND_treesm[icount] < 0.0) { ND_treesm[icount] = 0.0; }

			if (NH_treesj[icount] < 0.0) { NH_treesj[icount] = 0.0; }
			if (NI_treesj[icount] > 1.0) { NI_treesj[icount] = 1.0; }
			else if (NI_treesj[icount] < 0.0) { NI_treesj[icount] = 0.0; }
			if (NS_treesj[icount] > 1.0) { NS_treesj[icount] = 1.0; }
			else if (NS_treesj[icount] < 0.0) { NS_treesj[icount] = 0.0; }
			if (ND_treesj[icount] > 1.0) { ND_treesj[icount] = 1.0; }
			else if (ND_treesj[icount] < 0.0) { ND_treesj[icount] = 0.0; }

			//Now setting the Crop by age to read back into the next ash dieback SIR model//
			Grid::Settempmat(irow, jcol, 0 + icount * 4, NH_treesm[icount]);
			Grid::Settempmat(irow, jcol, 1 + icount * 4, NI_treesm[icount]);
			Grid::Settempmat(irow, jcol, 2 + icount * 4, NS_treesm[icount]);
			Grid::Settempmat(irow, jcol, 3 + icount * 4, ND_treesm[icount]);

			Grid::Settempjuv(irow, jcol, 0 + icount * 4, NH_treesj[icount]);
			Grid::Settempjuv(irow, jcol, 1 + icount * 4, NI_treesj[icount]);
			Grid::Settempjuv(irow, jcol, 2 + icount * 4, NS_treesj[icount]);
			Grid::Settempjuv(irow, jcol, 3 + icount * 4, ND_treesj[icount]);

		}

		//Getting the ash dieback status by age again and checking for errors
		double investHm = Grid::Gettempmat(irow, jcol, 0) + Grid::Gettempmat(irow, jcol, 4) + Grid::Gettempmat(irow, jcol, 8);
		double investIm = Grid::Gettempmat(irow, jcol, 1) + Grid::Gettempmat(irow, jcol, 5) + Grid::Gettempmat(irow, jcol, 9);
		double investSm = Grid::Gettempmat(irow, jcol, 2) + Grid::Gettempmat(irow, jcol, 6) + Grid::Gettempmat(irow, jcol, 10);
		double investDm = Grid::Gettempmat(irow, jcol, 3) + Grid::Gettempmat(irow, jcol, 7) + Grid::Gettempmat(irow, jcol, 11);

		//Check for errors in the mature tree calculations//
		double summattest = investHm + investIm + investSm + investDm;
		if (summattest > 1.000000001 ||
			(summattest < 1-0.00000001 && summattest > 0.00000001)||
			summattest < -0.00000001 || 
			(summattest < 0.00000001 && share > 0.000000001)||
		(summattest > 0.00000001 && share < 0.000000001)){
			throw std::logic_error("error in model adbSIRmat");
		}


		//Now set the mature crop for the next round of SIR//
		Grid::SetCropmat(irow, jcol, 0, investHm);
		Grid::SetCropmat(irow, jcol, 1, investIm);
		Grid::SetCropmat(irow, jcol, 2, investSm);
		Grid::SetCropmat(irow, jcol, 3, investDm);

		//Adjust for rounding errors//
		double sumcropmatb = Grid::GetCropmat(irow, jcol, 0) + Grid::GetCropmat(irow, jcol, 1) + Grid::GetCropmat(irow, jcol, 2) + Grid::GetCropmat(irow, jcol, 3);


		if (sumcropmatb > 0.00000001) {
			Grid::SetCropmat(irow, jcol, 0, Grid::GetCropmat(irow, jcol, 0) / (sumcropmatb));
			Grid::SetCropmat(irow, jcol, 1, Grid::GetCropmat(irow, jcol, 1) / (sumcropmatb));
			Grid::SetCropmat(irow, jcol, 2, Grid::GetCropmat(irow, jcol, 2) / (sumcropmatb));
			Grid::SetCropmat(irow, jcol, 3, Grid::GetCropmat(irow, jcol, 3) / (sumcropmatb));
		}



		//Now repeat for juvenile trees!!!//
		double investHj = Grid::Gettempjuv(irow, jcol, 0) + Grid::Gettempjuv(irow, jcol, 4) + Grid::Gettempjuv(irow, jcol, 8);
		double investIj = Grid::Gettempjuv(irow, jcol, 1) + Grid::Gettempjuv(irow, jcol, 5) + Grid::Gettempjuv(irow, jcol, 9);
		double investSj = Grid::Gettempjuv(irow, jcol, 2) + Grid::Gettempjuv(irow, jcol, 6) + Grid::Gettempjuv(irow, jcol, 10);
		double investDj = Grid::Gettempjuv(irow, jcol, 3) + Grid::Gettempjuv(irow, jcol, 7) + Grid::Gettempjuv(irow, jcol, 11);


		//Check for errors in the juvenile tree calculations//
		double sumjuvtest = investHj + investIj + investSj + investDj;
		if (sumjuvtest > 1.000000001 ||
			(sumjuvtest < 1 - 0.00000001 && sumjuvtest > 0.00000001) ||
			sumjuvtest < -0.00000001 ||
			(sumjuvtest < 0.00000001 && (1.0-share) > 0.000000001) ||
			(sumjuvtest > 0.00000001 && (1.0-share) < 0.000000001)) {
			throw std::logic_error("error in model adbSIRjuv");
		}


		//Now set the juvenile crop for the next model timestep//
		Grid::SetCropjuv(irow, jcol, 0, investHj);
		Grid::SetCropjuv(irow, jcol, 1, investIj);
		Grid::SetCropjuv(irow, jcol, 2, investSj);
		Grid::SetCropjuv(irow, jcol, 3, investDj);

		//Adjust for rounding errors//
		double sumcropjuvb = Grid::GetCropjuv(irow, jcol, 0) + Grid::GetCropjuv(irow, jcol, 1) + Grid::GetCropjuv(irow, jcol, 2) + Grid::GetCropjuv(irow, jcol, 3);


		if (sumcropjuvb > 0.00000001) {
			Grid::SetCropjuv(irow, jcol, 0, Grid::GetCropjuv(irow, jcol, 0) / (sumcropjuvb));
			Grid::SetCropjuv(irow, jcol, 1, Grid::GetCropjuv(irow, jcol, 1) / (sumcropjuvb));
			Grid::SetCropjuv(irow, jcol, 2, Grid::GetCropjuv(irow, jcol, 2) / (sumcropjuvb));
			Grid::SetCropjuv(irow, jcol, 3, Grid::GetCropjuv(irow, jcol, 3) / (sumcropjuvb));
		}


		//Storing proportion of trees from the original that are removed for both mature and juvenile ash//
		int s = iyr + lastIdx + 1;
		double Nnow = Grid::GetTreeDensity(irow,jcol);
		Grid::Setnremovedmat(irow, jcol, s, Grid::Getnremovedmat(irow, jcol, s - 1) + newDeadm * (share * Nnow) / (matprop * Ninit)); //Remove for NO ADB model
		Grid::Setnremovedjuv(irow, jcol, s, Grid::Getnremovedjuv(irow, jcol, s - 1) + newDeadj * ((1.0-share) * Nnow) / (juvprop * Ninit)); //Remove for NO ADB model

		//Grid::Setnremovedmat(irow, jcol, s, 0.0); //Add for NO ADB model
		//Grid::Setnremovedjuv(irow, jcol, s, 0.0); //Add for NO ADB model

		if (Grid::Getnremovedmat(irow, jcol, s) < Grid::Getnremovedmat(irow, jcol, s - 1) - 0.0000001 || Grid::Getnremovedmat(irow, jcol, s) > 1.0000001){
			throw std::logic_error("Issue with nremovedmat");
		}

		if (Grid::Getnremovedjuv(irow, jcol, s) < Grid::Getnremovedjuv(irow, jcol, s - 1) - 0.0000001 || Grid::Getnremovedjuv(irow, jcol, s) > 1.0000001) {
			throw std::logic_error("Issue with nremovedjuv");
		}

		//Now do the new SetCrop categories (i.e. both EAB and ADB) putting back in appropriate EAB categories//

		//Set initial Crop//
		Grid::SetCrop(irow, jcol, 0, (Grid::Gettempmat(irow, jcol, 0) * share) + (Grid::Gettempjuv(irow, jcol, 0) * (1.0-share)));
		Grid::SetCrop(irow, jcol, 1, (Grid::Gettempmat(irow, jcol, 1) * share) + (Grid::Gettempjuv(irow, jcol, 1) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 2, (Grid::Gettempmat(irow, jcol, 2) * share) + (Grid::Gettempjuv(irow, jcol, 2) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 3, (Grid::Gettempmat(irow, jcol, 3) * share) + (Grid::Gettempjuv(irow, jcol, 3) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 4, (Grid::Gettempmat(irow, jcol, 4) * share) + (Grid::Gettempjuv(irow, jcol, 4) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 5, (Grid::Gettempmat(irow, jcol, 5) * share) + (Grid::Gettempjuv(irow, jcol, 5) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 6, (Grid::Gettempmat(irow, jcol, 6) * share) + (Grid::Gettempjuv(irow, jcol, 6) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 7, (Grid::Gettempmat(irow, jcol, 7) * share) + (Grid::Gettempjuv(irow, jcol, 7) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 8, (Grid::Gettempmat(irow, jcol, 8) * share) + (Grid::Gettempjuv(irow, jcol, 8) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 9, (Grid::Gettempmat(irow, jcol, 9) * share) + (Grid::Gettempjuv(irow, jcol, 9) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 10, (Grid::Gettempmat(irow, jcol, 10) * share) + (Grid::Gettempjuv(irow, jcol, 10) * (1.0 - share)));
		Grid::SetCrop(irow, jcol, 11, (Grid::Gettempmat(irow, jcol, 11) * share) + (Grid::Gettempjuv(irow, jcol, 11) * (1.0 - share)));


		double sumcropallb = Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 3) +
			Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 7) +
			Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 9) + Grid::GetCrop(irow, jcol, 10) + Grid::GetCrop(irow, jcol, 11);

		//Check for errors//
		if (sumcropallb > 1.000000001 ||
			sumcropallb < 1-0.00000001) {
			throw std::logic_error("error in sumcropallb");
		}

		//Adjust for rounding errors//

		if (sumcropallb > 1.0) {
			Grid::SetCrop(irow, jcol, 0, Grid::GetCrop(irow, jcol, 0) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 1, Grid::GetCrop(irow, jcol, 1) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 2, Grid::GetCrop(irow, jcol, 2) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 3, Grid::GetCrop(irow, jcol, 3) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 4, Grid::GetCrop(irow, jcol, 4) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 5, Grid::GetCrop(irow, jcol, 5) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 6, Grid::GetCrop(irow, jcol, 6) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 7, Grid::GetCrop(irow, jcol, 7) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 8, Grid::GetCrop(irow, jcol, 8) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 9, Grid::GetCrop(irow, jcol, 9) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 10, Grid::GetCrop(irow, jcol, 10) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 11, Grid::GetCrop(irow, jcol, 11) / (sumcropallb));

		}

		if (sumcropallb < 1.0) {
			Grid::SetCrop(irow, jcol, 0, Grid::GetCrop(irow, jcol, 0) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 1, Grid::GetCrop(irow, jcol, 1) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 2, Grid::GetCrop(irow, jcol, 2) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 3, Grid::GetCrop(irow, jcol, 3) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 4, Grid::GetCrop(irow, jcol, 4) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 5, Grid::GetCrop(irow, jcol, 5) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 6, Grid::GetCrop(irow, jcol, 6) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 7, Grid::GetCrop(irow, jcol, 7) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 8, Grid::GetCrop(irow, jcol, 8) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 9, Grid::GetCrop(irow, jcol, 9) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 10, Grid::GetCrop(irow, jcol, 10) / (sumcropallb));
			Grid::SetCrop(irow, jcol, 11, Grid::GetCrop(irow, jcol, 11) / (sumcropallb));
		}

		////////////////////////////////////////////////////////////////////////////////////

		//Now we also have to shift larvae that overwintered on these trees//
		double phloemPerTree = Grid::GetPhloemPerTree();

		double TLarvaemcH = 0.0, TLarvaemcI = 0.0, TLarvaemcS = 0.0, TLarvaemcD = 0.0;

		Grid::GetOWLavmc_H(irow, jcol, TLarvaemcH);
		Grid::GetOWLavmc_I(irow, jcol, TLarvaemcI);
		Grid::GetOWLavmc_S(irow, jcol, TLarvaemcS);
		Grid::GetOWLavmc_D(irow, jcol, TLarvaemcD);

		double numTrees_L = Grid::GetTreeDensity(irow, jcol); //number of trees in each cell recalculated post removal

		//Now we get the number of infested trees before and after the SIR//
		double infBefore[4], infAfter[4];
		infBefore[0] = numTrees_L * (share * (H_treesm[1] + H_treesm[2]) + (1.0 - share) * (H_treesj[1] + H_treesj[2]));
		infBefore[1] = numTrees_L * (share * (I_treesm[1] + I_treesm[2]) + (1.0 - share) * (I_treesj[1] + I_treesj[2]));
		infBefore[2] = numTrees_L * (share * (S_treesm[1] + S_treesm[2]) + (1.0 - share) * (S_treesj[1] + S_treesj[2]));
		infBefore[3] = numTrees_L * (share * (D_treesm[1] + D_treesm[2]) + (1.0 - share) * (D_treesj[1] + D_treesj[2]));

		infAfter[0] = numTrees_L * (Grid::GetCrop(irow, jcol, 4)+ Grid::GetCrop(irow, jcol, 8));
		infAfter[1] = numTrees_L * (Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9));
		infAfter[2] = numTrees_L * (Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10));
		infAfter[3] = numTrees_L * (Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11));

		//Numbers of larvae before model timestep//
		double L_before[4];
		L_before[0] = TLarvaemcH * infBefore[0] * phloemPerTree;
		L_before[1] = TLarvaemcI * infBefore[1] * phloemPerTree;
		L_before[2] = TLarvaemcS * infBefore[2] * phloemPerTree;
		L_before[3] = TLarvaemcD * infBefore[3] * phloemPerTree;

		//Moving larvae according to model transition proportions//
		double mH_I = 0.0, mI_S = 0.0, mI_D = 0.0, mS_D = 0.0;
		if (infBefore[0] > 0.0) { mH_I = L_before[0] * (numTrees_L * fH_I) / infBefore[0]; }
		if (infBefore[1] > 0.0) {
			mI_S = L_before[1] * (numTrees_L * fI_S) / infBefore[1];
			mI_D = L_before[1] * (numTrees_L * fI_D) / infBefore[1];
		}
		if (infBefore[2] > 0.0) { mS_D = L_before[2] * (numTrees_L * fS_D) / infBefore[2]; }


		//Ensure more larvae than in categories are not moved (shouldn't happen - a control here)//
		if (mH_I > L_before[0]) { mH_I = L_before[0]; }
		if (mI_S + mI_D > L_before[1]) {
			double af_mL = L_before[1] / (mI_S + mI_D);
			mI_S = mI_S * af_mL; mI_D = mI_D * af_mL;
		}
		if (mS_D > L_before[2]) { mS_D = L_before[2]; }

		//Updating larval numbers//
		double L_New[4];
		L_New[0] = L_before[0] - mH_I;
		L_New[1] = L_before[1] + mH_I - mI_S - mI_D;
		L_New[2] = L_before[2] + mI_S - mS_D;
		L_New[3] = L_before[3] + mI_D + mS_D;

		//Just check we're not getting a mismatch//
		double sumL = L_before[0] + L_before[1] + L_before[2] + L_before[3];
		double sumNL = L_New[0] + L_New[1] + L_New[2] + L_New[3];

		if (sumL > 0.0 && (sumNL / sumL > 1.000001 || sumNL / sumL < 1 - 0.000001)) {
			throw std::logic_error("Problem moving larvae with ADB transitions");
		}

		//Now adjusting and resetting the density//
		double newDens[4] = { 0.0, 0.0, 0.0, 0.0 };

		//Ok, add a check here just to catch and check that we don't destroy or create larvae before saving//
		for (int k = 0; k < 4; k++) {
			if (infAfter[k] <= 0.0 && L_New[k] > 0.000001) {
				throw std::logic_error("Removed larvae when no hosts in category");
			}

			if (infAfter[k]* phloemPerTree > 0.01 && L_New[k] > 0.0) {
				newDens[k] = L_New[k] / (infAfter[k] * phloemPerTree);

				if (isnan(newDens[k]) || isinf(newDens[k]) || newDens[k] > 1000.0) {
					throw std::logic_error("Issue with larval density");
				}

			}

		}
	

		//Setting the new larval density categories//
		Grid::SetOWLavmc_H(irow, jcol, newDens[0]);
		Grid::SetOWLavmc_I(irow, jcol, newDens[1]);
		Grid::SetOWLavmc_S(irow, jcol, newDens[2]);
		Grid::SetOWLavmc_D(irow, jcol, newDens[3]);

		//Also then adjust the overwintering proportion of larvae per category due to the movement between categories//

		double P_LarvToAdultH = 0.0, P_LarvToAdultI = 0.0, P_LarvToAdultS = 0.0, P_LarvToAdultD = 0.0;
		Grid::GetPLarvH(irow, jcol, P_LarvToAdultH);
		Grid::GetPLarvI(irow, jcol, P_LarvToAdultI);
		Grid::GetPLarvS(irow, jcol, P_LarvToAdultS);
		Grid::GetPLarvD(irow, jcol, P_LarvToAdultD);

		//Ok, we know how many are moving to the different categories adjust the larval overwintering proportions accordingly//

		//The P_LarvToAdultH will obviously stay the same becuase nothing is moving in//

		//Doing I category (obviously only if we have larvae)//
		
		double updatedPLarvI = 0.0;
		if (L_New[1] > 0.0) {
			double original_IP = (L_before[1] - mI_S - mI_D) / L_New[1];
			double new_HP = mH_I / L_New[1];

			//Adding a quick check here//
			if (original_IP + new_HP < 1 - 0.000001 || original_IP + new_HP > 1.000001) {
				throw std::logic_error("Error in adjusting proportions of overwintering larvae");
			}

			updatedPLarvI = (original_IP * P_LarvToAdultI) + (new_HP * P_LarvToAdultH);
			if (updatedPLarvI > 0.95) { updatedPLarvI = 0.95; }
			if (updatedPLarvI < 0.05) { updatedPLarvI = 0.05; }
			Grid::SetPLarvI(irow, jcol, updatedPLarvI);
		}

		//Doing S category//
		
		double updatedPLarvS = 0.0;
		
		if (L_New[2] > 0.0) {
			double original_SP = (L_before[2] - mS_D) / L_New[2];
			double new_IP = mI_S / L_New[2];

			//Adding a quick check here//
			if (original_SP + new_IP < 1 - 0.000001 || original_SP + new_IP > 1.000001) {
				throw std::logic_error("Error in adjusting proportions of overwintering larvae");
			}

			updatedPLarvS = (original_SP * P_LarvToAdultS) + (new_IP * P_LarvToAdultI);
			if (updatedPLarvS > 0.95) { updatedPLarvS = 0.95; }
			if (updatedPLarvS < 0.05) { updatedPLarvS = 0.05; }
			Grid::SetPLarvS(irow, jcol, updatedPLarvS);

		}

		//Doing D category//
		double updatedPLarvD = 0.0;
		
		
		if (L_New[3] > 0.0) {
			double original_DP = L_before[3] / L_New[3];
			double new_IDP = mI_D / L_New[3];
			double new_SDP = mS_D / L_New[3];

			//Adding a quick check here//
			if (original_DP + new_IDP + new_SDP < 1 - 0.000001 || original_DP + new_IDP + new_SDP > 1.000001) {
				throw std::logic_error("Error in adjusting proportions of overwintering larvae");
			}

			updatedPLarvD = (original_DP * P_LarvToAdultD) + (new_IDP * P_LarvToAdultI) + (new_SDP * P_LarvToAdultS);
			if (updatedPLarvD > 0.95) { updatedPLarvD = 0.95; }
			if (updatedPLarvD < 0.05) { updatedPLarvD = 0.05; }
			Grid::SetPLarvD(irow, jcol, updatedPLarvD);

		}

	}
}

void AdjustEABStatusPostFlight(int irow, int jcol, int FellingType, double propFelled) // Felling type:  0 = no felling; 1 = fell EAB infected; 2= sanitation felling; propFelled tells the proportion of ill trees to fell
{
	//###########################################################################
			//Re-adjust tree infection status. We will want to think about this - for now I'm moving last years EAB undetected into detected.
			//And putting any new landings into present undetected. This will need to change

			//Consider those trees with ash die back: 
			// // This is No EAB + Asymptomatic=0, 
			//No EAB + Lightly stessed = 1, 
			//No EAB + Highly stressed=2, 
			//No EAB + Recently dead =3, 
				//EAB present + Asymptomatic =4, 
				//EAB present + Lightly stessed = 5, 
				//EAB present + Highly stressed=6, 
				// EAB present + Recently dead =7, 
					//EAB present (more than a year) + Asymptomatic =8, 
					//EAB present (more than a year) + Lightly stessed = 9, 
					//EAB present (more than a year) + Highly stressed=10, 
					// EAB present (more than a year) + Recently dead =11, 
	double numTrees = Grid::GetTreeDensity(irow, jcol);


	if (numTrees > 0.0)
	{
		if (FellingType == 2) // kill all trees
		{
			double TotalSurvivingTrees = 0.0;
			Grid::SetResProp(irow, jcol, 0.0);
			Grid::SetCrop(irow, jcol, 0, 1); // set proportions to all healthy - we have no trees left! 
			for (int iicount = 0; iicount < Grid::GetMaxNumStatus(); iicount++)
			{
				Grid::SetCrop(irow, jcol, iicount, 0.0);
			}
			Grid::SetTreeDensity(irow, jcol, TotalSurvivingTrees);

		}
		else
		{

			double TAdult = Grid::GetNumG(irow, jcol); // this is after flight not emerge
			double TreeDeath_H = Grid::GetEmergenceDensity_H(irow, jcol) * treekappa; // We want to calculate the emergence density per ADB category so we get a death rate per ADB category //
			double TreeDeath_I = Grid::GetEmergenceDensity_I(irow, jcol) * treekappa; // We want to calculate the emergence density per ADB category so we get a death rate per ADB category //
			double TreeDeath_S = Grid::GetEmergenceDensity_S(irow, jcol) * treekappa; // We want to calculate the emergence density per ADB category so we get a death rate per ADB category //
			double TreeDeath_D = Grid::GetEmergenceDensity_D(irow, jcol) * treekappa; // We want to calculate the emergence density per ADB category so we get a death rate per ADB category //

			//Add the bit here, where if it is nan or inf then the true value is 0.0.
			if (isnan(TreeDeath_H)) { TreeDeath_H = 0.0; } if (isinf(TreeDeath_H)) { TreeDeath_H = 0.0; }
			if (isnan(TreeDeath_I)) { TreeDeath_I = 0.0; } if (isinf(TreeDeath_I)) { TreeDeath_I = 0.0; }
			if (isnan(TreeDeath_S)) { TreeDeath_S = 0.0; } if (isinf(TreeDeath_S)) { TreeDeath_S = 0.0; }
			if (isnan(TreeDeath_D)) { TreeDeath_D = 0.0; } if (isinf(TreeDeath_D)) { TreeDeath_D = 0.0; }


			//Safety to make sure the death rate is capped at 1.0, so we're not killing more trees than we have//
			if (TreeDeath_H > 1.0) { TreeDeath_H = 1.0; }
			if (TreeDeath_I > 1.0) { TreeDeath_I = 1.0; }
			if (TreeDeath_S > 1.0) { TreeDeath_S = 1.0; }
			if (TreeDeath_D > 1.0) { TreeDeath_D = 1.0; }

			//Get the eggs laid by healthy females 
			if (TAdult > 0.0)
			{

				double tempTreeStates[12];
				for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
				{
					tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount);
				}

				//Add up last times EAB status 

				double EnoEAB_H = 0.0;
				double NewInfEAB_H = 0.0;
				double OldInfEAB_H = 0.0;
				double Eaten_H = 0.0;

				double EnoEAB_I = 0.0;
				double NewInfEAB_I = 0.0;
				double OldInfEAB_I = 0.0;
				double Eaten_I = 0.0;

				double EnoEAB_S = 0.0;
				double NewInfEAB_S = 0.0;
				double OldInfEAB_S = 0.0;
				double Eaten_S = 0.0;

				double EnoEAB_D = 0.0;
				double NewInfEAB_D = 0.0;
				double OldInfEAB_D = 0.0;
				double Eaten_D = 0.0;

				//Calculating the differences in attraction between groups//

				double Ldens = Grid::Getlarvdens(irow, jcol); //Gets the larval density per m2 phloem for healthy and dead trees
				if (Ldens < 1.0) {
					Ldens = 1.0;
				}
				//Now calculate beetle attraction to trees based on ash dieback status (Mercador et al., 2011 paper - Figure 1)//
				double Iattraction = (20.468 * pow(Ldens, -0.559));  //Adjust the baseline attraction accordingly here (half basline = *0.5; quarter basline = *0.25)
				double Sattraction = ((20.468 * pow(Ldens, -0.559))) / 2.0; //Adjust the baseline attraction accordingly here (half basline = *0.5; quarter basline = *0.25)

				if (Sattraction < 1.0) { Sattraction = 1.0; }
				if (Iattraction < 1.0) { Iattraction = 1.0; }

				//Calculating the proportion in each category//

				double Hprop = tempTreeStates[0];
				double Iprop = tempTreeStates[1];
				double Sprop = tempTreeStates[2];
				double Dprop = tempTreeStates[3];

				//Overall egg laying probability//
				double totaleablay = numTrees * exp(-P_LayingEgg * TAdult);

				double HnoEAB = (totaleablay * Hprop);
				double InoEAB = (totaleablay * Iprop);
				double SnoEAB = (totaleablay * Sprop);
				double DnoEAB = (totaleablay * Dprop);

				//Now the total infected//
				double Hinf = (numTrees * Hprop) - HnoEAB;
				double Iinf = (numTrees * Iprop) - InoEAB;
				double Sinf = (numTrees * Sprop) - SnoEAB;
				double Dinf = (numTrees * Dprop) - DnoEAB;

				double totalinf = Hinf + Iinf + Sinf + Dinf;
				double nSinf = Sinf * Sattraction;
				double nIinf = Iinf * Iattraction;

				//Setting the maximum number of trees in each status - we need to make sure we don't create
				//trees in any category
				double capacity[4] = { Hprop * numTrees,Iprop * numTrees,Sprop * numTrees,Dprop * numTrees };

				//Attraction weights to different categories//
				double weight[4] = { Hinf,nIinf,nSinf,Dinf };

				double alloc[4] = { 0.0, 0.0, 0.0, 0.0 };
				double wsum = weight[0] + weight[1] + weight[2] + weight[3]; //Total infestation weight sum

				if (wsum > 0.0) {

					for (int k = 0; k < 4; k++) {
						alloc[k] = totalinf * weight[k] / wsum;
						if (alloc[k] > capacity[k]) { alloc[k] = capacity[k]; }
					}
				}

				double nnHinf = alloc[0];
				double nnIinf = alloc[1];
				double nnSinf = alloc[2];
				double nnDinf = alloc[3];

				double newnoEABH = (Hprop * numTrees) - nnHinf;
				double newnoEABI = (Iprop * numTrees) - nnIinf;
				double newnoEABS = (Sprop * numTrees) - nnSinf;
				double newnoEABD = (Dprop * numTrees) - nnDinf;

				////////// Uninfected to infected state////////////////// 
				////////// For Healthy ash trees///////////////////

				EnoEAB_H = newnoEABH;  //trees left uninfested
				NewInfEAB_H = nnHinf; // trees just infested = difference between then and now 

				//test to make sure that is positive 
				if (NewInfEAB_H < 0.0)
				{
					NewInfEAB_H = 0.0;
				}
				OldInfEAB_H = (tempTreeStates[4] + tempTreeStates[8]) * numTrees; // number of trees infested for over a year
				if (FellingType == 1) // we fell a propotion of sick trees
				{
					OldInfEAB_H = OldInfEAB_H * propFelled;
				}
				//Eaten[iADB] = OldInfEAB[iADB] * (1 - treekappa);  // NB comment out
				if (TreeDeath_H > 0.0) {
					Eaten_H = OldInfEAB_H * TreeDeath_H;  // NB swap to this
					OldInfEAB_H = OldInfEAB_H - Eaten_H; // adjust old infected trees for those that have died //
				}


				///////////////// Repeat for ADB Infected ash trees///////////////////////////

				EnoEAB_I = newnoEABI; //trees left uninfested
				NewInfEAB_I = nnIinf; // trees just infested = difference between then and now 
				//test to make sure that is positive 
				if (NewInfEAB_I < 0.0)
				{
					NewInfEAB_I = 0.0;
				}
				OldInfEAB_I = (tempTreeStates[5] + tempTreeStates[9]) * numTrees; // number of trees infested for over a year
				if (FellingType == 1) // we fell a propotion of sick trees
				{
					OldInfEAB_I = OldInfEAB_I * propFelled;
				}
				//Eaten[iADB] = OldInfEAB[iADB] * (1 - treekappa);  // NB comment out
				if (TreeDeath_I > 0.0) {
					Eaten_I = OldInfEAB_I * TreeDeath_I;  // NB swap to this
					OldInfEAB_I = OldInfEAB_I - Eaten_I; // adjust old infected trees for those that have died 
				}

				/////////////////////////// Repeat for ADB stressed ash///////////////

				// For Stressed ash trees
				EnoEAB_S = newnoEABS; //trees left uninfested
				NewInfEAB_S = nnSinf; // trees just infested = difference between then and now 

				//test to make sure that is positive 
				if (NewInfEAB_S < 0.0)
				{
					NewInfEAB_S = 0.0;
				}
				OldInfEAB_S = (tempTreeStates[6] + tempTreeStates[10]) * numTrees; // number of trees infested for over a year
				if (FellingType == 1) // we fell a propotion of sick trees
				{
					OldInfEAB_S = OldInfEAB_S * propFelled;
				}
				//Eaten[iADB] = OldInfEAB[iADB] * (1 - treekappa);  // NB comment out
				if (TreeDeath_S > 0.0) {
					Eaten_S = OldInfEAB_S * TreeDeath_S;  // NB swap to this
					OldInfEAB_S = OldInfEAB_S - Eaten_S; // adjust old infected trees for those that have died
				}

				///////////////// Repeat for ADB dead ash//////////////////////////////////

				EnoEAB_D = newnoEABD; //trees left uninfested
				NewInfEAB_D = nnDinf; // trees just infested = difference between then and now 

				//test to make sure that is positive 
				if (NewInfEAB_D < 0.0)
				{
					NewInfEAB_D = 0.0;
				}
				OldInfEAB_D = (tempTreeStates[7] + tempTreeStates[11]) * numTrees; // number of trees infested for over a year
				if (FellingType == 1) // we fell a propotion of sick trees
				{
					OldInfEAB_D = OldInfEAB_D * propFelled;
				}
				if (TreeDeath_D > 0.0) {
					Eaten_D = OldInfEAB_D * TreeDeath_D;  // NB swap to this
					OldInfEAB_D = OldInfEAB_D - Eaten_D; // adjust old infected trees for those that have died
				}

				if (OldInfEAB_H < 0.0) { OldInfEAB_H = 0.0; }
				if (OldInfEAB_I < 0.0) { OldInfEAB_I = 0.0; }
				if (OldInfEAB_S < 0.0) { OldInfEAB_S = 0.0; }
				if (OldInfEAB_D < 0.0) { OldInfEAB_D = 0.0; }

				double TotalSurvivingTrees = EnoEAB_H + NewInfEAB_H +
					EnoEAB_I + NewInfEAB_I + EnoEAB_S + NewInfEAB_S +
					EnoEAB_D + NewInfEAB_D + OldInfEAB_H + OldInfEAB_I + OldInfEAB_S + OldInfEAB_D;

				//Checking output is heading in the right direction - i.e. we're not creating trees
				if (TotalSurvivingTrees > numTrees + 0.0000000001)
				{
					throw std::logic_error("EAB is creating trees and not killing them");
				}

				if (TotalSurvivingTrees <= 0.0)
				{
					TotalSurvivingTrees = 0.0;
					//	Grid::SetCrop(irow, jcol, 0, 1.0); // set proportions to all healthy - we have no trees left! 
					for (int iicount = 0; iicount < Grid::GetMaxNumStatus(); iicount++)
					{
						Grid::SetCrop(irow, jcol, iicount, 0.0);
					}

					Grid::SetTreeDensity(irow, jcol, 0.0);
					Grid::SetResProp(irow, jcol, 0.0);

				}
				else
				{
					//Converting to proportions//
					double propEnoEAB_H = EnoEAB_H / TotalSurvivingTrees;
					double propEnoEAB_I = EnoEAB_I / TotalSurvivingTrees;
					double propEnoEAB_S = EnoEAB_S / TotalSurvivingTrees;
					double propEnoEAB_D = EnoEAB_D / TotalSurvivingTrees;
					double propNewInfEAB_H = NewInfEAB_H / TotalSurvivingTrees;
					double propNewInfEAB_I = NewInfEAB_I / TotalSurvivingTrees;
					double propNewInfEAB_S = NewInfEAB_S / TotalSurvivingTrees;
					double propNewInfEAB_D = NewInfEAB_D / TotalSurvivingTrees;

					if (propEnoEAB_H < 0.0) { propEnoEAB_H = 0.0; }
					if (propEnoEAB_I < 0.0) { propEnoEAB_I = 0.0; }
					if (propEnoEAB_S < 0.0) { propEnoEAB_S = 0.0; }
					if (propEnoEAB_D < 0.0) { propEnoEAB_D = 0.0; }

					if (propNewInfEAB_H < 0.0) { propNewInfEAB_H = 0.0; }
					if (propNewInfEAB_I < 0.0) { propNewInfEAB_I = 0.0; }
					if (propNewInfEAB_S < 0.0) { propNewInfEAB_S = 0.0; }
					if (propNewInfEAB_D < 0.0) { propNewInfEAB_D = 0.0; }

					//Now do the rest//
					if (OldInfEAB_H / TotalSurvivingTrees <= 0.0) {
						Grid::SetCrop(irow, jcol, 8, 0.0);
					}
					else { Grid::SetCrop(irow, jcol, 8, OldInfEAB_H / TotalSurvivingTrees); }
					
					if (OldInfEAB_I / TotalSurvivingTrees <= 0.0) {
						Grid::SetCrop(irow, jcol, 9, 0.0);
					}
					else { Grid::SetCrop(irow, jcol, 9, OldInfEAB_I / TotalSurvivingTrees); }
						
					if (OldInfEAB_S / TotalSurvivingTrees <= 0.0) {
						Grid::SetCrop(irow, jcol, 10, 0.0);
					}
					else { Grid::SetCrop(irow, jcol, 10, OldInfEAB_S / TotalSurvivingTrees); }
						
					if (OldInfEAB_D / TotalSurvivingTrees <= 0.0) {
						Grid::SetCrop(irow, jcol, 11, 0.0);
					}
					else { Grid::SetCrop(irow, jcol, 11, OldInfEAB_D / TotalSurvivingTrees); }

					//Check the crop adds to one//

					//So this is our new proportion after some of our old trees have died//
					double aneed = propEnoEAB_H + propEnoEAB_I + propEnoEAB_S + propEnoEAB_D +
						propNewInfEAB_H + propNewInfEAB_I + propNewInfEAB_S + propNewInfEAB_D +
						Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 9) + Grid::GetCrop(irow, jcol, 10) + Grid::GetCrop(irow, jcol, 11);

					//Check trees aren't moving between categories//
					if (aneed > 1.000000001|| aneed < 1-0.000000001) {

						throw std::logic_error("Trees shifting categories during EAB death");

					}
					else {


						Grid::SetCrop(irow, jcol, 0, propEnoEAB_H);
						Grid::SetCrop(irow, jcol, 1, propEnoEAB_I);
						Grid::SetCrop(irow, jcol, 2, propEnoEAB_S);
						Grid::SetCrop(irow, jcol, 3, propEnoEAB_D);
						Grid::SetCrop(irow, jcol, 4, propNewInfEAB_H);
						Grid::SetCrop(irow, jcol, 5, propNewInfEAB_I);
						Grid::SetCrop(irow, jcol, 6, propNewInfEAB_S);
						Grid::SetCrop(irow, jcol, 7, propNewInfEAB_D);
						Grid::SetCrop(irow, jcol, 8, Grid::GetCrop(irow, jcol, 8));
						Grid::SetCrop(irow, jcol, 9, Grid::GetCrop(irow, jcol, 9));
						Grid::SetCrop(irow, jcol, 10, Grid::GetCrop(irow, jcol, 10));
						Grid::SetCrop(irow, jcol, 11, Grid::GetCrop(irow, jcol, 11));
					}



					//Junk code here - hangover junk code from earlier debugging - no need to have//
					double newaneed = Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 3) +
						Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 7) +
						Grid::GetCrop(irow, jcol, 8) + Grid::GetCrop(irow, jcol, 9) + Grid::GetCrop(irow, jcol, 10) + Grid::GetCrop(irow, jcol, 11);

					if (newaneed > 1.000000001 || newaneed < 1 - 0.000000001) {
						throw std::logic_error("Stop here 280324");
					}


					//Split the trees killed by EAB according the the ADB age class now to read back into the model later//

					//Calculating those killed for each category (i.e. ash start - ash end)
					double lostAsh[4];
					lostAsh[0] = (tempTreeStates[4] + tempTreeStates[8]) * numTrees - OldInfEAB_H;
					lostAsh[1] = (tempTreeStates[5] + tempTreeStates[9]) * numTrees - OldInfEAB_I;
					lostAsh[2] = (tempTreeStates[6] + tempTreeStates[10]) * numTrees - OldInfEAB_S;
					lostAsh[3] = (tempTreeStates[7] + tempTreeStates[11]) * numTrees - OldInfEAB_D;

					//Reading in the age split//
					double shareM = Grid::GetMatShare(irow, jcol);
					double newMat[4], newJuv[4];
					double sumMat = 0.0;
					double sumJuv = 0.0;

					for (int k = 0; k < 4; k++) {

						if (lostAsh[k] < 0.0) { lostAsh[k] = 0.0; }

						double matInClass = shareM * Grid::GetCropmat(irow, jcol, k) * numTrees;
						double juvInClass = (1.0 - shareM) * Grid::GetCropjuv(irow, jcol, k) * numTrees;
						double allInClass = matInClass + juvInClass;

						double lostMat = 0.0;
						double lostJuv = 0.0;
						if (allInClass > 0.0) {

							lostMat = lostAsh[k] * matInClass / allInClass;
							lostJuv = lostAsh[k] * juvInClass / allInClass;

						}

						newMat[k] = matInClass - lostMat;
						newJuv[k] = juvInClass - lostJuv;

						if (newMat[k] < 0.0) { newMat[k] = 0.0; }
						if (newJuv[k] < 0.0) { newJuv[k] = 0.0; }

						sumMat += newMat[k];
						sumJuv += newJuv[k];

					}

					//Adjust the proportion of resistant ash trees depending on those lost from the H category//
					double allH = (tempTreeStates[0] + tempTreeStates[4] + tempTreeStates[8]) * numTrees;
					double propsurvH = 1.0;

					if (allH > 0.0) {

						propsurvH = 1.0 - (lostAsh[0] / allH);
					}

					if (propsurvH > 1.000000001 || propsurvH < -0.000000001) {
						throw std::logic_error("Stop here propsurvH");
					}

					if (propsurvH < 0.0) { propsurvH = 0.0; }
					if (propsurvH > 1.0) { propsurvH = 1.0; }

					Grid::SetResProp(irow, jcol, Grid::GetResProp(irow, jcol)* propsurvH);

					//Now storing for age categories//

					//Note, we only store if above 0, for 

					if (sumMat > 0.0) {
						for (int k = 0; k < 4; k++) { Grid::SetCropmat(irow, jcol, k, newMat[k] / sumMat); }
					}
					else {
						for (int k = 0; k < 4; k++) { Grid::SetCropmat(irow, jcol, k, 0.0); }
					}

					if (sumJuv > 0.0) {
						for (int k = 0; k < 4; k++) { Grid::SetCropjuv(irow, jcol, k, newJuv[k] / sumJuv); }
					}

					else {
						for (int k = 0; k < 4; k++) { Grid::SetCropjuv(irow, jcol, k, 0.0); }
					}

					//Saving share of mature trees//
					if (sumMat + sumJuv > 0.0) {
						Grid::SetMatShare(irow, jcol, sumMat / (sumMat + sumJuv));
					}


					Grid::SetTreeDensity(irow, jcol, TotalSurvivingTrees);

				}


			}

		}





	}
}

void Flight()
{
	//Disperse first
	//Clear Temp Vector
	Grid::ClearTempBugs();
	int season(1); //summer
	int post(0); //pre=0 post=1
	
	
	BugsJump(); // first see if we have any jumpers
		
//	WriteBugsT(1);
	
	//Bugs Fly 
	for (int icount=0; icount<Grid::GetNumRows(); icount++)
	{
		for (int jcount=0; jcount<Grid::GetNumCols(); jcount++)
		{
				
				double I_Bug=Grid::GetNumG(icount, jcount); 
				if (I_Bug>0)
					Distribute2(icount, jcount, I_Bug);

				//Do we get a jump 
				//1.	Probability one jumps on a car = Function of number of EAB
				//	2.	Distance travelled – sampled from Nathan’s graphic
				//	3.	Direction of travel(assume all equally likely)

		}
	}
	//Set grid of Bugs with new values 


	for (int icount=0; icount<Grid::GetNumRows(); icount++)
	{
		for (int jcount=0; jcount<Grid::GetNumCols(); jcount++)
		{
				Grid::SetNumG(icount, jcount,  Grid::GetTempBug(icount, jcount));  //infected
			//	Grid::SetOWLav(icount, jcount, Grid::GetNumG(icount, jcount));
		}
	}
	
	
	//WriteBugsT(2);
	
}

void BugsJump()
{
	Grid::ClearTempBugs();

	
	double urand[3]; //first is probability that we set a bug, second and third define which cell in which plant we choose
	
	//int ifail = 1;
	//int lstate(0);
	//int subid(1);
	//int* state;
	//state = new int[lstate];
	//int Lseed = 1;
	//int seed[1];
	//seed[0] = 1;
	//G05KGF(3, subid, state, lstate, ifail);
	//G05KFF(1,subid, seed, Lseed, state,lstate,ifail); //always get same vale of random permutaion for a given seed
	//delete[] state;
	//state = new int[lstate];
	//ifail = 1;
	//G05KGF(3, subid, state, lstate, ifail);
	//G05KFF(1,subid, seed, Lseed, state,lstate,ifail);



	// Poisson
	//int nJumps[1];
	//double *r = 0;
	//int lr; 
	//NagError fail;
	int nJumps[1];
	//Bugs Fly 
	for (int icount = 0; icount < Grid::GetNumRows(); icount++)
	{
		for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
		{
			
			double I_Bug = Grid::GetNumG(icount, jcount); // Get number of adults 
			if (I_Bug > 1) // there must be at least one bug
			{
				double Pmean = 0.0002 * I_Bug; // needs lamda to set up generator memory
				//lr = 30 + 20 * sqrt(Pmean) + Pmean;
				//r = NAG_ALLOC(lr, double);
				//INIT_FAIL(fail);
				//g05tjc(Nag_InitializeAndGenerate, 1, Pmean, r, lr, state, nJumps, &fail); // poisson number of jumps
				nJumps[0] = RNG_Poisson(Pmean); // poisson number of jumps
				while (nJumps[0] > 0) { // if jumps happen
					urand[0] = RNG_Uniform();
					urand[1] = RNG_Uniform();
					urand[2] = RNG_Uniform();
						//Sample Distance
					//options
						//Calc inverse of CDF of exponential
						double myDistance = -  47 * log(1 - urand[1]); // Cambridge 20km year for 1997 to 2002; then 47km yeardouble 
						//double myDistance = -  87 * log(1 - urand[1]); // nb changes
						//double myDistance = -log(urand[1])/0.06; // USA
						//double myDistance = 10.125 * ((2 * urand[0] - 1) / urand[1]);//cauchy Russia
						//Sample theta 
						double theta = 2 * 3.141572 * urand[2];
						double myGridLen = Grid::GetGridLen();
						int myX = floor(myDistance * sin(theta) / myGridLen) + jcount;
						int myY = floor(myDistance * cos(theta) / myGridLen) + icount;

						if (myX >= Grid::GetNumCols())
							myX = Grid::GetNumCols() - 1;
						if (myY >= Grid::GetNumRows())
							myY = Grid::GetNumRows() - 1;
						if (myX < 0)
							myX = 0;
						if (myY < 0)
							myY = 0;

						//Look to see if there is a bug and add one 
						double myCrop = Grid::GetTreeDensity(myY, myX); //get treedensity
						int numMove = 50;
						if (I_Bug < numMove)
							numMove = 1;// if not enough bugs then just jump one

						if (myCrop > 0.0 ) //Trees to land on 
						{
							Grid::SetNumG(icount, jcount, I_Bug - numMove); // add catch for incase not 50 beetles there
							double J_Bug = Grid::GetNumG(myY, myX) + numMove;
							//Grid::SetNumG(myY, myX, J_Bug);
							Grid::SetTempBug(myY, myX, J_Bug);
						}
						else
						{ // edit to stop EAB travelling to the beach
							nJumps[0] = nJumps[0] + 1; // if no crop start again - cant jump to sea
						}
					nJumps[0] = nJumps[0] - 1;
				}
				//NAG_FREE(r);


			}
			
			//Do we get a jump 
			//1.	Probability one jumps on a car = Function of number of EAB
			//	2.	Distance travelled – sampled from Nathan’s graphic
			//	3.	Direction of travel(assume all equally likely)

		}
	}
	//Set grid of Bugs with new values 
	//delete[] state;
	//state = NULL;

		
}



void Distribute2(int irow, int jcol,  double num)
{//distribute the Bugs according to beta with parameter gamma and nu
	if (num == 0) //if there are no Bugs don't worry
	{
		return;
	}
	else
	{
		double P_Sum = 0; // to keep track of probability used so that we readjust
		int numSum = 0;
		std::vector<double> MyDist;
		//copy the distribution vector to MyDist
		FlightP::GetBugFlight(MyDist);

		int NRows = Grid::GetNumRows();
		int NCols = Grid::GetNumCols();
		int Len = MyDist.size();


		//central point 

		double NewV(0);
		double myCrop = Grid::GetTreeDensity(irow, jcol); //get treedensity

		int M = num;
		double P = MyDist[0];
		int X[1];
		X[0] = RNG_Binomial(M, P);
		P_Sum = P_Sum + MyDist[0];
		M = M - X[0];


		if (myCrop>0.0) //Trees to land on
		{

			NewV = Grid::GetTempBug(irow, jcol) + X[0];
			Grid::SetTempBug(irow, jcol,  NewV);
			numSum = numSum + X[0];
		}
		else
		{
			flyAgain(irow, jcol);
			NewV = Grid::GetTempBug(irow, jcol) + X[0];
			Grid::SetTempBug(irow, jcol,  NewV);
			numSum = numSum + X[0];
		}

		int sideCount(2);
		int LenCount(3);
		while (Len>LenCount - 1) //next square%
		{
			//sides
			int nrow = irow;
			int ncol = jcol + sideCount - 1;
			Reflect(nrow, ncol);

			double myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - sideCount]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - sideCount];
			M = M - X[0];

			if (X[0] > 0)
			{
				if (myCrop>0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
			}



			nrow = irow;
			ncol = jcol - sideCount + 1;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - sideCount]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - sideCount];
			M = M - X[0];

			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
			}

			nrow = irow + sideCount - 1;
			ncol = jcol;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //Get number in removed section

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - sideCount]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - sideCount];
			M = M - X[0];

			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Then there are trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
			}

			nrow = irow - sideCount + 1;
			ncol = jcol;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - sideCount]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - sideCount];
			M = M - X[0];

			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
			}

			//corners

			nrow = irow + sideCount - 1;
			ncol = jcol + sideCount - 1;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol);//get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - 1]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - 1];
			M = M - X[0];
			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
			}
			//NewV=Grid::GetTempBug(nrow, ncol, health)+MyDist[LenCount-1]*num;
			//Grid::SetTempBug(nrow, ncol, health, NewV);

			nrow = irow + sideCount - 1;
			ncol = jcol - sideCount + 1;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'
			if (X[0] > 0)
			{
				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - 1];
			M = M - X[0];
			if (X[0] > 0)
			{
				if (myCrop > 0.0)//Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
			}

			nrow = irow - sideCount + 1;
			ncol = jcol + sideCount - 1;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - 1]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - 1];
			M = M - X[0];
			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol, NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
			}

			nrow = irow - sideCount + 1;
			ncol = jcol - sideCount + 1;
			Reflect(nrow, ncol);
			myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

			if (P_Sum < 1)
			{
				P = (MyDist[LenCount - 1]) / (1 - P_Sum);
			}
			else
			{
				P = 1;
			}
			if (P>1)
				P = 1;
			X[0] = RNG_Binomial(M, P);
			P_Sum = P_Sum + MyDist[LenCount - 1];
			M = M - X[0];
			if (X[0] > 0)
			{
				if (myCrop > 0.0) //Trees to land on
				{
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
				else
				{
					flyAgain(nrow, ncol);
					NewV = Grid::GetTempBug(nrow, ncol) + X[0];
					Grid::SetTempBug(nrow, ncol,  NewV);
					numSum = numSum + X[0];
				}
			}
			//diagonalsides

			int Mid = sideCount - 2;

			for (int Mcount = 0; Mcount<Mid; Mcount++)
			{
				nrow = irow + Mcount + 1;
				ncol = jcol + sideCount - 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}

				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol, NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow - Mcount - 1;
				ncol = jcol + sideCount - 1;
				Reflect(nrow, ncol);
				double myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol, NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow + Mcount + 1;
				ncol = jcol - sideCount + 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow - Mcount - 1;
				ncol = jcol - sideCount + 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				////
				nrow = irow + sideCount - 1;
				ncol = jcol + Mcount + 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow + sideCount - 1;
				ncol = jcol - Mcount - 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //Get proportion removed

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				//NewV=Grid::GetTempBug(nrow, ncol, health)+MyDist[LenCount-sideCount+Mcount+1]*num;
				//Grid::SetTempBug(nrow, ncol, health, NewV);
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //There is crop to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow - sideCount + 1;
				ncol = jcol + Mcount + 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;

				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
				}

				nrow = irow - sideCount + 1;
				ncol = jcol - Mcount - 1;
				Reflect(nrow, ncol);
				myCrop = Grid::GetTreeDensity(nrow, ncol); //get proportion 'removed'

				if (P_Sum < 1)
				{
					P = (MyDist[LenCount - sideCount + Mcount + 1]) / (1 - P_Sum);
				}
				else
				{
					P = 1;
				}
				if (P>1)
					P = 1;
				X[0] = RNG_Binomial(M, P);
				P_Sum = P_Sum + MyDist[LenCount - sideCount + Mcount + 1];
				M = M - X[0];
				if (X[0] > 0)
				{
					if (myCrop > 0.0) //Trees to land on
					{
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol,  NewV);
						numSum = numSum + X[0];
					}
					else
					{
						flyAgain(nrow, ncol);
						NewV = Grid::GetTempBug(nrow, ncol) + X[0];
						Grid::SetTempBug(nrow, ncol, NewV);
						numSum = numSum + X[0];
					}
				}


			}


			sideCount++; //these keep track of where you are on square see Fig 3 ECBSpatialModel.pdf
			LenCount = LenCount + sideCount;

		}
		//delete[] state;
		//state = NULL;
		if (numSum < num)
		{
			NewV = Grid::GetTempBug(irow, jcol) + num - numSum;
			Grid::SetTempBug(irow, jcol,  NewV);
		}
	}

		

		
}

void Reflect(int& nrow, int& ncol)
{
	int NRows=Grid::GetNumRows();
	int NCols=Grid::GetNumCols(); 
	while((nrow>NRows-1)||(nrow<0))
	{
		if (nrow<0)
		{
		nrow=-1-nrow;
		}
		else if (nrow>NRows-1)
		{
		nrow=2*NRows-1-nrow;
		}
	}
	while((ncol>NCols-1)||(ncol<0))
	{
		if (ncol<0)
		{
			ncol=-1-ncol;
		}
		else if
		(ncol>NCols-1)
		{
		ncol=2*NCols-1-ncol;
		}
	}


}

void flyAgain(int& nrow, int& ncol)
{
	//Find a near by field of maize
	
	int maxDis=Grid::GetNumCols();
	if (maxDis<Grid::GetNumRows()) maxDis=Grid::GetNumRows();
	for (int kcount=1; kcount<maxDis; kcount++)
	for (int irow=nrow-kcount; irow<nrow+kcount+1; irow++)
	{
		for (int jcol=ncol-kcount; jcol<ncol+kcount+1; jcol++)
		{
			if ((irow<Grid::GetNumRows())&&(irow>-1)&&(jcol<Grid::GetNumCols())&&(jcol>-1))
			{
				double myCrop = Grid::GetTreeDensity(irow, jcol);
				//if (Grid::GetCrop(irow, jcol, Grid::GetMaxNumStatus()-1)<0.999999)
				if (myCrop >0.0)
				{
					nrow=irow;
					ncol=jcol;
					return;
				}
			}

		}
	}
	
	throw std::logic_error("No Oranges for the Bugs :-("); 


}

void WriteCrops(int nits)
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char Crop[50];
	int n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeH%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbH(Npath_buffer);


	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeI%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbI(Npath_buffer);


	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeS%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbS(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeD%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbD(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\ashstart%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFashstart(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\ashend%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFashend(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeMatH%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbMatH(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeMatI%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbMatI(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeMatS%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbMatS(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeMatD%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbMatD(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeJuvH%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbJuvH(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeJuvI%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbJuvI(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeJuvS%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbJuvS(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\newadbTreeJuvD%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbJuvD(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeJuvR%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbJuvR(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	n = sprintf_s(Crop, 50, "\\OutFiles\\adbTreeMatR%d.txt", nits);
	strcat_s(Npath_buffer, Crop);
	std::ofstream OutFadbMatR(Npath_buffer);


	if ((OutFadbH) && (OutFadbI) && (OutFadbS) && (OutFadbD) && (OutFashstart) && (OutFashend) && (OutFadbMatH) && (OutFadbMatI) && (OutFadbMatS) && (OutFadbMatD)
		&& (OutFadbJuvH) && (OutFadbJuvI) && (OutFadbJuvS) && (OutFadbJuvD) && (OutFadbMatR) && (OutFadbJuvR)) //&& (OutFadbR))
	{

		for (int jcount = 0; jcount < Grid::GetNumRows(); jcount++)
		{
			for (int icount = 0; icount < Grid::GetNumCols(); icount++)
			{
				OutFadbH << Grid::GetCrop(jcount, icount, 0) + Grid::GetCrop(jcount, icount, 4) + Grid::GetCrop(jcount, icount, 8) << '\t';
				OutFadbI << Grid::GetCrop(jcount, icount, 1) + Grid::GetCrop(jcount, icount, 5) + Grid::GetCrop(jcount, icount, 9) << '\t';
				OutFadbS << Grid::GetCrop(jcount, icount, 2) + Grid::GetCrop(jcount, icount, 6) + Grid::GetCrop(jcount, icount, 10) << '\t';
				OutFadbD << Grid::GetCrop(jcount, icount, 3) + Grid::GetCrop(jcount, icount, 7) + Grid::GetCrop(jcount, icount, 11) << '\t';

				OutFashstart << Grid::GetInitialTreeDensity(jcount, icount) << '\t';
				OutFashend << Grid::GetTreeDensity(jcount, icount) << '\t';

				OutFadbMatH << Grid::GetCropmat(jcount, icount, 0) << '\t';
				OutFadbMatI << Grid::GetCropmat(jcount, icount, 1) << '\t';
				OutFadbMatS << Grid::GetCropmat(jcount, icount, 2) << '\t';
				OutFadbMatD << Grid::GetCropmat(jcount, icount, 3) << '\t';

				OutFadbJuvH << Grid::GetCropjuv(jcount, icount, 0) << '\t';
				OutFadbJuvI << Grid::GetCropjuv(jcount, icount, 1) << '\t';
				OutFadbJuvS << Grid::GetCropjuv(jcount, icount, 2) << '\t';
				OutFadbJuvD << Grid::GetCropjuv(jcount, icount, 3) << '\t';

				int adbyear = Grid::Getadbarrival(jcount, icount);
				if (adbyear > -1) {
					OutFadbMatR << Grid::Getnremovedmat(jcount, icount, 13 - adbyear - 3) << '\t';
					OutFadbJuvR << Grid::Getnremovedjuv(jcount, icount, 13 - adbyear - 3) << '\t';
				}
				else {
					OutFadbMatR << -9999.0 << '\t';
					OutFadbJuvR << -9999.0 << '\t';
				}

			}
			OutFadbH << '\n';
			OutFadbI << '\n';
			OutFadbS << '\n';
			OutFadbD << '\n';

			OutFashstart << '\n';
			OutFashend << '\n';

			OutFadbMatH << '\n';
			OutFadbMatI << '\n';
			OutFadbMatS << '\n';
			OutFadbMatD << '\n';
			OutFadbMatR << '\n';

			OutFadbJuvH << '\n';
			OutFadbJuvI << '\n';
			OutFadbJuvS << '\n';
			OutFadbJuvD << '\n';
			OutFadbJuvR << '\n';

		}


	}
	else
		throw std::logic_error("Error in write crops");


}

void SaveDataInStruc(double SaveStructure[], double SaveStructureT[], double SaveStructureH[], double SaveStructureI[], double SaveStructureS[], double SaveStructureD[], int tcount)
{
	int Nrows = Grid::GetNumRows();
	int Ncols = Grid::GetNumCols();
	int countcell = 0;
	for (int irow = 0; irow < Nrows; irow++)
	{
		for (int jcol = 0; jcol < Ncols; jcol++)
		{
			double myTreeDensity = Grid::GetTreeDensity(irow, jcol);
			double myIniTree = Grid::GetInitialTreeDensity(irow, jcol);
			if (myTreeDensity > 0)
			{
				// This needs to be some function of probability to detect	
				double tempTreeStates[12];
				for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
				{
					tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount); // These are proportions
				}

				//Get Infected trees 
				double propENoEAB = tempTreeStates[0] + tempTreeStates[1] + tempTreeStates[2] + tempTreeStates[3];
				if (propENoEAB > 1)
					propENoEAB = 1.0; // stops rounding errors 
				double propEInfGTYr = tempTreeStates[8] + tempTreeStates[9] + tempTreeStates[10] + tempTreeStates[11]; // GT a year of EAB
				double NumTreesHealthy = Grid::GetTreeDensity(irow, jcol) * propENoEAB;  // NB Tree Density is actually number !
				double NumTreesInfested = Grid::GetTreeDensity(irow, jcol) * (1 - propENoEAB);
				
				double adbH = Grid::GetCrop(irow, jcol, 0) + Grid::GetCrop(irow, jcol, 4) + Grid::GetCrop(irow, jcol, 8);
				double adbI = Grid::GetCrop(irow, jcol, 1) + Grid::GetCrop(irow, jcol, 5) + Grid::GetCrop(irow, jcol, 9);
				double adbS = Grid::GetCrop(irow, jcol, 2) + Grid::GetCrop(irow, jcol, 6) + Grid::GetCrop(irow, jcol, 10);
				double adbD = Grid::GetCrop(irow, jcol, 3) + Grid::GetCrop(irow, jcol, 7) + Grid::GetCrop(irow, jcol, 11);

				double myNumLarvae(0);
				double c_parameter(0.026); // Girdelled 0.5096; Traps 0.052; //visual = 0.026
				double NumTreeInspect(10);
				Grid::GetOWLav(irow, jcol, myNumLarvae);
				double phloemPerTree = Grid::GetPhloemPerTree();
				double myDetect = 0;

				/////////////////////////////////////////////////////////////////////////
				double P_LarvToAdult(0.0);
				Grid::GetPLarv(irow, jcol, P_LarvToAdult);
				///////////////////////////////////////////////////////////////////////
				double AveLavPerTree = myNumLarvae * phloemPerTree * NumTreesInfested / (myTreeDensity * P_LarvToAdult);
				//double MCBUG = Grid::GetNumG(irow, jcol);
				//double MCLAV = myNumLarvae * phloemPerTree * NumTreesInfested;
				int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
				SaveStructure[indx] = AveLavPerTree;
				//SaveStructure[indx] = MCBUG;
				SaveStructureT[indx] = myTreeDensity;
				SaveStructureH[indx] = adbH;
				SaveStructureI[indx] = adbI;
				SaveStructureS[indx] = adbS;
				SaveStructureD[indx] = adbD;


			}
			else
			{
				int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
				SaveStructure[indx] = 0;
				SaveStructureT[indx] = 0;
				SaveStructureH[indx] = 0;
				SaveStructureI[indx] = 0;
				SaveStructureS[indx] = 0;
				SaveStructureD[indx] = 0;

			}
		}
	}

}

void WriteDetectionInforOpt_Track(int occassion, int infRow, int infCol, double myWeight, int MaxYrs, double SaveStructure[], double SaveStructureT[],
	double SaveStructureH[], double SaveStructureI[], double SaveStructureS[], double SaveStructureD[])
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	strcpy_s(ECBStr, "\\OutFiles\\DataForOptJuneRun1.txt");
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer, std::ifstream::app);
	if (OutF)
	{
		int Nrows = Grid::GetNumRows();
		int Ncols = Grid::GetNumCols();


		for (int irow = 0; irow < Nrows; irow++)
		{
			for (int jcol = 0; jcol < Ncols; jcol++)
			{

				double myIniTree = Grid::GetInitialTreeDensity(irow, jcol);
				if (myIniTree > 0)
				{
					double scount = .0;
					for (int iicount = 0; iicount < MaxYrs; iicount++)
					{
						int indx = iicount * Nrows * Ncols + irow * Ncols + jcol;
						scount = scount + SaveStructure[indx];
					}
					if (scount > 0)
					{
						double myIniTree = Grid::GetInitialTreeDensity(irow, jcol);
						//OutF << infRow * Ncols + infCol << '\t' << myWeight << '\t' << irow * Ncols + jcol;
						OutF << occassion <<'\t' << infRow << '\t' << infCol << '\t' << myWeight << '\t' << myIniTree << '\t' << irow << '\t' << jcol;

						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructure[indx];
						}
						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructureT[indx];
						}
						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructureH[indx];
						}
						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructureI[indx];
						}
						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructureS[indx];
						}
						for (int tcount = 0; tcount < MaxYrs; tcount++)
						{
							int indx = tcount * Nrows * Ncols + irow * Ncols + jcol;
							OutF << '\t' << SaveStructureD[indx];
						}

						OutF << '\n';
					}
					//else
					//{
						//throw std::logic_error("Bum2");
					//}
				}
				//else
				//{
					//throw std::logic_error("Bum");
				//}

			}
		}
		OutF.flush();
		OutF.close();
	}


}

void FinalSumLarv(int imth, double inputvar[202][301]) {
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\NLarvaeMC%d.txt", imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF) {
		for (int rcount = 0; rcount < 202; rcount++) {

			for (int ccount = 0; ccount < 301; ccount++) {

				OutF << inputvar[rcount][ccount] << '\t';
				OutF.flush();
			}
		OutF << '\n';
	}

	}

	else {
		throw std::logic_error("Error in write Larvae");
	}

}


void WriteSumBug(int imth, int repeat) {
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\SumBugMC%d_%d.txt", imth,repeat);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF) {


		OutF << Grid::GetSumBug(imth) << '\t';


	}

	else {
		throw std::logic_error("Error in write Larvae");
	}

}

void WriteCountBug(int imth, int repeat) {
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\CountBugMC%d.txt", imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF) {


		OutF << Grid::GetCountBug(imth) << '\t';


	}

	else {
		throw std::logic_error("Error in write Larvaeb");
	}

}

void WriteSumAsh(int imth, int repeat) {
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\SumashMC%d_%d.txt", imth, repeat);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF) {


		OutF << Grid::Getnumberash(imth) << '\t';


	}

	else {
		throw std::logic_error("Error in write Dead");
	}

}


void WriteCountDead(int imth, int repeat) {
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\SumDeadMC%d.txt", imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF) {


		OutF << Grid::GetCountDead(imth) << '\t';


	}

	else {
		throw std::logic_error("Error in write Deadb");
	}

}



void WriteLarvT(int imth)
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\LarvsT%d.txt", imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);
	if (OutF)
	{
		for (int jcount = 0; jcount < Grid::GetNumRows(); jcount++)
		{
			for (int icount = 0; icount < Grid::GetNumCols(); icount++)
			{

				double totPop(0);
				 Grid::GetOWLav(jcount, icount, totPop ); //healthy 
				double testlarv = totPop;
				if (testlarv > 0.0) {
					double junk = 0.0;
				}
				 OutF << totPop << '\t';
				OutF.flush();

			}
			OutF << '\n';
		}
	}
	else
		throw std::logic_error("Error in write Larvae");

}

void WriteBugsT(int imth)
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd( path_buffer, _MAX_PATH );
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
    int n=sprintf_s(ECBStr, 50, "\\OutFiles\\BugsT%d.txt", imth);
	strcat_s(Npath_buffer, ECBStr);
		
	std::ofstream OutF(Npath_buffer);
	if (OutF)
	{
		for (int jcount=0; jcount<Grid::GetNumRows(); jcount++)
		{
			for (int icount=0; icount<Grid::GetNumCols(); icount++)
			{
		
			    double totPop;
				totPop=Grid::GetNumG(jcount, icount ); //healthy 
				OutF<<totPop<<'\t';
				OutF.flush();

			}
			OutF<<'\n';
		}
	}
	else
		throw std::logic_error("Error in write bugs");

}


double ProbabilityofDetection(int LookHere[][ConstDefNumLocs], int NumLocs)
{
	double ProbDetect(0);

	for (int icount = 0; icount < NumLocs; icount++)
	{
		ProbDetect= ProbDetect + Grid::GetProbDetect(LookHere[0][icount], LookHere[1][icount]); //Number of Bugs
		
		//Grid::GetOWLav(LookHere[0][icount], LookHere[1][icount], totPop); //healthy 

	}

	return ProbDetect;


}


void DetectionFullRunW_Target(double RandIf, double kill_rate,  int ConstMaxInvYr)
{
	int iflag(0);
	//This function reads in the entry locations and runs the simulation for 8 years
	//The output is used in probability of detection in simulated annealing in a seperate code
	
	int MaxGridS = Grid::GetNumRows() * Grid::GetNumCols() * ConstMaxInvYr;
	double* mySaveStructure;
	double* mySaveStructureTree;
	double* mySaveStructureHealthy;
	double* mySaveStructureInfected;
	double* mySaveStructureSymptomatic;
	double* mySaveStructureDead;


	mySaveStructure = new double[MaxGridS];
	mySaveStructureTree = new double[MaxGridS];
	mySaveStructureHealthy = new double[MaxGridS];
	mySaveStructureInfected = new double[MaxGridS];
	mySaveStructureSymptomatic = new double[MaxGridS];
	mySaveStructureDead = new double[MaxGridS];

	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	strcpy_s(ECBStr, "\\TreeDensity\\SampleEntry.txt"); //This is a list of likely entry points. They resampled so repeat
	strcat_s(Npath_buffer, ECBStr);
	int myX, myY;
	int Occassion(0);
	
	std::ifstream InFme(Npath_buffer);
	if(InFme)
	{ 
		int LazyAlice = 1; //length of sample input file //From 10000
		for (int jcount = 0; jcount < LazyAlice; jcount++)
		{
				InFme>>myX;
				InFme>>myY;

				double TempTreeDensity = Grid::GetInitialTreeDensity(myY, myX);
				int iniiiADBarrival = Grid::Getadbarrival(myY, myX);
				//double adbpresent = Grid::GetCrop(myY, myX, 0) + Grid::GetCrop(myY, myX, 1) + Grid::GetCrop(myY, myX, 2) + Grid::GetCrop(myY, myX, 3) +
					//Grid::GetCrop(myY, myX, 4) + Grid::GetCrop(myY, myX, 5) + Grid::GetCrop(myY, myX, 6) + Grid::GetCrop(myY, myX, 7) +
					//Grid::GetCrop(myY, myX, 8) + Grid::GetCrop(myY, myX, 9) + Grid::GetCrop(myY, myX, 10) + Grid::GetCrop(myY, myX, 11);
				if (TempTreeDensity > 0&& iniiiADBarrival>-1)
				{
					Grid::ReSetLandBasic(myY, myX, iflag);

					double myWeight = Grid::GetInclustionProb(myY, myX);
					if (myWeight > 0) // if there is a chance this could be an entry point then run - else don't bother This is now a bug check!
					{
						for (int tcount = 0; tcount < ConstMaxInvYr; tcount++)
						{
							ModelMonth(tcount, RandIf, kill_rate);
							// create function to set probability of detection for each cell year
							SaveDataInStruc(mySaveStructure, mySaveStructureTree, mySaveStructureHealthy, mySaveStructureInfected, mySaveStructureSymptomatic, mySaveStructureDead, tcount);
						}
						WriteDetectionInforOpt_Track(Occassion, myY, myX, myWeight, ConstMaxInvYr, mySaveStructure, mySaveStructureTree, mySaveStructureHealthy, mySaveStructureInfected, mySaveStructureSymptomatic, mySaveStructureDead);
						Occassion = Occassion + 1;
					}
					else
						throw std::logic_error("Risk is zero please check");
				}
			
		}


	}
	else
		throw std::logic_error("Error in Read Samples");

	delete[] mySaveStructure;
	mySaveStructure = NULL;
	delete[] mySaveStructureTree;
	mySaveStructureTree = NULL;
	delete[] mySaveStructureHealthy;
	mySaveStructureHealthy = NULL;
	delete[] mySaveStructureInfected;
	mySaveStructureInfected = NULL;
	delete[] mySaveStructureSymptomatic;
	mySaveStructureSymptomatic = NULL;
	delete[] mySaveStructureDead;
	mySaveStructureDead = NULL;



}

void WriteAveLarvae(int imth, int Repeat)
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\AveLarvae%d_%d.txt",Repeat, imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	double phloemPerTree = Grid::GetPhloemPerTree();
	if (OutF)
	{
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{		
				double AveLavPerTree = 0.0;
				
				if (Grid::GetTreeDensity(icount, jcount) > 0.0)
				{
					double TreeDensity = Grid::GetTreeDensity(icount, jcount);
					// infected
					double tempTreeStates[12];
					for (int ii = 0; ii < Grid::GetMaxNumStatus(); ii++)
					{
						tempTreeStates[ii] = Grid::GetCrop(icount, jcount, ii); // These are proportions
					}

					//Get Infected trees 
					double propENoEAB = tempTreeStates[0] + tempTreeStates[1] + tempTreeStates[2] + tempTreeStates[3];
					if (propENoEAB > 1)
						propENoEAB = 1;
					double NumTreesInfested = TreeDensity * (1.0 - propENoEAB);

					double myNumLarvae(0.0);
					Grid::GetOWLav(icount, jcount, myNumLarvae);

					double P_LarvToAdult(0.0);
					Grid::GetPLarv(icount, jcount, P_LarvToAdult);
					AveLavPerTree = myNumLarvae * phloemPerTree * NumTreesInfested / (TreeDensity * P_LarvToAdult);
				}

					 
				OutF << AveLavPerTree << '\t';
				OutF.flush();

			}
			OutF << '\n';
		}
	}
	else
		throw std::logic_error("Error in write AveLarvaePerTree");

}

void WritePropDead(int imth, int Repeat)
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	int n = sprintf_s(ECBStr, 50, "\\OutFiles\\PropDeadAsh%d_%d.txt",Repeat, imth);
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	if (OutF)
	{
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double PropDead = 0.0;

				if (Grid::GetInitialTreeDensity(icount, jcount) > 0.0)
				{
					double TreeDensity = Grid::GetTreeDensity(icount, jcount);
					double InitialDensity = Grid::GetInitialTreeDensity(icount, jcount);

					PropDead = 1- TreeDensity / InitialDensity;
				}


				OutF << PropDead << '\t';
				OutF.flush();

			}
			OutF << '\n';
		}
	}
	else
		throw std::logic_error("Error in write ProportionAshDead");

}

void WriteProbDetectForR()
{
	char path_buffer[_MAX_PATH];
	char Npath_buffer[_MAX_PATH];
	_getcwd(path_buffer, _MAX_PATH);
	strcpy_s(Npath_buffer, path_buffer);
	char ECBStr[50];
	strcpy_s(ECBStr, 50, "\\OutFiles\\ProbDetectForR.txt");
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutF(Npath_buffer);

	strcpy_s(Npath_buffer, path_buffer);
	strcpy_s(ECBStr, 50, "\\OutFiles\\ProbDetectForMat.txt");
	strcat_s(Npath_buffer, ECBStr);

	std::ofstream OutFF(Npath_buffer);

	if (OutF)
	{
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double myprobDetect=Grid::GetProbDetect(icount, jcount);

				OutF <<icount << '\t' << jcount << '\t'<< myprobDetect<<'\n';
				OutF.flush();
				OutFF <<  myprobDetect << '\t';
				OutFF.flush();

			}
			OutFF << '\n';
		}
	}
	else
		throw std::logic_error("Error in write ProbDetectForR");

}