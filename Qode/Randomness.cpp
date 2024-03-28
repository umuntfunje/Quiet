#include "Randomness.h"

int Randomness::nextSeed = std::rand();

//*****************************************************************************************************************************************
Randomness::Randomness(float _min, float _max)
  : engine(nextSeed++)
  , distribution(_min, _max)
{}

//*****************************************************************************************************************************************
float Randomness::next()
{
  return distribution(engine);
}

//*****************************************************************************************************************************************
void Randomness::seed(int _newSeed)
{
  engine.seed(_newSeed);
}