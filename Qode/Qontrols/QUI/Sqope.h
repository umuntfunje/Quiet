#pragma once

#include "Qheme.h"

//*****************************************************************************************************************************************
class Sqope : public IControl
{
public:
  //***************************************************************************************************************************************
  Sqope(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const double& _phase, const bool& _showSidechain,
    const SampleBuffer& _inputBuffer, const SampleBuffer& _outputBuffer, const SampleBuffer& _sidechainBuffer);

  void Draw(IGraphics& _qui) override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  const double& phase;
  const bool& showSidechain;
  const SampleBuffer &sidechainBuffer, &inputBuffer, &outputBuffer;

  //***************************************************************************************************************************************
  bool showSqope() const;
  void drawSeeq(IGraphics& _qui);
  void drawSqope(IGraphics& _qui);
};