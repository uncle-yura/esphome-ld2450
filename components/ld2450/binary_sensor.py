import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_MOTION,
    DEVICE_CLASS_OCCUPANCY,
    DEVICE_CLASS_PRESENCE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_MOTION_SENSOR,
    ICON_ACCOUNT,
)
from . import LD2450, ld2450_ns, PresenceRegion
from .const import (CONF_HAS_TARGET, CONF_HAS_MOVING_TARGET,
    CONF_HAS_STILL_TARGET, CONF_PRESENCE, CONF_REGION_ID,
    CONF_PRESENCE_REGIONS, CONF_LD2450_ID)

DEPENDENCIES = ["ld2450"]

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_LD2450_ID): cv.use_id(LD2450),
    cv.Optional(CONF_HAS_TARGET): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_OCCUPANCY,
        icon=ICON_ACCOUNT,
    ),
    cv.Optional(CONF_HAS_MOVING_TARGET): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_MOTION,
        icon=ICON_MOTION_SENSOR,
    ),
    cv.Optional(CONF_HAS_STILL_TARGET): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_OCCUPANCY,
        icon=ICON_MOTION_SENSOR,
    ),
    cv.Optional(CONF_PRESENCE_REGIONS): cv.ensure_list(
        binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PRESENCE, icon=ICON_MOTION_SENSOR
        ).extend({
            cv.Required(CONF_REGION_ID): cv.use_id(PresenceRegion),
        }),
    )
}

async def to_code(config):
    hub = await cg.get_variable(config[CONF_LD2450_ID])
    if has_target_config := config.get(CONF_HAS_TARGET):
        sens = await binary_sensor.new_binary_sensor(has_target_config)
        cg.add(hub.set_target_binary_sensor(sens))
    if has_moving_target_config := config.get(CONF_HAS_MOVING_TARGET):
        sens = await binary_sensor.new_binary_sensor(has_moving_target_config)
        cg.add(hub.set_moving_target_binary_sensor(sens))
    if has_still_target_config := config.get(CONF_HAS_STILL_TARGET):
        sens = await binary_sensor.new_binary_sensor(has_still_target_config)
        cg.add(hub.set_still_target_binary_sensor(sens))
    if regions := config.get(CONF_PRESENCE_REGIONS):
        for region in regions:
            n = await cg.get_variable(region[CONF_REGION_ID])
            sens = await binary_sensor.new_binary_sensor(region)
            cg.add(n.set_presence_binary_sensor(sens))
