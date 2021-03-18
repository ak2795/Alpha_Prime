#include "sdk_common.h"
#include "ble_srv_common.h"
#include "ble_sls.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"
#include <string.h>

uint32_t ble_sls_init(ble_sls_t * p_sls, const ble_sls_init_t * p_sls_init)
{
  if (p_sls == NULL || p_sls_init == NULL)
  {
    return NRF_ERROR_NULL;
  }

  uint32_t    err_code;
  ble_uuid_t  ble_uuid;

  // Initialize the service structure
  p_sls->evt_handler = p_sls_init->evt_handler;
  p_sls->conn_handle = BLE_CONN_HANDLE_INVALID;
  
  // Add Sled Service UUID
  ble_uuid128_t base_uuid = {SLED_SERVICE_UUID_BASE};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_sls->uuid_type);
  VERIFY_SUCCESS(err_code);

  ble_uuid.type = p_sls->uuid_type;
  ble_uuid.uuid = SLED_SERVICE_UUID;

  // Add the Sled Service
  err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_sls->service_handle);
  if (err_code != NRF_SUCCESS)
  {
    return err_code;
  }

  p_sls->char_pwm_value_write_handler = p_sls_init->char_pwm_value_write_handler;

  // Add Sled Value characteristic
  err_code = sled_value_char_add(p_sls, p_sls_init);
  err_code = sled_pwm_char_add(p_sls, p_sls_init);

  return err_code;
}

/**@brief Function for adding the Custom Value characteristic.
 *
 * @param[in]   p_sls        Sled Service structure.
 * @param[in]   p_sls_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t sled_value_char_add(ble_sls_t * p_sls, const ble_sls_init_t * p_sls_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

   
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL; 

    memset(&cccd_md, 0, sizeof(cccd_md));

    // Read operation on Cccd should be possible without authentication.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.char_props.notify = 1;
    char_md.p_cccd_md         = &cccd_md;


    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_sls_init->sled_value_char_attr_md.read_perm;
    attr_md.write_perm = p_sls_init->sled_value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_sls->uuid_type;
    ble_uuid.uuid = SLED_VALUE_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint64_t);  // Size of characteristic

    err_code = sd_ble_gatts_characteristic_add(p_sls->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_sls->sled_value_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    return NRF_SUCCESS;
}

static uint32_t sled_pwm_char_add(ble_sls_t * p_sls, const ble_sls_init_t * p_sls_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

   
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL; 

    memset(&cccd_md, 0, sizeof(cccd_md));

    // Read operation on Cccd should be possible without authentication.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.char_props.notify = 1;
    char_md.p_cccd_md         = &cccd_md;


    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_sls_init->sled_value_char_attr_md.read_perm;
    attr_md.write_perm = p_sls_init->sled_value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_sls->uuid_type;
    ble_uuid.uuid = SLED_PWM_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint32_t);  // Size of characteristic

    err_code = sd_ble_gatts_characteristic_add(p_sls->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_sls->sled_pwm_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    return NRF_SUCCESS;
}


void ble_sls_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_sls_t * p_sls = (ble_sls_t *) p_context;

    if (p_sls == NULL || p_ble_evt == NULL)
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_sls, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_sls, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_sls, p_ble_evt);
            break;

        default:
            break;
    }
}


static void on_connect(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt)
{
    p_sls->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    ble_sls_evt_t evt;
    evt.evt_type = BLE_SLS_EVT_CONNECTED;
    p_sls->evt_handler(p_sls, & evt);
}


static void on_disconnect(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_sls->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_write(ble_sls_t * p_sls, ble_evt_t const * p_ble_evt)
{
     NRF_LOG_INFO("on_write: called");
     ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

     if (p_evt_write->handle == p_sls->sled_pwm_handles.value_handle)
    {
      int8_t len = p_evt_write->len;
      NRF_LOG_INFO("Write Happened!");
      //if (len != 4)
      //{
      //  NRF_LOG_INFO("ERROR: incomplete package");
      //  NRF_LOG_INFO("len: %d", len);
      //  return;
      //}
      // Data must be sent from in Little Endian Format and 4 bytes
      // Convert the little endian 4 bytes of data into 32 bit unsigned int
      uint32_t * char_pwm_value_adr;
      uint32_t char_pwm_value_val;
      char_pwm_value_adr = (uint32_t *) p_evt_write->data;
      char_pwm_value_val = *p_evt_write->data;

      NRF_LOG_INFO("PWM Value: %d", char_pwm_value_val);
      p_sls->char_pwm_value_write_handler(char_pwm_value_val);
    }

    // Check if the handle passed with the event matches the Custom Value Characteristic handle.
    if ((p_evt_write->handle == p_sls->sled_value_handles.cccd_handle)
        && (p_evt_write->len == 2))
    {
        
        // CCCD written, call application event handler
        if (p_sls->evt_handler != NULL)
        {
            ble_sls_evt_t evt;

            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_SLS_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_SLS_EVT_NOTIFICATION_DISABLED;
            }
            // Call the application event handler.
            p_sls->evt_handler(p_sls, &evt);
        }
    }

    
   
}



uint32_t ble_sls_sled_value_update(ble_sls_t * p_sls, uint64_t sled_value)
{
    if (p_sls == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = sizeof(uint64_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = (uint8_t*) &sled_value;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_sls->conn_handle,
                                      p_sls->sled_value_handles.value_handle,
                                      &gatts_value);
   
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_sls->conn_handle != BLE_CONN_HANDLE_INVALID))
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_sls->sled_value_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_sls->conn_handle, &hvx_params);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}
