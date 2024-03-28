#pragma once

#include "../../Qheme.h"

//*****************************************************************************************************************************************
class Sqope : public IControl
{
public:
  //***************************************************************************************************************************************
  Sqope(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const double& _phase, const bool& _showSidechain,
    sampleBuffer _ins, sampleBuffer _outs, sampleBuffer _sc);

  void Draw(IGraphics&) override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  const double& phase;
  const bool& showSidechain;
  sampleBuffer sc, ins, outs;

  //***************************************************************************************************************************************
  bool showSqope() const;
  void drawSeeq(IGraphics&);
  void drawSqope(IGraphics&);
};