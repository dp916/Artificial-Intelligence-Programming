/**************************************************
  SGA - Simple Genetic Algorithm
  ------------------------------
  Sin Bowl minimization 2012
    - recent bug fixes:   09/2012 by John Johnson
    - slight mod to rand: 11/2017 by Scott Gordon
  ------------------------------
  SGA Adapted from David Goldberg:
    "Genetic Algorithms in Search, Optimization, and Machine Learning"
     Addison-Wesley, 1989.
  ------------------------------
  Unix version
  Compile with:  g++ sga.c
***************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
using namespace std;

#define POPULATION_SIZE 100    // population size - number of strings
#define CHROM_LENGTH    40    // binary string length of each individual
#define PMUT            0.03  // probability of flipping each bit
#define MAX_GEN         100 // GA stops after this many generations
#define GEN_REP         1     // report is generated at these intervals
#define ELITE           0     // 1=elitism,  0=no elitism
#define MAXMIN          -1    // -1=minimize, 1=maximize

/***************************************************************
****  random fraction between 0.0 and 1.0                  *****
****************************************************************/
#define fracrand() ((double)rand()/RAND_MAX)

void   initialize_population();
void   crossover(int parent1, int parent2, int child1, int child2);
void   mutation();
void   tselection();
int    decode1(int index1);
int    decode2(int index2);
void   getpreviousbest();
double evaluate(int value1, int value2);
double convRange(int raw);
int    flip(double prob);
void   statistics();
void   elite();
void   finalreport();
void   vitality();

struct individual
{
  int value1, value2;
  unsigned char string[CHROM_LENGTH];
  double fitness;
};

struct individual pool[POPULATION_SIZE];
struct individual new_pool[POPULATION_SIZE];
struct individual beststring, verybest;

int selected[POPULATION_SIZE];
int generations;
int count = 0;

/*********************************************************/
int main()
{
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout.precision(4);
  int i;
  generations = 0;
  if (MAXMIN==-1) verybest.fitness = 999999; else verybest.fitness=-999999;

  srand(time(NULL));
  initialize_population();
  generations = 1;

  do
  {
    getpreviousbest();

    /*** SELECTION ***/
    tselection();

    /*** CROSSOVER ***/
    for (i=0; i<POPULATION_SIZE; i=i+2)
      crossover(selected[i],selected[i+1],i,i+1);

    /*** MUTATION ***/
    mutation();

/*********************************************************
This activates the new genetic operation inspired by nature
**********************************************************/
/*
    if (generations == 25)
    {
    vitality();
    }

    if (generations == 50)
    {
    vitality();
    }

    if (generations == 75)
    {
    vitality();
    }
*/

    /*** EVALUATE ***/
    for (i=0; i<POPULATION_SIZE; i++)
    {
      pool[i].value1 = decode1(i);
      pool[i].value2 = decode2(i);
      pool[i].fitness = evaluate(pool[i].value1, pool[i].value2);
    }

    if (ELITE==1)
      elite();

    if (generations % GEN_REP == 0)
      statistics();

  } while (++generations < MAX_GEN);

  finalreport();
  return(0);
}

