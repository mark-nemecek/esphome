#include "switchbot_wosensortho.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace switchbot_wosensortho {

static const char *const TAG = "switchbot_wosensortho";

void SwitchbotWoSensorTHO::dump_config() {
  ESP_LOGCONFIG(TAG,
                "Switchbot Outdoor Meter\n"
                "  MAC: %s",
                format_hex_pretty(this->address_, 12, ':').c_str());
  LOG_SENSOR("  ", "Temperature", this->temperature_);
  LOG_SENSOR("  ", "Humidity", this->humidity_);
  LOG_SENSOR("  ", "Battery Level", this->battery_level_);
}

bool SwitchbotWoSensorTHO::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
  if (device.address_uint64() != this->address_) {
    ESP_LOGVV(TAG, "parse_device(): unknown MAC address.");
    return false;
  }
  ESP_LOGVV(TAG, "parse_device(): MAC address %s found.", device.address_str().c_str());

  esphome::switchbot_ble::SwitchbotParseResult result;
  result.name = "WoSensorTHO";

  bool success = false;
  for (auto data : device.get_service_datas()) {
    ESP_LOGVV(TAG, "service data size: %d", data.data.size());
    // float temperature = (float(data.data[3] & 0x0F) * 0.1) + float(data.data[4] & 0x7F);
    // if (!(data.data[4] & 0x80)) {
    //   temperature = -temperature;
    // }
    // result.temperature = temperature;
    // result.humidity = data.data[5] & 0x7F;
    result.battery_level = data.data[2] & 0x7F;
  }

  for (auto data : device.get_manufacturer_datas()) {
    ESP_LOGVV(TAG, "manufacturer data size: %d", data.data.size());
    float temperature = (float(data.data[8] & 0x0F) * 0.1) + float(data.data[9] & 0x7F);
    if (!(data.data[9] & 0x80)) {
      temperature = -temperature;
    }
    result.temperature = temperature;
    result.humidity = data.data[10] & 0x7F;
  }

  if (result.temperature.has_value() && this->temperature_ != nullptr) {
    this->temperature_->publish_state(result.temperature.value());
  }
  if (result.humidity.has_value() && this->humidity_ != nullptr) {
    this->humidity_->publish_state(result.humidity.value());
  }
  if (result.battery_level.has_value() && this->battery_level_ != nullptr) {
    this->battery_level_->publish_state(result.battery_level.value());
  }

  success = true;

  return success;
}

}  // namespace switchbot_wosensortho
}  // namespace esphome

#endif
