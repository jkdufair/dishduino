#include <RingBuf.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>


Adafruit_MMA8451 mma = Adafruit_MMA8451();

// 30 sec of z axis samples
#define RING_BUFFER_SIZE 30
RingBuf<float, RING_BUFFER_SIZE> ringBuffer;

void setup() {
  Serial.begin(9600);
  initializeMma();
  ringBuffer.clear();
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
  delay(1000);
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
  printRingBuffer();  
}

void printRingBuffer() {
  const int size = ringBuffer.size();
  for (int i = 0; i < size; i++) {
    Serial.print(ringBuffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}
