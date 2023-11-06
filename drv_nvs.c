/* *****************************************************************************
 * File:   drv_nvs.c
 * Author: XX
 *
 * Created on YYYY MM DD
 * 
 * Description: ...
 * 
 **************************************************************************** */

/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "drv_nvs.h"
#include "cmd_nvs.h"

#include <sdkconfig.h>

#include "nvs_flash.h"

/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */
#define TAG "drv_nvs"

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

/* *****************************************************************************
 * Prototype of functions definitions
 **************************************************************************** */

/* *****************************************************************************
 * Functions
 **************************************************************************** */
void drv_nvs_init(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    cmd_nvs_register();
}

void drv_nvs_reset(void)
{
    esp_err_t ret;
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
    ESP_ERROR_CHECK(ret);
}

esp_err_t drv_nvs_read_u32 ( char* partitionName, char* variableName, uint32_t* pnValue)
{
    esp_err_t eError;
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) %s... ", partitionName);
    nvs_handle_t my_handle;
    eError = nvs_open(partitionName, NVS_READONLY, &my_handle);
    if (eError != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(eError));
    } 
    else 
    {
        printf("Done\n");
        // Read
        printf("Reading %s from NVS ... ", variableName);
        uint32_t u32Value = *pnValue; // value will use *pnValue, if not set yet in NVS
        eError = nvs_get_u32(my_handle, variableName, &u32Value);
        switch (eError) {
            case ESP_OK:
                printf("Done\n");
                printf("%s = %u\n", variableName,  (unsigned int)u32Value);
                *pnValue = u32Value;
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(eError));
        }

        // Close
        nvs_close(my_handle);
    }
    return eError;
}


esp_err_t drv_nvs_write_u32 ( char* partitionName, char* variableName, uint32_t u32InputValue )
{
    esp_err_t eError;
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) %s... ", partitionName);
    nvs_handle_t my_handle;
    eError = nvs_open(partitionName, NVS_READWRITE, &my_handle);
    if (eError != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(eError));
    } 
    else 
    {
        printf("Done\n");

        // Write
        printf("Updating %s in NVS ... ",variableName);
        uint32_t u32Value = u32InputValue; // value will use defaultValue, if not set yet in NVS
        eError = nvs_set_u32(my_handle, variableName, u32Value);
        printf((eError != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        eError = nvs_commit(my_handle);
        printf((eError != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
    return eError;
}

esp_err_t drv_nvs_read_string ( char* partitionName, char* variableName, char* pString, size_t nMaxSize)
{
    esp_err_t eError;
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) %s... ", partitionName);
    nvs_handle_t my_handle;
    eError = nvs_open(partitionName, NVS_READONLY, &my_handle);
    if (eError != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(eError));
    } 
    else 
    {
        printf("Done\n");
        // Read
        printf("Reading %s from NVS ... ", variableName);
        eError = nvs_get_str(my_handle, variableName, pString, &nMaxSize);
        switch (eError) {
            case ESP_OK:
                printf("Done\n");
                printf("%s = %s\n", variableName, pString);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(eError));
        }

        // Close
        nvs_close(my_handle);
    }
    return eError;
}


esp_err_t drv_nvs_write_string ( char* partitionName, char* variableName, char* pString )
{
    esp_err_t eError;
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) %s... ", partitionName);
    nvs_handle_t my_handle;
    eError = nvs_open(partitionName, NVS_READWRITE, &my_handle);
    if (eError != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(eError));
    } 
    else 
    {
        printf("Done\n");

        // Write
        printf("Updating %s in NVS ... ", variableName);
        eError = nvs_set_str(my_handle, variableName, pString);
        printf((eError != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        eError = nvs_commit(my_handle);
        printf((eError != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
    return eError;
}

