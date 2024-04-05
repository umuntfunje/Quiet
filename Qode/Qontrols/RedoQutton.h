#pragma once

#include "Qontrol.h"

//*****************************************************************************************************************************************
class RedoQutton : public Qontrol
{
public:
  //***************************************************************************************************************************************
  RedoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui);

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  void drawRedo(IGraphics& _qui);
};