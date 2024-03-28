#pragma once

#include "../Qheme.h"

//*****************************************************************************************************************************************
class UndoQutton : public IControl
{
public:
  //***************************************************************************************************************************************
  UndoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics&);

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseOut() override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;

  //***************************************************************************************************************************************
  float glow = .023f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow, .123f};

  //***************************************************************************************************************************************
  void drawUndo(IGraphics&, float);
};