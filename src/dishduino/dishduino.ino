#include <StateMachine.h>
#include <RingBuf.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

#define SAMPLE_PERIOD_MS 1000
#define RING_BUFFER_SIZE 10 // seconds of samples
RingBuf<sensors_vec_t, RING_BUFFER_SIZE> ringBuffer;

StateMachine stateMachine = StateMachine();
State* Clean = stateMachine.addState(&clean);
State* Unloading = stateMachine.addState(&unloading);
State* Loadable = stateMachine.addState(&loadable);
State* Running = stateMachine.addState(&running);

void clean() {
  
}

bool transitionCleanUnloading() {
  
}

void unloading() {
  Serial.println("unloading");
}

bool transitionUnloadingLoadable() {
  Serial.print("tul avg: "); Serial.println(ringBufferAvg().z);
  return ringBufferAvg().z > -2.0;
}

void loadable() {
  Serial.println("loadable");
}

bool transitionLoadableRunning() {
  
}

void running() {
  
}

bool transitionRunningClean() {
  
}

//temp
bool transitionLoadableUnloading() {
  Serial.print("tlu avg: "); Serial.println(ringBufferAvg().z);
  return ringBufferAvg().z < -2.0;
}

void setup() {
  Serial.begin(9600);
  initializeMma();
  ringBuffer.clear();
  stateMachine.transitionTo(Loadable);

  //Clean->addTransition(&transitionCleanUnloading, Unloading);
  Unloading->addTransition(&transitionUnloadingLoadable, Loadable);
  //Loadable->addTransition(&transitionLoadableRunning, Unloading);
  Loadable->addTransition(&transitionLoadableUnloading, Unloading); 
  //Running->addTransition(&transitionRunningClean, Clean);
}

void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  sampleData(event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print(" Y: "); Serial.print(event.acceleration.y);
  Serial.print(" Z: "); Serial.println(event.acceleration.z);
  Serial.println();
  stateMachine.run();
  delay(SAMPLE_PERIOD_MS);
}

void initializeMma() {
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");  
}

void sampleData(sensors_event_t event) {
  sensors_vec_t data;
  if (ringBuffer.isFull())
    ringBuffer.pop(data);
  ringBuffer.push(event.acceleration);
  //printRingBuffer();  
}

void printRingBuffer() {
  const int size = ringBuffer.size();
  for (int i = 0; i < size; i++) {
    //Serial.print(ringBuffer[i]);
    //Serial.print(" ");
  }
  Serial.println();
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

float ringBufferZVariance() {
  //float min = ringBufferZMin();
  //float max = ringBufferZMax();
  //return max - min;
}
