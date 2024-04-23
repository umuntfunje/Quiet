#include "CircularBuffer.h"

//*****************************************************************************************************************************************
template <class T>
CircularBuffer<T>::CircularBuffer()
{}

template <class T>
CircularBuffer<T>::~CircularBuffer()
{
  clearBuffer();
}

//*****************************************************************************************************************************************
template <class T>
void CircularBuffer<T>::resize(unsigned int _size)
{
  if (buffer)
    clearBuffer();

  size = _size;
  currPosition = 0;

  if (_size > 0)
    buffer = new T[_size]{0};
}

//*****************************************************************************************************************************************
template <class T>
void CircularBuffer<T>::clean()
{
  for (unsigned int i = 0; i < size; i++)
    buffer[i] = 0;

  currPosition = 0;
}

//*****************************************************************************************************************************************
template <class T>
void CircularBuffer<T>::write(T _sample)
{
  if (size == 0)
    return;

  buffer[currPosition++] = _sample;
  currPosition %= size;
}

//*****************************************************************************************************************************************
template <class T>
T CircularBuffer<T>::read() const
{
  if (size == 0)
    return 0;
  else
    return buffer[currPosition];
}

//*****************************************************************************************************************************************
template <class T>
unsigned int CircularBuffer<T>::getSize() const
{
  return size;
}

//*****************************************************************************************************************************************
template <class T>
void CircularBuffer<T>::clearBuffer()
{
  delete[] buffer;
  buffer = nullptr;
}

//*****************************************************************************************************************************************
template class CircularBuffer<double>;