#include <mbed.h>
#include "zigbee_on_off_light.h"
#include "../zboss/endpoints/zigbee_on_off_light_implementation.h"

ZigbeeOnOffOutput::ZigbeeOnOffOutput(WriteStateCallback state_callback) : ZigbeeOnOffOutput("On Off Light v1", state_callback)
{
}

ZigbeeOnOffOutput::ZigbeeOnOffOutput(const char model_id[], WriteStateCallback state_callback) : ZigbeeOnOffOutput("On Off Light v1", ZigbeeEndpoint::SourceType::kDcSource, state_callback, nullptr)
{
}

ZigbeeOnOffOutput::ZigbeeOnOffOutput(
    const char model_id[],
    ZigbeeEndpoint::SourceType power_source_type,
    WriteStateCallback state_callback,
    ZigbeeEndpoint::IdentifyCallback identify_callback
    ) : 
    ZigbeeEndpoint(
        new ZigbeeOnOffOutputImplementation(
            this, 
            model_id, 
            static_cast<unsigned int>(power_source_type)
            ), 
        identify_callback),
    m_write_state(state_callback)
{
}

ZigbeeOnOffOutput::~ZigbeeOnOffOutput() {}

void ZigbeeOnOffOutput::setState(bool value)
{
    implementation()->setState(value);
}

ZigbeeOnOffOutputImplementation *ZigbeeOnOffOutput::implementation() const
{
    return static_cast<ZigbeeOnOffOutputImplementation *>(m_impl.get());
}