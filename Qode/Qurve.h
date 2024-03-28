#pragma once

#include "Qtilities.h"
#include "Randomness.h"

//*****************************************************************************************************************************************
/* A curve class adapted from <> */
class Qurve
{
public:
  //***************************************************************************************************************************************
  struct QurveState
  {
    int qnodeCount = 2;
    bool isSmooth = false;
    float qurvature[maxQnodes] = {.0f};
    std::pair<float, float> qnodeList[maxQnodes] = {{.0f, .0f}};
  };

  //***************************************************************************************************************************************
  Qurve();

  void qreateQurve();

  void addQnode(int, std::pair<float, float>);
  void removeQnode(int);
  void setQnode(int, std::pair<float, float>);
  std::pair<float, float> getQnode(int) const;
  void setQnodeCount(int);
  int getQnodeCount() const;

  float* getQurveBuffer() const;
  void setQurvature(int, float);
  float getQurvature(int) const;
  void setMultiplyQurvature(bool);
  void setQurveMultiplier(float);

  void flipVertical();
  //void flipHorizontal();
  
  void setSmooth(bool);
  bool getSmooth() const;

  void setQurveName(const char*);
  const char* getQurveName() const;

  float getValueAtPhase(float) const;

  void randomizeQurve();

  void undoCurrentState();
  void redoCurrentState();
  void updateCurrentState();
  void updateCurrentState(const QurveState&);
  bool undoPossible() const;
  bool redoPossible() const;

  QurveState getNow() const;
  
private:
  //***************************************************************************************************************************************
  bool multiplyQurvature = false;
  float qurveMultiplier = 1.5f;
  std::string qurveName = "init";
  std::unique_ptr<float[]> qurveBuffer;
  Randomness randomness = Randomness(.0f, 1.f);

  //***************************************************************************************************************************************
  QurveState now;
  std::vector<QurveState> pastStates, futureStates;

  //***************************************************************************************************************************************
  float smoothTransition(float) const;
  float qurveScale(float, float) const;
  float getValueBetween(float, int, int) const;
};