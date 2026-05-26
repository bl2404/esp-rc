idf_component_register(SRCS "motor.c"
                       INCLUDE_DIRS "include"
                       REQUIRES driver esp-idf-dshot esp_timer my_crsf)