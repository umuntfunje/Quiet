#pragma once

#include "Qontrol.h"

//*****************************************************************************************************************************************
class Qlider : public Qontrol
{
public:
  //***************************************************************************************************************************************
  Qlider(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;
  void OnResize() override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseDrag(float _x, float _y, float _dX, float _dY, const IMouseMod& _mod) override;
  void OnMouseUp(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseWheel(float _x, float _y, const IMouseMod& _mod, float _d) override;

private:
  //***************************************************************************************************************************************
  float handleSize = 8.f, traqSize = 2.f;
  double mouseDragValue = .0, qliderGearing = 4.;

  //***************************************************************************************************************************************
  void drawQlider(IGraphics& _qui, const IColor& _color);
  bool isFineQontrol(const IMouseMod& _mod, bool _wheel) const;
};