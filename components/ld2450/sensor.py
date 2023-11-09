import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_TYPE,
    CONF_NUMBER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CENTIMETER,
    ICON_ACCELERATION_Y,
    ICON_ACCELERATION_X,
    CONF_ICON,
    CONF_UNIT_OF_MEASUREMENT,
    ICON_GAUGE,
    ICON_RULER,
    UNIT_SECOND,
    ICON_ACCOUNT
)
from . import LD2450
from .const import CONF_LD2450_ID

DEPENDENCIES = ["ld2450"]

from .const import CONF_TARGET_COUNT, CONF_POSITION_X, CONF_POSITION_Y, CONF_SPEED, CONF_RESOLUTION

SENSORS = [CONF_POSITION_X, CONF_POSITION_Y, CONF_SPEED, CONF_RESOLUTION]

UNIT_MM = "mm"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_LD2450_ID): cv.use_id(LD2450),
        cv.Optional(CONF_TARGET_COUNT): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_ACCOUNT,
        )
    }
)

CONFIG_SCHEMA = CONFIG_SCHEMA.extend(
    {
        cv.Optional(f"person_{x}"): cv.Schema(
            {
                cv.Optional(CONF_POSITION_X): sensor.sensor_schema(
                    accuracy_decimals=1,
                    state_class=STATE_CLASS_MEASUREMENT,
                    icon=ICON_ACCELERATION_X,
                    unit_of_measurement=UNIT_CENTIMETER
                ),
                cv.Optional(CONF_POSITION_Y): sensor.sensor_schema(
                    accuracy_decimals=1,
                    state_class=STATE_CLASS_MEASUREMENT,
                    icon=ICON_ACCELERATION_Y,
                    unit_of_measurement=UNIT_CENTIMETER
                ),
                cv.Optional(CONF_SPEED): sensor.sensor_schema(
                    accuracy_decimals=0,
                    state_class=STATE_CLASS_MEASUREMENT,
                    icon=ICON_GAUGE,
                    unit_of_measurement=UNIT_CENTIMETER + "/" + UNIT_SECOND
                ),
                cv.Optional(CONF_RESOLUTION): sensor.sensor_schema(
                    accuracy_decimals=0,
                    state_class=STATE_CLASS_MEASUREMENT,
                    icon=ICON_RULER,
                    unit_of_measurement=UNIT_MM
                )
            }
        )
        for x in range(3)
    }
)

async def to_code(config):
    hub = await cg.get_variable(config[CONF_LD2450_ID])
    for x in range(3):
        if person_config := config.get(f"person_{x}"):
            for sensor_type in SENSORS:
                if sensor_config := person_config.get(sensor_type):
                    var = await sensor.new_sensor(sensor_config)
                    func = getattr(hub, f"set_{sensor_type}_{x}_sensor")
                    cg.add(func(var))

    if target_count_config := config.get(CONF_TARGET_COUNT):
        var = await sensor.new_sensor(target_count_config)
        func = getattr(hub, f"set_{CONF_TARGET_COUNT}_sensor")
        cg.add(func(var))
