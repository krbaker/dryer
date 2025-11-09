#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32
#include <driver/pulse_cnt.h>
#endif

namespace esphome {
namespace dryer_vent_sensor {

#define TEST_PERIOD     86400000 // one day in milliseconds
#define MAX_TEST_TIME   3 // Maximum number of cycles until we see our test response
#define FIRST_TEST      40000 // First test delay (millis)
#define RING_SIZE 1500

class DryerVentSensor : public PollingComponent {
 public:
  DryerVentSensor() : PollingComponent(15000) {} // poll every 15s

  void set_count_pin(int pin) { count_pin_ = pin; }
  void set_test_pin(int pin) { test_pin_ = pin; }
  
  void set_overheat_sensor(sensor::Sensor *sens) { overheat_sensor_ = sens; }
  void set_clog_sensor(sensor::Sensor *sens) { clog_sensor_ = sens; }
  void set_selftest_sensor(sensor::Sensor *sens) { selftest_sensor_ = sens; }
  void set_short_packet_sensor(sensor::Sensor *sens) { short_packet_sensor_ = sens; }
  void set_short_start_packet(sensor::Sensor *sens) { short_start_packet_ = sens; }
  void set_long_start_packet(sensor::Sensor *sens) { long_start_packet_ = sens; }
  void set_short_clog_packet(sensor::Sensor *sens) { short_clog_packet_ = sens; }
  void set_long_clog_packet(sensor::Sensor *sens) { long_clog_packet_ = sens; }
  void set_short_overheat_packet(sensor::Sensor *sens) { short_overheat_packet_ = sens; }
  void set_long_overheat_packet(sensor::Sensor *sens) { long_overheat_packet_ = sens; }
  void set_unknown_packet_sensor(sensor::Sensor *sens) { unknown_packet_sensor_ = sens; }
  void set_selftest_counter(sensor::Sensor *sens) { selftest_counter_ = sens; }

  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void setup() override;
  void update() override;

 protected:
  int count_pin_{2};  // default count pin
  int test_pin_{15};  // default test pin
  pcnt_unit_handle_t pcnt_unit_{nullptr};
  
  sensor::Sensor *overheat_sensor_{nullptr};
  sensor::Sensor *clog_sensor_{nullptr};
  sensor::Sensor *selftest_sensor_{nullptr};
  sensor::Sensor *short_packet_sensor_{nullptr};
  sensor::Sensor *short_start_packet_{nullptr};
  sensor::Sensor *long_start_packet_{nullptr};
  sensor::Sensor *short_clog_packet_{nullptr};
  sensor::Sensor *long_clog_packet_{nullptr};
  sensor::Sensor *short_overheat_packet_{nullptr};
  sensor::Sensor *long_overheat_packet_{nullptr};
  sensor::Sensor *unknown_packet_sensor_{nullptr};
  sensor::Sensor *selftest_counter_{nullptr};

  unsigned long next_test = FIRST_TEST;
  unsigned long time_since_test = 0;
  bool test_outstanding = false;
  bool test_failed = false;
  uint16_t review_position = 0;
  bool in_pulse = false;
  bool in_packet = false;
  short packet_pulses = 0;
  uint16_t pulse_length = 0;
  uint16_t break_length = 0;
  bool clog = false;
  bool overheat = false;
  uint16_t short_start = 0;
  uint16_t long_start = 0;
  uint16_t short_clog = 0;
  uint16_t long_clog = 0;
  uint16_t short_overheat = 0;
  uint16_t long_overheat = 0;
  uint16_t short_packet = 0;
  uint16_t unknown_packet = 0;
  uint16_t selftest_count = 0;

  static void IRAM_ATTR timer_isr(void *arg);
};

} // namespace dryer_vent_sensor
} // namespace esphome