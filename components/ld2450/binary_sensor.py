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
from . import CONF_LD2450_ID, LD2450

DEPENDENCIES = ["ld2450"]
CONF_HAS_TARGET = "has_target"
CONF_HAS_MOVING_TARGET = "has_moving_target"
CONF_HAS_STILL_TARGET = "has_still_target"

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
