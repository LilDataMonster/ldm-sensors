#ifndef __CAM_HPP__
#define __CAM_HPP__

#include <cJSON.h>
#include <esp_err.h>

#include <sensor.hpp>

#if CONFIG_CAMERA_SENSOR_ENABLED
#include <esp_camera.h>

namespace LDM {
class Camera : public Sensor {
public:
    Camera(
      framesize_t frame_size=FRAMESIZE_SVGA,
      pixformat_t pixel_format=PIXFORMAT_JPEG,
      uint8_t jpeg_quality=12,
      uint8_t fb_count=2,
      int xclk_freq=20000000
    );

    size_t getWidth(void);
    size_t getHeight(void);

    esp_err_t init(void);
    esp_err_t deinit(void);
    esp_err_t readSensor(void);
    const char* getSensorName(void);
    cJSON *buildJson(void);

    camera_fb_t * getFrameBuffer(void);
    uint8_t * getJpgBuffer(void);
    size_t getJpgBufferLength(void);
    esp_err_t releaseData(void);

    // allocates a char* which must be freed by caller
    char *encodeString(size_t output_length=0, size_t offset=0, size_t padding=0);

    void releaseFrameBuffer(void);
    uint8_t * getEncodedBuffer(void);

    // get esp32-camera sensor struct
    sensor_t * getSensor(void);

private:
    // camera config
    camera_config_t camera_config;

    // camera frame buffer
    camera_fb_t* fb;

    // jpeg buffer
    size_t jpg_buf_len;
    uint8_t* jpg_buf;

    // frame info
    size_t width;
    size_t height;

    // JSON buffer
    uint8_t *encoded_buffer;
    // cJSON * encoded_json_ref;
};
}


#endif // CONFIG_CAMERA_SENSOR_ENABLED

#endif // __CAMERA_HPP__
