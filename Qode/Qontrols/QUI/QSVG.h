#pragma once

#include "../../Qheme.h"

//*****************************************************************************************************************************************
class QSVG : public IControl
{
public:
  //***************************************************************************************************************************************
  QSVG(const IRECT& _bounds, const Qheme& _qheme, const ISVG& _svg, IActionFunction _actionFunq = nullptr, bool _trace = false, bool _qolorA = true);

  void Draw(IGraphics&) override;

  void OnMouseOver(float, float, const IMouseMod&) override {}
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseOut() override {}

private:
  //***************************************************************************************************************************************
  const ISVG svg;
  const Qheme& qheme;
  const bool trace, qolorA;
};