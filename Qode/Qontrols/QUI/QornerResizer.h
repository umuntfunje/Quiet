#pragma once

#include "Qommon.h"
#include "ICornerResizerControl.h"

//*****************************************************************************************************************************************
class QornerResizer : public ICornerResizerControl
{
public:
  //***************************************************************************************************************************************
  QornerResizer(const IRECT& _bounds, const Qheme& _qheme);

  void Draw(IGraphics& _qui) override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;

  //***************************************************************************************************************************************
  void drawResizer(IGraphics& _qui, const IColor& _color, bool _isDragging = false);
};