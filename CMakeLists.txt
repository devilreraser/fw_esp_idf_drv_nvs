idf_component_register(SRCS "drv_nvs.c" "cmd_nvs.c"
                    INCLUDE_DIRS "." 
                    REQUIRES "nvs_flash" "console"
                                      )
                 

