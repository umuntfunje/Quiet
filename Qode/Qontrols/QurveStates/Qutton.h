#pragma once

#include "Qontrols/Qontrol.h"

//*****************************************************************************************************************************************
class QuttonA : public Qontrol
{
public:
  //***************************************************************************************************************************************
  QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _ch, IActionFunction _actionFunq, float _textSize = 20.f);

  void Draw(IGraphics& _qui) override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  const char* ch;
  const float textSize;
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