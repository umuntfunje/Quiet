#pragma once

#include "Qontrol.h"

//*****************************************************************************************************************************************
class UndoQutton : public Qontrol
{
public:
  //***************************************************************************************************************************************
  UndoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui);

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  void drawUndo(IGraphics& _qui);
};