void initializeStateMachine() {
  stateMachine.transitionTo(Loadable);

  //Clean->addTransition(&transitionCleanUnloading, Unloading);
  Unloading->addTransition(&transitionUnloadingLoadable, Loadable);
  //Loadable->addTransition(&transitionLoadableRunning, Unloading);
  Loadable->addTransition(&transitionLoadableUnloading, Unloading);
  //Running->addTransition(&transitionRunningClean, Clean);
}

void clean() {

}

bool transitionCleanUnloading() {

}

void unloading() {
  Serial.println("unloading");
}

bool transitionUnloadingLoadable() {
  return abs(ringBufferAvg().z) < 2.0;
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
  return abs(ringBufferAvg().z) > 2.0;
}
