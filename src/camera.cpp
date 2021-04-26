#include <esp_log.h>
#include <esp_err.h>
#include <cJSON.h>
#include <cstring>

#define TAG "Camera"

#if CONFIG_CAMERA_SENSOR_ENABLED
#include <mbedtls/base64.h>

#include <camera_pins.hpp>
#include <camera.hpp>
#include <esp_camera.h>

LDM::Camera::Camera(framesize_t resolution,
                    pixformat_t pixel_format,
                    uint8_t jpeg_quality,
                    uint8_t fb_count,
                    int xclk_freq
                  ) {
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
        // .xclk_freq_hz = 20000000,
        .xclk_freq_hz = xclk_freq,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = pixel_format, //YUV422,GRAYSCALE,RGB565,JPEG
        .frame_size = resolution,     //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

        .jpeg_quality = jpeg_quality, //0-63 lower number means higher quality
        .fb_count = fb_count       //if more than one, i2s runs in continuous mode. Use only with JPEG
    };

#if CONFIG_CAMERA_MODEL_ESP_EYE || CONFIG_CAMERA_MODEL_ESP32_CAM_BOARD
    // IO13, IO14 is designed for JTAG by default,
    // to use it as generalized input,
    // firstly declare it as pullup input
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

    // encoded_json_ref = NULL;
}

esp_err_t LDM::Camera::init(void) {
    //power up the camera if PWDN pin is defined
    // if(CAM_PIN_PWDN != -1) {
    //     pinMode(CAM_PIN_PWDN, OUTPUT);
    //     digitalWrite(CAM_PIN_PWDN, LOW);
    // }

    this->encoded_buffer = NULL;
    this->fb = NULL;

    // initialize the camera
    esp_err_t err = esp_camera_init(&this->camera_config);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Initialization Failed");
    }

    this->initialized = err;
    return err;
}

esp_err_t LDM::Camera::readSensor(void) {
    if(this->fb != NULL) {
        this->releaseFrameBuffer();
        this->fb = NULL;
    }

    //acquire a frame
    this->fb = esp_camera_fb_get();
    if(!fb) {
        ESP_LOGE(TAG, "Camera Capture Failed");
        return ESP_FAIL;
    }

    // convert frame to jpeg
    if(fb->format != PIXFORMAT_JPEG) {
        this->width = fb->width;
        this->height = fb->height;
        bool jpeg_converted = frame2jpg(this->fb, 80, &this->jpg_buf, &this->jpg_buf_len);
        if(!jpeg_converted) {
            ESP_LOGE(TAG, "JPEG Compression Failed");
            esp_camera_fb_return(this->fb);
            this->fb = NULL;
            return ESP_FAIL;
        }
    } else {
        this->jpg_buf_len = fb->len;
        this->jpg_buf = fb->buf;
        this->width = fb->width;
        this->height = fb->height;
    }

    // TODO check if this invalidates buffer (resort to using releaseFrameBuffer)
    //return the frame buffer back to the driver for reuse
    // esp_camera_fb_return(this->fb);

    return ESP_OK;
}

esp_err_t LDM::Camera::releaseData(void) {
    this->releaseFrameBuffer();
    this->fb = NULL;

    if(this->encoded_buffer != NULL) {
        free(this->encoded_buffer);
    }
    this->encoded_buffer = NULL;

    return ESP_OK;
}

camera_fb_t * LDM::Camera::getFrameBuffer(void) {
    return this->fb;
}

uint8_t * LDM::Camera::getJpgBuffer(void) {
    return this->jpg_buf;
}

size_t LDM::Camera::getJpgBufferLength(void) {
    return this->jpg_buf_len;
}

void LDM::Camera::releaseFrameBuffer(void) {
    if(this->fb != NULL) {
        esp_camera_fb_return(this->fb);
    }
}

esp_err_t LDM::Camera::deinit(void) {
    this->releaseFrameBuffer();
    this->fb = NULL;

    if(this->encoded_buffer != NULL) {
        free(this->encoded_buffer);
    }
    this->encoded_buffer = NULL;

    esp_err_t err = esp_camera_deinit();

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Unable to deinitialize Camera Driver");
        if(err == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Camera Driver has not been initialized");
        }
    }

    return err;
}

