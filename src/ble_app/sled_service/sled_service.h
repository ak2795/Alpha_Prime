#ifndef SLED_SERVICE_H__
#define SLED_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_SS_DEF(_name)                                 \
static ble_ss_t_name;                                     \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                       \
                     BLE_LBS_BLE_OBSERVER_PRIO,           \
                     ble_lbs_onBle_evt, &_name)
          
#define SS_UUID_BASE         {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define SS_UUID_SERVICE      0xABCD

/**
 * @brief This structure contains various status information for our service. 
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs. 
 * 'ble’ indicates that it is a Bluetooth Low Energy relevant structure and 
 * ‘ss’ is short for Sled Service). 
 */

typedef struct ble_ss_s ble_ss_t;

struct ble_ss_s
{
    uint16_t service_handle;  /**< Handle of Sled Service (as provided by the BLE stack). */
    uint8_t  uuid_type;
};

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_sled_service       Pointer to Sled Service structure.
 */
void sled_service_init(ble_ss_t * p_sled_service);

#endif /* _SLED_SERVICE_H__ */