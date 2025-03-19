// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "accl.h"
#include "nrf_delay.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 8-bit read value
static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
      NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
      NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0)
  };
  ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
  }

  return rx_buf;
}

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  //TODO: implement me
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t merged_data[] = {reg_addr, data};
  nrf_twi_mngr_transfer_t const write_transfer[] = {NRF_TWI_MNGR_WRITE(i2c_addr, &merged_data, 2, 0)};
  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void lsm303agr_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize Accelerometer---

  // Reboot acclerometer
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG5_A, 0x80);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG4_A, 0x80);

  // Configure accelerometer at 100Hz, normal mode (10-bit)
  // Enable x, y and z axes
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG1_A, 0x57);

  // Read WHO AM I register
  // Always returns the same value if working
  //TODO: read the Accelerometer WHO AM I register and check the result
  printf("Magnometer Register Address: %x\n", i2c_reg_read(LSM303AGR_MAG_ADDRESS, WHO_AM_I_M));
  printf("Accelerometer Register Address: %x\n", i2c_reg_read(LSM303AGR_ACC_ADDRESS, WHO_AM_I_A));
  // ---Initialize Magnetometer---

  // Reboot magnetometer
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_A_M, 0x40);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_C_M, 0x10);

  // Configure magnetometer at 100Hz, continuous mode
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_A_M, 0x0C);

  // Read WHO AM I register
  //TODO: read the Magnetometer WHO AM I register and check the result

  // ---Initialize Temperature---

  // Enable temperature sensor
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, TEMP_CFG_REG_A, 0xC0);
}

// Read the internal temperature sensor
//
// Return measurement as floating point value in degrees C
float lsm303agr_read_temperature(void) {
  //TODO: implement me

  int16_t ls8b = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_TEMP_L_A);
  int16_t ms8b = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_TEMP_H_A);

  int16_t val = (ms8b << 8) + ls8b;
  float temp = (float)val * (1.0 / 256.0) + 25.0; 
  return temp;
}

lsm303agr_measurement_t lsm303agr_read_accelerometer(void) {
  //TODO: implement me

  lsm303agr_measurement_t measurement = {0};

  int16_t ls8bx = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_X_L_A);
  int16_t ms8bx = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_X_H_A);
  int16_t valx = (ms8bx << 8) + ls8bx;
  valx >>= 6;

  int16_t ls8by = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Y_L_A);
  int16_t ms8by = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Y_H_A);
  int16_t valy = (ms8by << 8) + ls8by;
  valy >>= 6;

  int16_t ls8bz = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Z_L_A);
  int16_t ms8bz = i2c_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Z_H_A);
  int16_t valz = (ms8bz << 8) + ls8bz;
  valz >>= 6;

  measurement.x_axis = valx * 3.9 / 1000.0;
  measurement.y_axis = valy * 3.9 / 1000.0;
  measurement.z_axis = valz * 3.9 / 1000.0;
  return measurement;
}

lsm303agr_measurement_t lsm303agr_read_tilt(void)
{
  // TODO: implement me

  lsm303agr_measurement_t a = lsm303agr_read_accelerometer();
  lsm303agr_measurement_t measurement = {0};

  measurement.x_axis = atan(a.x_axis / sqrt(a.y_axis * a.y_axis + a.z_axis * a.z_axis));
  measurement.y_axis = atan(a.y_axis / sqrt(a.x_axis * a.x_axis + a.z_axis * a.z_axis));
  measurement.z_axis = atan(sqrt(a.x_axis * a.x_axis + a.y_axis * a.y_axis) / a.z_axis);
  // measurement.z_axis = atan(a.z_axis/ sqrt(a.x_axis * a.x_axis + a.y_axis * a.y_axis));

  measurement.x_axis *= 360.0 / (2 * M_PI);
  measurement.y_axis *= 360.0 / (2 * M_PI);
  measurement.z_axis *= 360.0 / (2 * M_PI);

  return measurement;
}

lsm303agr_measurement_t lsm303agr_read_magnetometer(void) {
  //TODO: implement me

  lsm303agr_measurement_t measurement = {0};

  int16_t ls8bx = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTX_L_REG_M);
  int16_t ms8bx = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTX_H_REG_M);
  int16_t valx = (ms8bx << 8) + ls8bx;

  int16_t ls8by = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTY_L_REG_M);
  int16_t ms8by = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTY_H_REG_M);
  int16_t valy = (ms8by << 8) + ls8by;

  int16_t ls8bz = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTZ_L_REG_M);
  int16_t ms8bz = i2c_reg_read(LSM303AGR_MAG_ADDRESS, OUTZ_H_REG_M);
  int16_t valz = (ms8bz << 8) + ls8bz;

  measurement.x_axis = valx * 1.5 / 10.0;
  measurement.y_axis = valy * 1.5 / 10.0;
  measurement.z_axis = valz * 1.5 / 10.0;

  return measurement;
}
