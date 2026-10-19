#include "EABGrid.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <direct.h>



//int const NumCropTypes=4;
int const MaxNumNeighbours=50;
int const MaxNumPCells=12169; //maximum number cells per plantation



	
typedef double ThreeDoubles[3];

class PInfo
{
public:
	PInfo()
	{
	//	BlockNum=-1;
		DisStatus=0; //We assume the worry level is zero on initialisation
		Score=0;
		BeliefInControl = 0; 
		MonthlyLossofHealth = 0;
	//	numCells=0;
		//for (int icount=0; icount<MaxNumPCells; icount++)
		//{
		Cells.clear();
		//}

		for (int icount=0; icount<MaxNumNeighbours; icount++)
		{
			NeighbourBlockID[icount]=-1; // list of neighbours
		}
		NumNeighbours=0; //number of neighbours
		ConnectionType=-1;
		UniquePlantaID=-1;
		

	}
	PInfo(int myPlantaID,  int mySym, double myscore, double mybelief, std::vector<int> mycells)
	{
		//FCHMA=myCHMA; // now tree density needs renaming
	//	BlockNum=-1; //not used
		UniquePlantaID=myPlantaID;
		DisStatus=mySym; //0 if < 5% sympotmatic and 1 if more than that in any one cell
		Score=myscore;
		BeliefInControl = mybelief;
		MonthlyLossofHealth = 0;
		//numCells=myNumcells;
		Cells.clear();
		for (int icount=0; icount<mycells.size(); icount++)
		{
			Cells.push_back(mycells[icount]);
		}
		for (int icount=0; icount<MaxNumNeighbours; icount++)
		{
			NeighbourBlockID[icount]=-1; // list of neighbours
		}
		NumNeighbours=0; //number of neighbours
		ConnectionType=-1;
		
	}
	~PInfo()
	{
		NumNeighbours=0; //number of neighbours
		Cells.clear();
	}
	
	void SetPBelief(double mybelief)
	{
		BeliefInControl = mybelief;
	}
	void SetNeighbour(int NID)
	{
		if (NumNeighbours<MaxNumNeighbours) 
		{
			bool alreadyCounted=false;
			for (int icount=0; icount<NumNeighbours; icount++)
			{
				if (NeighbourBlockID[icount]==NID)
						alreadyCounted=true;
			}
			if (!alreadyCounted)
			{
				NeighbourBlockID[NumNeighbours]=NID;
				NumNeighbours++;
			}
		}
		else
		{
			throw std::logic_error("too many neighbours");
		}
	}
	void SetConnect(int myCon)//This tells us how the Block is connected to the information highway. -1 = no connection, 0=neighbour; 1=Planta; 2=CHMA
	{
		ConnectionType=myCon;
	}
	
	int GetnumCells(){return Cells.size();}
	int GetCellID(int CellCount){return Cells[CellCount];}
	int GetnumNeighbours(){return NumNeighbours;}
	int GetNeighbourID(int ncount){return NeighbourBlockID[ncount];}
	double GetScore(){return Score;}
	double GetBelief(){ return BeliefInControl; }
	//double GetFmCHMA(){return FCHMA;}
//	int GetFmPlanta(){return UniquePlantaID;}
	void GetNeighbour(int NID, int myNeighbs[], int& NumNeibs)
	{
		for (int icount=0; icount<NumNeighbours; icount++)
		{
			myNeighbs[icount]=NeighbourBlockID[icount];
		}

		NumNeibs=NumNeighbours;
		
	}
	int GetPlantNum(){return UniquePlantaID;}
	void DeleteNeigh(){NumNeighbours=0;}
	int GetConnect()//This tells us how the Block is connected to the information highway. -1 = no connection, 0=neighbour; 1=Planta; 2=CHMA
	{
		return ConnectionType;
	}
	int GetDisStatus(){ return DisStatus;}
	void SetDisStatus(int myStat){DisStatus=myStat;}
	void AddHLoss(double exloss){ MonthlyLossofHealth = MonthlyLossofHealth + exloss; }
	double GetHLoss(){ return MonthlyLossofHealth; }
	

private:
	//	int BlockNum;
		int DisStatus; //If symptoms above 5% somewhere on the plantation then worry sets in
		double Score; //this is the 'worry' 0 is not worried about infection and 1 is very worried
		double BeliefInControl; //If a farmer firmly believes in the control strategy then we score him 1 is not zero. 
		std::vector<int> Cells; // field locations field=icols*maxnumrows+irows
		//int numCells; // number of cells in a Block
		//double  FCHMA; 
		int NeighbourBlockID[MaxNumNeighbours]; // list of neighbours
		int NumNeighbours; //number of neighbours
		int ConnectionType; //This tells us how the Block is connected to the information highway. -1 = no connection, 0=neighbour; 1=Planta; 2=CHMA
        int UniquePlantaID; //the Planta the Block is in
		double MonthlyLossofHealth;
		
};

class TwoVars
{
public:
  TwoVars(){Xs[0]=0; Xs[1]=0;};
  TwoVars(double val1, double val2){Xs[0]=val1; Xs[1]=val2;};
  ~TwoVars(){};
  TwoVars(const TwoVars& ATheX){Xs[0]=ATheX.Xs[0]; Xs[1]=ATheX.Xs[1];};
  TwoVars& operator=(const  TwoVars& ATheX){Xs[0]=ATheX.Xs[0]; Xs[1]=ATheX.Xs[1]; return *this;};
  bool  operator<(const  TwoVars& ATheX)
  {
    bool A=false; 
    if(Xs[0]<ATheX.Xs[0]) A=true;
    return A;
  };
  bool  operator>(const  TwoVars& ATheX){bool A=false; if(Xs[0]>ATheX.Xs[0]) A=true; return A;};
  double Xs[2];
};

class Bugs
{
public:
	
	Bugs()
	{
			BugT=0;
	}

	~Bugs()
	{
		TotalOWLav = 0;
		BugT=0;
	}
	//double GetNum(int Health){return BugT[Health];} //get total number of specified type
	double GetNum() { return BugT; } //get total number of specified type
		
	//void SetNum(int Health, double NewNum){BugT[Health]=NewNum;} 
	void SetNum( double NewNum) { BugT = NewNum; }

	void SetOWLav(double totPop) { TotalOWLav = totPop; }
	
	void GetOWLav(double& totPop){totPop=TotalOWLav;}
	
	//void SetOWLavmc(double totPopmc) { TotalOWLavmc = totPopmc; }
	void SetOWLavmc_H(double totPopmc_H) { TotalOWLavmc_H = totPopmc_H; }
	void SetOWLavmc_I(double totPopmc_I) { TotalOWLavmc_I = totPopmc_I; }
	void SetOWLavmc_S(double totPopmc_S) { TotalOWLavmc_S = totPopmc_S; }
	void SetOWLavmc_D(double totPopmc_D) { TotalOWLavmc_D = totPopmc_D; }



	//void GetOWLavmc(double& totPopmc) { totPopmc = TotalOWLavmc; }
	void GetOWLavmc_H(double& totPopmc_H) { totPopmc_H = TotalOWLavmc_H; }
	void GetOWLavmc_I(double& totPopmc_I) { totPopmc_I = TotalOWLavmc_I; }
	void GetOWLavmc_S(double& totPopmc_S) { totPopmc_S = TotalOWLavmc_S; }
	void GetOWLavmc_D(double& totPopmc_D) { totPopmc_D = TotalOWLavmc_D; }

	void ClearTempBug()
	{
		//TempBug[0]=0; //healthy
		//TempBug[1]=0; //sick
		TempBug=0;
		
	}
	void Clear()
	{
		
		TotalOWLav=0;
		BugT = 0;
		TempBug = 0;

	}
	void SetTempBug(  double NewNum)
	{
		TempBug=NewNum;
	}
	double GetTempBug()
	{
		return TempBug;
	}


private:
	double BugT; // number of each type of adult Bug (average per plant/unit area)
	double TotalOWLav;
	//double TotalOWLavmc;
	double TotalOWLavmc_H;
	double TotalOWLavmc_I;
	double TotalOWLavmc_S;
	double TotalOWLavmc_D;
	double TempBug; //a structure to keep track of travelling Bugs
	
};



namespace Grid
{ 
	int const HistLen(12); // This is how far back growers look when assessing evidence. We may not need it
	//int const MaxNumYears(200);

	
	
	//Size of grid
	int const Nrows = 1212;   
	int const Ncols = 602;   
	int const NStatus=12; //Status of trees. 0-3 not infested by EAB; 4-7 infested last year; 8-11 infested before last year
	int const NStatusage = 4; //Represents the 4 ash dieback statuses (asymptomatic, lightly stressed, heavily stressed, recently dead)
	double const gridLen = 1.0; // grid length is 1 = 1km
	double const GridVal_P_LarvToAdultStart = 0.25; //0.95;//LarvaeToAdultInOneYear(Lyons, 2015) // set tp 0.95
	//double const GridVal_P_LarvToAdultStartmcS = 0.1875;
	//double const GridVal_P_LarvToAdultStartmcD = 0.125;
	double const TreesPerHa = 889; // Trees per ha informed from the National Forest Inventory
	double const G_phloemPerTree = 4.53765; //m2 of pholem per tree
	double GetGridLen() { return gridLen; };
	double GP_LarvToAdult[Nrows][Ncols];
	double GP_LarvToAdultH[Nrows][Ncols];
	double GP_LarvToAdultI[Nrows][Ncols];
	double GP_LarvToAdultS[Nrows][Ncols];
	double GP_LarvToAdultD[Nrows][Ncols];
	int const statusmc = 4; //4 statuses for trees with ash dieback, asymptomatic, lightly stressed, heavily stressed, recently dead
	int const nyrs = 44;
	

	double ldens[Nrows][Ncols];
	double Trees[Nrows][Ncols][NStatus]; //Holds proportion of trees in each CHMA listed under status
	double Treesremmat[Nrows][Ncols][nyrs];
	double Treesremjuv[Nrows][Ncols][nyrs];

	double G_matshare[Nrows][Ncols];
	double G_Resprop[Nrows][Ncols];

	double Treestempmat[Nrows][Ncols][NStatus];
	double Treestempjuv[Nrows][Ncols][NStatus];
	double Treesmat[Nrows][Ncols][NStatusage];
	double Treesjuv[Nrows][Ncols][NStatusage];
	double adbrTrees[Nrows][Ncols];
	double startTrees[Nrows][Ncols][NStatus];
	double startTreesmat[Nrows][Ncols][NStatusage];
	double startTreesjuv[Nrows][Ncols][NStatusage];
	//double juvTrees[Nrows][Ncols][NStatus];
	//double matTrees[Nrows][Ncols][NStatus];
	Bugs GridBugs[Nrows][Ncols];

	double ProbDetection[Nrows][Ncols]; // This holds the probability of detection for each location for each realisation and year
	
	double ProbInitialInfectionDetection[Nrows][Ncols]; // This holds the probability this site is the entry location 


	//May not be needed
	int G_Control[Nrows][Ncols][HistLen]; //We now hold control for 12 months. the zero position is control for last month the 1 position for the month before
	//Note that  areas that cannot be cropped are 4 with fix=1
	
	int G_FellType[Nrows][Ncols]; // This is the type of felling in the cell 0=none; 1 = a proportion of infested with EAB; 2 = sanitation felling
	double G_PropFelled[Nrows][Ncols]; // If G_FellType = 1 this tells you the prportion to fell

	int G_adbarrival[Nrows][Ncols]; //ash dieback arrival date

	int G_adbsoil[Nrows][Ncols]; //soil drainage data

	double G_adbeusoil[Nrows][Ncols]; //soil water content data

	double G_adbnonwoodp[Nrows][Ncols]; //small wood/ hedgerow proportion data

	double G_adbprecipp[Nrows][Ncols]; //Mean annual precipitation difference from 620 mm per year

	double G_adbstart_H[Nrows][Ncols]; //ash dieback status in 2026 - asymptomatic (H)

	double G_adbstart_I[Nrows][Ncols]; //ash dieback status in 2026 - lightly stressed (I)
	
	double G_adbstart_S[Nrows][Ncols]; //ash dieback status in 2026 - heavily stressed (S)

	double G_adbstart_D[Nrows][Ncols]; //ash dieback status in 2026 - recently dead (D)

	double G_adbstart_R[Nrows][Ncols]; //ash dieback status in 2026 - removed (R)

	double matG_adbstart_H[Nrows][Ncols]; //mature ash dieback status in 2026 - asymptomatic (H)

	double matG_adbstart_I[Nrows][Ncols]; //mature ash dieback status in 2026 - lightly stressed (I)

	double matG_adbstart_S[Nrows][Ncols]; //mature ash dieback status in 2026 - heavily stressed (S)

	double matG_adbstart_D[Nrows][Ncols]; //mature ash dieback status in 2026 - dead (D)

	double matG_adbstart_R[Nrows][Ncols]; //mature ash dieback status in 2026 - removed (R)

	double juvG_adbstart_H[Nrows][Ncols]; //juvenile ash dieback status in 2026 - asymptomatic (H)

	double juvG_adbstart_I[Nrows][Ncols]; //juvenile ash dieback status in 2026 - lightly stressed (I)

	double juvG_adbstart_S[Nrows][Ncols]; //juvenile ash dieback status in 2026 - heavily stressed (S)

	double juvG_adbstart_D[Nrows][Ncols]; //juvenile ash dieback status in 2026 - dead (D)

	double juvG_adbstart_R[Nrows][Ncols]; //juvenile ash dieback status in 2026 - removed (R)
	
	double G_adbmatgamma[Nrows][Ncols]; //The mature annual adb mortality if I want to keep it stored for a grid cell

	double G_adbsoileff[Nrows][Ncols]; //The adb soil effect on annual adb mortality if I want to keep it stored for a grid cell

	double IntVars[3]; //keeps parameters for dispersal integration can be made bigger
	//each cell has a CHMA, Planta and Block ID. The combination is unique.
	double G_TreeDensity[Nrows][Ncols]; // HoldsTreeDensity so implicitly the number removed (trees per km square)
	double IniG_TreeDensity[Nrows][Ncols]; // Holds Initial TreeDensity  (trees per km square)

	//Emergence density by ADB category
	double H_Emerge_m2[Nrows][Ncols];
	double I_Emerge_m2[Nrows][Ncols];
	double S_Emerge_m2[Nrows][Ncols];
	double D_Emerge_m2[Nrows][Ncols];

	int PlantaID[Nrows][Ncols]; // Holds ownership might need to be more complex as likely to have multiple ownders per 1km cell
	//int BlockID[Nrows][Ncols];

	int invasionYr[Nrows][Ncols]; // this is to store the year that the invasion of the Bug becomes > a fixed number
	std::vector<PInfo> PlantationInfo; //This stores the info about each Block. Uniquie Block ID is offset by one from indexing, i.e. Block 1 info is held in index 0
	int TotalPlantasFilled;

	int GetTotalPlantasFilled(){return TotalPlantasFilled;}

	int GetNumRows(){return Nrows;}
	
	int GetNumCols(){return Ncols;}

	double GetPropOverWinterLarvaeStart() { return GridVal_P_LarvToAdultStart; };

	//double GetPropOverWinterLarvaeStartmcS() { return GridVal_P_LarvToAdultStartmcS; };

	//double GetPropOverWinterLarvaeStartmcD() { return GridVal_P_LarvToAdultStartmcD; };

	double GetPhloemPerTree() { return G_phloemPerTree; }

	double totPop = 0.0;
	
