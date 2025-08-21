// Code to talk to the TOF sensor

#include <Wire.h>
#include "registers.h"
#define TOF_ADDR 0x29

TwoWire *bus;

void VL53L1X::writeReg(uint16_t reg, uint8_t value)
{
  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  bus->write(value);
  last_status = bus->endTransmission();
}

// Write a 16-bit register
void VL53L1X::writeReg16Bit(uint16_t reg, uint16_t value)
{
  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  bus->write((uint8_t)(value >> 8)); // value high byte
  bus->write((uint8_t)(value));      // value low byte
  last_status = bus->endTransmission();
}

// Write a 32-bit register
void VL53L1X::writeReg32Bit(uint16_t reg, uint32_t value)
{
  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  bus->write((uint8_t)(value >> 24)); // value highest byte
  bus->write((uint8_t)(value >> 16));
  bus->write((uint8_t)(value >>  8));
  bus->write((uint8_t)(value));       // value lowest byte
  last_status = bus->endTransmission();
}

// Read an 8-bit register
uint8_t VL53L1X::readReg(regAddr reg)
{
  uint8_t value;

  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  last_status = bus->endTransmission();

  bus->requestFrom(address, (uint8_t)1);
  value = bus->read();

  return value;
}

// Read a 16-bit register
uint16_t VL53L1X::readReg16Bit(uint16_t reg)
{
  uint16_t value;

  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  last_status = bus->endTransmission();

  bus->requestFrom(address, (uint8_t)2);
  value  = (uint16_t)bus->read() << 8; // value high byte
  value |=           bus->read();      // value low byte

  return value;
}

// Read a 32-bit register
uint32_t VL53L1X::readReg32Bit(uint16_t reg)
{
  uint32_t value;

  bus->beginTransmission(address);
  bus->write((uint8_t)(reg >> 8)); // reg high byte
  bus->write((uint8_t)(reg));      // reg low byte
  last_status = bus->endTransmission();

  bus->requestFrom(address, (uint8_t)4);
  value  = (uint32_t)bus->read() << 24; // value highest byte
  value |= (uint32_t)bus->read() << 16;
  value |= (uint16_t)bus->read() <<  8;
  value |=           bus->read();       // value lowest byte

  return value;
}

