#ifdef CONFIG_CAMERA_SENSOR_ENABLED

#define TAG "Camera"

#include <camera_pins.hpp>
#include <camera.hpp>

LDM::Camera::Camera(pixformat_t resolution, pixformat_t pixel_format) {
    this->camera_config = {
        .pin_pwdn = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        .pin_sscb_sda = CAM_PIN_SIOD,
        .pin_sscb_scl = CAM_PIN_SIOC,

        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,
        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,

        //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = pixel_format, //YUV422,GRAYSCALE,RGB565,JPEG
        .frame_size = frame_size,     //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

        .jpeg_quality = 12, //0-63 lower number means higher quality
        .fb_count = 2       //if more than one, i2s runs in continuous mode. Use only with JPEG
    };

#if CONFIG_CAMERA_MODEL_ESP_EYE || CONFIG_CAMERA_MODEL_ESP32_CAM_BOARD
    /* IO13, IO14 is designed for JTAG by default,
     * to use it as generalized input,
     * firstly declare it as pullup input */
    gpio_config_t conf;
    conf.mode = GPIO_MODE_INPUT;
    conf.pull_up_en = GPIO_PULLUP_ENABLE;
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.intr_type = GPIO_INTR_DISABLE;
    conf.pin_bit_mask = 1LL << 13;
    gpio_config(&conf);
    conf.pin_bit_mask = 1LL << 14;
    gpio_config(&conf);
#endif
}

esp_err_t init(void) {
    //power up the camera if PWDN pin is defined
    if(CAM_PIN_PWDN != -1) {
        pinMode(CAM_PIN_PWDN, OUTPUT);
        digitalWrite(CAM_PIN_PWDN, LOW);
    }

    // initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Initialization Failed");
    }

    return err;
}

esp_err_t readSensor(void) {
    this->fb = NULL;

    //acquire a frame
    this->fb = esp_camera_fb_get();
    if(!fb) {
        ESP_LOGE(TAG, "Camera Capture Failed");
        return ESP_FAIL;
    }

    // convert frame to jpeg
    if(fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(this->fb, 80, &this->jpg_buf, &this->jpg_buf_len);
        if(!jpeg_converted) {
            ESP_LOGE(TAG, "JPEG Compression Failed");
            esp_camera_fb_return(fb);
            return ESP_FAIL;
        }
    } else {
        this->jpg_buf_len = fb->len;
        this->jpg_buf = fb->buf;
    }

    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(this->fb);

    return ESP_OK;
}

esp_err_t deinit(void) {
    esp_err_t err = esp_camera_deinit();

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Unable to deinitialize Camera Driver");
        if(err == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Camera Driver has not been initialized");
        }
    }

    return err;
}
/*
    Camera(pixformat_t resolution=PIXFORMAT_JPEG);
    // BME680();
    // float getHumidity(void);
    // float getTemperature(void);
    // float getPressure(void);
    // float getGas(void);
    // void setHumidity(float humidity);
    // void setTemperature(float temperature);
    // void setPressure(float pressure);
    // void setGas(float gas);

    esp_err_t init(void);
    esp_err_t deinit(void);
    esp_err_t readSensor(void);
    cJSON *buildJson(void);
*/

#endif // CONFIG_CAMERA_SENSOR_ENABLED
