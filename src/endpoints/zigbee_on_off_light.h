#ifndef ZIGBEE_ON_OFF_LIGHT_H__
#define ZIGBEE_ON_OFF_LIGHT_H__

#include "zigbee_endpoint.h"

class ZigbeeOnOffOutputImplementation;

class ZigbeeOnOffOutput : public ZigbeeEndpoint
{
public:
    typedef void (*WriteStateCallback)(const bool state);

    ZigbeeOnOffOutput(WriteStateCallback state_callback);
    ZigbeeOnOffOutput(
        const char model_id[],
        WriteStateCallback state_callback
    );
    ZigbeeOnOffOutput(
        const char model_id[],
        ZigbeeEndpoint::SourceType power_source_type,
        WriteStateCallback state_callback,
        ZigbeeEndpoint::IdentifyCallback identify_callback
    );
    ~ZigbeeOnOffOutput();

    /**
     * @brief Switch the lamp ON or OFF
     * @param[in] value = true or false
     */
    void setState(bool value);

    ZigbeeOnOffOutputImplementation *implementation() const;

private:
    const WriteStateCallback m_write_state;

    friend class ZigbeeOnOffOutputImplementation;
};

#endif // ZIGBEE_ON_OFF_LIGHT_H__
