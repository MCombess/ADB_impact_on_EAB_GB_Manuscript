#ifndef __EABGrid_H__
#define __EABGrid_H__

#include <vector>
#include "EABPortable.h"
#include "EABRandom.h"

namespace Grid
{
	int GetNumRows();
	int GetNumCols();
	double GetPropOverWinterLarvaeStart(); //Starting proportion of EAB larvae completing cycle in one year

	//Setting and getting the emergence density of EAB larvae by ADB severity category//
	void SetEmergenceDensity_H(int irow, int jcol, double Stype);
	void SetEmergenceDensity_I(int irow, int jcol, double Stype);
	void SetEmergenceDensity_S(int irow, int jcol, double Stype);
	void SetEmergenceDensity_D(int irow, int jcol, double Stype);
	
	double GetEmergenceDensity_H(int irow, int jcol);
	double GetEmergenceDensity_I(int irow, int jcol);
	double GetEmergenceDensity_S(int irow, int jcol);
	double GetEmergenceDensity_D(int irow, int jcol);

	//Getting and setting numbers 
	double GetNumG(int irow, int jcol); //get total number of adults in a cell
	void SetNumG(int irow, int jcol, double NewNum); //set total number of bugs
	
	void SetTempBug(int irow, int jcol,  double NewNum);
	double GetTempBug(int irow, int jcol);
	double GetGridLen();
	
	//Setting and getting overwintering larval density
	void SetOWLav(int irow, int jcol, double totPopp);
	void GetOWLav(int irow, int jcol,  double& totPop);
	
	//Setting and getting overwintering larval density by ADB severity category//
	void SetOWLavmc_H(int irow, int jcol, double totPoppmc_H);
	void GetOWLavmc_H(int irow, int jcol, double& totPopmc_H);

	void SetOWLavmc_I(int irow, int jcol, double totPoppmc_I);
	void GetOWLavmc_I(int irow, int jcol, double& totPopmc_I);

	void SetOWLavmc_S(int irow, int jcol, double totPoppmc_S);
	void GetOWLavmc_S(int irow, int jcol, double& totPopmc_S);

	void SetOWLavmc_D(int irow, int jcol, double totPoppmc_D);
	void GetOWLavmc_D(int irow, int jcol, double& totPopmc_D);

	// Setting and getting proportion of univoltine larvae 
	void GetPLarv(int irow, int jcol, double& P_LarvToAdult); 
	void SetPLarv(int irow, int jcol, double P_LarvToAdult);
	
	// Setting and getting proportion of univoltine larvae by ADB severity
	void GetPLarvH(int irow, int jcol, double& P_LarvToAdultH);
	void SetPLarvH(int irow, int jcol, double P_LarvToAdultH);

	void GetPLarvI(int irow, int jcol, double& P_LarvToAdultI);
	void SetPLarvI(int irow, int jcol, double P_LarvToAdultI);

	void GetPLarvS(int irow, int jcol, double& P_LarvToAdultS);
	void SetPLarvS(int irow, int jcol, double P_LarvToAdultS);

	void GetPLarvD(int irow, int jcol, double& P_LarvToAdultD);
	void SetPLarvD(int irow, int jcol, double P_LarvToAdultD);



	//Gets and sets the larval density per m2 phloem for healthy and dead (in last 3 years) trees
	double Getlarvdens(int irow, int jcol);
	void Setlarvdens(int irow, int jcol, double dens);

	//Getting and setting trees status
	void SetCrop(int irow, int jcol, int myStat, double per); // sets proportion of trees at a given ADB status
	double GetCrop(int irow, int jcol, int myStat);

	//Setting and getting removed ash by age category
	void Setnremovedmat(int irow, int jcol, int nyrs, double per);
	double Getnremovedmat(int irow, int jcol, int nyrs);

	void Setnremovedjuv(int irow, int jcol, int nyrs, double per);
	double Getnremovedjuv(int irow, int jcol, int nyrs);
	
	//Setting and getting proportion of mature ash
	void SetMatShare(int irow, int jcol, double share);
	double GetMatShare(int irow, int jcol);

	//Setting and getting a small proportion of ash resistant in a cell//
	void SetResProp(int irow, int jcol, double prop);
	double GetResProp(int irow, int jcol);

	int GetMaxNumStatus(); // retuns the number of ADB status a tree can be in , i.e. healthy, symptomatic, heavily sympomatic, recently dead
	
	//Temporary storage of mature and juvenile variables
	void Settempmat(int irow, int jcol, int myStat, double per); // sets proportion of trees at a given ADB status
	double Gettempmat(int irow, int jcol, int myStat);

	void Settempjuv(int irow, int jcol, int myStat, double per); // sets proportion of trees at a given ADB status
	double Gettempjuv(int irow, int jcol, int myStat);

