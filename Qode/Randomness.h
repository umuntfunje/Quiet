#pragma once

#include <random>

//*****************************************************************************************************************************************
/* A random generator class I stole from <> */
class Randomness
{
public:
  //***************************************************************************************************************************************
  static int nextSeed;

  /* Constructs randomness
   * @param _min Minimum value to be produced by the generator
   * @param _max Maximum value to be produced by the generator */
  Randomness(float, float);

  /* Gets the next random value
   * @return A random value */
  float next();

  /* Sets a new seed
   * @param _newSeed The new seed */
  void seed(int);

private:
  //***************************************************************************************************************************************
  std::mt19937 engine;
  std::uniform_real_distribution<float> distribution;
};