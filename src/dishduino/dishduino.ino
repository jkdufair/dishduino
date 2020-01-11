#include <StateMachine.h>
#include <RingBuf.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

#define SAMPLE_PERIOD_MS 1000
#define RING_BUFFER_SIZE 10 // seconds of samples
RingBuf<float, RING_BUFFER_SIZE> ringBuffer;

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
  return ringBufferAvg() > -2.0;
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
  return ringBufferAvg() < -2.0;
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
  //Serial.print("X: "); Serial.print(event.acceleration.x);
  //Serial.print(" Y: "); Serial.print(event.acceleration.y);
  //Serial.print(" Z: "); Serial.println(event.acceleration.z);
  //Serial.println();
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
  float data;
  if (ringBuffer.isFull())
    ringBuffer.pop(data);
  ringBuffer.push(event.acceleration.z);
  //printRingBuffer();  
}

void printRingBuffer() {
  const int size = ringBuffer.size();
  for (int i = 0; i < size; i++) {
    Serial.print(ringBuffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}

float ringBufferAvg() {
  const int size = ringBuffer.size();
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += ringBuffer[i];
  }
  return sum/size;
}

float ringBufferMin() {
  const int size = ringBuffer.size();
  float min = 10.0;
    for (int i = 0; i < size; i++) {
    if (ringBuffer[i] < min) {
      min = ringBuffer[i];
    }
  }
  return min;
}

float ringBufferMax() {
  const int size = ringBuffer.size();
  float max = -10.0;
    for (int i = 0; i < size; i++) {
    if (ringBuffer[i] > max) {
      max = ringBuffer[i];
    }
  }
  return max;  
}

float ringBufferVariance() {
  float min = ringBufferMin();
  float max = ringBufferMax();
  return max - min;
}
