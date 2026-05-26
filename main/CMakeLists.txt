idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES ESP_CRSF
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES my_motor my_servo my_crsf
                       REQUIRES driver)
