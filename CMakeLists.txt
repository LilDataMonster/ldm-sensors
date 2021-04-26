if(CONFIG_CAMERA_SENSOR_ENABLED)
    set(camera_dep esp32-camera)
else()
    set(camera_dep "")
endif()

idf_component_register(
    SRC_DIRS src
    INCLUDE_DIRS include
    REQUIRES dht
             bme680
             json
             mbedtls
             i2cdev
             esp32-camera
             ${camera_dep}
)
