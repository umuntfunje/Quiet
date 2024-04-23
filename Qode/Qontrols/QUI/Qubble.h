#pragma once

#include "Qommon.h"
#include "IBubbleControl.h"

//*****************************************************************************************************************************************
class Qubble : public IBubbleControl
{
public:
  //***************************************************************************************************************************************
  Qubble(const Qheme& _qheme);

  void Draw(IGraphics& _qui) override;

  void showQubble(IControl* _qaller, const IRECT& _bounds, const char* _display);

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
};