#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#ifdef CONFIG_CAMERA_SENSOR_ENABLED
#include <esp_camera.h>

#include <camera_pins.hpp>

namespace LDM {
class Camera : public Sensor{
public:
    Camera(framesize_t frame_size=FRAMESIZE_SVGA, pixformat_t pixel_format=PIXFORMAT_JPEG);
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

private:
    // camera config
    camera_config_t camera_config;

    // camera frame buffer
    camera_fb_t* fb;

    // jpeg buffer
    size_t jpg_buf_len;
    uint8_t* jpg_buf;

    // float temperature;
    // float humidity;
    // int pressure;
    // int gas;
    //
    // bme680_t sensor;
    // uint32_t duration;
    //
    // static const gpio_num_t scl_gpio = static_cast<gpio_num_t>(I2C_SCL);
    // static const gpio_num_t sda_gpio = static_cast<gpio_num_t>(I2C_SDA);
    // static const gpio_num_t i2c_port_num = static_cast<gpio_num_t>(I2C_PORT);
};
}


#endif // CONFIG_CAMERA_SENSOR_ENABLED

#endif // __CAMERA_HPP__
