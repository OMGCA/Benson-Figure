#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "cgp-sls.h"
#include <time.h>

#include "fitness_functions.h"

double threshIncre;
double threshold;
double classNumber;

void cgpExecute(void);

int main(void)
{
    cgpExecute();
}

void cgpExecute(void)
{
    /* CGP basic structure initialization */
	struct parameters   *params = NULL;
	struct chromosome   *chromo = NULL;
	struct dataSet      *trainingData = NULL;
	struct dataSet      *validationData = NULL;
	struct dataSet      *testData = NULL;

    char    dataTR[50], dataVA[50], dataTE[50];
	char**  cgp_params  = importFile("cgp_params.txt");
	char**  cgp_params2 = importFile("cgp_params2.txt");

	/* Parse parameters from external text file */
	threshold   = atof(cgp_params[0]);
	threshIncre = atof(cgp_params[1]);
	classNumber = atof(cgp_params[2]);

	/* Basic CGP parameters */
	int numNodes    = atoi(cgp_params[3]);
	int nodeArity   = atoi(cgp_params[4]);
	int numGens     = atoi(cgp_params[5]);
	int updateFreq  = atoi(cgp_params[6]);

	int numInputs   = atoi(cgp_params2[1]);
	int numOutputs  = atoi(cgp_params2[2]);

	/* The fold needed */
	int kFoldIndex  = atoi(cgp_params[9]);

	/* Dummy variable for API requirement, not used due to the cost function */
	double targetFitness = 0.1;

	/* Initialise all parameters in the param struct */
	params = initialiseParameters(numInputs, numNodes, numOutputs, nodeArity);

	/* Further CGP model parameters */
	setRandomNumberSeed(atoi(cgp_params[7]));
	addNodeFunction(params, "add, sub, mul, div");
	setTargetFitness(params, targetFitness);
	setMutationRate(params, atof(cgp_params[8]));
	setShortcutConnections(params, 0);
	setUpdateFrequency(params, updateFreq);

	/* If kFold index is non-zero, load one of the fold dataset */
	/* Otherwise, load the default dataset */
    if(kFoldIndex != 0)
    {
        char index[1];
        sprintf(index,"%d",kFoldIndex-1);

        char kFoldDataSrc[50] = "./kfolddata/fold_";
        strcat(kFoldDataSrc,index);
        strcat(kFoldDataSrc,"/01_training.csv");
        strcpy(dataTR, kFoldDataSrc);

        strcpy(kFoldDataSrc,"./kfolddata/fold_");
        strcat(kFoldDataSrc,index);
        strcat(kFoldDataSrc,"/02_validation.csv");
        strcpy(dataVA, kFoldDataSrc);

        strcpy(kFoldDataSrc,"./kfolddata/fold_");
        strcat(kFoldDataSrc,index);
        strcat(kFoldDataSrc,"/03_testing.csv");
        strcpy(dataTE, kFoldDataSrc);
    }
    else
    {
        strcpy(dataTR,"./01_training.csv");
        strcpy(dataVA,"./02_validation.csv");
        strcpy(dataTE,"./03_testing.csv");
    }
    printParameters(params);

    /* Import dataset to the program */
    trainingData    = initialiseDataSetFromFile(dataTR);
    validationData  = initialiseDataSetFromFile(dataVA);
    testData        = initialiseDataSetFromFile(dataTE);

    /* Set fitness function from the Java output file */
    setFitnessFromText(strtok(cgp_params2[0],"\n"), params);

    /* Execute CGP with vali and test enabled, result stored in file */
	chromo = runValiTestCGP(params, trainingData, validationData, testData, numGens);

	printChromosome(chromo, 0);

    /* Save the chromosome in external file */
    char chromoFileName[30] = "./CGP_Chromo/";
    strtok(cgp_params[3],"\n");
    strtok(cgp_params[4],"\n");
    strtok(cgp_params[8],"\n");
    double mutRate = atof(cgp_params[8]);

    /* Process the mutation rate entity, in case of identified as file extension */
    mutRate*=100;

    char mutRateChar[3];

    sprintf(mutRateChar, "%.f",mutRate);

    /* Generate chromo file name */
    strcat(chromoFileName,cgp_params[3]);
    strcat(chromoFileName,"_");
    strcat(chromoFileName,cgp_params[4]);
    strcat(chromoFileName,"_");
    strcat(chromoFileName,mutRateChar);
    strcat(chromoFileName,"_chromo.chromo");

	saveChromosome(chromo, chromoFileName);
	strcat(chromoFileName,".dot");
	saveChromosomeDot(chromo, 0,chromoFileName);

	/* Display test dataset detailed result */
    setDisplayAction(strtok(cgp_params2[0],"\n"), chromo, testData);

    /* Generate CGP evolution process file name */
    char outputFileName[30];
    strcpy(outputFileName, "_CGP_Output.txt");

    char randomNum[30];
    char kFoldIndexFLN[10];

    strtok(cgp_params[7],"\n");
    strtok(cgp_params[9],"\n");

    strcpy(randomNum, cgp_params[7]);
    strcpy(kFoldIndexFLN, cgp_params[9]);

    strcat(randomNum,outputFileName);

    strcat(kFoldIndexFLN,"_");
    strcat(kFoldIndexFLN,randomNum);
    char outputPath[50] = "./CGP_Outputs/";
    strcat(outputPath, kFoldIndexFLN);

    if(kFoldIndex == 0)
        printf("\nData set: original\n");
    else
        printf("\nData set: Fold %d\n",kFoldIndex);

    /* Read the evolution process file to automatically select best generation */
    double* bestTemp;
    double tmpBest[5];
    bestTemp = getBestEntity(outputPath, testData,tmpBest);

    exportBestChromo(bestTemp,kFoldIndex);

    /* Free allocated memories */
	free(cgp_params);
    free(cgp_params2);

	freeDataSet(trainingData);
	freeDataSet(validationData);
	freeDataSet(testData);

	freeChromosome(chromo);
	freeParameters(params);
}