/*********************************************************
  3-2 Tournament Selection
**********************************************************/
void tselection()
{ int i;
  for (i=0; i<POPULATION_SIZE; i+=2)
  {
    int r = (int) (fracrand()*POPULATION_SIZE);
    int s = (int) (fracrand()*POPULATION_SIZE);
    int t = (int) (fracrand()*POPULATION_SIZE);

    if ( ((MAXMIN*pool[r].fitness) >= (MAXMIN*pool[s].fitness))
      || ((MAXMIN*pool[r].fitness) >= (MAXMIN*pool[t].fitness)))
    {
      if ((MAXMIN*pool[s].fitness) > (MAXMIN*pool[t].fitness))
        { selected[i] = r; selected[i+1] = s; }
      else
        { selected[i] = r; selected[i+1] = t; }
    }
    else
    {
      if ( ((MAXMIN*pool[s].fitness) >= (MAXMIN*pool[r].fitness))
        || ((MAXMIN*pool[s].fitness) >= (MAXMIN*pool[t].fitness)))
      {
        if ((MAXMIN*pool[r].fitness) > (MAXMIN*pool[t].fitness))
        { selected[i] = s; selected[i+1] = r; }
        else
        { selected[i] = s; selected[i+1] = t; }
      }
      else
      {
        if ( ((MAXMIN*pool[t].fitness) >= (MAXMIN*pool[r].fitness))
          || ((MAXMIN*pool[t].fitness) >= (MAXMIN*pool[s].fitness)))
        {
          if ((MAXMIN*pool[r].fitness) > (MAXMIN*pool[s].fitness))
          { selected[i] = t; selected[i+1] = r; }
          else
          { selected[i] = t; selected[i+1] = s;}
} } } } }

/*********************************************************
  Elitism - copy best string to 0th position of new pool
**********************************************************/
void elite()
{
  if ((MAXMIN*beststring.fitness) > (MAXMIN*evaluate(decode1(0), decode2(0))))
  {
    pool[0].fitness = beststring.fitness;
    pool[0].value1 = beststring.value1;
    pool[0].value2 = beststring.value2;
    for (int j=0; j<CHROM_LENGTH; j++)
      pool[0].string[j] = beststring.string[j];
  }
}

/*********************************************************
    Initialize pool to random binary values
**********************************************************/
void initialize_population()
{
  for (int i=0; i<POPULATION_SIZE; i++)
  {
    for (int j=0; j<CHROM_LENGTH; j++ )
      pool[i].string[j] = flip(0.5);
    pool[i].value1 = decode1(i);
    pool[i].value2 = decode2(i);
    pool[i].fitness = evaluate(pool[i].value1, pool[i].value2);
  }
  statistics();
}

/*************************************************************
  - Determine and display best string from previous generation.
  - Maintain very best string from all runs.
**************************************************************/
void getpreviousbest()
{
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout.precision(4);

  if (MAXMIN==-1) beststring.fitness=999999; else beststring.fitness=-999999;

  for (int i=0; i<POPULATION_SIZE; i++)
  {
    if ((MAXMIN*pool[i].fitness) > (MAXMIN*beststring.fitness))
    {
      beststring.value1 = pool[i].value1;
      beststring.value2 = pool[i].value2;
      for (int j=0; j<CHROM_LENGTH; j++)
        beststring.string[j] = pool[i].string[j];
      beststring.fitness = pool[i].fitness;
    }
  }

  if (generations % GEN_REP == 0)
  {
    cout << endl << " Best string: ";
    for (int j=0;j<CHROM_LENGTH;j++)
      cout << (int) beststring.string[j];
    cout << " Value X: " << convRange(beststring.value1);
    cout << " Yalue Y: " << convRange(beststring.value2);
    cout << " Fitness: " << beststring.fitness << endl;
  }

  if ((MAXMIN*beststring.fitness) > (MAXMIN*verybest.fitness))
  {
    verybest.value1 = beststring.value1;
    verybest.value2 = beststring.value2;
    for (int j=0; j<CHROM_LENGTH; j++)
      verybest.string[j] = beststring.string[j];
    verybest.fitness = beststring.fitness;
  }
}

/*********************************************************
      one-point crossover
**********************************************************/
void crossover (int parent1, int parent2, int child1, int child2)
{
  int i, site;
  site = (int) (fracrand()*CHROM_LENGTH);
  for (i=0; i<CHROM_LENGTH; i++)
  {
    if ((i<=site) || (site==0))
    {
      new_pool[child1].string[i] = pool[parent1].string[i];
      new_pool[child2].string[i] = pool[parent2].string[i];
    }
    else
    {
      new_pool[child1].string[i] = pool[parent2].string[i];
      new_pool[child2].string[i] = pool[parent1].string[i];
    }
  }
}

