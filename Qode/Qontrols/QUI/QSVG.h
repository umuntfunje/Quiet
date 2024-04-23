#pragma once

#include "Qommon.h"

//*****************************************************************************************************************************************
class QSVG : public IControl
{
public:
  //***************************************************************************************************************************************
  QSVG(const IRECT& _bounds, const Qheme& _qheme, const ISVG& _svg, IActionFunction _actionFunq = nullptr, bool _trace = false, bool _qolorA = true);

  void Draw(IGraphics& _qui) override;

  void OnMouseOver(float _x, float _y, const IMouseMod& _mod) override {}
  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseOut() override {}

private:
  //***************************************************************************************************************************************
  const ISVG svg;
  const Qheme& qheme;
  const bool trace, qolorA;
};