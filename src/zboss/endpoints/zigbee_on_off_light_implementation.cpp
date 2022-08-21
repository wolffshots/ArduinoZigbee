#include <mbed.h>
#include "zigbee_on_off_light_implementation.h"
#include "../../endpoints/zigbee_on_off_light.h"

#define interface() static_cast<ZigbeeOnOffOutput *>(m_interface)

ZigbeeOnOffOutputImplementation::ZigbeeOnOffOutputImplementation(ZigbeeOnOffOutput *interface, const zb_char_t model_id[], unsigned int power_source_type) : ZigbeeEndpointImplementation(interface, model_id, power_source_type)
{
    /* WARNING: do not use the interface object inside this constructor because it is not fully constructed. */
    memset(&m_zboss_data, 0, sizeof(m_zboss_data));

    /* On/Off cluster attributes data */
    m_zboss_data.on_off_attr.on_off = (zb_bool_t)ZB_ZCL_ON_OFF_IS_ON;

    m_zboss_data.cluster_revision_scenes_attr_list = 1;
    zb_zcl_attr_t scenes_attr_list[] = {
        {0xfffdU, 0x21U, 0x01U, (void *)&(m_zboss_data.cluster_revision_scenes_attr_list)},
        {ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID, 0x20U, 0x01U, (void *)(&m_zboss_data.scenes_attr.scene_count)},
        {ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID, 0x20U, 0x01U, (void *)(&m_zboss_data.scenes_attr.current_scene)},
        {ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID, 0x21U, 0x01U, (void *)(&m_zboss_data.scenes_attr.current_group)},
        {ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID, 0x10U, 0x01U, (void *)(&m_zboss_data.scenes_attr.scene_valid)},
        {ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID, 0x18U, 0x01U, (void *)(&m_zboss_data.scenes_attr.name_support)},
        {(zb_uint16_t)(-1), 0, 0, __null}};

    static_assert(sizeof(m_zboss_data.scenes_attr_list) == sizeof(scenes_attr_list), "Check vector size");
    memcpy(m_zboss_data.scenes_attr_list, scenes_attr_list, sizeof(scenes_attr_list));

    m_zboss_data.cluster_revision_groups_attr_list = 1;
    zb_zcl_attr_t groups_attr_list[] = {
        {0xfffdU, 0x21U, 0x01U, (void *)&(m_zboss_data.cluster_revision_groups_attr_list)},
        {ZB_ZCL_ATTR_GROUPS_NAME_SUPPORT_ID, 0x18U, 0x01U, (void *)(&m_zboss_data.groups_attr.name_support)},
        {(zb_uint16_t)(-1), 0, 0, __null}};

    static_assert(sizeof(m_zboss_data.groups_attr_list) == sizeof(groups_attr_list), "Check vector size");
    memcpy(m_zboss_data.groups_attr_list, groups_attr_list, sizeof(groups_attr_list));

    m_zboss_data.cluster_revision_on_off_attr_list = 1;
    zb_zcl_attr_t on_off_attr_list[] = {
        {0xfffdU, 0x21U, 0x01U, (void *)&(m_zboss_data.cluster_revision_on_off_attr_list)},
        {ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID, 0x10U, 0x01U | 0x04U | 0x10U, (void *)(&m_zboss_data.on_off_attr.on_off)},
        {ZB_ZCL_ATTR_ON_OFF_GLOBAL_SCENE_CONTROL, 0x10U, 0x01U, (void *)(&m_zboss_data.on_off_attr.global_scene_ctrl)},
        {ZB_ZCL_ATTR_ON_OFF_ON_TIME, 0x21U, (0x01U | 0x02U), (void *)(&m_zboss_data.on_off_attr.on_time)},
        {ZB_ZCL_ATTR_ON_OFF_OFF_WAIT_TIME, 0x21U, (0x01U | 0x02U), (void *)(&m_zboss_data.on_off_attr.off_wait_time)},
        {(zb_uint16_t)(-1), 0, 0, __null}};
    static_assert(sizeof(m_zboss_data.on_off_attr_list) == sizeof(on_off_attr_list), "Check vector size");
    memcpy(m_zboss_data.on_off_attr_list, on_off_attr_list, sizeof(on_off_attr_list));

    ZB_HA_DECLARE_ON_OFF_OUTPUT_CLUSTER_LIST(clusters, m_zboss_data.on_off_attr_list, m_zboss_basic_data.basic_attr_list, m_zboss_basic_data.identify_attr_list, m_zboss_data.groups_attr_list, m_zboss_data.scenes_attr_list);
    static_assert(sizeof(m_zboss_data.clusters) == sizeof(clusters), "Check vector size");
    memcpy(m_zboss_data.clusters, clusters, sizeof(clusters));

    m_zboss_data.simple_desc = {
        m_endpoint_id,
        0x0104U,
        ZB_HA_ON_OFF_LIGHT_DEVICE_ID,
        1,
        0,
        5,
        0,
        {
            0x0003U, 
            0x0000U, 
            0x0006U,
            0x0005U, 
            0x0004U
        }
    };

    m_zboss_data.ep =  {
        m_endpoint_id,
        0x0104U,
        __null,
        __null,
        0,
        (void *)__null,
        (sizeof(m_zboss_data.clusters) / sizeof(zb_zcl_cluster_desc_t)),
        m_zboss_data.clusters,
        (zb_af_simple_desc_1_1_t *)&m_zboss_data.simple_desc,
        (1 + 1),
        m_zboss_data.reporting_info,
        1,
        m_zboss_data.cvc_alarm_info
    };

    setDescriptor(&m_zboss_data.ep);
}