/*********************************************************
    Bitwise mutation  - also transfers strings to pool
**********************************************************/
void mutation()
{
  int i,j;
  for (i=0; i<POPULATION_SIZE; i++)
  {
    for (j=0; j<CHROM_LENGTH; j++)
      if (flip(PMUT)==1)
        pool[i].string[j] = ~new_pool[i].string[j] & 0x01;
      else
        pool[i].string[j] = new_pool[i].string[j] & 0x01;
  }
}

/*********************************************************
    Convert bitstring to positive integer
**********************************************************/
int decode1(int index1)
{
  int value1 = 0;
  for (int i=0; i<CHROM_LENGTH - 20; i++)
    value1 += (int) pow(2.0,(double)i) * pool[index1].string[CHROM_LENGTH-1-i-20];
  return (value1);
}

int decode2(int index2)
{
  int value2 = 0;
  for (int i=0; i<CHROM_LENGTH - 20; i++)
    value2 += (int) pow(2.0,(double)i) * pool[index2].string[CHROM_LENGTH-1-i];
  return (value2);
}

/*********************************************************
   f(x,y) = cos(x) + sqrt(x^2+y^2)
*********************************************************/
double evaluate(int value1, int value2)
{
  double x = convRange(value1);
  double y = convRange(value2);
  double g = cos(x) + (sqrt(pow(x,2) + pow(y,2)));
  return(g);
}

/*********************************************************
    Modified Genetic Operator
    Instead of having a mutation rate at 3 percent we activate this mutation
    by calling this method every 25 generations and modify the generations.
    Half of the population and half of the chromosomes are affected.
**********************************************************/
void vitality()
{
  int i,j;
  for (i=0; i<50; i++)
  {
    for (j=0; j<20; j++)
        pool[i].string[j] = ~new_pool[i].string[j] & 0x01;
  }
}

/*********************************************************
 Convert positive integer to desired floating point range.
 Problem-specific - change for different string lengths
**********************************************************/
double convRange(int raw)
{
  double outval = ((((double)raw)/1048575.0)*10.0)-5.0;
  return(outval);
}

/*********************************************************
    Do a biased coin toss based on a probability
**********************************************************/
int flip(double prob)
{
  return((fracrand()<prob)?1:0);
}

/*********************************************************
    Report printed at each generation
**********************************************************/
void statistics()
{
  int i,j;
  cout.setf(ios::fixed); cout.setf(ios::showpoint); cout.precision(4);

  cout << "\nGENERATION: " << generations << endl << "Selected Strings: ";
  for (i=0; i<POPULATION_SIZE; i++) cout << selected[i] << " ";
  cout << endl << "\n\tX\tY\tf(x,y)\t\tnew_str\t\t\t\t\t\t X\t Y";
  for (i=0; i<POPULATION_SIZE; i++)
  {
    cout << endl << "   ";
    cout << "\t" << convRange(pool[i].value1) << " " << convRange(pool[i].value2) << "\t" <<pool[i].fitness << "\t\t";
    for (j=0; j<CHROM_LENGTH; j++)
      cout << (int) pool[i].string[j];
    cout << "\t" << convRange(decode1(i));
    cout << "\t" << convRange(decode2(i));
  }
  cout << endl;
}

/*********************************************************
    Report printed at the very end of execution
**********************************************************/
void finalreport()
{
  cout << "===================================================================================================================" << endl;
  cout << "Best result over all generations:" << endl;
  for (int j=0; j<CHROM_LENGTH; j++)
    cout << (int) verybest.string[j];
  cout << endl;
  cout << "Decoded X = " << convRange(verybest.value1);
  cout << "\nDecoded Y = " << convRange(verybest.value2);
  cout << "\nFitness = " << verybest.fitness << endl;
}
