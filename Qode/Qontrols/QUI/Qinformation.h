#pragma once

#include "../../Qheme.h"

//*****************************************************************************************************************************************
class Qinformation : public IControl
{
public:
  //***************************************************************************************************************************************
  Qinformation(const IRECT& _bounds, const Qheme& _qheme);

  void Draw(IGraphics&) override;

  void OnMouseOver(float, float, const IMouseMod&) override {}
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseOut() override {}

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
};