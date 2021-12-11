void sysRestart() {
  Serial.println("[cmd][restart] Restarting...");
  ESP.restart();
}

void sysSleep() {
  Serial.println("[cmd][sleep] Going to sleep...");
  ESP.deepSleep(0);
}