zb_uint8_t ZigbeeOnOffOutputImplementation::processCommandEP(zb_bufid_t bufid, zb_zcl_parsed_hdr_t *cmd_params)
{
    if (cmd_params->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV &&
        !cmd_params->is_common_command &&
        cmd_params->cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF &&
        cmd_params->cmd_id == ZB_ZCL_CMD_ON_OFF_OFF_WITH_EFFECT_ID)
    {
        setState((zb_bool_t) false);
        return ZB_TRUE;
    }
    return ZB_FALSE;
}

zb_ret_t ZigbeeOnOffOutputImplementation::processCommandDV(zb_zcl_device_callback_param_t *cmd_params)
{
    switch (cmd_params->device_cb_id)
    {
    case ZB_ZCL_SET_ATTR_VALUE_CB_ID:
        // Inform user about attribute value modification
        {
            zb_uint8_t result = setAttribute(&cmd_params->cb_param.set_attr_value_param);
            return (result == ZB_TRUE) ? RET_OK : RET_ERROR;
        }
    case ZB_ZCL_IDENTIFY_EFFECT_CB_ID:
        onIdentify(&cmd_params->cb_param.identify_effect_value_param);
        return RET_OK;
    default:
        return RET_ERROR;
    }
}

void ZigbeeOnOffOutputImplementation::begin(bool load_from_memory)
{
    m_started = true;
    ZB_ZCL_SET_ATTRIBUTE(m_endpoint_id,
                         ZB_ZCL_CLUSTER_ID_ON_OFF,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                         &m_zboss_data.on_off_attr.on_off,
                         ZB_FALSE);
    interface()->m_write_state(m_zboss_data.on_off_attr.on_off ? 1 : 0);
}

void ZigbeeOnOffOutputImplementation::end()
{
    m_started = false;
}

void ZigbeeOnOffOutputImplementation::setState(zb_bool_t value)
{
    if (m_started)
    {
        ZB_ZCL_SET_ATTRIBUTE(m_endpoint_id,
                             ZB_ZCL_CLUSTER_ID_ON_OFF,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                             (zb_uint8_t *)&value,
                             ZB_FALSE);
    }
    else
    {
        m_zboss_data.on_off_attr.on_off = value;
    }
    interface()->m_write_state(m_zboss_data.on_off_attr.on_off ? 1 : 0);
}

zb_uint8_t ZigbeeOnOffOutputImplementation::setAttribute(zb_zcl_set_attr_value_param_t *attr_p)
{
    switch (attr_p->cluster_id)
    {
    case ZB_ZCL_CLUSTER_ID_ON_OFF:
        if (attr_p->attr_id == ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID)
        {
            // ON - OFF management
            setState((zb_bool_t)attr_p->values.data8);
            return ZB_TRUE;
        }
        break;
    default:;
    }
    return ZB_FALSE;
}