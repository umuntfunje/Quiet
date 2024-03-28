#pragma once

#include "../Qheme.h"

//*****************************************************************************************************************************************
class Qnob : public IControl
{
public:
  //***************************************************************************************************************************************
  Qnob(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq);

  void Draw(IGraphics&) override;
  void OnResize() override;

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseDrag(float, float, float, float, const IMouseMod&) override;
  void OnMouseUp(float, float, const IMouseMod&) override;
  void OnMouseWheel(float, float, const IMouseMod&, float) override;
  void OnMouseOut() override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  float minAngle = -135.f, maxAngle = 135.f;
  double mouseDragValue = 1., qnobGearing = 4.;
  const float traqSize = thiQness, anqorAngle = -135.f;

  //***************************************************************************************************************************************
  float glow = .63f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow};

  //***************************************************************************************************************************************
  void drawQnob(IGraphics&, const IColor&);
  bool isFineQontrol(const IMouseMod&, bool) const;
};