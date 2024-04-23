#pragma once

#include "Qontrols/Qontrol.h"

//*****************************************************************************************************************************************
class QuttonA : public Qontrol
{
public:
  //***************************************************************************************************************************************
  QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _display, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  const char* display;
};


//*****************************************************************************************************************************************
class QuttonB : public Qontrol
{
public:
  //***************************************************************************************************************************************
  QuttonB(const IRECT& _bounds, const Qheme& _qheme, const DrawQutton& _quttonFunq, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  const DrawQutton quttonFunq;
};