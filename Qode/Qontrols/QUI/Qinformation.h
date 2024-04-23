#pragma once

#include "Qommon.h"

//*****************************************************************************************************************************************
class Qinformation : public IControl
{
public:
  //***************************************************************************************************************************************
  Qinformation(const IRECT& _bounds, const Qheme& _qheme);

  void Draw(IGraphics& _qui) override;

  void OnMouseOver(float _x, float _y, const IMouseMod& _mod) override {}
  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseOut() override {}

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
};