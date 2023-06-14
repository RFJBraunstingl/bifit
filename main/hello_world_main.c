/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include <esp_log.h>
#include <esp_system.h>

// #define BIFIT_CONFIG_ENABLE_DEBUG_LOGGING

#include "bifit.h"

void print_heap_usage() {
        //printf("esp_get_minimum_free_heap_size: %d bytes\n", esp_get_minimum_free_heap_size());
        printf("heap_caps_get_free_size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
}

void periodically_print_heap_usage() {
        const esp_timer_create_args_t timer_args = {
                .callback = &print_heap_usage,
                .name = "heap-ticker"
        };

        esp_timer_handle_t heap_tick_timer;
        esp_err_t result = esp_timer_create(&timer_args, &heap_tick_timer);
        if (ESP_OK != result) {
                ESP_LOGE("main", "failed to register timer! %s", esp_err_to_name(result));
                return;
        }

        ESP_LOGI("main", "heap ticker created!");

        const uint64_t INTERVALL_IN_US = 10 * 1000;
        result = esp_timer_start_periodic(heap_tick_timer, INTERVALL_IN_US);
        if (ESP_OK != result) {
                ESP_LOGE("main", "error starting heap ticker! %s", esp_err_to_name(result));
                return;
        }

        ESP_LOGI("main", "heap ticker started!");
}

void create_bifit_task() {

    ESP_LOGI("main", "running bifit...");
    
    TaskHandle_t taskHandle = NULL;
    xTaskCreate(bifit_run, "bifit main task", 4096, NULL, tskIDLE_PRIORITY, &taskHandle);
}

void app_main(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    print_heap_usage();
    periodically_print_heap_usage();

    create_bifit_task();

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();
}