	void Setadbstart_H(int irow, int jcol, double adbstartH) //initial ash dieback status in 2026 (asymptomatic (H) trees)//
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbstart_H[irow][jcol] = adbstartH;
		}
		else
			throw std::logic_error("Error in Setadbstart_H");
		int junk = 1;

	}

	void Setadbstart_I(int irow, int jcol, double adbstartH) //initial ash dieback status in 2026 (lightly stressed (I) trees)//
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbstart_I[irow][jcol] = adbstartH;
		}
		else
			throw std::logic_error("Error in Setadbstart_I");
		int junk = 1;

	}


	void Setadbstart_S(int irow, int jcol, double adbstartS) //initial ash dieback status in 2026 (heavily stressed (S) trees)//
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbstart_S[irow][jcol] = adbstartS;
		}
		else
			throw std::logic_error("Error in Setadbstart_S");
		int junk = 1;

	}

	void Setadbstart_D(int irow, int jcol, double adbstartD) //initial ash dieback status in 2026 (recently dead (D) trees)//
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbstart_D[irow][jcol] = adbstartD;
		}
		else
			throw std::logic_error("Error in Setadbstart_D");
		int junk = 1;

	}

	void Setadbstart_R(int irow, int jcol, double adbstartR) //initial ash dieback status in 2026 (removed (R) trees)//
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbstart_R[irow][jcol] = adbstartR;
		}
		else
			throw std::logic_error("Error in Setadbstart_R");
		int junk = 1;

	}



	void initialiseadbmatgamma()
	{

		/////////////////////////
		//Don't actually use this for my model - just if want a sampler for mortality rate parameter
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double a = 0.009;
				//double b = 0.037;
				double b = 0.032; 
				double tempadbmatgamma = a + (b - a) * RNG_Uniform();

				Grid::Setadbmatgamma(icount, jcount, tempadbmatgamma);

			}
		}
		/////////////end of loop

	}
	
	void initialiseadbsoileff() //Don't actually use this for my model
	{


		/////////////////////////
		//Don't actually use this for my model - just if want a sampler for soil effect on mortality rate parameter
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double a = 1.857;
				double b = 2.044;
				double tempadbsoileff = a + (b - a) * RNG_Uniform();

				Grid::Setadbsoileff(icount, jcount, tempadbsoileff);

			}
		}
		/////////////end of loop
		//final tidy 

	}



	
	
	
	void SetInclustionProb(int irow, int jcol, double IncPr) //Probability that this is the entry location of the first infection 
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			ProbInitialInfectionDetection[irow][jcol] = IncPr;
		}
	}
	double GetInclustionProb(int irow, int jcol) //Probability that this is the entry location of the first infection 
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			return ProbInitialInfectionDetection[irow][jcol];
		}
	}


	void SetEmergenceDensity_H(int irow, int jcol, double Stype) 
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			H_Emerge_m2[irow][jcol] = Stype;
		}
		else
			throw std::logic_error("Error in SetEmergeDensity");
		//int junk=1;

	}

	void SetEmergenceDensity_I(int irow, int jcol, double Stype) 
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			I_Emerge_m2[irow][jcol] = Stype;
		}
		else
			throw std::logic_error("Error in SetEmergeDensity");
		//int junk=1;

	}

	void SetEmergenceDensity_S(int irow, int jcol, double Stype)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			S_Emerge_m2[irow][jcol] = Stype;
		}
		else
			throw std::logic_error("Error in SetEmergeDensity");
		//int junk=1;

	}

	void SetEmergenceDensity_D(int irow, int jcol, double Stype)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			D_Emerge_m2[irow][jcol] = Stype;
		}
		else
			throw std::logic_error("Error in SetEmergeDensity");
		//int junk=1;

	}
	

	double GetEmergenceDensity_H(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return H_Emerge_m2[irow][jcol];
		else
			throw std::logic_error("error in get Emergence Density");
		//int junk=-9;
	}

	double GetEmergenceDensity_I(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return I_Emerge_m2[irow][jcol];
		else
			throw std::logic_error("error in get Emergence Density");
		//int junk=-9;
	}

	double GetEmergenceDensity_S(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return S_Emerge_m2[irow][jcol];
		else
			throw std::logic_error("error in get Emergence Density");
		//int junk=-9;
	}

	double GetEmergenceDensity_D(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return D_Emerge_m2[irow][jcol];
		else
			throw std::logic_error("error in get Emergence Density");
		//int junk=-9;
	}

    double GetNumG(int irow, int jcol)
	{
		if ((irow<GetNumRows())&&(jcol<GetNumCols())&&(irow>-1)&&(jcol>-1))
		{
			return GridBugs[irow][jcol].GetNum();
		}
		else
			throw std::logic_error("Error in GetNumG");
	
	} //get total number of specified type
	
	
	int GetNumCells(int BlockID){return PlantationInfo[BlockID].GetnumCells();}

	int GetCellLocation(int PlantaID, int cellCount){ return PlantationInfo[PlantaID].GetCellID(cellCount);}

	void SetTotalPlantasFilled(int myCounts){ TotalPlantasFilled=myCounts;}

	void SetNumG(int irow, int jcol, double NewNum)
	{ 
		if ((irow<GetNumRows())&&(jcol<GetNumCols()))
		{
			GridBugs[irow][jcol].SetNum(NewNum);
		}
		else
			throw std::logic_error("Error initialising in grid cells that don't exist");

	} 

	
	void SetOWLav(int irow, int jcol, double totPop){ GridBugs[irow][jcol].SetOWLav(totPop);}
	
	//void SetOWLavmc(int irow, int jcol, int statusmc, double totPopmc) { GridBugs[irow][jcol].SetOWLavmc(totPopmc); }
	
	void SetOWLavmc_H(int irow, int jcol, double totPopmc_H) { GridBugs[irow][jcol].SetOWLavmc_H(totPopmc_H); }
	void SetOWLavmc_I(int irow, int jcol, double totPopmc_I) { GridBugs[irow][jcol].SetOWLavmc_I(totPopmc_I); }
	void SetOWLavmc_S(int irow, int jcol, double totPopmc_S) { GridBugs[irow][jcol].SetOWLavmc_S(totPopmc_S); }
	void SetOWLavmc_D(int irow, int jcol, double totPopmc_D) { GridBugs[irow][jcol].SetOWLavmc_D(totPopmc_D); }

	void SetPlanta(int irow, int jcol, int Ctype)
	{
		if ((irow<GetNumRows())&&(jcol<GetNumCols())&&(irow>-1)&&(jcol>-1))
		{
			PlantaID[irow][jcol]=Ctype; 
		}
		else
			throw std::logic_error("Error in SetPlanta");
			//int junk=1;

	}



	void GetPLarv(int irow, int jcol, double& P_LarvToAdult)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			P_LarvToAdult =GP_LarvToAdult[irow][jcol];
		}
		else
			throw std::logic_error("Error in GetPLarv");
	}

	void SetPLarv(int irow, int jcol, double P_LarvToAdult)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			 GP_LarvToAdult[irow][jcol] = P_LarvToAdult;
		}
		else
			throw std::logic_error("Error in GetPLarv");
	}

	void GetPLarvH(int irow, int jcol, double& P_LarvToAdultH)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			P_LarvToAdultH = GP_LarvToAdultH[irow][jcol];
		}
		else
			throw std::logic_error("Error in GetPLarvH");
	}

	void SetPLarvH(int irow, int jcol, double P_LarvToAdultH)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			GP_LarvToAdultH[irow][jcol] = P_LarvToAdultH;
		}
		else
			throw std::logic_error("Error in SetPLarvH");

	}

	void GetPLarvI(int irow, int jcol, double& P_LarvToAdultI)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			P_LarvToAdultI = GP_LarvToAdultI[irow][jcol];
		}
		else
			throw std::logic_error("Error in GetPLarvI");
	}

	void SetPLarvI(int irow, int jcol, double P_LarvToAdultI)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			GP_LarvToAdultI[irow][jcol] = P_LarvToAdultI;
		}
		else
			throw std::logic_error("Error in SetPLarvI");

	}

	void GetPLarvS(int irow, int jcol, double& P_LarvToAdultS)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			P_LarvToAdultS = GP_LarvToAdultS[irow][jcol];
		}
		else
			throw std::logic_error("Error in GetPLarvS");
	}

	void SetPLarvS(int irow, int jcol, double P_LarvToAdultS)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			GP_LarvToAdultS[irow][jcol] = P_LarvToAdultS;
		}
		else
			throw std::logic_error("Error in SetPLarvS");

	}

	void GetPLarvD(int irow, int jcol, double& P_LarvToAdultD)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			P_LarvToAdultD = GP_LarvToAdultD[irow][jcol];
		}
		else
			throw std::logic_error("Error in GetPLarvD");
	}

	void SetPLarvD(int irow, int jcol, double P_LarvToAdultD)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			GP_LarvToAdultD[irow][jcol] = P_LarvToAdultD;
		}
		else
			throw std::logic_error("Error in SetPLarvD");

	}
	
	void SetTreeDensity(int irow, int jcol, double Stype) 
	{
		if ((irow<GetNumRows())&&(jcol<GetNumCols())&&(irow>-1)&&(jcol>-1))
		{
			G_TreeDensity[irow][jcol]=Stype;
		}
		else
			throw std::logic_error("Error in SetTreeDensity");
			//int junk=1;

	}

	void Setadbarrival(int irow, int jcol, int temparrival) //ash dieback arrival date
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbarrival[irow][jcol] = temparrival;
		}
		else
			throw std::logic_error("Error in Setadbarrival");
		//int junk=1;

	}


	void Setadbsoil(int irow, int jcol, int tempadbsoil) //soil drainage
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbsoil[irow][jcol] = tempadbsoil;
		}
		else
			throw std::logic_error("Error in Setadbsoil");
		//int junk=1;

	}


	void Setadbeusoil(int irow, int jcol, double tempadbeusoil) //soil water content
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbeusoil[irow][jcol] = tempadbeusoil;
		}
		else
			throw std::logic_error("Error in Seteusoil");
		//int junk=1;

	}


	void Setnonwoodp(int irow, int jcol, double tempnonwoodp) //small wood/ hedgerow proportion
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbnonwoodp[irow][jcol] = tempnonwoodp;
		}
		else
			throw std::logic_error("Error in Setnonwoodp");
		//int junk=1;

	}

	

	void Setadbmatgamma(int irow, int jcol, double tempadbmatgamma) //The mature annual adb mortality if I want to keep it stored for a grid cell
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbmatgamma[irow][jcol] = tempadbmatgamma;
		}
		else
			throw std::logic_error("Error in Setadbmatgamma");
		int junk=1;

	}

	void Setadbsoileff(int irow, int jcol, double tempadbsoileff) //The adb soil effect on annual adb mortality if I want to keep it stored for a grid cell
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			G_adbsoileff[irow][jcol] = tempadbsoileff;
		}
		else
			throw std::logic_error("Error in Setadbsoileff");
		int junk = 1;

	}


	void SetInitialTreeDensity(int irow, int jcol, double Stype) // Intial ash number in a cell
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			IniG_TreeDensity[irow][jcol] = Stype;
		}
		else
			throw std::logic_error("Error in SetTreeDensity");
		//int junk=1;

	}
	
	void SetCrop(int irow, int jcol, int ist, double per)
	{
		if ((irow<GetNumRows())&&(jcol<GetNumCols())&&(irow>-1)&&(jcol>-1) &&(ist<NStatus))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				int junk = 1;
				throw std::logic_error("Error in SetCrop1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Trees[irow][jcol][ist]=per; 
			
		}
		else
			throw std::logic_error("Error in SetCrop2");
			//int junk=1;

	}

	void Setnremovedmat(int irow, int jcol, int nyrs, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (nyrs > -1) && (nyrs < 44))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				//	int junk = 1;
				throw std::logic_error("Error in Setnremovedmat1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treesremmat[irow][jcol][nyrs] = per;

		}
		else
			throw std::logic_error("Error in Setnremovedmat2");
		//int junk=1;

	}

	void Setnremovedjuv(int irow, int jcol, int nyrs, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (nyrs > -1) && (nyrs < 44))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				//	int junk = 1;
				throw std::logic_error("Error in Setnremovedjuv1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treesremjuv[irow][jcol][nyrs] = per;

		}
		else
			throw std::logic_error("Error in Setnremovedjuv2");
		//int junk=1;

	}

	void Settempmat(int irow, int jcol, int ist, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (ist < NStatus))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				//	int junk = 1;
				throw std::logic_error("Error in Settempmat1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treestempmat[irow][jcol][ist] = per;

		}
		else
			throw std::logic_error("Error in Settempmat2");
		//int junk=1;

	}

	void Settempjuv(int irow, int jcol, int ist, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (ist < NStatus))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				//	int junk = 1;
				throw std::logic_error("Error in Settempjuv");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treestempjuv[irow][jcol][ist] = per;

		}
		else
			throw std::logic_error("Error in Settempjuv2");
		//int junk=1;

	}

	void SetCropmat(int irow, int jcol, int ist, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (ist < NStatusage))
		{

			if ((per < -0.00001) || (per > 1.00001)) { // allowing rounding error
					int junk = 1;
				throw std::logic_error("Error in SetCropmat1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treesmat[irow][jcol][ist] = per;

		}
		else
			throw std::logic_error("Error in SetCropmat2");
		//int junk=1;

	}

	void SetCropjuv(int irow, int jcol, int ist, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1) && (ist < NStatusage))
		{

			if ((per < -0.00001) || (per > 1.00001)) { // allowing rounding error
					int junk = 1;
				throw std::logic_error("Error in SetCropjuv1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			Treesjuv[irow][jcol][ist] = per;

		}
		//else
		//	throw std::logic_error("Error in SetCropjuv2");
		//int junk=1;

	}

	void Setlarvdens(int irow, int jcol, double dens)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{
			ldens[irow][jcol] = dens;
		}
		else
			throw std::logic_error("Error in Setadblarv");
		//int junk=1;

	}

	void Setadbremoved(int irow, int jcol, double per)
	{
		if ((irow < GetNumRows()) && (jcol < GetNumCols()) && (irow > -1) && (jcol > -1))
		{

			if ((per < -0.00000001) || (per > 1.000000001)) { // allowing rounding error
				//	int junk = 1;
				throw std::logic_error("Error in Setadbremoved1");
			}
			if ((per > 1)) // if a little over pull it back
				per = 1.0;
			if ((per < 0))
				per = 0.0;
			adbrTrees[irow][jcol] = per;

		}
		else
			throw std::logic_error("Error in Setadbremoved");
		//int junk=1;

	}


	double Getlarvdens(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return ldens[irow][jcol];
		else
			throw std::logic_error("error in get crop");
		//return -9;
	}

	
	double GetCrop(int irow, int jcol, int ist)
	{
		if ((irow>-1)&&(irow<Nrows)&&(jcol>-1)&&(jcol<Ncols))
			return Trees[irow][jcol][ist];
		else
			throw std::logic_error("error in get crop");
			//return -9;
	}

	double Getnremovedmat(int irow, int jcol, int nyrs)
	{
		if (nyrs < 0)
			return 0.0;
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols) && (nyrs < 44))
			return Treesremmat[irow][jcol][nyrs];
		else
			throw std::logic_error("error in Getnremovedmat");
		//return -9;
	}

	double Getnremovedjuv(int irow, int jcol, int nyrs)
	{
		if (nyrs < 0)
			return 0.0;
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols) && (nyrs < 44))
			return Treesremjuv[irow][jcol][nyrs];
		else
			throw std::logic_error("error in Getnremovedjuv");
		//return -9;
	}

	void SetMatShare(int irow, int jcol, double share)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols) && (share >= 0.0) && (share <= 1.0))
			G_matshare[irow][jcol] = share;
		else
			throw std::logic_error("error in SetMatShare");


	}

	double GetMatShare(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_matshare[irow][jcol];
		else
			throw std::logic_error("error in GetMatShare");


	}

	void SetResProp(int irow, int jcol, double prop) {

		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols) && (prop >= 0.0) && (prop <= 1.0))
			G_Resprop[irow][jcol] = prop;
		else
			throw std::logic_error("error in SetResProp");

	}

	double GetResProp(int irow, int jcol) {

		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_Resprop[irow][jcol];
		else
			throw std::logic_error("error in GetResProp");

	}


	double GetCropmat(int irow, int jcol, int ist)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return Treesmat[irow][jcol][ist];
		else
			throw std::logic_error("error in get crop mat");
		//return -9;
	}

	double GetCropjuv(int irow, int jcol, int ist)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return Treesjuv[irow][jcol][ist];
		else
			throw std::logic_error("error in get crop juv");
		//return -9;
	}

	double Gettempmat(int irow, int jcol, int ist)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return Treestempmat[irow][jcol][ist];
		else
			throw std::logic_error("error in get crop temp mat");
		//return -9;
	}

	double Gettempjuv(int irow, int jcol, int ist)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return Treestempjuv[irow][jcol][ist];
		else
			throw std::logic_error("error in get crop temp juv");
		//return -9;
	}


	double Getadbremoved(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return adbrTrees[irow][jcol];
		else
			throw std::logic_error("error in get crop");
		//return -9;
	}


	
	double GetTreeDensity(int irow, int jcol) 
	{
		if ((irow>-1)&&(irow<Nrows)&&(jcol>-1)&&(jcol<Ncols))
			return G_TreeDensity[irow][jcol];
		else
			throw std::logic_error("error in get TreeDensity");
			//int junk=-9;
	}

	int Getadbarrival(int irow, int jcol) //ash dieback arrival date
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbarrival[irow][jcol];
		else
			throw std::logic_error("error in get adbarrival");
		//int junk=-9;
	}


	int Getadbsoil(int irow, int jcol) //soil drainage 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbsoil[irow][jcol];
		else
			throw std::logic_error("error in get adbsoil");
		//int junk=-9;
	}


	double Getadbeusoil(int irow, int jcol) //soil water content
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbeusoil[irow][jcol];
		else
			throw std::logic_error("error in get eusoil");
		//int junk=-9;
	}


	double Getnonwoodp(int irow, int jcol) //small wood/ hedgerow proportion
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbnonwoodp[irow][jcol];
		else
			throw std::logic_error("error in get nonwoodp");
		//int junk=-9;
	}

	double Getprecipp(int irow, int jcol) //Reading in UK precipitation map with precipitation difference from the minimum UK precipitation//
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbprecipp[irow][jcol];
		else
			throw std::logic_error("error in get precipp");
		//int junk=-9;
	}

	double Getadbstart_H(int irow, int jcol) 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbstart_H[irow][jcol];
		else
			throw std::logic_error("error in get adbstart_H");
		//int junk=-9;
	}

	double Getadbstart_I(int irow, int jcol) 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbstart_I[irow][jcol];
		else
			throw std::logic_error("error in get adbstart_H");
		//int junk=-9;
	}

	double Getadbstart_S(int irow, int jcol) 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbstart_S[irow][jcol];
		else
			throw std::logic_error("error in get adbstart_S");
		//int junk=-9;
	}

	double Getadbstart_D(int irow, int jcol) 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbstart_D[irow][jcol];
		else
			throw std::logic_error("error in get adbstart_D");
		//int junk=-9;
	}

	double Getadbstart_R(int irow, int jcol) 
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbstart_R[irow][jcol];
		else
			throw std::logic_error("error in get adbstart_R");
		//int junk=-9;
	}

	double Getadbmatgamma(int irow, int jcol)  //The annual adb mature mortality if I want to keep it stored for a grid cell
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbmatgamma[irow][jcol];
		else
			throw std::logic_error("error in get getadbmatgamma");
		//int junk=-9;
	}
	 
	double Getadbsoileff(int irow, int jcol)  //The adb soil effect on annual adb mortality if I want to keep it stored for a grid cell
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return G_adbsoileff[irow][jcol];
		else
			throw std::logic_error("error in get getadbsoileff");
		//int junk=-9;
	}


	double GetInitialTreeDensity(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Nrows) && (jcol > -1) && (jcol < Ncols))
			return IniG_TreeDensity[irow][jcol];
		else
			throw std::logic_error("error in get TreeDensity");
		//int junk=-9;
	}

	double GetSumBug(int imth) {
		double totPop = 0.0;
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{

				double iniPop = 0.0;
				iniPop = Grid::GetNumG(icount, jcount); 
				if (iniPop > 0.0) {
					totPop += iniPop;
				}

				else {

					totPop += 0.0;

				}

			}
		}
		return totPop;
	
	}

	double Getnumberash(int imth) {
		double totDead = 0.0;
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{

				double iniash = 0.0;
				iniash = Grid::GetTreeDensity(icount, jcount);
				if (iniash > 0.0) {
					totDead += iniash;
				}

				else {

					totDead += 0.0;

				}

			}
		}
		return totDead;

	}


	int GetCountDead(int imth) {
		int totDeadCount = 0.0;
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double iniash = 0.0;
				iniash = Grid::GetInitialTreeDensity(icount, jcount) - Grid::GetTreeDensity(icount, jcount); 
				if (iniash > 0.0) {
					totDeadCount += 1;
				}

				else {
					totDeadCount += 0;
				}

			}
		}
		return totDeadCount;

	}


	int GetCountBug(int imth) {
		int totPopCount = 0.0;
		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				double iniPop = 0.0;
				iniPop = Grid::GetNumG(icount, jcount);
				if (iniPop > 0.0) {
					totPopCount += 1;
				}

				else {
					totPopCount += 0;
				}

			}
		}
		return totPopCount;

	}




	int GetMaxNumStatus(){return NStatus;}
			
	void GetOWLav(int irow, int jcol,  double& totPop){ GridBugs[irow][jcol].GetOWLav(totPop);}
	void GetOWLavmc_H(int irow, int jcol, double& totPopmc_H) { GridBugs[irow][jcol].GetOWLavmc_H(totPopmc_H); }
	void GetOWLavmc_I(int irow, int jcol, double& totPopmc_I) { GridBugs[irow][jcol].GetOWLavmc_I(totPopmc_I); }
	void GetOWLavmc_S(int irow, int jcol, double& totPopmc_S) { GridBugs[irow][jcol].GetOWLavmc_S(totPopmc_S); }
	void GetOWLavmc_D(int irow, int jcol, double& totPopmc_D) { GridBugs[irow][jcol].GetOWLavmc_D(totPopmc_D); }
	
	
	void ClearData()
	{
		for (int icount=0; icount<Nrows; icount++)
		{
			for (int jcount=0; jcount<Ncols; jcount++)
			{
				GridBugs[icount][jcount].Clear();
				for (int scount=0; scount<NStatus; scount++)
				{
					Trees[icount][jcount][scount]=0;
				}
			}
		}

		PlantationInfo.clear();

	}

	void ClearTempBugs()
	{
		for (int icount=0; icount<Nrows; icount++)
		{
			for (int jcount=0; jcount<Ncols; jcount++)
			{
				GridBugs[icount][jcount].ClearTempBug();
				
			}
		}
	}

	void SetTempBug(int irow, int jcol,  double NewNum)
	{
		GridBugs[irow][jcol].SetTempBug( NewNum);

	}
	double GetTempBug(int irow, int jcol)
	{
		if ((irow>-1)&&(irow<Grid::GetNumRows())&&(jcol>-1)&&(jcol<Grid::GetNumCols()))
			return GridBugs[irow][jcol].GetTempBug();
		else
			throw std::logic_error("index out of bounds");
	}

	
	//Apply felling or not
	void GetTreeFellingStats(int irow, int jcol, int& FellingType, double& propFelled) // Felling type:  0 = no felling; 1 = fell EAB infected; 2= sanitation felling; propFelled tells the proportion of ill trees to fell
	{
		if ((irow > -1) && (irow < Grid::GetNumRows()) && (jcol > -1) && (jcol < Grid::GetNumCols()))
		{
			FellingType = G_FellType[irow][jcol];
			propFelled  = G_PropFelled[irow][jcol];
		}

		else
			throw std::logic_error("Error in GetTreeFellingStats index out of bounds");

	}
		
	void SetTreeFellingStats(int irow, int jcol, int FellingType, double propFelled) // Felling type:  0 = no felling; 1 = fell EAB infected; 2= sanitation felling; propFelled tells the proportion of ill trees to fell
	{
		if ((irow > -1) && (irow < Grid::GetNumRows()) && (jcol > -1) && (jcol < Grid::GetNumCols()))
		{
			G_FellType[irow][jcol]= FellingType;
			G_PropFelled[irow][jcol]= propFelled;
		}

		else
			throw std::logic_error("Error in SetTreeFellingStats index out of bounds");

	}

	void SetIntVars(double jdist, double gridLen, double lambda)
	{
		IntVars[0]=jdist;
		IntVars[1]=gridLen;
		IntVars[2]=lambda;

	}

	void GetIntVars(double& jdist, double& gridLen, double& lambda)
	{
		jdist=IntVars[0];
		gridLen=IntVars[1];
		lambda=IntVars[2];
		
	}

	
	void SetInvasion(int irow, int jcol, int yr) //stores where Bugs have reached in an invasion from left to right
	{
		invasionYr[irow][jcol]=yr;
	}

	int GetInvasion(int irow, int jcol)
	{
		return invasionYr[irow][jcol];
	}


	void SetLand(int newL, int& iflag) //double alpha_W, double beta_W, double alpha_B, double beta_B are paraters for beta dristribution describing initial belief structures 
	//void SetLand(int newL, double alpha_W, double beta_W, double alpha_B, double beta_B, int& iflag) //double alpha_W, double beta_W, double alpha_B, double beta_B are paraters for beta dristribution describing initial belief structures 
	{
		
		int UBlockID=1; //unique Block ID	

		for (int icount = 0; icount < Nrows; icount++)
		{
			for (int jcount = 0; jcount < Ncols; jcount++)
			{
				SetTreeDensity(icount, jcount, 0.0); // This is just cleaning it before we start ; 
				Setadbarrival(icount, jcount, 0);
				Setadbeusoil(icount, jcount, 0.0);
				Setadbsoil(icount, jcount, 0);
				Setnonwoodp(icount, jcount, 0.0);
				Setadbstart_H(icount, jcount, 0.0);
				Setadbstart_I(icount, jcount, 0.0);
				Setadbstart_S(icount, jcount, 0.0);
				Setadbstart_D(icount, jcount, 0.0);
				Setadbstart_R(icount, jcount, 0.0);

				SetInitialTreeDensity(icount, jcount, 0.0); // This is just cleaning it before we start ; 
				SetPlanta(icount, jcount, -9);
				//SetBlock(icount,jcount,-9); //removed all block stuff for now don't think we need it
				for (int kcount = 0; kcount < NStatus; kcount++)
					SetCrop(icount, jcount, kcount, 0);  //set all status to zero

			}
		}

		if (newL==0) //then read grid spec from file 
		{
			initialiseadbmatgamma();
			initialiseadbsoileff();

			char path_buffer[_MAX_PATH];
			char Npath_buffer[_MAX_PATH];
			_getcwd( path_buffer, _MAX_PATH );
			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\Owners_GB.txt");

			std::ifstream    InF(Npath_buffer);

			//strcpy_s(Npath_buffer, path_buffer);
			//strcat_s(Npath_buffer, "\\TreeDensity\\UKGridADB.txt"); 
			//strcat_s(Npath_buffer, "\\TreeDensity\\ADBInfection_SE.txt"); 
			//strcat_s(Npath_buffer, "\\TreeDensity\\ADBInfection_GB.txt");
			//std::ifstream    InFF(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\treeDensity_GB.txt");
			std::ifstream	 InFFF(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			
			strcat_s(Npath_buffer, "\\TreeDensity\\InclusionProbGB.txt");

			std::ifstream	 InF4(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\arrival_GB3011.txt");
			std::ifstream    InFarrival(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\eusoil_GB3011.txt");
			std::ifstream    InFeusoil(Npath_buffer);


			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\soil_GB3011.txt");
			std::ifstream    InFsoil(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\nonwoodp_GB3011.txt");
			std::ifstream    InFnonwoodp(Npath_buffer);

			strcpy_s(Npath_buffer, path_buffer);
			strcat_s(Npath_buffer, "\\TreeDensity\\precipp_GB3011.txt");
			std::ifstream    InFprecipp(Npath_buffer);


			int TempOwner;
			double TempTreeDensity, ADBInf, infectionStatus;
			int Sick(0); //Flag to see of sick tree set/
			int iyr=2;
			int MaxPlantID=-1; // This keeps track of the maximum ID number for the plantations for use later 
			
			
			///if (InF && InFF && InFFF)
			if (InF&&InFFF&&InF4&&InFarrival&&InFeusoil&&InFsoil&&InFnonwoodp&&InFprecipp)
			{
				//Moved the inclusion probability here!//
	//Sample from uniform //
//Sample from uniform //
				double urand[1];
				urand[0] = RNG_Uniform();
				double myCountProb = 0;
				int infCelliCol = 0;
				int infCelljRow = 0;
				urand[0] = urand[0] * 100.0; //turn to percents to matxh input file
				int notFoundIT = 1;


				for (int jcount = 0; jcount < Nrows; jcount++)
				{
					for (int icount = 0; icount < Ncols; icount++)
					{
						InF4 >> infectionStatus;
						myCountProb = myCountProb + infectionStatus;
						if ((myCountProb > urand[0]) && (notFoundIT))
						{
							infCelliCol = icount;
							infCelljRow = jcount;
							notFoundIT = 0;
						}
					}
				}

				for (int jcount=0; jcount<Nrows; jcount++)
				{
					for (int icount=0; icount<Ncols; icount++)
					{
						InF>> TempOwner;
						PlantaID[jcount][icount]= TempOwner;
						if (TempOwner >MaxPlantID)
							MaxPlantID= TempOwner;
						InFFF>> TempTreeDensity;
						TempTreeDensity = TempTreeDensity*TreesPerHa; //This is number of trees per cell = ha / cell * trees / ha
						G_TreeDensity[jcount][icount]= TempTreeDensity;
						IniG_TreeDensity[jcount][icount] = TempTreeDensity;
						if (TempTreeDensity < 1.0)
							TempTreeDensity = 1.0;
						double ProbIniTreeInfection = 1.0 / TempTreeDensity; // This is now set so we have 1 tree infected per ha.
						
						int Temparrival;
						InFarrival >> Temparrival;
						G_adbarrival[jcount][icount] = Temparrival; //Filling our grid with arrival dates

						int Tempsoil;
						InFsoil >> Tempsoil;
						G_adbsoil[jcount][icount] = Tempsoil; //Filling our grid with soil drainage values

						double Tempeusoil;
						InFeusoil >> Tempeusoil;
						G_adbeusoil[jcount][icount] = Tempeusoil; //Filling our grid with soil saturation values

						double Tempnonwoodp;
						InFnonwoodp >> Tempnonwoodp;
						G_adbnonwoodp[jcount][icount] = Tempnonwoodp; //Filling our grid with the proportion of nonwoodtrees

						double Tempprecipp;
						InFprecipp >> Tempprecipp;
						G_adbprecipp[jcount][icount] = Tempprecipp; //Filling our grid with the proportion of nonwoodtrees


						double adbstartH = 0.0, adbstartI = 0.0, adbstartS = 0.0, adbstartD = 0.0, adbstartR = 0.0, iniremovefunction = 0.0;
						double adbstartHmat = 0.0, adbstartImat = 0.0, adbstartSmat = 0.0, adbstartDmat = 0.0, adbstartRmat = 0.0, iniremovefunctionmat;
						double adbstartHjuv = 0.0, adbstartIjuv = 0.0, adbstartSjuv = 0.0, adbstartDjuv = 0.0, adbstartRjuv = 0.0, iniremovefunctionjuv;


						int adbyear = G_adbarrival[jcount][icount];

							double numTrees = GetTreeDensity(jcount, icount); //number of trees in each cell
							
							//Proportion of mature ash
							SetMatShare(jcount, icount, 0.29);

							//Setting proportion of resistant ash
							double ResProp = 0.019 + 0.036 * RNG_Uniform();
							SetResProp(jcount, icount, ResProp);

							if (numTrees > 0 && adbyear >-1) {

								//Define the start conditions for each cell.
								double iniH_m = 0.99;
								double iniI_m = 0.01;
								double iniS_m = 0.00;
								double iniD_m = 0.00;

								double iniH_j = 0.99;
								double iniI_j = 0.01;
								double iniS_j = 0.00;
								double iniD_j = 0.00;

								//ADB arrival working with assumption that cells where disease was detected in 2013, ADB symtpoms were
								//present throughout these cells (i.e. had been present a number of years - only detected when symptoms clear), this then spreads at a rate of
								//50 km per year from these cells.

								int adbyear = G_adbarrival[jcount][icount]; //will return the year of disease arrival from the cell
								int adbsoil = G_adbsoil[jcount][icount]; // soil drainage
								double adbeusoil = G_adbeusoil[jcount][icount]; //estimate soil water content
								double nonwoodp = G_adbnonwoodp[jcount][icount]; //proportion of trees in non-wood setting
								double inimatgamma = 0.0205; //mature gamma values (0.009 for minimum, 0.0205 for median, 0.032 for max)
								double inijuvgamma = inimatgamma * (0.1 / 0.032); //Calculate juvenile mortality from the conversions in the Marcais 2017 paper.
								double adbeta = 0.5; //Beta value
								double nonwoodeff = 1.0 - (0.19 / 0.41); //This is the difference in crown dieback between Hedgerow and Woodland trees
								double matprop = 0.29; //define the proportion of mature ash in GB woodlands
								double juvprop = 0.71; //define the proportion of juvenile ash in GB woodlands
								double matstresseff = 0.724; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
								double juvstresseff = 0.712; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
								double mortratio = 3.0; //Difference in rate severly stressed trees die vs lightly stressed trees - will adjust with q values//
								double precipmortklesse = (1.0 / (1.0 + exp(-(0.5 - 6.7)))); //Mortality given a unit precipitation increase
								double noprecipmortklesse = (1.0 / (1.0 + exp(-(-6.7)))); //Mortality at minimum precipitation
								double precippercentchange = ((precipmortklesse / noprecipmortklesse) - 1) / (312.0 + 1305.575); //Percent change in mortality per unit increase in rainfall


								double precippmat = (inimatgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount])))-inimatgamma; //i.e. the additional gamma value per cell//
								double precippjuv = (inijuvgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount])))- inijuvgamma; //i.e. the additional gamma value per cell//

								double soilp = 1.0;
								if (adbsoil > 4 && adbeusoil > 0.3605875) {
									soilp = 1.9505; //soil moisture adjusted transition rate multiplier for wet sites (1.857 for minimum, 1.9505 for median, 2.04 for max)
								}
								else {  soilp = 1.0; }

								double soilpmat = (inimatgamma * soilp)-inimatgamma; //i.e. the additional gamma value per cell//
								double soilpjuv = (inijuvgamma * soilp)- inijuvgamma; //i.e. the additional gamma value per cell//

								//Adding the effect of soil and precipitation to the base ADB gamma value
								double matgammaa = inimatgamma + soilpmat + precippmat;
								double juvgammaa = inijuvgamma + soilpjuv + precippjuv;

								//Now adjusting the gamma value dependent on the proportion of ADB in a woodland
								double matgamma = matgammaa - (matgammaa * nonwoodp * nonwoodeff);
								double juvgamma = juvgammaa - (juvgammaa * nonwoodp * nonwoodeff);

								//Calculating the rate at which trees move to the heavily stressed category//
								double matalpha = matgamma / matstresseff;
								double juvalpha = juvgamma / juvstresseff;

								//Initialising variables//
								double NH_m, NI_m, NS_m, ND_m;
								double NH_j, NI_j, NS_j, ND_j;
								int niyr;

								//Running for 13 years(2013 until 2026) - how many years it took for disease to arrive after 2013//
								for (int i = 0; i < 14 - adbyear; i++) { //Change the 14 to 19 if we want EAB arrival in 2031 rather than 2026; and to 27 if want ADB base layer in 2039

								//Add some resistant ash and more S trees dying than I trees//
								
								//Calculating mortality split between categories - with S trees dying three times the rate than I trees
								// with same overall annual mortality rate. Accounting for the resistant ash in a cell - i.e. these say in H category
									
									double gI_m = matgamma;
									double gS_m = matgamma;
									double den_m = iniI_m + (mortratio * iniS_m);

									if (den_m > 0.0) {

										gI_m = matgamma * (iniI_m + iniS_m) / den_m;
										gS_m = mortratio * gI_m;
									}

									if (gS_m > 1.0) { gS_m = 1.0; }

									double availH_m = iniH_m - ResProp;
									if (availH_m < 0.0) { availH_m = 0.0; }

									double gI_j = juvgamma;
									double gS_j = juvgamma;
									double den_j = iniI_j + (mortratio * iniS_j);

									if (den_j > 0.0) {

										gI_j = juvgamma * (iniI_j + iniS_j) / den_j;
										gS_j = mortratio * gI_j;
									}

									if (gS_j > 1.0) { gS_j = 1.0; }

									double availH_j = iniH_j - ResProp;
									if (availH_j < 0.0) { availH_j = 0.0; }

									//Compartmental model for mature and juvenile ash separately//
									NH_m = -adbeta * availH_m;
									NI_m = (adbeta * availH_m) - (gI_m * iniI_m) - (matalpha * iniI_m);
									NS_m = (matalpha * iniI_m) - (gS_m * iniS_m);
									ND_m = (gI_m * iniI_m) + (gS_m * iniS_m);

									NH_j = -adbeta * availH_j;
									NI_j = (adbeta * availH_j) - (gI_j * iniI_j) - (juvalpha * iniI_j);
									NS_j = (juvalpha * iniI_j) - (gS_j * iniS_j);
									ND_j = (gI_j * iniI_j) + (gS_j * iniS_j);




								iniH_m += NH_m;
								iniI_m += NI_m;
								iniS_m += NS_m;
								iniD_m += ND_m;

								iniH_j += NH_j;
								iniI_j += NI_j;
								iniS_j += NS_j;
								iniD_j += ND_j;

								
								//Check for impossible values (errors)//
								if (iniH_m + iniI_m + iniS_m + iniD_m > 1.000000001 ||
									iniH_m + iniI_m + iniS_m + iniD_m < 1 - 0.000000001) {
									throw std::logic_error("error in ini_m sum");
								}

								if (iniH_j + iniI_j + iniS_j + iniD_j > 1.000000001 ||
									iniH_j + iniI_j + iniS_j + iniD_j < 1 - 0.000000001) {
									throw std::logic_error("error in ini_j sum");
								}

								//Junk code with no need - controlled above//
								if (iniH_m > 1.0) { iniH_m = 1.0; }
								else if (iniH_m < 0.0) { iniH_m = 0.0; }
								if (iniI_m > 1.0) { iniI_m = 1.0; }
								else if (iniI_m < 0.0) { iniI_m = 0.0; }
								if (iniS_m > 1.0) { iniS_m = 1.0; }
								else if (iniS_m < 0.0) { iniS_m = 0.0; }
								if (iniD_m > 1.0) { iniD_m = 1.0; }
								else if (iniD_m < 0.0) { iniD_m = 0.0; }

								if (iniH_j > 1.0) { iniH_j = 1.0; }
								else if (iniH_j < 0.0) { iniH_j = 0.0; }
								if (iniI_j > 1.0) { iniI_j = 1.0; }
								else if (iniI_j < 0.0) { iniI_j = 0.0; }
								if (iniS_j > 1.0) { iniS_j = 1.0; }
								else if (iniS_j < 0.0) { iniS_j = 0.0; }
								if (iniD_j > 1.0) { iniD_j = 1.0; }
								else if (iniD_j < 0.0) { iniD_j = 0.0; }

								double round_check_m_u = iniH_m + iniI_m + iniS_m + iniD_m;

								if (iniH_m + iniI_m + iniS_m + iniD_m > 1.0) { 
									iniH_m = iniH_m/(round_check_m_u);
									iniI_m = iniI_m/(round_check_m_u);
									iniS_m = iniS_m/ (round_check_m_u);
									iniD_m = iniD_m/ (round_check_m_u);
								}
								
								double round_check_m_l = iniH_m + iniI_m + iniS_m + iniD_m;

								if (iniH_m + iniI_m + iniS_m + iniD_m < 1.0) {
									iniH_m = iniH_m/(round_check_m_l);
									iniI_m = iniI_m/(round_check_m_l);
									iniS_m = iniS_m/(round_check_m_l);
									iniD_m = iniD_m/(round_check_m_l);
								}

								double round_check_j_u = iniH_j + iniI_j + iniS_j + iniD_j;

								if (iniH_j + iniI_j + iniS_j + iniD_j > 1.0) {
									iniH_j = iniH_j/(round_check_j_u);
									iniI_j = iniI_j/(round_check_j_u);
									iniS_j = iniS_j/(round_check_j_u);
									iniD_j = iniD_j/(round_check_j_u);
								}

								double round_check_j_l = iniH_j + iniI_j + iniS_j + iniD_j;

								if (iniH_j + iniI_j + iniS_j + iniD_j < 1.0) {
									iniH_j = iniH_j / (round_check_j_l);
									iniI_j = iniI_j / (round_check_j_l);
									iniS_j = iniS_j / (round_check_j_l);
									iniD_j = iniD_j / (round_check_j_l);
								}

								//How many years the model ran for this cell//
								niyr = i;
								
								//Save ash dieback status independent of tree age//
								adbstartH = (iniH_j * juvprop) + (iniH_m * matprop);
								adbstartI = (iniI_j * juvprop) + (iniI_m * matprop);
								adbstartS = (iniS_j * juvprop) + (iniS_m * matprop);
								adbstartD = (iniD_j * juvprop) + (iniD_m * matprop);

								//For mature trees//
								adbstartHmat = iniH_m;
								adbstartImat = iniI_m;
								adbstartSmat = iniS_m;
								adbstartDmat = iniD_m;
								Setnremovedmat(jcount, icount, niyr, iniD_m); //Turn off NO ADB model
								//Setnremovedmat(jcount, icount, niyr, 0.0); //Turn on NO ADB model

								//For juvenile trees//
								adbstartHjuv = iniH_j;
								adbstartIjuv = iniI_j;
								adbstartSjuv = iniS_j;
								adbstartDjuv = iniD_j;
								Setnremovedjuv(jcount, icount, niyr, iniD_j); //Turn off NO ADB model
								//Setnremovedjuv(jcount, icount, niyr, 0.0); //Turn on NO ADB model

										}
								
								//Check for impossible values (i.e. an error) and then removing trees
								double sumadbstart = adbstartH + adbstartI + adbstartS + adbstartD;
								if (sumadbstart > 1.000000001 ||
									sumadbstart < 1-0.00000001) {
									throw std::logic_error("error in get sumadbstart");
								}
								//Change the 13 below to 18 if delaying EAB arrival by 5 years until 2031, and to 26 if wanting to examine ADB base layers in 2039//
								double totalremoved = (Getnremovedjuv(jcount, icount, 13 - adbyear - 3)*juvprop)+ (Getnremovedmat(jcount, icount, 13 - adbyear - 3) * matprop); //TURN OFF FOR NO ADB, TURN ON LINE BELOW
								//double totalremoved = 0.0; // add this line if we want to examine ash dieback SIR without removing trees// TURN ON FOR NO ADB MODEL
								double nadbstartD = adbstartD;
								if (totalremoved > 0.00000001) {
									nadbstartD = adbstartD - totalremoved;
								}
								if (nadbstartD < 0.0) { nadbstartD = 0.0; }

								double newprop = adbstartH+ adbstartI + adbstartS + nadbstartD;
								if (totalremoved > 0.00000001) {
									double junk = 0.0;
									double newH = adbstartH/newprop;
									double newI = adbstartI / newprop;
									double newS = adbstartS / newprop;
									double newD = nadbstartD / newprop;

									Setadbstart_H(jcount, icount, newH);
									Setadbstart_I(jcount, icount, newI);
									Setadbstart_S(jcount, icount, newS);
									Setadbstart_D(jcount, icount, newD);
								}

								else{
									double junk = 0.0;
									Setadbstart_H(jcount, icount, adbstartH);
									Setadbstart_I(jcount, icount, adbstartI);
									Setadbstart_S(jcount, icount, adbstartS);
									Setadbstart_D(jcount, icount, nadbstartD);
								}


								double sumadbstartb = Getadbstart_H(jcount,icount) + Getadbstart_I(jcount, icount) + Getadbstart_S(jcount, icount) + Getadbstart_D(jcount, icount);

								if (sumadbstartb > 1.000000001 ||
									sumadbstartb < 1 - 0.00000001) {
									throw std::logic_error("error in get sumadbstartb");
								}

								//Correct for rounding errors//
								if (sumadbstartb > 1.0) {
									Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount)/(sumadbstartb));
									Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount)/(sumadbstartb));
									Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount)/(sumadbstartb));
									Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount)/(sumadbstartb));
								}

								else if (sumadbstartb < 1.0) {
									Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount) / (sumadbstartb));
									Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount) / (sumadbstartb));
									Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount) / (sumadbstartb));
									Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount) / (sumadbstartb));
								}


								//Remove trees from mature ash category//
								double sumcropmat = adbstartHmat + adbstartImat + adbstartSmat + adbstartDmat;

								//Check for positive and negatives (i.e. overshot it)
								if (sumcropmat > 1.000000001 ||
									sumcropmat < 1-0.00000001) {
									throw std::logic_error("error in get sumcropmat");
								}

								double nadbstartDmat = adbstartDmat;
								if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
									nadbstartDmat = adbstartDmat - Getnremovedmat(jcount, icount, 13 - adbyear - 3);
								}
								if (nadbstartDmat < 0.0) { nadbstartDmat = 0.0; }
								double newpropmat = adbstartHmat+ adbstartImat+ adbstartSmat+ nadbstartDmat;

								//Adjust for removed trees//
								if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
									double newHm = adbstartHmat / newpropmat;
									double newIm = adbstartImat / newpropmat;
									double newSm = adbstartSmat / newpropmat;
									double newDm = nadbstartDmat / newpropmat;

									SetCropmat(jcount, icount, 0, newHm);
									SetCropmat(jcount, icount, 1, newIm);
									SetCropmat(jcount, icount, 2, newSm);
									SetCropmat(jcount, icount, 3, newDm);
								}
								else{
									SetCropmat(jcount, icount, 0, adbstartHmat);
									SetCropmat(jcount, icount, 1, adbstartImat);
									SetCropmat(jcount, icount, 2, adbstartSmat);
									SetCropmat(jcount, icount, 3, nadbstartDmat);
								}


								if (GetCropmat(jcount, icount,0) < 0.0) { SetCropmat(jcount, icount,0, 0.0); }
								if (GetCropmat(jcount, icount,1) < 0.0) { SetCropmat(jcount, icount,1, 0.0); }
								if (GetCropmat(jcount, icount,2) < 0.0) { SetCropmat(jcount, icount,2, 0.0); }
								if (GetCropmat(jcount, icount,3) < 0.0) { SetCropmat(jcount, icount,3, 0.0); }


								//Correct for rounding errors//
								double sumcropmatb = GetCropmat(jcount, icount, 0) + GetCropmat(jcount, icount, 1) + GetCropmat(jcount, icount, 2) + GetCropmat(jcount, icount, 3);

								if (sumcropmatb > 1.000000001 ||
									sumcropmatb < 1 - 0.00000001) {
									throw std::logic_error("error in get sumcropmatb");
								}

								if (sumcropmatb > 1.0) {
									SetCropmat(jcount, icount,0, GetCropmat(jcount, icount,0)/(sumcropmatb));
									SetCropmat(jcount, icount,1, GetCropmat(jcount, icount,1)/(sumcropmatb));
									SetCropmat(jcount, icount,2, GetCropmat(jcount, icount,2)/(sumcropmatb));
									SetCropmat(jcount, icount,3, GetCropmat(jcount, icount,3)/(sumcropmatb));
								}

								else if (sumcropmatb < 1.0) {
									SetCropmat(jcount, icount, 0, GetCropmat(jcount, icount, 0) / (sumcropmatb));
									SetCropmat(jcount, icount, 1, GetCropmat(jcount, icount, 1) / (sumcropmatb));
									SetCropmat(jcount, icount, 2, GetCropmat(jcount, icount, 2) / (sumcropmatb));
									SetCropmat(jcount, icount, 3, GetCropmat(jcount, icount, 3) / (sumcropmatb));
								}


								//Now repeat for juvenile trees//
								double sumcropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + adbstartDjuv;

								if (sumcropjuv > 1.000000001 ||
									sumcropjuv < 1-0.00000001) {
									throw std::logic_error("error in get startsumcropjuv");
								}

								double nadbstartDjuv = adbstartDjuv;
								if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
									nadbstartDjuv = adbstartDjuv - Getnremovedjuv(jcount, icount, 13 - adbyear - 3);
								}
								if (nadbstartDjuv < 0.0) { nadbstartDjuv = 0.0; }
								double newpropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + nadbstartDjuv;

								
								if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
									double newHj = adbstartHjuv / newpropjuv;
									double newIj = adbstartIjuv / newpropjuv;
									double newSj = adbstartSjuv / newpropjuv;
									double newDj = nadbstartDjuv / newpropjuv;

									SetCropjuv(jcount, icount, 0, newHj);
									SetCropjuv(jcount, icount, 1, newIj);
									SetCropjuv(jcount, icount, 2, newSj);
									SetCropjuv(jcount, icount, 3, newDj);
								}
								else{
									SetCropjuv(jcount, icount, 0, adbstartHjuv);
									SetCropjuv(jcount, icount, 1, adbstartIjuv);
									SetCropjuv(jcount, icount, 2, adbstartSjuv);
									SetCropjuv(jcount, icount, 3, nadbstartDjuv);
								}

								if (GetCropjuv(jcount, icount, 0) < 0.0) { SetCropjuv(jcount, icount, 0, 0.0); }
								if (GetCropjuv(jcount, icount, 1) < 0.0) { SetCropjuv(jcount, icount, 1, 0.0); }
								if (GetCropjuv(jcount, icount, 2) < 0.0) { SetCropjuv(jcount, icount, 2, 0.0); }
								if (GetCropjuv(jcount, icount, 3) < 0.0) { SetCropjuv(jcount, icount, 3, 0.0); }


								
								double sumcropjuvb = GetCropjuv(jcount, icount, 0) + GetCropjuv(jcount, icount, 1) + GetCropjuv(jcount, icount, 2) + GetCropjuv(jcount, icount, 3);

								if (sumcropjuvb > 1.000000001 ||
									sumcropjuvb < 1 - 0.00000001) {
									throw std::logic_error("error in get sumcropjuvb");
								}

								if (sumcropjuvb > 1.0) {
									SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
								}

								else if (sumcropjuvb < 1.0) {
									SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
									SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
								}

								if (totalremoved < 1.0) {

									SetMatShare(jcount, icount, matprop * (1.0 - Getnremovedmat(jcount, icount, 13 - adbyear - 3)) / (1.0 - totalremoved)); //i.e. mature survivors / all survivors
									//SetMatShare(jcount, icount, matprop); //TURN ON NO ADB MODEL - TURN OFF LINE ABOVE
								}

								SetTreeDensity(jcount, icount, GetTreeDensity(jcount, icount)* (1-totalremoved));

									}
						

						//Here we set the first cell EAB landed
						if (GetTreeDensity(jcount, icount) >0 && adbyear > -1)
						{
							double aH = Grid::Getadbstart_H(jcount, icount);
							double aI = Grid::Getadbstart_I(jcount, icount);
							double aS = Grid::Getadbstart_S(jcount, icount);
							double aD = Grid::Getadbstart_D(jcount, icount);

							SetCrop(jcount, icount, 0, Grid::Getadbstart_H(jcount, icount)); //No EAB + Asymptomatic
							SetCrop(jcount, icount, 1, Grid::Getadbstart_I(jcount, icount)); //No EAB + Lightly stessed
							SetCrop(jcount, icount, 2, Grid::Getadbstart_S(jcount, icount)); //No EAB + Highly stressed
							SetCrop(jcount, icount, 3, Grid::Getadbstart_D(jcount, icount)); //No EAB + Recently dead
							SetCrop(jcount, icount, 4, 0.0); //EAB present + Asymptomatic
							SetCrop(jcount, icount, 5, 0.0); //EAB present + Lightly stessed
							SetCrop(jcount, icount, 6, 0.0); //EAB present + Highly stressed
							SetCrop(jcount, icount, 7, 0.0); //EAB present + Recently dead
							SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
							SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
							SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
							SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

							double testrow = infCelljRow;
							double testcol = infCelliCol;
							if ((jcount == infCelljRow) && (icount == infCelliCol))
							{

								double EABonADB_H = (ProbIniTreeInfection)*Grid::Getadbstart_H(jcount, icount);
								double EABonADB_I = (ProbIniTreeInfection) * Grid::Getadbstart_I(jcount, icount);
								double EABonADB_S = (ProbIniTreeInfection) * Grid::Getadbstart_S(jcount, icount);
								double EABonADB_D = (ProbIniTreeInfection)*Grid::Getadbstart_D(jcount, icount);
								
								double noEABonADB_H = (1- ProbIniTreeInfection)*Grid::Getadbstart_H(jcount, icount);
								double noEABonADB_I = (1- ProbIniTreeInfection)*Grid::Getadbstart_I(jcount, icount);
								double noEABonADB_S = (1 - ProbIniTreeInfection)*Grid::Getadbstart_S(jcount, icount);
								double noEABonADB_D = (1 - ProbIniTreeInfection)*Grid::Getadbstart_D(jcount, icount);



								SetCrop(jcount, icount, 0, noEABonADB_H); //No EAB + Asymptomatic
								SetCrop(jcount, icount, 1, noEABonADB_I); //No EAB + Lightly stessed
								SetCrop(jcount, icount, 2, noEABonADB_S); //No EAB + Highly stressed
								SetCrop(jcount, icount, 3, noEABonADB_D); //No EAB + Recently dead
								SetCrop(jcount, icount, 4, EABonADB_H); //EAB present + Asymptomatic
								SetCrop(jcount, icount, 5, EABonADB_I); //EAB present + Lightly stessed
								SetCrop(jcount, icount, 6, EABonADB_S); //EAB present + Highly stressed
								SetCrop(jcount, icount, 7, EABonADB_D); // EAB present + Recently dead
								SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
								SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
								SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
								SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

								double sumcrops = GetCrop(jcount,icount,0)+ GetCrop(jcount, icount, 1)+ GetCrop(jcount, icount, 2)+
									GetCrop(jcount, icount, 3)+ GetCrop(jcount, icount, 4)+ GetCrop(jcount, icount, 5)+
									GetCrop(jcount, icount, 6)+ GetCrop(jcount, icount, 7)+ GetCrop(jcount, icount, 8)+
									GetCrop(jcount, icount, 9)+ GetCrop(jcount, icount, 10)+ GetCrop(jcount, icount, 11);
							
							
							
							}
							//if (Sick==155000) // we plop in a seed of EAB of 1% of the trees




						}
						
					
					}
	
				}
				InF.close();
				//InFF.close();
				InFFF.close();
				InF4.close();
				InFarrival.close();
				InFeusoil.close();
				InFsoil.close();
				InFnonwoodp.close();
				//InC.close();
			}
				else{
					throw std::logic_error("Error opening inpur files");}



			Grid::SetTotalPlantasFilled(-1);
			int mySymp(0); //Set to zero is max syptomatic trees in any one cell in the plantaion is less than 0.05
			double myWorry(0); //start unworried
			//int myCHMA(0);
			
			for (int pcount=0; pcount<MaxPlantID; pcount++)
			{
				//int myCells[MaxNumPCells]; 
				std::vector<int> myCells;
				int countcell=0;
				for (int jcount=0; jcount<Nrows; jcount++)
				{
					for (int icount=0; icount<Ncols; icount++)
					{
						if (pcount+1==PlantaID[jcount][icount])
						{
							myCells.push_back(CoordToGridID(jcount, icount)); //gathering the co-ordinates of all cells in a plantation(grove)
							//myCHMA=CHMAID[jcount][icount];
							
						}
					}
				}
				if (myCells.size()<1)
					throw std::logic_error("Number of cells in a plantation is less that 1");

		
				
			}

		}
		
	} //end of function SetLand

	void ReSetProbDetection()
	{
		for (int jcount = 0; jcount < Nrows; jcount++)
		{
			for (int icount = 0; icount < Ncols; icount++)
			{
				ProbDetection[jcount][icount] = 0;

			}
		}


	}
	void SetProbDetection()
	{
		int countcell = 0;
		for (int irow = 0; irow < Nrows; irow++)
		{
			for (int jcol = 0; jcol < Ncols; jcol++)
			{
				double myTreeDensity = Grid::GetTreeDensity(irow, jcol);
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
					double propEInfGTYr = tempTreeStates[8] + tempTreeStates[9] + tempTreeStates[10] + tempTreeStates[11]; // GT a year of EAB
					double NumTreesHealthy = Grid::GetTreeDensity(irow, jcol) * propENoEAB;
					double NumTreesInfested = Grid::GetTreeDensity(irow, jcol) * (1 - propENoEAB);
				
					double myNumLarvae(0);
			
					double c_parameter(0.026); // Girdelled 0.5096; Traps 0.052; //visual = 0.026
					double NumTreeInspect(10);
					Grid::GetOWLav(irow, jcol, myNumLarvae);
					double phloemPerTree = Grid::GetPhloemPerTree();
					double myDetect = 0;
				
					///////////////////////////////////////////////////////////////////////
					double P_LarvToAdult(0.0);
					Grid::GetPLarv(irow, jcol, P_LarvToAdult);
					///////////////////////////////////////////////////////////////////////

						double AveLavPerTree = myNumLarvae * phloemPerTree * NumTreesInfested / (myTreeDensity* P_LarvToAdult);
						myDetect = 1 - exp(-c_parameter * NumTreeInspect * AveLavPerTree);
				
					if (myDetect != 0)
						int junk = 1;
					ProbDetection[irow][jcol] = ProbDetection[irow][jcol] + myDetect;
				}
				//else
				//{
					//ProbDetection[irow][jcol] = 0;
				//}

			}
		}

	}

	void SetWeightedProbDetection(double myWeight)
	{
		int countcell = 0;
		for (int irow = 0; irow < Nrows; irow++)
		{
			for (int jcol = 0; jcol < Ncols; jcol++)
			{
				double myTreeDensity = Grid::GetTreeDensity(irow, jcol);
				if (myTreeDensity > 0)
				{
					double tempTreeStates[12];
					for (int icount = 0; icount < Grid::GetMaxNumStatus(); icount++)
					{
						tempTreeStates[icount] = Grid::GetCrop(irow, jcol, icount);
					}

					//Get Infected trees 
					double propENoEAB = tempTreeStates[0] + tempTreeStates[1] + tempTreeStates[2] + tempTreeStates[3];
					double propEInfGTYr = tempTreeStates[8] + tempTreeStates[9] + tempTreeStates[10] + tempTreeStates[11]; // GT a year of EAB
					double NumTreesHealthy = Grid::GetTreeDensity(irow, jcol) * propENoEAB; 
					double NumTreesInfested = Grid::GetTreeDensity(irow, jcol) * (1 - propENoEAB);

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
					myDetect = 1 - exp(-c_parameter * NumTreeInspect * AveLavPerTree);

					if (myDetect != 0)
						int junk = 1;
					ProbDetection[irow][jcol] = ProbDetection[irow][jcol] + myWeight*myDetect;
				}
				/*else
				{
					ProbDetection[irow][jcol] = 0;
				}*/

			}
		}

	}

	double GetProbDetect(int irow, int jcol)
	{
		if ((irow > -1) && (irow < Grid::GetNumRows()) && (jcol > -1) && (jcol < Grid::GetNumCols()))
			return ProbDetection[irow][jcol];
		else
			throw std::logic_error("index out of bounds in GetProbDetect");

	}

	void SetLandBasic(int& iflag)
	{

		int UBlockID = 1; //unique Block ID	

				for (int icount = 0; icount < Nrows; icount++)
				{
					for (int jcount = 0; jcount < Ncols; jcount++)
					{
						SetTreeDensity(icount, jcount, 0.0); // This is just cleaning it before we start; 
						Setadbarrival(icount, jcount, 0);
						Setadbeusoil(icount, jcount, 0.0);
						Setadbsoil(icount, jcount, 0);
						Setnonwoodp(icount, jcount, 0.0);
						Setadbstart_H(icount, jcount, 0.0);
						Setadbstart_I(icount, jcount, 0.0);
						Setadbstart_S(icount, jcount, 0.0);
						Setadbstart_D(icount, jcount, 0.0);
						Setadbstart_R(icount, jcount, 0.0);

						SetInitialTreeDensity(icount, jcount, 0.0); // This is just cleaning it before we start; 
						SetPlanta(icount, jcount, -9);
						//SetBlock(icount,jcount,-9); //removed all block stuff for now don't think we need it
						for (int kcount = 0; kcount < NStatus; kcount++)
							SetCrop(icount, jcount, kcount, 0);  //set all status to zero

					}
				}

					initialiseadbmatgamma();
					initialiseadbsoileff();

					char path_buffer[_MAX_PATH];
					char Npath_buffer[_MAX_PATH];
					_getcwd(path_buffer, _MAX_PATH);
					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\Owners_GB.txt");

					std::ifstream    InF(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\treeDensity_GB.txt");
					std::ifstream	 InFFF(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					//Create files for SE; 

					strcat_s(Npath_buffer, "\\TreeDensity\\InclusionProbGB.txt");

					std::ifstream	 InF4(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\arrival_GB3011.txt");
					std::ifstream    InFarrival(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\eusoil_GB3011.txt");
					std::ifstream    InFeusoil(Npath_buffer);


					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\soil_GB3011.txt");
					std::ifstream    InFsoil(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\nonwoodp_GB3011.txt");
					std::ifstream    InFnonwoodp(Npath_buffer);

					strcpy_s(Npath_buffer, path_buffer);
					strcat_s(Npath_buffer, "\\TreeDensity\\precipp_GB3011.txt");
					std::ifstream    InFprecipp(Npath_buffer);


					int TempOwner;
					double TempTreeDensity, ADBInf, infectionStatus;
					int Sick(0); //Flag to see of sick tree set/
					int iyr = 2;
					int MaxPlantID = -1; // This keeps track of the maximum ID number for the plantations for use later 


		
		if (InF && InFFF && InF4 && InFarrival && InFeusoil && InFsoil && InFnonwoodp && InFprecipp)
		{
			double myCountProb = 0;
			int infCelliCol = 0;
			int infCelljRow = 0;
			for (int jcount = 0; jcount < Nrows; jcount++)
			{
				for (int icount = 0; icount < Ncols; icount++)
				{
					InF >> TempOwner;
					PlantaID[jcount][icount] = TempOwner;
					if (TempOwner > MaxPlantID)
						MaxPlantID = TempOwner;
					InFFF >> TempTreeDensity;
					TempTreeDensity = TempTreeDensity*TreesPerHa;
					G_TreeDensity[jcount][icount] = TempTreeDensity;
					IniG_TreeDensity[jcount][icount] = TempTreeDensity;
					if (TempTreeDensity < 1.0)
						TempTreeDensity = 1.0;
					double ProbIniTreeInfection = 1 / TempTreeDensity; // This is now set so we have 1 tree infected per cell.

					int Temparrival;
					InFarrival >> Temparrival;
					G_adbarrival[jcount][icount] = Temparrival; //Filling our grid with arrival dates

					int Tempsoil;
					InFsoil >> Tempsoil;
					G_adbsoil[jcount][icount] = Tempsoil; //Filling our grid with soil drainage values

					double Tempeusoil;
					InFeusoil >> Tempeusoil;
					G_adbeusoil[jcount][icount] = Tempeusoil; //Filling our grid with soil water content values

					double Tempnonwoodp;
					InFnonwoodp >> Tempnonwoodp;
					G_adbnonwoodp[jcount][icount] = Tempnonwoodp; //Filling our grid with the proportion of nonwoodtrees

					double Tempprecipp;
					InFprecipp >> Tempprecipp;
					G_adbprecipp[jcount][icount] = Tempprecipp; //Filling our grid with precipitation difference from minimum values

					double adbstartH = 0.0, adbstartI = 0.0, adbstartS = 0.0, adbstartD = 0.0, adbstartR = 0.0, iniremovefunction = 0.0;
					double adbstartHmat = 0.0, adbstartImat = 0.0, adbstartSmat = 0.0, adbstartDmat = 0.0, adbstartRmat = 0.0, iniremovefunctionmat;
					double adbstartHjuv = 0.0, adbstartIjuv = 0.0, adbstartSjuv = 0.0, adbstartDjuv = 0.0, adbstartRjuv = 0.0, iniremovefunctionjuv;


					int adbyear = G_adbarrival[jcount][icount];

					double numTrees = GetTreeDensity(jcount, icount); //number of trees in each cell
					
					//Setting the proportion of mature trees in a cell//
					SetMatShare(jcount, icount, 0.29);

					//Setting a value for ADB Resistance in cells
					double ResProp = 0.019 + 0.036 * RNG_Uniform();
					SetResProp(jcount, icount, ResProp);

					if (numTrees > 0 && adbyear > -1) {

						//Define start conditions for compartmental model
						double iniH_m = 0.99;
						double iniI_m = 0.01;
						double iniS_m = 0.00;
						double iniD_m = 0.00;

						double iniH_j = 0.99;
						double iniI_j = 0.01;
						double iniS_j = 0.00;
						double iniD_j = 0.00;

						//ADB arrival working with assumption that cells where disease was detected in 2013, ADB symtpoms were
						//present throughout these cells (i.e. had been present a number of years - only detected when symptoms clear), this then spreads at a rate of
						//50 km per year from these cells.

						int adbyear = G_adbarrival[jcount][icount]; //will return the year of disease arrival from the cell
						int adbsoil = G_adbsoil[jcount][icount]; // soil drainage
						double adbeusoil = G_adbeusoil[jcount][icount]; //estimate soil water content
						double nonwoodp = G_adbnonwoodp[jcount][icount]; //proportion of trees in non-wood setting
						double inimatgamma = 0.0205; //mature gamma values (0.009 for minimum, 0.0205 for median, 0.032 for max)
						double inijuvgamma = inimatgamma * (0.1 / 0.032); //Calculate juvenile mortality
						double adbeta = 0.5; //Beta value
						double nonwoodeff = 1.0 - (0.19 / 0.41); //This is the difference in crown dieback between Hedgerow and Woodland trees
						double matprop = 0.29; //define the proportion of mature ash in GB woodlands
						double juvprop = 0.71; //define the proportion of juvenile ash in GB woodlands
						double matstresseff = 0.724; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
						double juvstresseff = 0.712; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
						double mortratio = 3.0; //Difference in rate severly stressed trees die vs lightly stressed trees - will adjust with q values//
						double precipmortklesse = (1.0 / (1.0 + exp(-(0.5 - 6.7)))); //Mortality given a unit precipitation increase
						double noprecipmortklesse = (1.0 / (1.0 + exp(-(-6.7)))); //Mortality at minimum precipitation
						double precippercentchange = ((precipmortklesse / noprecipmortklesse) - 1) / (312.0 + 1305.575); //Percent change in mortality per unit increase in rainfall//


						double precippmat = (inimatgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount]))) - inimatgamma; //i.e. the additional gamma value per cell//
						double precippjuv = (inijuvgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount]))) - inijuvgamma; //i.e. the additional gamma value per cell//

						double soilp = 1.0;
						if (adbsoil > 4 && adbeusoil > 0.3605875) { //adding additional ADB for 'wet' cells
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

						//Initialising variables//
						double NH_m, NI_m, NS_m, ND_m;
						double NH_j, NI_j, NS_j, ND_j;
						int niyr;

						//Running for 13 years(2013-2026) - how many years it took for disease to arrive after 2013//
						for (int i = 0; i < 14 - adbyear; i++) { //Change the 14 to 19 if we want EAB arrival in 2031 rather than 2026; and to 27 if want ADB base layer in 2039

							//Add some resistant ash and more S trees dying than I trees//

						    //Calculating mortality split between categories - with S trees dying three times the rate than I trees
							// with same overall annual mortality rate. Accounting for the resistant ash in a cell - i.e. these say in H category

							double gI_m = matgamma;
							double gS_m = matgamma;
							double den_m = iniI_m + (mortratio * iniS_m);

							if (den_m > 0.0) {

								gI_m = matgamma * (iniI_m + iniS_m) / den_m;
								gS_m = mortratio * gI_m;
							}

							if (gS_m > 1.0) { gS_m = 1.0; }

							double availH_m = iniH_m - ResProp;
							if (availH_m < 0.0) { availH_m = 0.0; }

							double gI_j = juvgamma;
							double gS_j = juvgamma;
							double den_j = iniI_j + (mortratio * iniS_j);

							if (den_j > 0.0) {

								gI_j = juvgamma * (iniI_j + iniS_j) / den_j;
								gS_j = mortratio * gI_j;
							}

							if (gS_j > 1.0) { gS_j = 1.0; }

							double availH_j = iniH_j - ResProp;
							if (availH_j < 0.0) { availH_j = 0.0; }

							//Compartmental model for mature and juvenile ash separately//
							NH_m = -adbeta * availH_m;
							NI_m = (adbeta * availH_m) - (gI_m * iniI_m) - (matalpha * iniI_m);
							NS_m = (matalpha * iniI_m) - (gS_m * iniS_m);
							ND_m = (gI_m * iniI_m) + (gS_m * iniS_m);

							NH_j = -adbeta * availH_j;
							NI_j = (adbeta * availH_j) - (gI_j * iniI_j) - (juvalpha * iniI_j);
							NS_j = (juvalpha * iniI_j) - (gS_j * iniS_j);
							ND_j = (gI_j * iniI_j) + (gS_j * iniS_j);

							iniH_m += NH_m;
							iniI_m += NI_m;
							iniS_m += NS_m;
							iniD_m += ND_m;

							iniH_j += NH_j;
							iniI_j += NI_j;
							iniS_j += NS_j;
							iniD_j += ND_j;

							//Check for impossible values (i.e. errors)//
							if (iniH_m + iniI_m + iniS_m + iniD_m > 1.000000001 ||
								iniH_m + iniI_m + iniS_m + iniD_m < 1-0.000000001) {
								throw std::logic_error("error in ini_m sum");
							}

							if (iniH_j + iniI_j + iniS_j + iniD_j > 1.000000001 ||
								iniH_j + iniI_j + iniS_j + iniD_j < 1-0.000000001) {
								throw std::logic_error("error in ini_j sum");
							}

							//Junk code with no need - controlled above//
							if (iniH_m > 1.0) { iniH_m = 1.0; }
							else if (iniH_m < 0.0) { iniH_m = 0.0; }
							if (iniI_m > 1.0) { iniI_m = 1.0; }
							else if (iniI_m < 0.0) { iniI_m = 0.0; }
							if (iniS_m > 1.0) { iniS_m = 1.0; }
							else if (iniS_m < 0.0) { iniS_m = 0.0; }
							if (iniD_m > 1.0) { iniD_m = 1.0; }
							else if (iniD_m < 0.0) { iniD_m = 0.0; }

							if (iniH_j > 1.0) { iniH_j = 1.0; }
							else if (iniH_j < 0.0) { iniH_j = 0.0; }
							if (iniI_j > 1.0) { iniI_j = 1.0; }
							else if (iniI_j < 0.0) { iniI_j = 0.0; }
							if (iniS_j > 1.0) { iniS_j = 1.0; }
							else if (iniS_j < 0.0) { iniS_j = 0.0; }
							if (iniD_j > 1.0) { iniD_j = 1.0; }
							else if (iniD_j < 0.0) { iniD_j = 0.0; }

							double round_check_m_u = iniH_m + iniI_m + iniS_m + iniD_m;

							if (iniH_m + iniI_m + iniS_m + iniD_m > 1.0) {
								iniH_m = iniH_m / (round_check_m_u);
								iniI_m = iniI_m / (round_check_m_u);
								iniS_m = iniS_m / (round_check_m_u);
								iniD_m = iniD_m / (round_check_m_u);
							}

							double round_check_m_l = iniH_m + iniI_m + iniS_m + iniD_m;
							if (iniH_m + iniI_m + iniS_m + iniD_m < 1.0) {
								iniH_m = iniH_m / (round_check_m_l);
								iniI_m = iniI_m / (round_check_m_l);
								iniS_m = iniS_m / (round_check_m_l);
								iniD_m = iniD_m / (round_check_m_l);
							}

							double round_check_j_u = iniH_j + iniI_j + iniS_j + iniD_j;
							if (iniH_j + iniI_j + iniS_j + iniD_j > 1.0) {
								iniH_j = iniH_j / (round_check_j_u);
								iniI_j = iniI_j / (round_check_j_u);
								iniS_j = iniS_j / (round_check_j_u);
								iniD_j = iniD_j / (round_check_j_u);
							}

							double round_check_j_l = iniH_j + iniI_j + iniS_j + iniD_j;
							if (iniH_j + iniI_j + iniS_j + iniD_j < 1.0) {
								iniH_j = iniH_j / (round_check_j_l);
								iniI_j = iniI_j / (round_check_j_l);
								iniS_j = iniS_j / (round_check_j_l);
								iniD_j = iniD_j / (round_check_j_l);
							}

							//How many years the model ran for this cell//
							niyr = i;
							
							//Save ash dieback status independent of tree age//
							adbstartH = (iniH_j * juvprop) + (iniH_m * matprop);
							adbstartI = (iniI_j * juvprop) + (iniI_m * matprop);
							adbstartS = (iniS_j * juvprop) + (iniS_m * matprop);
							adbstartD = (iniD_j * juvprop) + (iniD_m * matprop);

							//For mature trees//
							adbstartHmat = iniH_m;
							adbstartImat = iniI_m;
							adbstartSmat = iniS_m;
							adbstartDmat = iniD_m;
							Setnremovedmat(jcount, icount, niyr, iniD_m); // Remove NO ADB model
							//Setnremovedmat(jcount, icount, niyr, 0.0); // Add for NO ADB model

							//For juvenile trees//
							adbstartHjuv = iniH_j;
							adbstartIjuv = iniI_j;
							adbstartSjuv = iniS_j;
							adbstartDjuv = iniD_j;
							Setnremovedjuv(jcount, icount, niyr, iniD_j); // Remove NO ADB model
							//Setnremovedjuv(jcount, icount, niyr, 0.0); // Add for NO ADB model
						}

						//For trees irrespective of age - check for impossible values again and then removing trees//
						double sumadbstart = adbstartH + adbstartI + adbstartS + adbstartD;
						if (sumadbstart > 1.000000001 ||
							sumadbstart < 1-0.00000001) {
							throw std::logic_error("error in get sumadbstart");
						}
						//Change the 13 below to 18 if delaying EAB arrival by 5 years until 2031, and to 26 if wanting to examine ADB base layers in 2039//
						double totalremoved = (Getnremovedjuv(jcount, icount, 13 - adbyear - 3)*juvprop)+ (Getnremovedmat(jcount, icount, 13 - adbyear - 3) * matprop);  // TURN OFF FOR NO ADB MODEL, TURN ON LINE BELOW
						//double totalremoved = 0.0; // add this line if we want to examine ash dieback SIR without removing trees// TURN ON FOR NO ADB MODEL
						double nadbstartD = adbstartD;
						if (totalremoved > 0.00000001) {
							nadbstartD = adbstartD - totalremoved;
						}
						if (nadbstartD < 0.0) { nadbstartD = 0.0; }

						double newprop = adbstartH + adbstartI + adbstartS + nadbstartD;
						if (totalremoved > 0.00000001) {
							double junk = 0.0;
							double newH = adbstartH / newprop;
							double newI = adbstartI / newprop;
							double newS = adbstartS / newprop;
							double newD = nadbstartD / newprop;

							Setadbstart_H(jcount, icount, newH);
							Setadbstart_I(jcount, icount, newI);
							Setadbstart_S(jcount, icount, newS);
							Setadbstart_D(jcount, icount, newD);
						}

						else {
							double junk = 0.0;
							Setadbstart_H(jcount, icount, adbstartH);
							Setadbstart_I(jcount, icount, adbstartI);
							Setadbstart_S(jcount, icount, adbstartS);
							Setadbstart_D(jcount, icount, nadbstartD);
						}


						double sumadbstartb = Getadbstart_H(jcount, icount) + Getadbstart_I(jcount, icount) + Getadbstart_S(jcount, icount) + Getadbstart_D(jcount, icount);
						//check for errors//
						if (sumadbstartb > 1.000000001 ||
							sumadbstartb < 1 - 0.00000001) {
							throw std::logic_error("error in get sumadbstart");
						}

						if (sumadbstartb > 1.0) {
							Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount) / (sumadbstartb));
							Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount) / (sumadbstartb));
							Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount) / (sumadbstartb));
							Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount) / (sumadbstartb));
						}

						else if (sumadbstartb < 1.0) {
							Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount) / (sumadbstartb));
							Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount) / (sumadbstartb));
							Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount) / (sumadbstartb));
							Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount) / (sumadbstartb));
						}


						//Remove trees from mature ash category//
						double sumcropmat = adbstartHmat + adbstartImat + adbstartSmat + adbstartDmat;
						
						//check for errors//
						if (sumcropmat > 1.000000001 ||
							sumcropmat < 1-0.00000001) {
							throw std::logic_error("error in get startsumcropmat");
						}

						double nadbstartDmat = adbstartDmat;
						if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
							nadbstartDmat = adbstartDmat - Getnremovedmat(jcount, icount, 13 - adbyear - 3);
						}
						if (nadbstartDmat < 0.0) { nadbstartDmat = 0.0; }
						double newpropmat = adbstartHmat + adbstartImat + adbstartSmat + nadbstartDmat;

						//Adjust for removed trees//
						if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
							double newHm = adbstartHmat / newpropmat;
							double newIm = adbstartImat / newpropmat;
							double newSm = adbstartSmat / newpropmat;
							double newDm = nadbstartDmat / newpropmat;

							SetCropmat(jcount, icount, 0, newHm);
							SetCropmat(jcount, icount, 1, newIm);
							SetCropmat(jcount, icount, 2, newSm);
							SetCropmat(jcount, icount, 3, newDm);
						}
						else {
							SetCropmat(jcount, icount, 0, adbstartHmat);
							SetCropmat(jcount, icount, 1, adbstartImat);
							SetCropmat(jcount, icount, 2, adbstartSmat);
							SetCropmat(jcount, icount, 3, nadbstartDmat);
						}


						if (GetCropmat(jcount, icount, 0) < 0.0) { SetCropmat(jcount, icount, 0, 0.0); }
						if (GetCropmat(jcount, icount, 1) < 0.0) { SetCropmat(jcount, icount, 1, 0.0); }
						if (GetCropmat(jcount, icount, 2) < 0.0) { SetCropmat(jcount, icount, 2, 0.0); }
						if (GetCropmat(jcount, icount, 3) < 0.0) { SetCropmat(jcount, icount, 3, 0.0); }


						
						double sumcropmatb = GetCropmat(jcount, icount, 0) + GetCropmat(jcount, icount, 1) + GetCropmat(jcount, icount, 2) + GetCropmat(jcount, icount, 3);

						//check for errors//
						if (sumcropmatb > 1.000000001 ||
							sumcropmatb < 1 - 0.00000001) {
							throw std::logic_error("error in get sumcropmatb");
						}

						if (sumcropmatb > 1.0) {
							SetCropmat(jcount, icount, 0, GetCropmat(jcount, icount, 0) / (sumcropmatb));
							SetCropmat(jcount, icount, 1, GetCropmat(jcount, icount, 1) / (sumcropmatb));
							SetCropmat(jcount, icount, 2, GetCropmat(jcount, icount, 2) / (sumcropmatb));
							SetCropmat(jcount, icount, 3, GetCropmat(jcount, icount, 3) / (sumcropmatb));
						}

						else if (sumcropmatb < 1.0) {
							SetCropmat(jcount, icount, 0, GetCropmat(jcount, icount, 0) / (sumcropmatb));
							SetCropmat(jcount, icount, 1, GetCropmat(jcount, icount, 1) / (sumcropmatb));
							SetCropmat(jcount, icount, 2, GetCropmat(jcount, icount, 2) / (sumcropmatb));
							SetCropmat(jcount, icount, 3, GetCropmat(jcount, icount, 3) / (sumcropmatb));
						}


						//Now repeat for juvenile trees//
						double sumcropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + adbstartDjuv;

						if (sumcropjuv > 1.000000001 ||
							sumcropjuv < 1-0.00000001) {
							throw std::logic_error("error in get startsumcropjuv");
						}

						double nadbstartDjuv = adbstartDjuv;
						if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
							nadbstartDjuv = adbstartDjuv - Getnremovedjuv(jcount, icount, 13 - adbyear - 3);
						}
						if (nadbstartDjuv < 0.0) { nadbstartDjuv = 0.0; }
						double newpropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + nadbstartDjuv;

						
						if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
							double newHj = adbstartHjuv / newpropjuv;
							double newIj = adbstartIjuv / newpropjuv;
							double newSj = adbstartSjuv / newpropjuv;
							double newDj = nadbstartDjuv / newpropjuv;

							SetCropjuv(jcount, icount, 0, newHj);
							SetCropjuv(jcount, icount, 1, newIj);
							SetCropjuv(jcount, icount, 2, newSj);
							SetCropjuv(jcount, icount, 3, newDj);
						}
						else {
							SetCropjuv(jcount, icount, 0, adbstartHjuv);
							SetCropjuv(jcount, icount, 1, adbstartIjuv);
							SetCropjuv(jcount, icount, 2, adbstartSjuv);
							SetCropjuv(jcount, icount, 3, nadbstartDjuv);
						}

						if (GetCropjuv(jcount, icount, 0) < 0.0) { SetCropjuv(jcount, icount, 0, 0.0); }
						if (GetCropjuv(jcount, icount, 1) < 0.0) { SetCropjuv(jcount, icount, 1, 0.0); }
						if (GetCropjuv(jcount, icount, 2) < 0.0) { SetCropjuv(jcount, icount, 2, 0.0); }
						if (GetCropjuv(jcount, icount, 3) < 0.0) { SetCropjuv(jcount, icount, 3, 0.0); }


						//check for errors//
						double sumcropjuvb = GetCropjuv(jcount, icount, 0) + GetCropjuv(jcount, icount, 1) + GetCropjuv(jcount, icount, 2) + GetCropjuv(jcount, icount, 3);

						if (sumcropjuvb > 1.000000001 ||
							sumcropjuvb < 1 - 0.00000001) {
							throw std::logic_error("error in get sumcropjuvb");
						}

						if (sumcropjuvb > 1.0) {
							SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
						}

						else if (sumcropjuvb < 1.0) {
							SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
							SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
						}

						if (totalremoved < 1.0) {

							SetMatShare(jcount, icount, matprop * (1.0 - Getnremovedmat(jcount, icount, 13 - adbyear - 3)) / (1.0 - totalremoved)); //i.e. mature survivors / all survivors

						}

						SetTreeDensity(jcount, icount, GetTreeDensity(jcount, icount) * (1 - totalremoved));

					}

					double IncPr(0);
					InF4 >> IncPr;
					SetInclustionProb(jcount, icount, IncPr / 100.0);

					if (GetTreeDensity(jcount, icount) > 0 && adbyear > -1)
					{
						double aH = Grid::Getadbstart_H(jcount, icount);
						double aI = Grid::Getadbstart_I(jcount, icount);
						double aS = Grid::Getadbstart_S(jcount, icount);
						double aD = Grid::Getadbstart_D(jcount, icount);

						SetCrop(jcount, icount, 0, Grid::Getadbstart_H(jcount, icount)); //No EAB + Asymptomatic
						SetCrop(jcount, icount, 1, Grid::Getadbstart_I(jcount, icount)); //No EAB + Lightly stessed
						SetCrop(jcount, icount, 2, Grid::Getadbstart_S(jcount, icount)); //No EAB + Highly stressed
						SetCrop(jcount, icount, 3, Grid::Getadbstart_D(jcount, icount)); //No EAB + Recently dead
						SetCrop(jcount, icount, 4, 0.0);  //EAB present + Asymptomatic
						SetCrop(jcount, icount, 5, 0.0); //EAB present + Lightly stessed
						SetCrop(jcount, icount, 6, 0.0); //EAB present + Highly stressed
						SetCrop(jcount, icount, 7, 0.0); //EAB present + Recently dead
						SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
						SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
						SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
						SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

						double testrow = infCelljRow;
						double testcol = infCelliCol;
						if ((jcount == infCelljRow) && (icount == infCelliCol))
						{

							double EABonADB_H = (ProbIniTreeInfection)*Grid::Getadbstart_H(jcount, icount);
							double EABonADB_I = (ProbIniTreeInfection)*Grid::Getadbstart_I(jcount, icount);
							double EABonADB_S = (ProbIniTreeInfection)*Grid::Getadbstart_S(jcount, icount);
							double EABonADB_D = (ProbIniTreeInfection)*Grid::Getadbstart_D(jcount, icount);

							double noEABonADB_H = (1 - ProbIniTreeInfection) * Grid::Getadbstart_H(jcount, icount);
							double noEABonADB_I = (1 - ProbIniTreeInfection) * Grid::Getadbstart_I(jcount, icount);
							double noEABonADB_S = (1 - ProbIniTreeInfection) * Grid::Getadbstart_S(jcount, icount);
							double noEABonADB_D = (1 - ProbIniTreeInfection) * Grid::Getadbstart_D(jcount, icount);



							SetCrop(jcount, icount, 0, noEABonADB_H); //No EAB + Asymptomatic
							SetCrop(jcount, icount, 1, noEABonADB_I); //No EAB + Lightly stessed
							SetCrop(jcount, icount, 2, noEABonADB_S); //No EAB + Highly stressed
							SetCrop(jcount, icount, 3, noEABonADB_D); ///No EAB + Recently dead
							SetCrop(jcount, icount, 4, EABonADB_H); //EAB present + Asymptomatic
							SetCrop(jcount, icount, 5, EABonADB_I); //EAB present + Lightly stessed
							SetCrop(jcount, icount, 6, EABonADB_S); //EAB present + Highly stressed
							SetCrop(jcount, icount, 7, EABonADB_D); //EAB present + Recently dead
							SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
							SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
							SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
							SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

							double sumcrops = GetCrop(jcount, icount, 0) + GetCrop(jcount, icount, 1) + GetCrop(jcount, icount, 2) +
								GetCrop(jcount, icount, 3) + GetCrop(jcount, icount, 4) + GetCrop(jcount, icount, 5) +
								GetCrop(jcount, icount, 6) + GetCrop(jcount, icount, 7) + GetCrop(jcount, icount, 8) +
								GetCrop(jcount, icount, 9) + GetCrop(jcount, icount, 10) + GetCrop(jcount, icount, 11);

							int stopere = 0;

						}
						//if (Sick==155000) // we plop in a seed of EAB of 1% of the trees




					}



						
						}

					}


				

			
			InF.close();
			InFFF.close();
			InF4.close();
			InFarrival.close();
			InFeusoil.close();
			InFsoil.close();
			InFnonwoodp.close();


		}
		else
			throw std::logic_error("Error opening inpur files");

		double P_LarvToAdultStart = Grid::GetPropOverWinterLarvaeStart();
		//double P_LarvToAdultStartmcS = Grid::GetPropOverWinterLarvaeStartmcS();
		//double P_LarvToAdultStartmcD = Grid::GetPropOverWinterLarvaeStartmcD();

		for (int icount = 0; icount < Grid::GetNumRows(); icount++)
		{
			for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
			{
				if ((Grid::GetCrop(icount, jcount, 4) + Grid::GetCrop(icount, jcount, 5) + Grid::GetCrop(icount, jcount, 6) + Grid::GetCrop(icount, jcount, 7)) > 0.0) ////Status of trees/////
				{


					Grid::SetNumG(icount, jcount, 10.0); //Sets total number of adults in a cell
					Grid::SetOWLav(icount, jcount, 0);
					Grid::SetInvasion(icount, jcount, -1);

					Grid::SetPLarv(icount, jcount, P_LarvToAdultStart);
					Grid::SetPLarvH(icount, jcount, P_LarvToAdultStart); //Sets proportion of overwintering larvae, we do this for the different ash dieback status
					Grid::SetPLarvI(icount, jcount, P_LarvToAdultStart);
					Grid::SetPLarvS(icount, jcount, P_LarvToAdultStart);
					Grid::SetPLarvD(icount, jcount, P_LarvToAdultStart);
					Grid::SetOWLavmc_H(icount, jcount, 0.0); //Stores vector of larvae on trees of different ash dieback status
					Grid::SetOWLavmc_I(icount, jcount, 0.0);
					Grid::SetOWLavmc_S(icount, jcount, 0.0);
					Grid::SetOWLavmc_D(icount, jcount, 0.0);

				}
				else
				{
					Grid::SetNumG(icount, jcount, 0.0); //Sets total number of adults in a cell
					Grid::SetOWLav(icount, jcount, 0);
					Grid::SetInvasion(icount, jcount, -1);

					Grid::SetPLarv(icount, jcount, P_LarvToAdultStart);
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

		Grid::SetTotalPlantasFilled(-1);
		int mySymp(0); //Set to zero is max syptomatic trees in any one cell in the plantaion is less than 0.05
		double myWorry(0); //start unworried
		//int myCHMA(0);




	} //end of function SetLandBasic

	void ReSetLandBasic(int infCelljRow, int infCelliCol, int& iflag)
	{

		int UBlockID = 1; //unique Block ID	
		char path_buffer[_MAX_PATH];
		char Npath_buffer[_MAX_PATH];
		_getcwd(path_buffer, _MAX_PATH);

		initialiseadbmatgamma();
		initialiseadbsoileff();

		for (int jcount = 0; jcount < Nrows; jcount++)
		{
			for (int icount = 0; icount < Ncols; icount++)
			{
				double TempTreeDensity = Grid::GetInitialTreeDensity(jcount, icount);
				if (TempTreeDensity < 1.0)
					TempTreeDensity = 1;
				double ProbIniTreeInfection = 1 / TempTreeDensity; // This is now set so we have 1 tree infected per cell.

				G_adbarrival[jcount][icount] = Grid::Getadbarrival(jcount, icount);
				G_adbsoil[jcount][icount] = Grid::Getadbsoil(jcount, icount);
				G_adbeusoil[jcount][icount] = Grid::Getadbeusoil(jcount, icount);
				G_adbnonwoodp[jcount][icount] = Grid::Getnonwoodp(jcount, icount);
				G_adbprecipp[jcount][icount] = Grid::Getprecipp(jcount, icount);

				double adbstartH = 0.0, adbstartI = 0.0, adbstartS = 0.0, adbstartD = 0.0, adbstartR = 0.0, iniremovefunction = 0.0;
				double adbstartHmat = 0.0, adbstartImat = 0.0, adbstartSmat = 0.0, adbstartDmat = 0.0, adbstartRmat = 0.0, iniremovefunctionmat;
				double adbstartHjuv = 0.0, adbstartIjuv = 0.0, adbstartSjuv = 0.0, adbstartDjuv = 0.0, adbstartRjuv = 0.0, iniremovefunctionjuv;


				int adbyear = G_adbarrival[jcount][icount];
				Grid::SetTreeDensity(jcount, icount, Grid::GetInitialTreeDensity(jcount, icount));
				double numTrees = Grid::GetTreeDensity(jcount, icount); //number of trees in each cell
				
				//Proportion of mature trees in a cell//
				SetMatShare(jcount, icount, 0.29);

				//Setting resistant ash value in each cell//
				double ResProp = 0.019 + 0.036 * RNG_Uniform();
				SetResProp(jcount, icount, ResProp);

				Grid::Setlarvdens(jcount, icount, 0.0);

				if (numTrees > 0 && adbyear > -1) {

					//Define the starting conditions for each cell
					double iniH_m = 0.99;
					double iniI_m = 0.01;
					double iniS_m = 0.00;
					double iniD_m = 0.00;

					double iniH_j = 0.99;
					double iniI_j = 0.01;
					double iniS_j = 0.00;
					double iniD_j = 0.00;

					//This will then fetch these variables for each cell

					//ADB arrival working with assumption that cells where disease was detected in 2013, ADB symtpoms were
					//present throughout these cells (i.e. had been present a number of years - only detected when symptoms clear), this then spreads at a rate of
					//50 km per year from these cells.

					int adbyear = G_adbarrival[jcount][icount]; //will return the year of disease arrival from the cell
					int adbsoil = G_adbsoil[jcount][icount]; // soil drainage
					double adbeusoil = G_adbeusoil[jcount][icount]; //estimate soil water content
					double nonwoodp = G_adbnonwoodp[jcount][icount]; //proportion of trees in non-wood setting
					double inimatgamma = 0.0205; //mature gamma values (0.009 for minimum, 0.0205 for median, 0.032 for max)
					double inijuvgamma = inimatgamma * (0.1 / 0.032); //Calculate juvenile mortality
					double adbeta = 0.5; //Beta value
					double nonwoodeff = 1.0 - (0.19 / 0.41); //This is the difference in crown dieback between Hedgerow and Woodland trees
					double matprop = 0.29; //define the proportion of mature ash in GB woodlands
					double juvprop = 0.71; //define the proportion of juvenile ash in GB woodlands
					double matstresseff = 0.724; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
					double juvstresseff = 0.712; // q value converting annual mortality rate to annual transition from lightly stressed to heavily stressed//
					double mortratio = 3.0; //Difference in rate severly stressed trees die vs lightly stressed trees - will adjust with q values//
					double precipmortklesse = (1.0 / (1.0 + exp(-(0.5 - 6.7)))); //Mortality given a unit precipitation increase
					double noprecipmortklesse = (1.0 / (1.0 + exp(-(-6.7)))); //Mortality at minimum precipitation
					double precippercentchange = ((precipmortklesse / noprecipmortklesse) - 1) / (312.0 + 1305.575); //Percent change in mortality per unit increase in rainfall


					double precippmat = (inimatgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount]))) - inimatgamma; //i.e. the additional gamma value per cell//
					double precippjuv = (inijuvgamma * (1.0 + (precippercentchange * G_adbprecipp[jcount][icount]))) - inijuvgamma; //i.e. the additional gamma value per cell//

					double soilp = 1.0;
					if (adbsoil > 4 && adbeusoil > 0.3605875) { //adding mortality for 'wet' cells
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

					//Initialising variables//
					double NH_m, NI_m, NS_m, ND_m;
					double NH_j, NI_j, NS_j, ND_j;
					int niyr;

					//Running for 13 years(2013-2026) - how many years it took for disease to arrive after 2013//
					for (int i = 0; i < 14 - adbyear; i++) { //Change the 14 to 19 if we want EAB arrival in 2031 rather than 2026; and to 27 if want ADB base layer in 2039

						//Add some resistant ash and more S trees dying than I trees//

						//Calculating mortality split between categories - with S trees dying three times the rate than I trees
						// with same overall annual mortality rate. Accounting for the resistant ash in a cell - i.e. these say in H category

						double gI_m = matgamma;
						double gS_m = matgamma;
						double den_m = iniI_m + (mortratio * iniS_m);

						if (den_m > 0.0) {

							gI_m = matgamma * (iniI_m + iniS_m) / den_m;
							gS_m = mortratio * gI_m;
						}

						if (gS_m > 1.0) { gS_m = 1.0; }

						double availH_m = iniH_m - ResProp;
						if (availH_m < 0.0) { availH_m = 0.0; }

						double gI_j = juvgamma;
						double gS_j = juvgamma;
						double den_j = iniI_j + (mortratio * iniS_j);

						if (den_j > 0.0) {

							gI_j = juvgamma * (iniI_j + iniS_j) / den_j;
							gS_j = mortratio * gI_j;
						}

						if (gS_j > 1.0) { gS_j = 1.0; }

						double availH_j = iniH_j - ResProp;
						if (availH_j < 0.0) { availH_j = 0.0; }

						//Defining the ADB compartmental model for each age class//
						NH_m = -adbeta * availH_m;
						NI_m = (adbeta * availH_m) - (gI_m * iniI_m) - (matalpha * iniI_m);
						NS_m = (matalpha * iniI_m) - (gS_m * iniS_m);
						ND_m = (gI_m * iniI_m) + (gS_m * iniS_m);

						NH_j = -adbeta * availH_j;
						NI_j = (adbeta * availH_j) - (gI_j * iniI_j) - (juvalpha * iniI_j);
						NS_j = (juvalpha * iniI_j) - (gS_j * iniS_j);
						ND_j = (gI_j * iniI_j) + (gS_j * iniS_j);

						iniH_m += NH_m;
						iniI_m += NI_m;
						iniS_m += NS_m;
						iniD_m += ND_m;

						iniH_j += NH_j;
						iniI_j += NI_j;
						iniS_j += NS_j;
						iniD_j += ND_j;

						
						//Check for errors//
						if (iniH_m + iniI_m + iniS_m + iniD_m > 1.000000001 ||
							iniH_m + iniI_m + iniS_m + iniD_m < 1-0.000000001) {
							throw std::logic_error("error in ini_m sum");
						}

						if (iniH_j + iniI_j + iniS_j + iniD_j > 1.000000001 ||
							iniH_j + iniI_j + iniS_j + iniD_j < 1-0.000000001) {
							throw std::logic_error("error in ini_j sum");
						}

						//Junk code with no need - controlled above
						if (iniH_m > 1.0) { iniH_m = 1.0; }
						else if (iniH_m < 0.0) { iniH_m = 0.0; }
						if (iniI_m > 1.0) { iniI_m = 1.0; }
						else if (iniI_m < 0.0) { iniI_m = 0.0; }
						if (iniS_m > 1.0) { iniS_m = 1.0; }
						else if (iniS_m < 0.0) { iniS_m = 0.0; }
						if (iniD_m > 1.0) { iniD_m = 1.0; }
						else if (iniD_m < 0.0) { iniD_m = 0.0; }

						if (iniH_j > 1.0) { iniH_j = 1.0; }
						else if (iniH_j < 0.0) { iniH_j = 0.0; }
						if (iniI_j > 1.0) { iniI_j = 1.0; }
						else if (iniI_j < 0.0) { iniI_j = 0.0; }
						if (iniS_j > 1.0) { iniS_j = 1.0; }
						else if (iniS_j < 0.0) { iniS_j = 0.0; }
						if (iniD_j > 1.0) { iniD_j = 1.0; }
						else if (iniD_j < 0.0) { iniD_j = 0.0; }

						double round_check_m_u = iniH_m + iniI_m + iniS_m + iniD_m;

						if (iniH_m + iniI_m + iniS_m + iniD_m > 1.0) {
							iniH_m = iniH_m / (round_check_m_u);
							iniI_m = iniI_m / (round_check_m_u);
							iniS_m = iniS_m / (round_check_m_u);
							iniD_m = iniD_m / (round_check_m_u);
						}

						double round_check_m_l = iniH_m + iniI_m + iniS_m + iniD_m;
						if (iniH_m + iniI_m + iniS_m + iniD_m < 1.0) {
							iniH_m = iniH_m / (round_check_m_l);
							iniI_m = iniI_m / (round_check_m_l);
							iniS_m = iniS_m / (round_check_m_l);
							iniD_m = iniD_m / (round_check_m_l);
						}

						double round_check_j_u = iniH_j + iniI_j + iniS_j + iniD_j;
						if (iniH_j + iniI_j + iniS_j + iniD_j > 1.0) {
							iniH_j = iniH_j / (round_check_j_u);
							iniI_j = iniI_j / (round_check_j_u);
							iniS_j = iniS_j / (round_check_j_u);
							iniD_j = iniD_j / (round_check_j_u);
						}

						double round_check_j_l = iniH_j + iniI_j + iniS_j + iniD_j;
						if (iniH_j + iniI_j + iniS_j + iniD_j < 1.0) {
							iniH_j = iniH_j / (round_check_j_l);
							iniI_j = iniI_j / (round_check_j_l);
							iniS_j = iniS_j / (round_check_j_l);
							iniD_j = iniD_j / (round_check_j_l);
						}


						niyr = i;
						//Save ash dieback status independent of tree age//
						adbstartH = (iniH_j * juvprop) + (iniH_m * matprop);
						adbstartI = (iniI_j * juvprop) + (iniI_m * matprop);
						adbstartS = (iniS_j * juvprop) + (iniS_m * matprop);
						adbstartD = (iniD_j * juvprop) + (iniD_m * matprop);

						//For mature trees//
						adbstartHmat = iniH_m;
						adbstartImat = iniI_m;
						adbstartSmat = iniS_m;
						adbstartDmat = iniD_m;
						Setnremovedmat(jcount, icount, niyr, iniD_m); //Remove NO ADB model
						//Setnremovedmat(jcount, icount, niyr, 0.0); //Add NO ADB model

						//For juvenile trees//
						adbstartHjuv = iniH_j;
						adbstartIjuv = iniI_j;
						adbstartSjuv = iniS_j;
						adbstartDjuv = iniD_j;
						Setnremovedjuv(jcount, icount, niyr, iniD_j); //Remove NO ADB model
						//Setnremovedjuv(jcount, icount, niyr, 0.0); //Add NO ADB model
					}

					//For trees irrespective of age - check for errors before removing trees//
					double sumadbstart = adbstartH + adbstartI + adbstartS + adbstartD;
					if (sumadbstart > 1.000000001 ||
						sumadbstart < 1-0.00000001) {
						throw std::logic_error("error in get sumadbstart");
					}
					//Change the 13 below to 18 if delaying EAB arrival by 5 years until 2031, and to 26 if wanting to examine ADB base layers in 2039//
					double totalremoved = (Getnremovedjuv(jcount, icount, 13 - adbyear - 3)*juvprop)+ (Getnremovedmat(jcount, icount, 13 - adbyear - 3) * matprop); //TURN OFF FOR NO ADB MODEL, TURN ON LINE BELOW
					//double totalremoved = 0.0; // add this line if we want to examine ash dieback SIR without removing trees// TURN ON FOR NO ADB MODEL
					double nadbstartD = adbstartD;
					if (totalremoved > 0.00000001) {
						nadbstartD = adbstartD - totalremoved;
					}
					if (nadbstartD < 0.0) { nadbstartD = 0.0; }

					double newprop = adbstartH + adbstartI + adbstartS + nadbstartD;
					if (totalremoved > 0.00000001) {
						double junk = 0.0;
						double newH = adbstartH / newprop;
						double newI = adbstartI / newprop;
						double newS = adbstartS / newprop;
						double newD = nadbstartD / newprop;

						Setadbstart_H(jcount, icount, newH);
						Setadbstart_I(jcount, icount, newI);
						Setadbstart_S(jcount, icount, newS);
						Setadbstart_D(jcount, icount, newD);
					}

					else {
						double junk = 0.0;
						Setadbstart_H(jcount, icount, adbstartH);
						Setadbstart_I(jcount, icount, adbstartI);
						Setadbstart_S(jcount, icount, adbstartS);
						Setadbstart_D(jcount, icount, nadbstartD);
					}


					double sumadbstartb = Getadbstart_H(jcount, icount) + Getadbstart_I(jcount, icount) + Getadbstart_S(jcount, icount) + Getadbstart_D(jcount, icount);
					//Check for errors//
					if (sumadbstartb > 1.000000001 ||
						sumadbstartb < 1 - 0.00000001) {
						throw std::logic_error("error in get sumadbstartb");
					}

					if (sumadbstartb > 1.0) {
						Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount) / (sumadbstartb));
						Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount) / (sumadbstartb));
						Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount) / (sumadbstartb));
						Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount) / (sumadbstartb));
					}

					else if (sumadbstartb < 1.0) {
						Setadbstart_H(jcount, icount, Getadbstart_H(jcount, icount) / (sumadbstartb));
						Setadbstart_I(jcount, icount, Getadbstart_I(jcount, icount) / (sumadbstartb));
						Setadbstart_S(jcount, icount, Getadbstart_S(jcount, icount) / (sumadbstartb));
						Setadbstart_D(jcount, icount, Getadbstart_D(jcount, icount) / (sumadbstartb));
					}


					//Remove trees from mature ash category after checking for errors//
					double sumcropmat = adbstartHmat + adbstartImat + adbstartSmat + adbstartDmat;

					if (sumcropmat > 1.000000001 ||
						sumcropmat < 1-0.00000001) {
						throw std::logic_error("error in get startsumcropmat");
					}

					double nadbstartDmat = adbstartDmat;
					if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
						nadbstartDmat = adbstartDmat - Getnremovedmat(jcount, icount, 13 - adbyear - 3);
					}
					if (nadbstartDmat < 0.0) { nadbstartDmat = 0.0; }
					double newpropmat = adbstartHmat + adbstartImat + adbstartSmat + nadbstartDmat;

					//Adjust for removed trees//
					if (Getnremovedmat(jcount, icount, 13 - adbyear - 3) > 0.0) {
						double newHm = adbstartHmat / newpropmat;
						double newIm = adbstartImat / newpropmat;
						double newSm = adbstartSmat / newpropmat;
						double newDm = nadbstartDmat / newpropmat;

						SetCropmat(jcount, icount, 0, newHm);
						SetCropmat(jcount, icount, 1, newIm);
						SetCropmat(jcount, icount, 2, newSm);
						SetCropmat(jcount, icount, 3, newDm);
					}
					else {
						SetCropmat(jcount, icount, 0, adbstartHmat);
						SetCropmat(jcount, icount, 1, adbstartImat);
						SetCropmat(jcount, icount, 2, adbstartSmat);
						SetCropmat(jcount, icount, 3, nadbstartDmat);
					}


					if (GetCropmat(jcount, icount, 0) < 0.0) { SetCropmat(jcount, icount, 0, 0.0); }
					if (GetCropmat(jcount, icount, 1) < 0.0) { SetCropmat(jcount, icount, 1, 0.0); }
					if (GetCropmat(jcount, icount, 2) < 0.0) { SetCropmat(jcount, icount, 2, 0.0); }
					if (GetCropmat(jcount, icount, 3) < 0.0) { SetCropmat(jcount, icount, 3, 0.0); }


					double sumcropmatb = GetCropmat(jcount, icount, 0) + GetCropmat(jcount, icount, 1) + GetCropmat(jcount, icount, 2) + GetCropmat(jcount, icount, 3);
					//Check for errors//
					if (sumcropmatb > 1.000000001 ||
						sumcropmatb < 1 - 0.00000001) {
						throw std::logic_error("error in get startsumcropmatb");
					}

					if (sumcropmatb > 1.0) {
						SetCropmat(jcount, icount, 0, GetCropmat(jcount, icount, 0) / (sumcropmatb));
						SetCropmat(jcount, icount, 1, GetCropmat(jcount, icount, 1) / (sumcropmatb));
						SetCropmat(jcount, icount, 2, GetCropmat(jcount, icount, 2) / (sumcropmatb));
						SetCropmat(jcount, icount, 3, GetCropmat(jcount, icount, 3) / (sumcropmatb));
					}

					else if (sumcropmatb < 1.0) {
						SetCropmat(jcount, icount, 0, GetCropmat(jcount, icount, 0) / (sumcropmatb));
						SetCropmat(jcount, icount, 1, GetCropmat(jcount, icount, 1) / (sumcropmatb));
						SetCropmat(jcount, icount, 2, GetCropmat(jcount, icount, 2) / (sumcropmatb));
						SetCropmat(jcount, icount, 3, GetCropmat(jcount, icount, 3) / (sumcropmatb));
					}


					//Now repeat for juvenile trees//
					double sumcropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + adbstartDjuv;

					if (sumcropjuv > 1.000000001 ||
						sumcropjuv < 1-0.00000001) {
						throw std::logic_error("error in get startsumcropjuv");
					}

					double nadbstartDjuv = adbstartDjuv;
					if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
						nadbstartDjuv = adbstartDjuv - Getnremovedjuv(jcount, icount, 13 - adbyear - 3);
					}
					if (nadbstartDjuv < 0.0) { nadbstartDjuv = 0.0; }
					double newpropjuv = adbstartHjuv + adbstartIjuv + adbstartSjuv + nadbstartDjuv;

					//Adjust for removed trees//
					if (Getnremovedjuv(jcount, icount, 13 - adbyear - 3) > 0.0) {
						double newHj = adbstartHjuv / newpropjuv;
						double newIj = adbstartIjuv / newpropjuv;
						double newSj = adbstartSjuv / newpropjuv;
						double newDj = nadbstartDjuv / newpropjuv;

						SetCropjuv(jcount, icount, 0, newHj);
						SetCropjuv(jcount, icount, 1, newIj);
						SetCropjuv(jcount, icount, 2, newSj);
						SetCropjuv(jcount, icount, 3, newDj);
					}
					else {
						SetCropjuv(jcount, icount, 0, adbstartHjuv);
						SetCropjuv(jcount, icount, 1, adbstartIjuv);
						SetCropjuv(jcount, icount, 2, adbstartSjuv);
						SetCropjuv(jcount, icount, 3, nadbstartDjuv);
					}

					if (GetCropjuv(jcount, icount, 0) < 0.0) { SetCropjuv(jcount, icount, 0, 0.0); }
					if (GetCropjuv(jcount, icount, 1) < 0.0) { SetCropjuv(jcount, icount, 1, 0.0); }
					if (GetCropjuv(jcount, icount, 2) < 0.0) { SetCropjuv(jcount, icount, 2, 0.0); }
					if (GetCropjuv(jcount, icount, 3) < 0.0) { SetCropjuv(jcount, icount, 3, 0.0); }

					double sumcropjuvb = GetCropjuv(jcount, icount, 0) + GetCropjuv(jcount, icount, 1) + GetCropjuv(jcount, icount, 2) + GetCropjuv(jcount, icount, 3);

					//Check for errors//
					if (sumcropjuvb > 1.000000001 ||
						sumcropjuvb < 1 - 0.00000001) {
						throw std::logic_error("error in get startsumcropjuvb");
					}

					if (sumcropjuvb > 1.0) {
						SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
					}

					else if (sumcropjuvb < 1.0) {
						SetCropjuv(jcount, icount, 0, GetCropjuv(jcount, icount, 0) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 1, GetCropjuv(jcount, icount, 1) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 2, GetCropjuv(jcount, icount, 2) / (sumcropjuvb));
						SetCropjuv(jcount, icount, 3, GetCropjuv(jcount, icount, 3) / (sumcropjuvb));
					}

					if (totalremoved < 1.0) {

						SetMatShare(jcount, icount, matprop * (1.0 - Getnremovedmat(jcount, icount, 13 - adbyear - 3)) / (1.0 - totalremoved)); //i.e. mature survivors / all survivors

					}

					SetTreeDensity(jcount, icount, GetTreeDensity(jcount, icount) *(1 - totalremoved));

				}

				if (GetTreeDensity(jcount, icount) > 0 && adbyear > -1)
				{
					double aH = Grid::Getadbstart_H(jcount, icount);
					double aI = Grid::Getadbstart_I(jcount, icount);
					double aS = Grid::Getadbstart_S(jcount, icount);
					double aD = Grid::Getadbstart_D(jcount, icount);

					SetCrop(jcount, icount, 0, Grid::Getadbstart_H(jcount, icount)); //No EAB + Asymptomatic
					SetCrop(jcount, icount, 1, Grid::Getadbstart_I(jcount, icount)); //No EAB + Lightly stessed
					SetCrop(jcount, icount, 2, Grid::Getadbstart_S(jcount, icount)); //No EAB + Highly stressed
					SetCrop(jcount, icount, 3, Grid::Getadbstart_D(jcount, icount)); //No EAB + Recently dead
					SetCrop(jcount, icount, 4, 0.0); //EAB present + Asymptomatic
					SetCrop(jcount, icount, 5, 0.0); //EAB present + Lightly stessed
					SetCrop(jcount, icount, 6, 0.0); //EAB present + Highly stressed
					SetCrop(jcount, icount, 7, 0.0); //EAB present + Recently dead
					SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
					SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
					SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
					SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

					double testrow = infCelljRow;
					double testcol = infCelliCol;
					if ((jcount == infCelljRow) && (icount == infCelliCol))
					{

						double EABonADB_H = (ProbIniTreeInfection)*Grid::Getadbstart_H(jcount, icount);
						double EABonADB_I = (ProbIniTreeInfection)*Grid::Getadbstart_I(jcount, icount);
						double EABonADB_S = (ProbIniTreeInfection)*Grid::Getadbstart_S(jcount, icount);
						double EABonADB_D = (ProbIniTreeInfection)*Grid::Getadbstart_D(jcount, icount);

						double noEABonADB_H = (1 - ProbIniTreeInfection) * Grid::Getadbstart_H(jcount, icount);
						double noEABonADB_I = (1 - ProbIniTreeInfection) * Grid::Getadbstart_I(jcount, icount);
						double noEABonADB_S = (1 - ProbIniTreeInfection) * Grid::Getadbstart_S(jcount, icount);
						double noEABonADB_D = (1 - ProbIniTreeInfection) * Grid::Getadbstart_D(jcount, icount);



						SetCrop(jcount, icount, 0, noEABonADB_H); //No EAB + Asymptomatic
						SetCrop(jcount, icount, 1, noEABonADB_I); //No EAB + Lightly stessed
						SetCrop(jcount, icount, 2, noEABonADB_S); //No EAB + Highly stressed
						SetCrop(jcount, icount, 3, noEABonADB_D); //No EAB + Recently dead
						SetCrop(jcount, icount, 4, EABonADB_H); //EAB present + Asymptomatic
						SetCrop(jcount, icount, 5, EABonADB_I); //EAB present + Lightly stessed
						SetCrop(jcount, icount, 6, EABonADB_S); //EAB present + Highly stressed
						SetCrop(jcount, icount, 7, EABonADB_D); //EAB present + Recently dead
						SetCrop(jcount, icount, 8, 0.0); // EAB present (longer) + Asymptomatic
						SetCrop(jcount, icount, 9, 0.0); // EAB present (longer) + Lightly stessed
						SetCrop(jcount, icount, 10, 0.0); // EAB present(longer) + Highly stressed
						SetCrop(jcount, icount, 11, 0.0); // EAB present(longer) + Recently dead

						double sumcrops = GetCrop(jcount, icount, 0) + GetCrop(jcount, icount, 1) + GetCrop(jcount, icount, 2) +
							GetCrop(jcount, icount, 3) + GetCrop(jcount, icount, 4) + GetCrop(jcount, icount, 5) +
							GetCrop(jcount, icount, 6) + GetCrop(jcount, icount, 7) + GetCrop(jcount, icount, 8) +
							GetCrop(jcount, icount, 9) + GetCrop(jcount, icount, 10) + GetCrop(jcount, icount, 11);

					}
					//if (Sick==155000) // we plop in a seed of EAB of 1% of the trees




				}




			}

		}

		//InFF.close();


			double P_LarvToAdultStart = Grid::GetPropOverWinterLarvaeStart();
			//double P_LarvToAdultStartmcS = Grid::GetPropOverWinterLarvaeStartmcS();
			//double P_LarvToAdultStartmcD = Grid::GetPropOverWinterLarvaeStartmcD();

			for (int icount = 0; icount < Grid::GetNumRows(); icount++)
			{
				for (int jcount = 0; jcount < Grid::GetNumCols(); jcount++)
				{
					if ((Grid::GetCrop(icount, jcount, 4) + Grid::GetCrop(icount, jcount, 5) + Grid::GetCrop(icount, jcount, 6) + Grid::GetCrop(icount, jcount, 7)) > 0.0) ////Status of trees/////
					{


						Grid::SetNumG(icount, jcount, 10.0); //Sets total number of adults in a cell
						Grid::SetOWLav(icount, jcount, 0);
						Grid::SetInvasion(icount, jcount, -1);

						Grid::SetPLarv(icount, jcount, P_LarvToAdultStart);
						Grid::SetPLarvH(icount, jcount, P_LarvToAdultStart); //Sets proportion of overwintering larvae, we do this for the different ash dieback status
						Grid::SetPLarvI(icount, jcount, P_LarvToAdultStart);
						Grid::SetPLarvS(icount, jcount, P_LarvToAdultStart);
						Grid::SetPLarvD(icount, jcount, P_LarvToAdultStart);
						Grid::SetOWLavmc_H(icount, jcount, 0.0); //Stores vector of larvae on trees of different ash dieback status
						Grid::SetOWLavmc_I(icount, jcount, 0.0);
						Grid::SetOWLavmc_S(icount, jcount, 0.0);
						Grid::SetOWLavmc_D(icount, jcount, 0.0);

					}
					else
					{
						Grid::SetNumG(icount, jcount, 0.0); //Sets total number of adults in a cell
						Grid::SetOWLav(icount, jcount, 0);
						Grid::SetInvasion(icount, jcount, -1);

						Grid::SetPLarv(icount, jcount, P_LarvToAdultStart);
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

			Grid::SetTotalPlantasFilled(-1);
			int mySymp(0); //Set to zero is max syptomatic trees in any one cell in the plantaion is less than 0.05
			double myWorry(0); //start unworried
			//int myCHMA(0);




	}
	}


////////////////////////////END OF NAMESPACE GRID///////////////////////////////////////////

namespace FlightP // keeps a vector of the proportions of Bugs of each type that fly each distance
	
{
	std::vector<double> BugFlight; //
	
	void ClearData()
	{
		
		BugFlight.clear(); //
		

	};


	void MakeProportions(std::vector<double>& Type, double lambda)
	{
			//Type 1 (Type[0]) centre cell for other types see 
			int idist=0;
			int jdist=0;
			
			Type.push_back(Integrate(idist, jdist, lambda)); //position 0
			Type.push_back(Integrate(0, 1,  lambda)); //position 1
			Type.push_back(Integrate(1, 1, lambda)); //position 2
			//Test to see how much of the population is in this inner square
			double popT=Type[0]+4*Type[1]+4*Type[2];
			//if (popT<0.99) //keep calculating
			int Mcount=3;
			while (popT<0.95) 
			{
				for (int icount=0; icount<Mcount; icount++)
				{
					double Ival=Integrate(icount, Mcount-1, lambda);
					Type.push_back(Ival);
					if ((icount>0)&&(icount<Mcount-1))
						popT=popT+8*Ival;
					else
						popT=popT+4*Ival;
				}
				Mcount++;
				if (Mcount>900)
					int junk=0;
								
			}
			//put remained in the centre
			
			Type[0]=Type[0]+1.0-popT;
			if (Type[0]<0) Type[0]=0;
	

	}

	void Initialise()
	{ 
		double lambda = 0.0414; // 0.0255; // 0.00035; // For weekly time step. The value of 0.46 is based on Schrader et al. 2020 50% traveled 1.5 km in a year
			MakeProportions(BugFlight, lambda);
		


	}

	void GetBugFlight(std::vector<double>& MyDist)
	{
		int Len=BugFlight.size();
		for (int icount=0; icount<Len; icount++)
		{
			MyDist.push_back(BugFlight[icount]);
		}
	}

	


		
} //end of namespace

double Integrate(int idist, int jdist, double lambda)
{
	double MygridLen = Grid::GetGridLen();
	Grid::SetIntVars(jdist, MygridLen, lambda);

	const double Pi = 3.14159265358979323846;
	double xlb = (idist - 0.5) * MygridLen;
	double xub = (idist + 0.5) * MygridLen;
	double ylb = (jdist - 0.5) * MygridLen;
	double yub = (jdist + 0.5) * MygridLen;

	double px = (atan(xub / lambda) - atan(xlb / lambda)) / Pi;
	double py = (atan(yub / lambda) - atan(ylb / lambda)) / Pi;
	double ANS = px * py;

	if (ANS < 0)
		throw std::logic_error("Error in integration");
	return ANS;
}

void GridIDToCoord(int Floc, int& irow, int& icol)
{
	irow = Floc % Grid::GetNumRows();  // % is remainder after dividing
	icol=(floor(Floc/Grid::GetNumRows())); 
}
int CoordToGridID(int jrow, int icol)
{
	return icol*Grid::GetNumRows()+jrow;
}



