#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "leds.hpp"
//#include "ble.hpp"

extern "C" void app_main() 
{
    while(true)
        vTaskDelay(pdMS_TO_TICKS(16));
}
