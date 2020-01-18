void sampleData(sensors_event_t event) {
  sensors_vec_t data;
  if (ringBuffer.isFull())
    ringBuffer.pop(data);
  ringBuffer.push(event.acceleration);
}

void initializeRingBuffer() {
	ringBuffer.clear();
}

sensors_vec_t ringBufferAvg() {
  const int size = ringBuffer.size();
  sensors_vec_t sum;
  sum.x = 0.0f;
  sum.y = 0.0f;
  sum.z = 0.0f;
  for (int i = 0; i < size; i++) {
    sum.x += ringBuffer[i].x;
    sum.y += ringBuffer[i].y;
    sum.z += ringBuffer[i].z;
  }
  sum.x = sum.x/size;
  sum.y = sum.y/size;
  sum.z = sum.z/size;
  return sum;
}

sensors_vec_t ringBufferMin() {
  const int size = ringBuffer.size();
  sensors_vec_t min;
  min.x = 10;
  min.y = 10;
  min.z = 10;
    for (int i = 0; i < size; i++) {
    if (ringBuffer[i].x < min.x) {
      min.x = ringBuffer[i].x;
    }
    if (ringBuffer[i].y < min.y) {
      min.y = ringBuffer[i].y;
    }
    if (ringBuffer[i].z < min.z) {
      min.z = ringBuffer[i].z;
    }
  }
  return min;
}

sensors_vec_t ringBufferMax() {
  const int size = ringBuffer.size();
  sensors_vec_t max;
  max.x = -10;
  max.y = -10;
  max.z = -10;
    for (int i = 0; i < size; i++) {
    if (ringBuffer[i].x > max.x) {
      max.x = ringBuffer[i].x;
    }
    if (ringBuffer[i].y > max.y) {
      max.y = ringBuffer[i].y;
    }
    if (ringBuffer[i].z > max.z) {
      max.z = ringBuffer[i].z;
    }
  }
  return max;
}

float maxValue(float a, float b) { return a > b ? a : b; }

float ringBufferMaxAvgVariance() {
  const int size = ringBuffer.size();
  sensors_vec_t deltas[size - 1];
  for (int i = 0; i < size - 1; i++) {
    deltas[i].x = ringBuffer[i + 1].x - ringBuffer[i].x;
    deltas[i].y = ringBuffer[i + 1].y - ringBuffer[i].y;
    deltas[i].z = ringBuffer[i + 1].z - ringBuffer[i].z;
  }

  sensors_vec_t avg;
  avg.x = 0.0f;
  avg.y = 0.0f;
  avg.z = 0.0f;
  for (int i = 0; i < size - 1; i++) {
    avg.x += deltas[i].x;
    avg.y += deltas[i].y;
    avg.z += deltas[i].z;
  }
  avg.x /= (size - 1);
  avg.y /= (size - 1);
  avg.z /= (size - 1);

  return maxValue(maxValue(abs(avg.x), abs(avg.y)), abs(avg.z));
}
