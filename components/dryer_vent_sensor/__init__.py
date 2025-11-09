import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    UNIT_EMPTY,
)

DEPENDENCIES = ['esp32']
AUTO_LOAD = ['sensor']

dryer_vent_ns = cg.esphome_ns.namespace('dryer_vent_sensor')
DryerVentSensor = dryer_vent_ns.class_('DryerVentSensor', cg.PollingComponent)

CONF_OVERHEAT_SENSOR = "overheat_sensor"
CONF_CLOG_SENSOR = "clog_sensor"
CONF_SELFTEST_SENSOR = "selftest_sensor"
CONF_SHORT_PACKET_SENSOR = "short_packet_sensor"
CONF_SHORT_START_PACKET = "short_start_packet"
CONF_LONG_START_PACKET = "long_start_packet"
CONF_SHORT_CLOG_PACKET = "short_clog_packet"
CONF_LONG_CLOG_PACKET = "long_clog_packet"
CONF_SHORT_OVERHEAT_PACKET = "short_overheat_packet"
CONF_LONG_OVERHEAT_PACKET = "long_overheat_packet"
CONF_UNKNOWN_PACKET_SENSOR = "unknown_packet_sensor"
CONF_SELFTEST_COUNTER = "selftest_counter"
CONF_COUNT_PIN = "count_pin"
CONF_TEST_PIN = "test_pin"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DryerVentSensor),
    cv.Required(CONF_COUNT_PIN): cv.int_,
    cv.Required(CONF_TEST_PIN): cv.int_,
    cv.Optional(CONF_OVERHEAT_SENSOR): sensor.sensor_schema(),
    cv.Optional(CONF_CLOG_SENSOR): sensor.sensor_schema(),
    cv.Optional(CONF_SELFTEST_SENSOR): sensor.sensor_schema(),
    cv.Optional(CONF_SHORT_PACKET_SENSOR): sensor.sensor_schema(),
    cv.Optional(CONF_SHORT_START_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_LONG_START_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_SHORT_CLOG_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_LONG_CLOG_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_SHORT_OVERHEAT_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_LONG_OVERHEAT_PACKET): sensor.sensor_schema(),
    cv.Optional(CONF_UNKNOWN_PACKET_SENSOR): sensor.sensor_schema(),
    cv.Optional(CONF_SELFTEST_COUNTER): sensor.sensor_schema(),
}).extend(cv.polling_component_schema('15s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    cg.add(var.set_count_pin(config[CONF_COUNT_PIN]))
    cg.add(var.set_test_pin(config[CONF_TEST_PIN]))

    if CONF_OVERHEAT_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_OVERHEAT_SENSOR])
        cg.add(var.set_overheat_sensor(sens))
    if CONF_CLOG_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_CLOG_SENSOR])
        cg.add(var.set_clog_sensor(sens))
    if CONF_SELFTEST_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_SELFTEST_SENSOR])
        cg.add(var.set_selftest_sensor(sens))
    if CONF_SHORT_PACKET_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_SHORT_PACKET_SENSOR])
        cg.add(var.set_short_packet_sensor(sens))
    if CONF_SHORT_START_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_SHORT_START_PACKET])
        cg.add(var.set_short_start_packet(sens))
    if CONF_LONG_START_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_LONG_START_PACKET])
        cg.add(var.set_long_start_packet(sens))
    if CONF_SHORT_CLOG_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_SHORT_CLOG_PACKET])
        cg.add(var.set_short_clog_packet(sens))
    if CONF_LONG_CLOG_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_LONG_CLOG_PACKET])
        cg.add(var.set_long_clog_packet(sens))
    if CONF_SHORT_OVERHEAT_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_SHORT_OVERHEAT_PACKET])
        cg.add(var.set_short_overheat_packet(sens))
    if CONF_LONG_OVERHEAT_PACKET in config:
        sens = await sensor.new_sensor(config[CONF_LONG_OVERHEAT_PACKET])
        cg.add(var.set_long_overheat_packet(sens))
    if CONF_UNKNOWN_PACKET_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_UNKNOWN_PACKET_SENSOR])
        cg.add(var.set_unknown_packet_sensor(sens))
    if CONF_SELFTEST_COUNTER in config:
        sens = await sensor.new_sensor(config[CONF_SELFTEST_COUNTER])
        cg.add(var.set_selftest_counter(sens))
