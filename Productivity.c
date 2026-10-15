#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_config.h"

#define BUTTON_PIN 15
#define LED_PIN 14

int main(){
    stdio_init_all();
    sleep_ms(2000);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    bool last_button_state = true;
    bool led_state = false;

if(cyw43_arch_init()){
    printf("Wifi chip failed to start\n");
    return -1;
}
cyw43_arch_enable_sta_mode();
if(cyw43_arch_wifi_connect_timeout_ms(
    WIFI_SSID , WIFI_PASSWORD,
    CYW43_AUTH_WPA2_AES_PSK,10000)){
    printf("Wifi connection failed\n");
    return - 1;
    }
    printf("Wifi connection success!\n");

    while (true)
    {
        bool current_button_state = gpio_get(BUTTON_PIN);

        if (last_button_state == true && current_button_state == false)
        {
            printf("button pressed\n");
            led_state = !led_state;
            gpio_put(LED_PIN, led_state);
        }
        last_button_state = current_button_state;
        sleep_ms(100);
    }
}
