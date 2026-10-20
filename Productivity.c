#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_config.h"
#include "lwip/tcp.h"
#include <string.h>

#define BUTTON_PIN 15
#define LED_PIN 14
err_t on_connected(void *arg, struct tcp_pcb *tpcb,err_t er){
    const char *request = 
    "POST /DAILY HTTP/1.1\r\n"
    "Host: 10.60.76.23:5000\r\n"
    "Content-Length: 0\r\n"
     "Connection: close\r\n"
     "\r\n";
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    return ERR_OK;

}

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
    ip_addr_t server_ip;
    ip4addr_aton("10.60.76.23", &server_ip);
    while (true)
    {
        bool current_button_state = gpio_get(BUTTON_PIN);

        if (last_button_state == true && current_button_state == false)
        {
            printf("button pressed\n");
            led_state = !led_state;
            gpio_put(LED_PIN, led_state);
            printf("led_state is now: %d\n", led_state);
            struct tcp_pcb *pcb = tcp_new();           
            cyw43_arch_lwip_begin();
            tcp_connect(pcb, &server_ip,5000,on_connected);
            cyw43_arch_lwip_end();
        }
        last_button_state = current_button_state;
        sleep_ms(100);
    }
}
