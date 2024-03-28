#pragma once

#include "../Qheme.h"

//*****************************************************************************************************************************************
class Qlider : public IControl
{
public:
  //***************************************************************************************************************************************
  Qlider(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq);

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
  float handleSize = 8.f, traqSize = 2.f;
  double mouseDragValue = .0, qliderGearing = 4.;

  //***************************************************************************************************************************************
  float glow = .67f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow};

  //***************************************************************************************************************************************
  void drawQlider(IGraphics&, const IColor&);
  bool isFineQontrol(const IMouseMod&, bool) const;
};