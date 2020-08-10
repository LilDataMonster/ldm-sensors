#ifndef __CAM_PINS_HPP__
#define __CAM_PINS_HPP__

// #ifdef CONFIG_CAMERA_SENSOR_ENABLED
// #include <esp_camera.h>
// // #include <esp_http_server.h>
// // #include <esp_timer.h>
// #endif

#ifdef CONFIG_CAMERA_MODEL_HUZZAH32
#define CAM_BOARD       "HUZZAH32"
#define CAM_PIN_PWDN    -1 //power down is not used
#define CAM_PIN_RESET   -1 //software reset will be performed
#define CAM_PIN_XCLK    21
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

// #define CAM_PIN_D7      35
#define CAM_PIN_D7      33
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      19
#define CAM_PIN_D2      18
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

#elif CONFIG_CAMERA_MODEL_WROVER_KIT
#define CAM_BOARD       "WROVER-KIT"
#define CAM_PIN_PWDN    -1 //power down is not used
#define CAM_PIN_RESET   -1 //software reset will be performed
#define CAM_PIN_XCLK    21
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      19
#define CAM_PIN_D2      18
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

#elif CONFIG_CAMERA_MODEL_ESP_EYE
#define CAM_BOARD       "ESP-EYE"
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    4
#define CAM_PIN_SIOD    18
#define CAM_PIN_SIOC    23

#define CAM_PIN_D7      36
#define CAM_PIN_D6      37
#define CAM_PIN_D5      38
#define CAM_PIN_D4      39
#define CAM_PIN_D3      35
#define CAM_PIN_D2      14
#define CAM_PIN_D1      13
#define CAM_PIN_D0      34
#define CAM_PIN_VSYNC   5
#define CAM_PIN_HREF    27
#define CAM_PIN_PCLK    25

#elif CONFIG_CAMERA_MODEL_M5STACK_PSRAM
#define CAM_BOARD       "M5CAM"
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   15
#define CAM_PIN_XCLK    27
#define CAM_PIN_SIOD    25
#define CAM_PIN_SIOC    23

#define CAM_PIN_D7      19
#define CAM_PIN_D6      36
#define CAM_PIN_D5      18
#define CAM_PIN_D4      39
#define CAM_PIN_D3       5
#define CAM_PIN_D2      34
#define CAM_PIN_D1      35
#define CAM_PIN_D0      32
#define CAM_PIN_VSYNC   22
#define CAM_PIN_HREF    26
#define CAM_PIN_PCLK    21

#elif CONFIG_CAMERA_MODEL_M5STACK_WIDE
#define CAM_BOARD       "M5CAMW"
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   15
#define CAM_PIN_XCLK    27
#define CAM_PIN_SIOD    22
#define CAM_PIN_SIOC    23

#define CAM_PIN_D7      19
#define CAM_PIN_D6      36
#define CAM_PIN_D5      18
#define CAM_PIN_D4      39
#define CAM_PIN_D3       5
#define CAM_PIN_D2      34
#define CAM_PIN_D1      35
#define CAM_PIN_D0      32
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    26
#define CAM_PIN_PCLK    21

#elif CONFIG_CAMERA_MODEL_AI_THINKER
#define CAM_BOARD       "AI-THINKER"
#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK     0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0       5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

// TODO: rendered as strings?
#elif CONFIG_CAMERA_MODEL_CUSTOM
#define CAM_BOARD       "CUSTOM"
#define CAM_PIN_PWDN    CONFIG_CAM_PIN_PWDN
#define CAM_PIN_RESET   CONFIG_CAM_PIN_RESET
#define CAM_PIN_XCLK    CONFIG_CAM_PIN_XCLK
#define CAM_PIN_SIOD    CONFIG_CAM_PIN_SIOD
#define CAM_PIN_SIOC    CONFIG_CAM_PIN_SIOC

#define CAM_PIN_D7      CONFIG_CAM_PIN_D7
#define CAM_PIN_D6      CONFIG_CAM_PIN_D6
#define CAM_PIN_D5      CONFIG_CAM_PIN_D5
#define CAM_PIN_D4      CONFIG_CAM_PIN_D4
#define CAM_PIN_D3      CONFIG_CAM_PIN_D3
#define CAM_PIN_D2      CONFIG_CAM_PIN_D2
#define CAM_PIN_D1      CONFIG_CAM_PIN_D1
#define CAM_PIN_D0      CONFIG_CAM_PIN_D0
#define CAM_PIN_VSYNC   CONFIG_CAM_PIN_VSYNC
#define CAM_PIN_HREF    CONFIG_CAM_PIN_HREF
#define CAM_PIN_PCLK    CONFIG_CAM_PIN_PCLK

#endif

#endif // __CAM_PINS_HPP__