char* LDM::Camera::encodeString(size_t output_length, size_t offset, size_t padding) {
      // size_t output_length = 0;
      int err;
      if(output_length == 0) {
          // fetch size to allocate
          err = mbedtls_base64_encode(
                                  NULL,
                                  0,
                                  &output_length,
                                  this->jpg_buf,
                                  this->jpg_buf_len
                              );

          ESP_LOGI(TAG, "Encoding Image of Buffer Size: %u", this->jpg_buf_len);
          ESP_LOGI(TAG, "Image Encoding Required Buffer Size: %u", output_length);
      }

      // allocate memory
      if(this->encoded_buffer != NULL) {
          free(this->encoded_buffer);
          this->encoded_buffer = NULL;
      }
      this->encoded_buffer = (uint8_t*)malloc(sizeof(uint8_t)*output_length);

      // populate buffer and add offset if provided
      err = mbedtls_base64_encode(
                              this->encoded_buffer,
                              output_length,
                              &output_length,
                              this->jpg_buf,
                              this->jpg_buf_len
                          );

      // std::string str_buff(reinterpret_cast<char*>(this->encoded_buffer));

      // check base64 encoder
      if(err == MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
          ESP_LOGE(TAG, "Error Encoding Buffer: Buffer Too Small : %u bytes written", output_length);
      } else if(err) {
          ESP_LOGE(TAG, "Error Encoding Buffer: %d : %u bytes written", err, output_length);
      } else {
          ESP_LOGI(TAG, "Successfully Encoded Buffer: %u bytes written, string length: %u",
                      output_length, strlen(reinterpret_cast<char*>(this->encoded_buffer)));//str_buff.length());
      }

      // return str_buff.c_str();
      return (char*)this->encoded_buffer;
}

cJSON* LDM::Camera::buildJson(void) {
    const char* encode_str = encodeString();
    // printf("Encode String: %s\n\n", encode_str);

    // ESP_LOGI(TAG, "Encoding String: String Length: %u\n%s", str_buff.length(), str_buff.c_str());

    cJSON *root = cJSON_CreateObject();
    // cJSON_AddItemToObject(root, "camera1", cJSON_CreateString(str_buff.substr(0, 10000).c_str()));
    // cJSON_AddItemToObject(root, "camera2", cJSON_CreateString(str_buff.substr(10000+1).c_str()));
    cJSON_AddItemToObject(root, "camera", cJSON_CreateString(encode_str));
    // cJSON_AddItemReferenceToObject(root, "camera", cJSON_CreateStringReference(encode_str));

    // using cJSON_AddItemToObject with cJSON_Print() seems to cause a NULL response
    // when creating a string with a large buffer, workaround seems to be to form
    // JSON object by first generating the JSON sting and then parsing it
    // Heap space is very limited, so free allocated memory back whenever possible!

    // // get encoded string
    // const char* encode_str = encodeString(); // same as this->encoded_buffer
    // size_t data_buffer_size = strlen(encode_str);
    //
    // // if(encoded_json_ref != NULL) {
    // //     cJSON_Delete(encoded_json_ref);
    // //     encoded_json_ref = NULL;
    // // }
    // // encoded_json_ref = cJSON_CreateStringReference(encode_str);
    // //
    // // return encoded_json_ref;
    //
    // // allocate and populate buffer
    // char *post_data_buffer = (char*)malloc(sizeof(char) * (data_buffer_size+256));
    // snprintf(post_data_buffer, data_buffer_size+256, "{ \"camera\": \"%s\"}", encode_str);
    // // printf("%s\n", post_data_buffer);
    //
    // free encoded buffer from heap
    free(this->encoded_buffer);
    this->encoded_buffer = NULL;
    //
    // // form cjson response
    // cJSON *root = cJSON_Parse(post_data_buffer);
    //
    // // free post data buffer from heap
    // free(post_data_buffer);

    return root;
}

uint8_t * LDM::Camera::getEncodedBuffer(void) {
    return this->encoded_buffer;
}

// char* LDM::Camera::getSensorName(void) {
// #define STR(_x) #_x
//     return "CAM" const_cast<char*>(STR(CAM_BOARD));
// #undef STR

const char* LDM::Camera::getSensorName(void) {
    return "Camera";
}

sensor_t * LDM::Camera::getSensor(void) {
    return esp_camera_sensor_get();
}

#endif // CONFIG_CAMERA_SENSOR_ENABLED
