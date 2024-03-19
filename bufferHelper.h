template<typename T, int N>
T getMax(const CircularBuffer<T, N>& buffer) {
  T max = buffer[0];

  for (int i = 1; i < buffer.count(); i++) {
    T yCurr = buffer[i];

    if (yCurr > max)
      max = yCurr;
  }

  return max;
}

template<typename T, int N>
T getMin(const CircularBuffer<T, N>& buffer) {
  T min = buffer[0];

  for (int i = 1; i < buffer.count(); i++) {
    T yCurr = buffer[i];

    if (yCurr < min)
      min = yCurr;
  }

  return min;
}