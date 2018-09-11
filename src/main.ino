#include <BlynkSimpleParticle.h>

#include "env.h"
#include "MAX17043.h"

MAX17043 lipo;

void setup() {
    Blynk.begin(ENV_BLYNK_AUTH);

    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    lipo = MAX17043::getInstance();
    lipo.initialize();

    delay(1000);
}

void loop() {
    if (Particle.connected()) {
        Blynk.run();
        
        float voltage = lipo.getVoltage();
        float charge = lipo.getStateOfCharge();

        Particle.publish("voltage", String(voltage));
        Particle.publish("charge", String(charge));

        Blynk.virtualWrite(V0, voltage);
        Blynk.virtualWrite(V1, charge);

        delay(5000);

        System.sleep(SLEEP_MODE_DEEP, 600);
    }
}