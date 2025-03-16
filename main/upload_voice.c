#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
// #include "nvs_flash.h"
#include "sys/socket.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "netdb.h"
#include "driver/i2s.h"
#include "arpa/inet.h"

#define TAG "app"
#define HOST_IP_ADDR "192.168.2.153"
#define HOST_PORT 8888
#define bufferLen 512

int16_t sBuffer[512];
 
static const char *payload = "Hello from ESP32\r\n";
static TaskHandle_t client_task_handle;
 
 

static void udp_client_task(void *args)
{
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(HOST_PORT);
 
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, HOST_PORT);
 
    while (1) {
        size_t bytesIn = 0;
        esp_err_t result = i2s_read(I2S_NUM_0, &sBuffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);
        if (result == ESP_OK && bytesIn > 0) {
            int err = sendto(sock, (uint8_t*)sBuffer, bytesIn, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                ESP_LOGI(TAG, "Hono Message not sent");
            }
            ESP_LOGI(TAG, "Message sent");
        }
        else{
            ESP_LOGI(TAG, "Hono Message not sent");
        }

 
        // c
    }
 
    close(sock);
    vTaskDelete(NULL);
}
 
static void wifi_event_handler(void* arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void* event_data)
{
    if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, &client_task_handle);
        }
    } else if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            vTaskDelete(client_task_handle);
        } else if (event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        }
    }
}
 

void wifi_init_sta(void)
{
    /* 初始化WiFi协议栈 */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
 
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
 
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
 
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
 
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Stark-5G",
            .password = "fengzhiping,1101",
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
        },
    };
 
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

