//platform/esp32/main_esp32.cpp
extern "C" {
    #include "cyan_os.h"
    #include "console/cyan_console.h"
}
#include <Arduino.h>
#include "esp32_hardware.h"

static unsigned long lastTime = 0;
// True once cyan_init() has brought up the full OS (display, clay, apps, ...). When hardware
// (MCP23017/display/SD/etc.) isn't connected, init degrades gracefully rather than failing, so
// this should end up true in practice - but if something unexpected does fail, we still fall back
// to console-only mode below instead of hanging, since cyan_console_init() runs first thing
// inside cyan_init() and shell access shouldn't depend on the rest of the OS coming up.
static bool cyanReady = false;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.setDebugOutput(true);
    setvbuf(stdout, NULL, _IONBF, 0);

    esp32_hardware_init();
    cyanReady = cyan_init();
    if (!cyanReady) {
        Serial.println("cyan_init() failed - continuing in console-only mode");
    }
    lastTime = millis();
}

void loop() {
    unsigned long now = millis();
    float dt = (float) (now - lastTime) / 1000.0f;
    lastTime = now;

    if (cyanReady) {
        bool running = true;
        cyan_update(dt, &running);
    } else {
        cyan_console_poll();
        delay(10);
    }
}