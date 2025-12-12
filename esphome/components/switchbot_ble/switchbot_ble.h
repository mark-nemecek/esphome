#pragma once

#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/core/component.h"

#include <vector>

#ifdef USE_ESP32

namespace esphome {
namespace switchbot_ble {

struct SwitchbotParseResult {
  enum {
    TYPE_WOSENSORTH,
    TYPE_WOSENSORTHO,
  } type;
  std::string name;
  optional<float> temperature;
  optional<float> humidity;
  optional<float> battery_level;
};

bool parse_switchbot_value(uint16_t value_type, const uint8_t *data, uint8_t value_length,
                           SwitchbotParseResult &result);
bool parse_switchbot_message(const std::vector<uint8_t> &message, SwitchbotParseResult &result);
// optional<SwitchbotParseResult> parse_switchbot_header(const esp32_ble_tracker::ServiceData &service_data);
bool report_switchbot_results(const optional<SwitchbotParseResult> &result, const std::string &address);

class SwitchbotListener : public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
};

}  // namespace switchbot_ble
}  // namespace esphome

#endif
