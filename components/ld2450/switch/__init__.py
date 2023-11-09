import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_SWITCH,
    ICON_BLUETOOTH,
    ENTITY_CATEGORY_CONFIG,
    ICON_PULSE,
    CONF_INITIAL_VALUE,
    CONF_RESTORE_VALUE
)
from .. import LD2450, ld2450_ns
from ..const import CONF_SINGLE_TARGET, CONF_BLUETOOTH, CONF_LD2450_ID

BluetoothSwitch = ld2450_ns.class_("BluetoothSwitch",
    switch.Switch,
    cg.Component,
    cg.Parented.template(LD2450)
)
SingleTargetSwitch = ld2450_ns.class_("SingleTargetSwitch",
    switch.Switch,
    cg.Parented.template(LD2450)
)

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_LD2450_ID): cv.use_id(LD2450),
    cv.Optional(CONF_SINGLE_TARGET): switch.switch_schema(
        SingleTargetSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_PULSE,
    ),
    cv.Optional(CONF_BLUETOOTH): switch.switch_schema(
        BluetoothSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_BLUETOOTH,
    ).extend({
        cv.Optional(CONF_INITIAL_VALUE, default=True): cv.boolean,
        cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
    }),
}


async def to_code(config):
    ld2450 = await cg.get_variable(config[CONF_LD2450_ID])
    if single_target_config := config.get(CONF_SINGLE_TARGET):
        s = await switch.new_switch(single_target_config)
        await cg.register_parented(s, config[CONF_LD2450_ID])
        cg.add(ld2450.set_single_target_switch(s))
    if bluetooth_config := config.get(CONF_BLUETOOTH):
        s = await switch.new_switch(bluetooth_config)
        await cg.register_component(s, bluetooth_config)
        cg.add(s.set_initial_value(bluetooth_config[CONF_INITIAL_VALUE]))
        cg.add(s.set_restore_value(bluetooth_config[CONF_RESTORE_VALUE]))

        hub = await cg.get_variable(config[CONF_LD2450_ID])
        cg.add(s.set_parent(hub))
