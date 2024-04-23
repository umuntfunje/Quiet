#pragma once

#include "Qommon.h"

//*****************************************************************************************************************************************
class Sqope : public IControl
{
public:
  //***************************************************************************************************************************************
  Sqope(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const double& _phase, const bool& _showSidechain, const bool& _avoidInputs,
    const float* _inputBuffer, const float* _outputBuffer, const float* _sidechainBuffer);

  void Draw(IGraphics& _qui) override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  const double& phase;
  const bool& avoidInputs;
  const bool& showSidechain;
  const float *sidechainBuffer = nullptr, *inputBuffer = nullptr, *outputBuffer = nullptr;

  //***************************************************************************************************************************************
  bool showSqope() const;
  void drawSeeq(IGraphics& _qui) const;
  void drawSqope(IGraphics& _qui) const;
};