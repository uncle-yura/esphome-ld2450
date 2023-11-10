import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    DEVICE_CLASS_EMPTY,
    UNIT_DEGREES,
    ICON_ROTATE_RIGHT,
    ENTITY_CATEGORY_CONFIG,
    CONF_INITIAL_VALUE,
    CONF_RESTORE_VALUE,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
    CONF_ENTITY_CATEGORY,
    DEVICE_CLASS_DISTANCE,
    CONF_ID,
    ICON_ACCELERATION_X,
    ICON_ACCELERATION_Y,
    UNIT_CENTIMETER,
    UNIT_SECOND,
    CONF_UNIT_OF_MEASUREMENT
)
from .. import LD2450, ld2450_ns, PresenceRegion

from ..const import (CONF_ROTATE, CONF_PRESENCE_TIMEOUT, CONF_PRESENCE_REGIONS,
    CONF_ENTRY_POINTS, CONF_X0, CONF_Y0, CONF_X1, CONF_Y1, CONF_X, CONF_Y,
    CONF_LD2450_ID)

NUMBERS = [CONF_X0, CONF_Y0, CONF_X1, CONF_Y1]

DEPENDENCIES = ["ld2450"]

RotateNumber = ld2450_ns.class_(
    "RotateNumber",
    number.Number,
    cg.Component,
    cg.Parented.template(LD2450)
)
PresenceTimeoutNumber = ld2450_ns.class_(
    "PresenceTimeoutNumber",
    number.Number,
    cg.Component,
    cg.Parented.template(LD2450)
)
RegionNumber = ld2450_ns.class_(
    "RegionNumber",
    number.Number,
    cg.Parented.template(LD2450)
)
EntryPoint = ld2450_ns.class_("EntryPoint")

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_LD2450_ID): cv.use_id(LD2450),
    cv.Optional(CONF_ROTATE): number.NUMBER_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(RotateNumber),
            cv.Optional(
                CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG
            ): cv.entity_category,
            cv.Optional(CONF_INITIAL_VALUE, default=0): cv.positive_float,
            cv.Optional(CONF_MAX_VALUE, default=360): cv.positive_float,
            cv.Optional(CONF_MIN_VALUE, default=0): cv.positive_float,
            cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
            cv.Optional(CONF_STEP, default=0.1): cv.positive_float,
            cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_DEGREES): cv.string_strict
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.Optional(CONF_PRESENCE_TIMEOUT): number.NUMBER_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PresenceTimeoutNumber),
            cv.Optional(
                CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG
            ): cv.entity_category,
            cv.Optional(CONF_INITIAL_VALUE, default=0): cv.positive_float,
            cv.Optional(CONF_MAX_VALUE, default=6000): cv.positive_float,
            cv.Optional(CONF_MIN_VALUE, default=0): cv.positive_float,
            cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
            cv.Optional(CONF_STEP, default=0.1): cv.positive_float,
            cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_SECOND): cv.string_strict
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.Optional(CONF_ENTRY_POINTS): cv.ensure_list(
        cv.All(
            cv.Schema(
                {
                    cv.GenerateID(): cv.declare_id(EntryPoint),
                    cv.Required(CONF_X): cv.float_,
                    cv.Required(CONF_Y): cv.float_
                }
            )
        )
    ),
    cv.Optional(CONF_PRESENCE_REGIONS): cv.ensure_list(
        cv.All(
            cv.Schema(
                {
                    cv.GenerateID(): cv.declare_id(PresenceRegion),
                    cv.Required(CONF_X0): cv.float_,
                    cv.Required(CONF_Y0): cv.float_,
                    cv.Required(CONF_X1): cv.float_,
                    cv.Required(CONF_Y1): cv.float_
                }
            )
        )
    )
})

CONFIG_SCHEMA = CONFIG_SCHEMA.extend(
    {
        cv.Optional(f"region_{x}"): cv.Schema(
            {
                cv.Required(CONF_X0): number.number_schema(
                    RegionNumber,
                    device_class=DEVICE_CLASS_DISTANCE,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_ACCELERATION_X,
                    unit_of_measurement=UNIT_CENTIMETER
                ),
                cv.Required(CONF_Y0): number.number_schema(
                    RegionNumber,
                    device_class=DEVICE_CLASS_DISTANCE,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_ACCELERATION_Y,
                    unit_of_measurement=UNIT_CENTIMETER
                ),
                cv.Required(CONF_X1): number.number_schema(
                    RegionNumber,
                    device_class=DEVICE_CLASS_DISTANCE,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_ACCELERATION_X,
                    unit_of_measurement=UNIT_CENTIMETER
                ),
                cv.Required(CONF_Y1): number.number_schema(
                    RegionNumber,
                    device_class=DEVICE_CLASS_DISTANCE,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_ACCELERATION_Y,
                    unit_of_measurement=UNIT_CENTIMETER
                )
            }
        )
        for x in range(3)
    }
)


async def to_code(config):
    ld2450 = await cg.get_variable(config[CONF_LD2450_ID])
    if rotate_config := config.get(CONF_ROTATE):
        var = await number.new_number(
            rotate_config,
            min_value=rotate_config[CONF_MIN_VALUE],
            max_value=rotate_config[CONF_MAX_VALUE],
            step=rotate_config[CONF_STEP],
        )
        await cg.register_component(var, rotate_config)
        cg.add(var.set_initial_value(rotate_config[CONF_INITIAL_VALUE]))
        cg.add(var.set_restore_value(rotate_config[CONF_RESTORE_VALUE]))

        hub = await cg.get_variable(config[CONF_LD2450_ID])
        func = getattr(hub, f"set_rotate_number")
        cg.add(func(var))
        cg.add(var.set_parent(hub))
    
    if presence_timeout_config := config.get(CONF_PRESENCE_TIMEOUT):
        var = await number.new_number(
            presence_timeout_config,
            min_value=presence_timeout_config[CONF_MIN_VALUE],
            max_value=presence_timeout_config[CONF_MAX_VALUE],
            step=presence_timeout_config[CONF_STEP],
        )
        await cg.register_component(var, presence_timeout_config)
        cg.add(var.set_initial_value(presence_timeout_config[CONF_INITIAL_VALUE]))
        cg.add(var.set_restore_value(presence_timeout_config[CONF_RESTORE_VALUE]))

        hub = await cg.get_variable(config[CONF_LD2450_ID])
        func = getattr(hub, f"set_presence_timeout_number")
        cg.add(func(var))
        cg.add(var.set_parent(hub))

    if entry_points := config.get(CONF_ENTRY_POINTS):
        for entry_point in entry_points:
            n = cg.new_Pvariable(
                entry_point[CONF_ID],
                entry_point[CONF_X],
                entry_point[CONF_Y]
            )
            cg.add(ld2450.add_entry_point(n))

    if presence_regions := config.get(CONF_PRESENCE_REGIONS):
        for presence_region in presence_regions:
            n = cg.new_Pvariable(
                presence_region[CONF_ID],
                presence_region[CONF_X0],
                presence_region[CONF_Y0],
                presence_region[CONF_X1],
                presence_region[CONF_Y1]
            )
            cg.add(ld2450.add_presence_region(n))

    for x in range(3):
        if region_config := config.get(f"region_{x}"):
            for i, coordinate in enumerate(NUMBERS):
                coord_config = region_config[coordinate]
                n = cg.new_Pvariable(coord_config[CONF_ID], x)
                await number.register_number(
                    n, coord_config, min_value=-1000, max_value=1000, step=1
                )
                await cg.register_parented(n, config[CONF_LD2450_ID])
                cg.add(ld2450.set_region_number(x, i, n))
