#include <StateMachine.h>
#include <RingBuf.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//TODO: Any time vibrating, put in RUNNING mode

#define SAMPLE_PERIOD_MS 1000
#define RING_BUFFER_SIZE 10 // seconds of samples

RingBuf<sensors_vec_t, RING_BUFFER_SIZE> ringBuffer;
StateMachine stateMachine = StateMachine();
Adafruit_MMA8451 mma = Adafruit_MMA8451();

State* Clean = stateMachine.addState(&clean);
State* Unloading = stateMachine.addState(&unloading);
State* Loadable = stateMachine.addState(&loadable);
State* Running = stateMachine.addState(&running);

void setup() {
  Serial.begin(9600);
  initializeMma();
	initializeRingBuffer();
	initializeStateMachine();
}

void loop() {
  sensors_event_t event;
  mma.getEvent(&event);
  sampleData(event);

  /* Display the results (acceleration is measured in m/s^2) */
  /*
	Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print(" Y: "); Serial.print(event.acceleration.y);
  Serial.print(" Z: "); Serial.println(event.acceleration.z);
  Serial.println();
	*/

  stateMachine.run();
  delay(SAMPLE_PERIOD_MS);
}

void initializeMma() {
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  mma.setRange(MMA8451_RANGE_2_G);
}
