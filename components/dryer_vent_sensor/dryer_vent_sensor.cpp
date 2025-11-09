#include "dryer_vent_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dryer_vent_sensor {

static const char *TAG = "dryer_vent.sensor";

static int16_t ring[RING_SIZE] = {0};
static uint16_t current_item = 0;
static hw_timer_t *timer = nullptr;

void IRAM_ATTR DryerVentSensor::timer_isr(void *arg) {
  int16_t count;
  pcnt_unit_handle_t unit = (pcnt_unit_handle_t)arg;
  pulse_cnt_get_count(unit, &count);
  ring[current_item] = count;
  pulse_cnt_zero(unit);
  if (current_item >= RING_SIZE - 1) {
    current_item = 0;
  } else {
    current_item++;
  }
}

void DryerVentSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Dryer Vent Sensor...");
  
  // Configure Pulse PIN
  pinMode(this->count_pin_, INPUT);

  // Configure pulse counter
  pulse_cnt_config_t unit_config = {
    .gpio_num = this->count_pin_,
    .edge_config = {
      .pos_mode = PCNT_COUNT_INC,  // Count up on rising edge
      .neg_mode = PCNT_COUNT_DIS,  // Disable counting on falling edge
      .zero_mode = PCNT_COUNT_DIS, // Keep the counter value
      .pe_mode = PCNT_COUNT_DIS,   // Keep the counter value
    },
    .intr_priority = 0,
  };

  pcnt_unit_handle_t unit = nullptr;
  pulse_cnt_new_unit(&unit_config, &unit);

  pulse_cnt_glitch_filter_config_t filter_config = {
    .max_glitch_ns = 500,
  };
  pulse_cnt_set_glitch_filter(unit, &filter_config);
  pulse_cnt_enable(unit);

  // Store the unit handle
  this->pcnt_unit_ = unit;

  // Setup timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &DryerVentSensor::timer_isr, true);
  timerAlarmWrite(timer, 20000, true);
  timerAlarmEnable(timer);

  // Configure Test PIN
  pinMode(this->test_pin_, OUTPUT);
  digitalWrite(this->test_pin_, HIGH);
}

void DryerVentSensor::update() {
  int16_t end_position = current_item;
  ESP_LOGD(TAG, "Checking %i -> %i", this->review_position, end_position);

  while (this->review_position != end_position) {
    if (ring[this->review_position] > 5) {
      if (!this->in_pulse) {
        if (!this->in_packet) {
          this->in_packet = true;
          this->packet_pulses = 1;
        } else {
          this->packet_pulses++;
        }
        this->in_pulse = true;
        this->pulse_length = 0;
      }
      this->pulse_length++;
    }

    if (ring[this->review_position] <= 5) {
      if (this->in_pulse) {
        ESP_LOGD(TAG, "Pulse Length %i", this->pulse_length);
        if (this->pulse_length == 1) {
          this->short_packet++;
        }
        this->in_pulse = false;
        this->break_length = 0;
      }
      this->break_length++;

      if (this->break_length > 50 && this->in_packet) {
        if (this->packet_pulses == 1) {
          if (this->pulse_length < 25) {
            ESP_LOGD(TAG, "1 Pulse Packet, %i long (unknown)", this->pulse_length);
            this->short_start++;
          } else if (this->pulse_length > 45) {
            ESP_LOGD(TAG, "1 Pulse Packet, %i long (unknown)", this->pulse_length);
            this->long_start++;
          } else {
            ESP_LOGD(TAG, "1 Pulse Packet, %i long (test/startup)", this->pulse_length);
            if (this->test_outstanding) {
              this->test_outstanding = false;
              this->test_failed = false;
              this->selftest_count++;
            }
          }
        } else if (this->packet_pulses == 3) {
          ESP_LOGD(TAG, "3 Pulse Packet (clog)");
          if (this->pulse_length < 3) {
            this->short_clog++;
          } else if (this->pulse_length > 7) {
            this->long_clog++;
          }
          this->clog = true;
        } else if (this->packet_pulses == 5) {
          ESP_LOGD(TAG, "5 Pulse Packet (overheat)");
          this->overheat = true;
          if (this->pulse_length < 3) {
            this->short_overheat++;
          } else if (this->pulse_length > 7) {
            this->long_overheat++;
          }
        } else {
          ESP_LOGD(TAG, "%i Pulse Packet (error?)", this->packet_pulses);
          this->unknown_packet++;
        }
        this->in_packet = false;
      }

      if (this->break_length > 65000) {
        this->break_length = 101;
      }
    }

    this->review_position++;
    if (this->review_position >= RING_SIZE - 1) {
      this->review_position = 0;
    }
  }

  unsigned long t = millis();

  if (t > this->next_test) {
    ESP_LOGD(TAG, "starting self test");
    this->test_outstanding = true;
    this->time_since_test = 0;
    digitalWrite(this->test_pin_, LOW);
    delay(25);
    digitalWrite(this->test_pin_, HIGH);
    this->next_test = t + TEST_PERIOD;
  }

  if (this->test_outstanding) {
    this->time_since_test++;
    if (this->time_since_test > MAX_TEST_TIME) {
      ESP_LOGD(TAG, "self test failed");
      this->test_failed = true;
      this->test_outstanding = false;
    }
  }

  if (this->overheat_sensor_ != nullptr)
    this->overheat_sensor_->publish_state(this->overheat);
  if (this->clog_sensor_ != nullptr)
    this->clog_sensor_->publish_state(this->clog);
  if (this->selftest_sensor_ != nullptr)
    this->selftest_sensor_->publish_state(this->test_failed);
  if (this->short_packet_sensor_ != nullptr)
    this->short_packet_sensor_->publish_state(this->short_packet);
  if (this->short_start_packet_ != nullptr)
    this->short_start_packet_->publish_state(this->short_start);
  if (this->long_start_packet_ != nullptr)
    this->long_start_packet_->publish_state(this->long_start);
  if (this->short_clog_packet_ != nullptr)
    this->short_clog_packet_->publish_state(this->short_clog);
  if (this->long_clog_packet_ != nullptr)
    this->long_clog_packet_->publish_state(this->long_clog);
  if (this->short_overheat_packet_ != nullptr)
    this->short_overheat_packet_->publish_state(this->short_overheat);
  if (this->long_overheat_packet_ != nullptr)
    this->long_overheat_packet_->publish_state(this->long_overheat);
  if (this->unknown_packet_sensor_ != nullptr)
    this->unknown_packet_sensor_->publish_state(this->unknown_packet);
  if (this->selftest_counter_ != nullptr)
    this->selftest_counter_->publish_state(this->selftest_count);
}

} // namespace dryer_vent_sensor
}  // namespace esphome