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

  void addQnode(int _index, std::pair<float, float> _position);
  void removeQnode(int _index);
  void setQnode(int _index, std::pair<float, float> _newPosition);
  std::pair<float, float> getQnode(int _index) const;
  void setQnodeCount(int _qnodeCount);
  int getQnodeCount() const;

  float* getQurveBuffer() const;
  void setQurvature(int _index, float _qurvature);
  float getQurvature(int _index) const;
  void setMultiplyQurvature(bool _multiplyQurvature);
  void setQurvatureMultiplier(float _qurvatureMultiplier);

  void flipVertical();
  //void flipHorizontal();
  
  void setSmooth(bool _isSmooth);
  bool getSmooth() const;

  void setQurveName(const char* _qurveName);
  const char* getQurveName() const;

  float getValueAtPhase(float _phase) const;

  void randomizeQurve();

  void undoCurrentState();
  void redoCurrentState();
  void updateCurrentState();
  void updateCurrentState(const QurveState& _pastState);
  bool undoPossible() const;
  bool redoPossible() const;

  QurveState getNow() const;
  
private:
  //***************************************************************************************************************************************
  bool multiplyQurvature = false;
  float qurvatureMultiplier = 1.5f;
  std::string qurveName = "init";
  std::unique_ptr<float[]> qurveBuffer;
  Randomness randomness = Randomness(.0f, 1.f);

  //***************************************************************************************************************************************
  QurveState now;
  std::vector<QurveState> pastStates, futureStates;

  //***************************************************************************************************************************************
  float smoothTransition(float _t) const;
  float qurveScale(float _value, float _qurvature) const;
  float getValueBetween(float _phase, int _from, int _to) const;
};