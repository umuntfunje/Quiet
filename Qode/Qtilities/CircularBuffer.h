#pragma once

//*****************************************************************************************************************************************
/* A "simpler" circular buffer class */
template <class T>
class CircularBuffer
{
public:
  //***************************************************************************************************************************************
  /* Default constructor */
  CircularBuffer();

  /* Destructor */
  ~CircularBuffer();

  /* Initialize the buffer
   * @param _size The size of the new buffer (typically in samples) */
  void resize(unsigned int _size);

  /* Cleans the buffer (sets every value to zero, sets current position to zero) */
  void clean();

  /* Add a new value to the buffer
   * @param _sample The value that updates the buffer (typically a sample) */
  void write(T _sample);

  /* Returns the delayed value from the buffer (look ahead)
   * @return The delayed value (typically a sample) */
  T read() const;

  /* Returns the current size the buffer
   * @return The current size the buffer (typically in samples) */
  unsigned int getSize() const;

protected:
  //***************************************************************************************************************************************
  T* buffer = nullptr;
  unsigned int currPosition = 0, size = 0;

  //***************************************************************************************************************************************
  /* Clears the circular buffer from memory */
  void clearBuffer();
};