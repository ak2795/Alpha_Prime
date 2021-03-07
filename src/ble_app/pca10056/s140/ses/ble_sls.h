#ifndef BLE_SLS
#define BLE_SLS

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_SLS_DEF(_name)                                                        \
static ble_sls_t _name;                                                           \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                               \
                    BLE_HRS_BLE_OBSERVER_PRIO,                                    \
                    ble_sls_on_ble_evt, &_name)

#define SLED_SERVICE_UUID_BASE  {0x1C, 0x6B, 0x5D, 0x0F, 0x48, 0x14, 0x26, 0xA2,  \
                                0x7F, 0x4F, 0xCF, 0x0F, 0x0E, 0xD0, 0x3C, 0x82}

#define SLED_SERVICE_UUID       0x1400
#define SLED_VALUE_CHAR_UUID    0x1401

typedef enum
{
    BLE_SLS_EVT_NOTIFICATION_ENABLED,
    BLE_SLS_EVT_NOTIFICATION_DISABLED,
    BLE_SLS_EVT_DISCONNECTED,
    BLE_SLS_EVT_CONNECTED
} ble_sls_evt_type_t;

// Forward declaration of the ble_sls_t type
typedef struct ble_sls_s ble_sls_t;

/**@brief Sled Service event */
typedef struct
{
    ble_sls_evt_type_t evt_type;
} ble_sls_evt_t;

/**@brief Sled Service event handler type. */
typedef void (*ble_sls_evt_handler_t) (ble_sls_t * p_sls, ble_sls_evt_t * p_evt);

/**@brief Sled Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
  ble_sls_evt_handler_t         evt_handler;                  /**< Event handler to be called for handling events in Sled Service */
  uint8_t                       initial_sled_value;           /**< Initial sled value */
  ble_srv_cccd_security_mode_t  sled_value_char_attr_md;      /**< Initial security level for Sled characteristics attribute */
} ble_sls_init_t;

/**@brief Sled Service structure. This contains various status information for the service. */
struct ble_sls_s
{
  ble_sls_evt_handler_t     evt_handler;
  uint16_t                  service_handle;         /**< Handle of Sled Service (as provided by the BLE stack) */
  ble_gatts_char_handles_t  sled_value_handles;     /**< Handles related to the Sled Value characteristic */
  uint16_t                  conn_handle;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection) */
  uint8_t                   uuid_type;
};



/**@brief Function for initializing the Sled Service.
 *
 * @param[out]  p_cus       Sled Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_cus_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_sls_init(ble_sls_t * p_sls, const ble_sls_init_t * p_sls_init);

/**@brief Function for adding the Sled Value characteristic.
 *
 * @param[in]   p_sls        Sled Service structure.
 * @param[in]   p_sls_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t sled_value_char_add(ble_sls_t * p_sls, const ble_sls_init_t * p_sls_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Sled Service structure.
 */
void ble_sls_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_sls       Sled Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt);

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_sls       Sled Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt);

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_sls       Sled Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt);

/**@brief Function for updating the sled value.
 *
 * @details The application calls this function when the cutom value should be updated. If
 *          notification has been enabled, the sled value characteristic is sent to the client.
 *
 * @note 
 *       
 * @param[in]   p_sls          Sled Service structure.
 * @param[in]   Sled value 
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_sls_sled_value_update(ble_sls_t * p_sls, uint64_t sled_value);


#endif