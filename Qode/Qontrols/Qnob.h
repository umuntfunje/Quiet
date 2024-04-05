#pragma once

#include "Qontrol.h"

//*****************************************************************************************************************************************
class Qnob : public Qontrol
{
public:
  //***************************************************************************************************************************************
  Qnob(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;
  void OnResize() override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseDrag(float _x, float _y, float _dX, float _dY, const IMouseMod& _mod) override;
  void OnMouseUp(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseWheel(float _x, float _y, const IMouseMod& _mod, float _d) override;

private:
  //***************************************************************************************************************************************
  float minAngle = -135.f, maxAngle = 135.f;
  double mouseDragValue = 1., qnobGearing = 4.;
  const float traqSize = thiQness, anqorAngle = -135.f;

  //***************************************************************************************************************************************
  void drawQnob(IGraphics& _qui, const IColor& _color);
  bool isFineQontrol(const IMouseMod& _mod, bool _wheel) const;
};