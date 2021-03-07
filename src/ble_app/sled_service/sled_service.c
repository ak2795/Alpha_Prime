#include <stdint.h>
#include <string.h>
#include "sled_service.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"

/**@brief Function for initiating our new service.
 *
 * @param[in]   p_sled_service        Sled Service structure.
 *
 */
void sled_service_init(ble_ss_t * p_sled_service)
{
    // Declare 16 bit service and 128 bit base UUIDs and add them to BLE stack table     
    uint32_t                  err_code;
    ble_uuid_t                service_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_SLED_BASE_UUID;
    service_uuid.uuid =       BLE_UUID_SLED_SERVICE;
    
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    // STEP 4: Add our service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_sled_service->service_handle);
    APP_ERROR_CHECK(err_code);
}