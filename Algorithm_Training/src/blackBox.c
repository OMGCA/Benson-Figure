#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "cgp-sls.h"
#include <time.h>

#include "fitness_functions.h"
#include <limits.h>

double threshIncre;
double threshold;
double classNumber;


void idSortTest(void);
void stringCastTest(void);
void memoryLeakTest(void);
void chromoFileNameTest(void);
void stcConfidenceTest(void);

int main(void)
{
    writeExistFile();
}

void writeExistFile(void)
{
    FILE *pFile = fopen("test.txt","a");
    char buffer[256] = "\nXTTest";


    fprintf(pFile, "%s", buffer);


}


void stcConfidenceTest(void)
{
    int thresholdMargin[2] = {100,200};
    double chromoOutput = 120;
    double confidence = stcConfidence(thresholdMargin,chromoOutput);
    printf("%f%%\n",confidence*100);

}

void chromoFileNameTest(void)
{
    char **cgpArr = importFile("cgp_params.txt");
    char chromoFileName[30];
    strtok(cgpArr[3],"\n");
    strtok(cgpArr[4],"\n");
    strtok(cgpArr[8],"\n");
    double mutRate = atof(cgpArr[8]);

    /* Process the mutation rate entity, in case of identified as file extension */
    mutRate*=100;

    char mutRateChar[3];

    sprintf(mutRateChar, "%.f",mutRate);

    strcpy(chromoFileName,cgpArr[3]);
    strcat(chromoFileName,"_");
    strcat(chromoFileName,cgpArr[4]);
    strcat(chromoFileName,"_");
    strcat(chromoFileName,mutRateChar);
    strcat(chromoFileName,"_chromo.chromo");

    printf("%s\n",chromoFileName);

}

void memoryLeakTest(void)
{
    int i = 0;
    for(i = 0; i < 20000000; i++)
    {
        printf("%d\n",i);
    }
}

void stringCastTest(void)
{
    int i = 0;
    char path[20] = "./kfolddata/fold_";
    char index[2];

    for(i = 0; i < 10; i++)
    {
        strcpy(path,"./kfolddata/fold_");
        sprintf(index,"%d",i);
        strcat(path,index);

        printf("%s\n",path);
    }
}

void idSortTest(void)
{
    int i = 0;
    char **controlData = importFile("dataSet.csv");
    printf("%d\n",(int)sizeof(controlData));
    int entries = 163;
    char **controlID = malloc(entries * sizeof(char*));
    for(i = 0; i < entries; i++)
    {
        controlID[i] = (char*)malloc(10*sizeof(char));
    }

    double *controlDataArr = malloc(entries * sizeof(double));

    for(i = 0; i < entries; i++)
    {
        char* tmpID = (char*)strSplit(controlData[i],",",0);
        strcpy(controlID[i],tmpID);
        free(tmpID);

        char* tmpData = strSplit(controlData[i],",",1);
        controlDataArr[i] = atof(tmpData);
        free(tmpData);
    }

    insertionSort(controlDataArr,controlID,entries);

    for(i = 0; i < entries; i++)
    {
        printf("%s %.2f\n", controlID[i], controlDataArr[i]);
    }

    printf("%s\n", controlID[binarySearch(controlDataArr,entries,43238.0)]);

    for(i = 0; i < entries; i++)
    {
        free(controlData[i]);
        free(controlID[i]);
    }
    free(controlData);
    free(controlID);
    free(controlDataArr);

}
