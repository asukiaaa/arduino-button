#include <button_asukiaaa.h>

#define PIN_BUTTON 39  // m5stack button a
#define TIMES_TO_DETECT 2
#define MS_TO_DETECT_LESS_THAN 1000UL

button_asukiaaa::Button btn(PIN_BUTTON);
button_asukiaaa::MultiTimesPressDetector multiTimesPress(
    TIMES_TO_DETECT, MS_TO_DETECT_LESS_THAN);

void setup() {
  Serial.begin(115200);
  btn.begin();
}

void loop() {
  btn.update();
  multiTimesPress.updateByBtnState(btn);
  if (multiTimesPress.detectAndResetIfTrue()) {
    Serial.println("detect " + String(multiTimesPress.getTimesPress()) +
                   " times press less than " +
                   String(multiTimesPress.getMsLessThan()) + "ms at " +
                   String(millis()));
  }
  delay(10);
}
