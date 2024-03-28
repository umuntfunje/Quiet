#include "QnodeSystem.h"

//*****************************************************************************************************************************************
QnodeSystem::QnodeSystem(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq)
  : IControl(_bounds, _actionFunq)
  , qnodeDiameter(_bounds.H() * .056f) // kuru....
  , qurve(_qurve)
  , qheme(_qheme)
  , tempState(_qurve.getNow())
{}

//*****************************************************************************************************************************************
void QnodeSystem::Draw(IGraphics& _qui)
{
  shadeSeqt(_qui);
  drawQurve(_qui);
  drawQnodes(_qui);
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseOver(float _x, float _y, const IMouseMod&)
{
  int qnodeCount = qurve.getQnodeCount();

  overQnodeIndex = -1;
  overQurveIndex = -1;

  for (int i = 0; i < qnodeCount; i++)
    if (pairToRect(qurve.getQnode(i)).Contains(_x, _y))
    {
      overQnodeIndex = i;
      SetDirty(false);
      return;
    }

  for (int i = 0; i < qnodeCount; i++)
    if (getQurveSeqt(i).Contains(_x, _y))
    {
      overQurveIndex = i;
      SetDirty(false);
      return;
    }
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    if (overQnodeIndex != -1)
    {
      downQnodeIndex = overQnodeIndex;
      GetUI()->HideMouseCursor(true, false);
      SetDirty(false);
    }
    else if (overQurveIndex != -1)
    {
      downQurveIndex = overQurveIndex;
      GetUI()->HideMouseCursor();
    }
  }
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseDrag(float _x, float _y, float, float _dY, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    if (downQnodeIndex != -1)
    {
      fromMouseDrag = true;
      mRECT.Constrain(_x, _y);

      if (downQnodeIndex == 0 || downQnodeIndex == qurve.getQnodeCount() - 1)
      {
        
        int first = 0, last = qurve.getQnodeCount() - 1;
        float newY = (_y - mRECT.T) / mRECT.H();

        snapToGrid(newY);
        qurve.setQnode(first, {.0f, newY});
        qurve.setQnode(last, {1.f, newY});
      }
      else
      {
        float newX = (_x - mRECT.L) / mRECT.W();
        float newY = (_y - mRECT.T) / mRECT.H();

        newX = Clip(newX, getMinX(downQnodeIndex), getMaxX(downQnodeIndex));

        snapToGrid(newX);
        snapToGrid(newY);
        qurve.setQnode(downQnodeIndex, {newX, newY});
      }

      qurve.qreateQurve();
      SetDirty(false);
    }
    else if (downQurveIndex != -1)
    {
      fromMouseDrag = true;

      if (_mod.A)
      {
        int lower = downQurveIndex;
        int upper = downQurveIndex + 1;
        float newY1 = qurve.getQnode(lower).second + (_dY / mRECT.H());
        float newY2 = qurve.getQnode(upper).second + (_dY / mRECT.H());

        newY1 = Clip(newY1, .0f, 1.f);
        newY2 = Clip(newY2, .0f, 1.f);
        
        qurve.setQnode(lower, {qurve.getQnode(lower).first, newY1});
        qurve.setQnode(upper, {qurve.getQnode(upper).first, newY2});

        if (lower == 0 && upper != qurve.getQnodeCount() - 1)
          qurve.setQnode(qurve.getQnodeCount() - 1, {qurve.getQnode(qurve.getQnodeCount() - 1).first, newY1});
        else if (upper == qurve.getQnodeCount() - 1 && lower != 0)
          qurve.setQnode(0, {qurve.getQnode(0).first, newY2});
      }
      else
      {
        float qurvature = qurve.getQurvature(downQurveIndex);

        if (qurve.getQnode(downQurveIndex).second < qurve.getQnode((downQurveIndex + 1) % qurve.getQnodeCount()).second)
          qurvature -= _dY / 15.f;
        else
          qurvature += _dY / 15.f;

        qurve.setQurvature(downQurveIndex, Clip(qurvature, -maxQurvature, maxQurvature));
      }

      qurve.qreateQurve();
      SetDirty(false);
    }
  }
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseUp(float, float, const IMouseMod& _mod)
{
  if (fromMouseDrag)
  {
    qurve.updateCurrentState(tempState);
    tempState = qurve.getNow();
  }

  if (downQnodeIndex != -1)
  {
    downQnodeIndex = -1;
    GetUI()->HideMouseCursor(false);
    SetDirty(fromMouseDrag);

    if (fromMouseDrag)
      fromMouseDrag = false;
  }
  else if (downQurveIndex != -1)
  {
    downQurveIndex = -1;
    GetUI()->HideMouseCursor(false);
    SetDirty(fromMouseDrag);

    if (fromMouseDrag)
      fromMouseDrag = false;
  }
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseOut()
{
  overQnodeIndex = -1;
  downQnodeIndex = -1;

  overQurveIndex = -1;
  downQurveIndex = -1;
  
  SetDirty(false);
}

//*****************************************************************************************************************************************
void QnodeSystem::OnMouseDblClick(float _x, float _y, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    qurve.updateCurrentState();

    int qnodeCount = qurve.getQnodeCount();

    if (overQnodeIndex == -1 && qnodeCount < maxQnodes)
    {
      int index = 0;
      float x = (_x - mRECT.L) / mRECT.W();
      float y = (_y - mRECT.T) / mRECT.H();
      
      for (; index < qnodeCount; ++index)
        if (qurve.getQnode(index).first > x)
          break;

      qurve.addQnode(index, std::pair{x, y});
      SetDirty();

      tempState = qurve.getNow();
    }
    else if (overQnodeIndex != -1 && qnodeCount > 2)
    {
      if (overQnodeIndex != 0 && overQnodeIndex != qurve.getQnodeCount() - 1)
      {
        qurve.removeQnode(overQnodeIndex);
        SetDirty();

        tempState = qurve.getNow();
      }
    }
  }
  else if (_mod.R && overQurveIndex != -1)
  {
    qurve.updateCurrentState();
    qurve.setQurvature(overQurveIndex, .0f);
    qurve.qreateQurve();
    SetDirty();

    tempState = qurve.getNow();
  }
}

//*****************************************************************************************************************************************
void QnodeSystem::setSnap(bool _snap)
{
  snap = _snap;
}

//*****************************************************************************************************************************************
void QnodeSystem::setGridSize(int _gridSize)
{
  gridSize = _gridSize;
}

//*****************************************************************************************************************************************
void QnodeSystem::shadeSeqt(IGraphics& _qui)
{
  _qui.FillRect(qheme.foreGround.WithOpacity(.00943f), mRECT);

  if (overQurveIndex != -1)
    _qui.FillRect(qheme.foreGround.WithOpacity(.0436f), getQurveSeqt(overQurveIndex));
}

//*****************************************************************************************************************************************
void QnodeSystem::drawQurve(IGraphics& _qui)
{
  _qui.DrawData(qheme.foreGround.WithOpacity(.73f), mRECT, qurve.getQurveBuffer(), maxResolution + xtraValues, nullptr, 0, thiQness);
}

//*****************************************************************************************************************************************
void QnodeSystem::drawQnodes(IGraphics& _qui)
{
  int qnodeCount = qurve.getQnodeCount();

  for (int i = 0; i < qnodeCount; i++)
  {
    float x = qurve.getQnode(i).first;
    float y = qurve.getQnode(i).second;

    if (downQnodeIndex == i)
    {
      _qui.DrawRoundRect(qheme.foreGround.WithOpacity(.67f), pairToRect(std::pair{x, y}), 2.81f, 0, thiQness);
      _qui.FillRoundRect(qheme.foreGround.WithOpacity(.67f), pairToRect(std::pair{x, y}).GetPadded(-2), 2.81f);
    }
    else if (overQnodeIndex == i)
      _qui.DrawRoundRect(qheme.foreGround, pairToRect(std::pair{x, y}), 2.81f, 0, thiQness);
    else
      _qui.DrawRoundRect(qheme.foreGround.WithOpacity(.67f), pairToRect(std::pair{x, y}), 2.81f, 0, thiQness);
  }
}

//*****************************************************************************************************************************************
float QnodeSystem::getMinX(int _index) const
{
  return qurve.getQnode(_index - 1).first;
}

//*****************************************************************************************************************************************
float QnodeSystem::getMaxX(int _index) const
{
  return qurve.getQnode(_index + 1).first;
}

//*****************************************************************************************************************************************
void QnodeSystem::snapToGrid(float& _value)
{
  if (snap && gridSize > 1)
  {
    float snapRadius = .02f; // kuru...
    float snappedValue = std::roundf(_value * gridSize) / gridSize;

    if (fabsf(snappedValue - _value) < snapRadius)
      _value = snappedValue;
  }
}

//*****************************************************************************************************************************************
bool QnodeSystem::isFineQontrol(const IMouseMod& _mod) const
{
#ifdef PROTOOLS
  #ifdef OS_WIN
  return _mod.C;
  #endif
#else
  return (_mod.C || _mod.S);
#endif
}

//*****************************************************************************************************************************************
IRECT QnodeSystem::pairToRect(const std::pair<float, float>& _pair) const
{
  float x = (_pair.first * mRECT.W() + mRECT.L) - (qnodeDiameter / 2.f);
  float y = (_pair.second * mRECT.H() + mRECT.T) - (qnodeDiameter / 2.f);

  return IRECT::MakeXYWH(x, y, qnodeDiameter, qnodeDiameter);
}

//*****************************************************************************************************************************************
IRECT QnodeSystem::getQurveSeqt(int _index) const
{
  if (_index == qurve.getQnodeCount() - 1)
    return IRECT();

  float x = qurve.getQnode(_index).first * mRECT.W() + mRECT.L;
  float upper = qurve.getQnode(_index + 1).first * mRECT.W() + mRECT.L;
  float lower = qurve.getQnode(_index).first * mRECT.W() + mRECT.L;

  return IRECT::MakeXYWH(x, mRECT.T, upper - lower, mRECT.H());
}