/* *****************************************************************************
 * File:   cmd_nvs.c
 * Author: Dimitar Lilov
 *
 * Created on 2022 06 18
 * 
 * Description: ...
 * 
 **************************************************************************** */

/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "cmd_nvs.h"

#include "drv_nvs.h"

#include <string.h>

#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"

#include "argtable3/argtable3.h"

/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */
#define TAG "cmd_nvs"

/* *****************************************************************************
 * Constants and Macros Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Enumeration Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Type Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Function-Like Macros
 **************************************************************************** */

/* *****************************************************************************
 * Variables Definitions
 **************************************************************************** */

static struct {
    struct arg_str *command;
    struct arg_end *end;
} nvs_args;

char null_string_nvs[] = "";

/* *****************************************************************************
 * Prototype of functions definitions
 **************************************************************************** */

/* *****************************************************************************
 * Functions
 **************************************************************************** */
static int update_nvs(int argc, char **argv)
{
    ESP_LOGI(__func__, "argc=%d", argc);
    for (int i = 0; i < argc; i++)
    {
        ESP_LOGI(__func__, "argv[%d]=%s", i, argv[i]);
    }

    int nerrors = arg_parse(argc, argv, (void **)&nvs_args);
    if (nerrors != ESP_OK)
    {
        arg_print_errors(stderr, nvs_args.end, argv[0]);
        return ESP_FAIL;
    }

    if (strcmp(nvs_args.command->sval[0],"erase") == 0)
    {
        ESP_LOGI(TAG, "NVS Reset/Erase");
        drv_nvs_reset();
    }



    return 0;
}

static void register_nvs(void)
{
    nvs_args.command = arg_strn(NULL, NULL, "<command>", 0, 1, "Command can be : nvs {erase}");
    nvs_args.end = arg_end(1);

    const esp_console_cmd_t cmd_nvs = {
        .command = "nvs",
        .help = "Nvs Settings Update Request",
        .hint = NULL,
        .func = &update_nvs,
        .argtable = &nvs_args,
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_nvs));
}


void cmd_nvs_register(void)
{
    register_nvs();
}
