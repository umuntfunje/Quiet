#include "Qurve.h"
#include <assert.h>

//*****************************************************************************************************************************************
Qurve::Qurve()
{
  qurveBuffer = std::make_unique<float[]>(maxResolution + xtraValues);

  now.qnodeList[0] = {0.0f, 0.0f};
  now.qurvature[0] = now.qurvature[1] = 0;
  now.qnodeList[1] = {1.0f, 0.0f};

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::qreateQurve()
{
  int qnodeIndex = 0;
  float currQurvature = 0;
  std::pair<float, float> prevQnode = now.qnodeList[qnodeIndex];
  std::pair<float, float> currQnode = now.qnodeList[qnodeIndex];

  for (int i = 0; i < maxResolution; i++)
  {
    float x = i / static_cast<float>(maxResolution - 1);
    float temp = 1;

    if (currQnode.first > prevQnode.first)
      temp = (x - prevQnode.first) / (currQnode.first - prevQnode.first);

    if (now.isSmooth)
      temp = smoothTransition(temp);

    temp = std::min(std::max(qurveScale(temp, currQurvature), 0.0f), 1.0f); 

    float y = prevQnode.second + temp * (currQnode.second - prevQnode.second);

    qurveBuffer[i + 1] = 1 - y;

    while (x > currQnode.first && qnodeIndex < now.qnodeCount)
    {
      currQurvature = now.qurvature[qnodeIndex % now.qnodeCount];
      qnodeIndex++;
      prevQnode = currQnode;
      currQnode = now.qnodeList[qnodeIndex % now.qnodeCount];

      if (qnodeIndex >= now.qnodeCount)
      {
        currQnode.first += 1;
        break;
      }
    }
  }

  //qurve[0] = qurve[maxResolution];
  //qurve[maxResolution + 1] = qurve[1];
  //qurve[maxResolution + 2] = qurve[2];
  
  //qurve[0] = qurve[1];
  //qurve[maxResolution + 1] = qurve[maxResolution];
  //qurve[maxResolution + 2] = qurve[maxResolution];

  qurveBuffer[0] = qurveBuffer[1];
  qurveBuffer[maxResolution + 1] = qurveBuffer[0];
}

//*****************************************************************************************************************************************
void Qurve::addQnode(int _index, std::pair<float, float> _position)
{
  assert(_index >= 0 && _index < maxQnodes);

  for (int i = now.qnodeCount; i > _index; i--)
  {
    now.qnodeList[i] = now.qnodeList[i - 1];
    now.qurvature[i] = now.qurvature[i - 1];
  }

  now.qnodeCount++;
  now.qnodeList[_index] = _position;
  now.qurvature[_index] = 0;

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::removeQnode(int _index)
{
  assert(_index >= 0 && _index < maxQnodes);

  now.qnodeCount--;

  for (int i = _index; i < now.qnodeCount; i++)
  {
    now.qnodeList[i] = now.qnodeList[i + 1];
    now.qurvature[i] = now.qurvature[i + 1];
  }

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setQnode(int _index, std::pair<float, float> _newPosition)
{
  assert(_index >= 0 && _index < maxQnodes);

  now.qnodeList[_index] = _newPosition;
}

//*****************************************************************************************************************************************
std::pair<float, float> Qurve::getQnode(int _index) const
{
  assert(_index >= 0 && _index < maxQnodes);

  return now.qnodeList[_index];
}

//*****************************************************************************************************************************************
void Qurve::setQnodeCount(int _qnodeCount)
{
  now.qnodeCount = _qnodeCount;
}

//*****************************************************************************************************************************************
int Qurve::getQnodeCount() const
{
  return now.qnodeCount;
}

//*****************************************************************************************************************************************
float* Qurve::getQurveBuffer() const
{
  return qurveBuffer.get();
}

//*****************************************************************************************************************************************
void Qurve::setQurvature(int _index, float _qurvature)
{
  assert(_index >= 0 && _index < maxQnodes);

  now.qurvature[_index] = _qurvature;
}

//*****************************************************************************************************************************************
float Qurve::getQurvature(int _index) const
{
  return now.qurvature[_index];
}

//*****************************************************************************************************************************************
void Qurve::setMultiplyQurvature(bool _multiplyQurvature)
{
  if (_multiplyQurvature == multiplyQurvature)
    return;

  multiplyQurvature = _multiplyQurvature;
  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setQurvatureMultiplier(float _qurvatureMultiplier)
{
  qurvatureMultiplier = _qurvatureMultiplier;
}

//*****************************************************************************************************************************************
void Qurve::flipVertical()
{
  for (int i = 0; i < now.qnodeCount; i++)
    now.qnodeList[i].second = 1 - now.qnodeList[i].second;

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setSmooth(bool _isSmooth)
{
  now.isSmooth = _isSmooth;
}

//*****************************************************************************************************************************************
bool Qurve::getSmooth() const
{
  return now.isSmooth;
}

//*****************************************************************************************************************************************
void Qurve::setQurveName(const char* _qurveName)
{
  qurveName = _qurveName;
}

//*****************************************************************************************************************************************
const char* Qurve::getQurveName() const
{
  return qurveName.c_str();
}

//*****************************************************************************************************************************************
float Qurve::getValueAtPhase(float _phase) const
{
  for (int i = 0; i < now.qnodeCount - 1; i++)
    if (now.qnodeList[i].first <= _phase && now.qnodeList[i + 1].first >= _phase)
      return getValueBetween(_phase, i, i + 1);

  return now.qnodeList[now.qnodeCount - 1].second;
}

//*****************************************************************************************************************************************
void Qurve::randomizeQurve()
{
  qurveName = "random";
  now.isSmooth = static_cast<int>(std::roundf(randomness.next()));
  now.qnodeCount = static_cast<int>(std::roundf(randomness.next() * (maxQnodes / 10))); // 100 is too many, i think...

  if (now.qnodeCount <= 2 || now.qnodeCount > maxQnodes)
  {
    now.qnodeCount = 2;
    now.qnodeList[0] = {0.0f, randomness.next()};
    now.qurvature[0] = now.qurvature[now.qnodeCount - 1] = randomness.next() * maxQurvature;
    now.qnodeList[now.qnodeCount - 1] = {1.0f, now.qnodeList[0].second};

    std::srand(static_cast<uint32_t>(time(NULL)));
    randomness.seed(std::rand());

    qreateQurve();
    return;
  }

  now.qnodeList[0] = {0.0f, randomness.next()};
  now.qurvature[0] = now.qurvature[now.qnodeCount - 1] = (randomness.next() * 2 - 1) * maxQurvature; // -maxQurvature to maxQurvature
  now.qnodeList[now.qnodeCount - 1] = {1.0f, now.qnodeList[0].second};

  std::vector<float> x;

  for (int i = 1; i < now.qnodeCount - 1; i++)
    x.push_back(randomness.next());

  std::sort(x.begin(), x.end());

  for (int j = 1; j < now.qnodeCount - 1; j++)
  {
    now.qurvature[j] = (randomness.next() * 2 - 1) * maxQurvature; // -maxQurvature to maxQurvature
    now.qnodeList[j] = {x[j - 1], randomness.next()};
  }

  std::srand(static_cast<uint32_t>(time(NULL)));
  randomness.seed(std::rand());

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::undoCurrentState()
{
  if (pastStates.empty())
    return;

  futureStates.push_back(now);
  now = pastStates.back();
  pastStates.pop_back();

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::redoCurrentState()
{
  if (futureStates.empty())
    return;

  pastStates.push_back(now);
  now = futureStates.back();
  futureStates.pop_back();

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::updateCurrentState()
{
  pastStates.push_back(now);
  futureStates.clear();
}

//*****************************************************************************************************************************************
void Qurve::updateCurrentState(const QurveState& _pastState)
{
  pastStates.push_back(_pastState);
  futureStates.clear();
}

//*****************************************************************************************************************************************
bool Qurve::undoPossible() const
{
  return !pastStates.empty();
}

//*****************************************************************************************************************************************
bool Qurve::redoPossible() const
{
  return !futureStates.empty();
}

//*****************************************************************************************************************************************
Qurve::QurveState Qurve::getNow() const
{
  return now;
}

//*****************************************************************************************************************************************
float Qurve::smoothTransition(float _temp) const
{
  static const double pI = 3.1415926535897932384626433832795;

  return 0.5f * sinf((_temp - 0.5f) * static_cast<float>(pI)) + 0.5f;
}

//*****************************************************************************************************************************************
float Qurve::qurveScale(float _value, float _qurvature) const
{
  static constexpr float minQurvature = 0.01f;

  if (fabsf(_qurvature) < minQurvature)
    return _value;

  float numerator = expf(_qurvature * _value) - 1;
  float denominator = expf(_qurvature) - 1;

  return numerator / denominator;
}

//*****************************************************************************************************************************************
float Qurve::getValueBetween(float _phase, int _from, int _to) const
{
  assert(_from >= 0 && _to < now.qnodeCount);

  std::pair<float, float> first = now.qnodeList[_from];
  std::pair<float, float> second = now.qnodeList[_to];
  float _qurvature = now.qurvature[_from];

  if (multiplyQurvature)
    _qurvature *= qurvatureMultiplier;

  float width = second.first - first.first;

  if (width <= 0)
    return 1 - second.second;

  float temp = (_phase - first.first) / width;

  if (now.isSmooth)
    temp = smoothTransition(temp);

  temp = std::min(std::max(qurveScale(temp, _qurvature), 0.0f), 1.0f); 

  return 1 - (temp * (second.second - first.second) + first.second);
}