#include "Qenu.h"
#include <fstream>
#include <sstream>

//*****************************************************************************************************************************************
Qenu::Qenu(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq)
  : QDirBrowseControlBase(_bounds, "quiet", _actionFunq)
  , qurve(_qurve)
  , qheme(_qheme)
{
  WDL_String presetPath;

  UserHomePath(presetPath);
  addPath(strcat(presetPath.Get(), "\\Documents\\umuntfuNje\\Quiet Presets"), "");
  setupQenu();

  bool found = false;
  std::string initPresetName = qurve.getQurveName();

  for (int i = 0; i < presetList.GetSize(); i++)
  {
    if (initPresetName == presetList.Get(i)->GetText())
    {
      found = true;
      cheqSelectedPreset(i);
      loadPresetAtCurrentIndex(true);
      break;
    }
  }

  if (!found)
    setPresetName(initPresetName.c_str());

  mDblAsSingleClick = true;
  setupAnimation(0.251f, 0.73f);
}

//*****************************************************************************************************************************************
void Qenu::Draw(IGraphics& _qui)
{
  _qui.FillRoundRect(qheme.foreGround.WithOpacity(map(smoothedValue, 0.73f, 1.0f, 0.023f, 0.111f)), mTargetRECT, 3);
  _qui.DrawText(IText(16, EAlign::Center, qheme.foreGround.WithOpacity(smoothedValue)), currentPresetName.Get(), mRECT);
  animateQontrol();
}

//*****************************************************************************************************************************************
void Qenu::OnPopupMenuSelection(IPopupMenu* _selectedQenu, int)
{
  if (_selectedQenu)
  {
    IPopupMenu::Item* preset = _selectedQenu->GetChosenItem();

    if (preset)
    {
      cheqSelectedPreset(presetList.Find(preset));
      loadPresetAtCurrentIndex();
    }
  }
}

//*****************************************************************************************************************************************
void Qenu::OnMouseOver(float, float, const IMouseMod&)
{
  qAnimation = QAnimation::qBrighten;
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qenu::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
    GetUI()->CreatePopupMenu(*this, mainQenu, mRECT);
}

//*****************************************************************************************************************************************
void Qenu::OnMouseOut()
{
  qAnimation = QAnimation::qDim;
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qenu::setPresetName(const char* _presetName, bool _setDirty)
{
  currentPresetName.Set(_presetName);

  if (_setDirty)      // usually on add asterisk
    SetDirty(false);
}

//*****************************************************************************************************************************************
void Qenu::savePreset()
{
  WDL_String fileName;
  WDL_String presetPath{path};

  presetPath.Append("\\User");
  GetUI()->PromptForFile(fileName, presetPath, EFileAction::Save, fileExtension.Get());

  if (fileName.GetLength()) // ...if something saved
  {
    std::ofstream outFile;

    outFile.open(fileName.Get(), std::ios::out);

    std::string c;
    std::istringstream line(fileName.get_filepart());

    getline(line, c, '.');
    qurve.setQurveName(c.c_str());

    outFile << c << std::endl;
    outFile << qurve.getQnodeCount() << std::endl;
    outFile << qurve.getSmooth() << std::endl;

    for (int i = 0; i < qurve.getQnodeCount(); i++)
    {
      outFile << qurve.getQurvature(i) << std::endl;
      outFile << qurve.getQnode(i).first << " " << qurve.getQnode(i).second << std::endl;
    }

    outFile.close();
  }

  setupQenu();

  bool found = false;
  std::string initPresetName = qurve.getQurveName();

  for (int i = 0; i < presetList.GetSize(); i++)
    if (initPresetName == presetList.Get(i)->GetText())
    {
      cheqSelectedPreset(i);
      loadPresetAtCurrentIndex(true);
      found = true;
      break;
    }

  if (!found)
  {
    if (initPresetName.back() == '*')
      return;
    else
      loadPreviousPreset();
  }
}

//*****************************************************************************************************************************************
void Qenu::loadNextPreset()
{
  if (presetList.GetSize() == 0)
    return;

  int i = selectedIndex + 1;

  if (i >= presetList.GetSize())
    i = 0;

  cheqSelectedPreset(i);
  loadPresetAtCurrentIndex();
}

//*****************************************************************************************************************************************
void Qenu::loadPreviousPreset()
{
  if (presetList.GetSize() == 0)
    return;

  int i = selectedIndex - 1;

  if (i < 0)
    i = presetList.GetSize() - 1;

  cheqSelectedPreset(i);
  loadPresetAtCurrentIndex();
}

//*****************************************************************************************************************************************
void Qenu::loadPresetAtCurrentIndex(bool _initializing)
{
  if (selectedIndex > -1 && selectedIndex < presetList.GetSize())
  {
    std::ifstream inFile;
    const char* fileName = fileList.Get(selectedIndex)->Get();

    inFile.open(fileName, std::ios::in);

    if (!inFile.is_open())
      return;

    std::string pName;

    getline(inFile, pName);

    if (pName.empty())
      return;

    bool isSmooth;
    int qnodeCount;

    if (!_initializing)
      qurve.updateCurrentState();

    setPresetName(pName.c_str());

    qurve.setQurveName(pName.c_str());
    inFile >> qnodeCount;
    qurve.setQnodeCount(qnodeCount);
    inFile >> isSmooth;
    qurve.setSmooth(isSmooth);

    for (int i = 0; i < qnodeCount; i++)
    {
      float qurvature;
      std::pair<float, float> qnode;

      inFile >> qurvature;
      qurve.setQurvature(i, qurvature);
      inFile >> qnode.first >> qnode.second;
      qurve.setQnode(i, qnode);
    }

    inFile.close();
    qurve.qreateQurve();

    SetDirty();
  }
}

//*****************************************************************************************************************************************
void Qenu::animateQontrol()
{
  switch (qAnimation)
  {
  case QAnimation::qNothing:
    break;

  case QAnimation::qBrighten:
    if (smoothedValue == smoothAnimation.getMaxValue())
      qAnimation = QAnimation::qNothing;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMaxValue());
      SetDirty(false);
    }
    break;

  case QAnimation::qDim:
    if (smoothedValue == smoothAnimation.getMinValue())
      qAnimation = QAnimation::qNothing;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMinValue());
      SetDirty(false);
    }
    break;
  }
}

//*****************************************************************************************************************************************
void Qenu::cheqSelectedPreset(int _index)
{
  int prevSelectedPreset = selectedIndex;

  if (prevSelectedPreset != -1)
    presetList.Get(prevSelectedPreset)->SetChecked(false);

  selectedIndex = _index;
  presetList.Get(_index)->SetChecked(true);
}

//*****************************************************************************************************************************************
void Qenu::setupAnimation(float _duationInMS, float _startValue, float _endValue)
{
  smoothedValue = _startValue;
  smoothAnimation = Smoothing<float>(_startValue, _duationInMS, true, _startValue, _endValue);
}