	//Tree ADB status by age
	void SetCropmat(int irow, int jcol, int myStat, double per); // sets proportion of trees at a given ADB status
	double GetCropmat(int irow, int jcol, int myStat);

	void SetCropjuv(int irow, int jcol, int myStat, double per); // sets proportion of trees at a given ADB status
	double GetCropjuv(int irow, int jcol, int myStat);

	//These are related to number of cells in plantations might need revising for our needs
	int GetNumCells(int PlantaCount); //returns number of grid cells in a plantation. Probably always 4
	int GetCellLocation(int PlantaCount, int cellCount); // this returns a number representing location icol*numRows+irow
	
	//Set up landscape comands
	void SetLand(int newL, int& iflag); 
	void SetLandBasic(int& iflag);
	void ReSetLandBasic(int infCelljRow, int infCelliCol, int& iflag);
		
	void SetPlanta(int irow, int jcol, int Ctype);
	void SetTreeDensity(int irow, int jcol, double Ctype); // Setting the ash within a cell
	void SetInitialTreeDensity(int irow, int jcol, double Ctype); // Setting the ash originally within a cell
	
	void Setadbarrival(int irow, int jcol, int temparrival); //if want to set ash dieback arrival date

	void Setadbsoil(int irow, int jcol, int tempadbsoil); //if want to set soil drainage ash dieback

	void Setadbeusoil(int irow, int jcol, double tempadbeusoil); //if want to set soil water content ash dieback

	void Setnonwoodp(int irow, int jcol, double tempnonwoodp); //if want to set non-wood/ hedgerow proportion ash dieback
	
	void Setadbmatgamma(int irow, int jcol, double tempadbmatgamma); //if want to set Gamma

	void Setadbsoileff(int irow, int jcol, double tempadbsoileff); //if want to set the soil effect
	
	void Setadbremoved(int irow, int jcol, double per); // sets proportion of all trees removed
	double Getadbremoved(int irow, int jcol); // gets proportion of all trees removed

	double GetSumBug(int imth);
	int GetCountBug(int imth);

	double Getnumberash(int imth);
	int GetCountDead(int imth);

	//Get landscape info
	
	double GetTreeDensity(int irow, int jcol);  // This is now tree density needsGetCHMA
	double GetInitialTreeDensity(int irow, int jcol);  // This is now tree density needsGetCHMA
	
	int Getadbarrival(int irow, int jcol); //gets ash dieback arrival date

	int Getadbsoil(int irow, int jcol); //gets soil drainage ash dieback

	double Getadbeusoil(int irow, int jcol); //gets soil water content ash dieback

	double Getnonwoodp(int irow, int jcol); //gets non-wood/ hedgerow proportion ash dieback

	double Getprecipp(int irow, int jcol); //if want to get the precipation difference value for a cell (see paper)
	
	double Getadbmatgamma(int irow, int jcol); //if want to get Gamma

	double Getadbsoileff(int irow, int jcol); //if want to get the soil effect



	//Apply felling or not
	void GetTreeFellingStats(int icount, int jcount, int& FellingType, double& propFelled); // Felling type:  0 = no felling; 1 = fell EAB infected; 2= sanitation felling; propFelled tells the proportion of ill trees to fell
	void SetTreeFellingStats(int icount, int jcount, int FellingType, double propFelled); // Felling type:  0 = no felling; 1 = fell EAB infected; 2= sanitation felling; propFelled tells the proportion of ill trees to fell

	void SetInclustionProb(int jrow, int icol, double IncPr); //Probability that this is the entry location of the first infection 
	double GetInclustionProb(int jrow, int icol); //Probability that this is the entry location of the first infection 

	void ClearData();
	void ClearTempBugs();
	
	void SetIntVars(double jdist, double gridLen, double lambda);
	void GetIntVars(double& jdist, double& gridLen, double& lambda);
	
		
	//Comands to track invasion may not need
	void SetInvasion(int irow, int jcol, int yr);
	int GetInvasion(int irow, int jcol);
	double GetPhloemPerTree();
	void SetTotalPlantasFilled(int mycount);
	int GetTotalPlantasFilled();

	//OptimisationStff
	void ReSetProbDetection();
	void SetProbDetection();
	void SetWeightedProbDetection(double myWeight); // DO NOT MIX 
	double GetProbDetect(int irow, int jcol);
	
	//ADB
	void initialiseadbmatgamma();
	void initialiseadbsoileff();
	void initialiseadbstart(int irow, int jcol);

}

namespace FlightP
{
	void ClearData();
	void Initialise();
	void GetBugFlight(std::vector<double>& MyDist);


}

double Integrate(int idist, int jdist, double lambda);


void GridIDToCoord(int Floc, int& irow, int& icol);
int CoordToGridID(int irow, int icol);

//void 
//extern "C" void __stdcall fFunc2(const int& NDIM, const double x[], const int& NumFunc, double funcY[] );






#endif