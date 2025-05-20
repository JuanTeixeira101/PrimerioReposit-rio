// bmp180.c - Simula um sensor BMP180 com slider de pressão

#include "wokwi-api.h"
#include <stdio.h>
#include <string.h>

#define BMP180_ADDRESS 0x77

// Valores simulados
float temperature = 25.0;
uint32_t pressure = 100000; // em Pa (1000 hPa)

static const char *slider_key = "pressure";

// Handler de escrita/leitura I2C
int16_t bmp180_i2c_read(const device_t *dev, uint8_t reg) {
  if (reg == 0xF6) { // MSB da leitura de pressão simulada
    return (pressure >> 8) & 0xFF;
  } else if (reg == 0xF7) { // LSB
    return pressure & 0xFF;
  } else if (reg == 0xF4) { // ID da leitura
    return 0x34; // Comando de leitura de pressão
  } else if (reg == 0xD0) { // ID do chip
    return 0x55;
  }
  return 0x00;
}

void bmp180_tick(void *user_data) {
  (void)user_data;
  // Atualiza o valor da pressão a partir do slider
  pressure = (uint32_t)wokwi_slider_get(slider_key, 90000, 110000, 100000);
}

// Registro do chip
void chip_init() {
  const i2c_config_t i2c = {
    .scl_pin = 1,
    .sda_pin = 0,
    .read = bmp180_i2c_read,
  };

  device_config_t config = {
    .i2c = &i2c,
    .tick_hz = 10,
    .tick = bmp180_tick,
  };

  wokwi_chip_register(&config);
}
