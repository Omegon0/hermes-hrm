#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3D  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define MAX_ADDR 0x57

#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MAX_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(MAX_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX_ADDR, 1);
  return Wire.read();
}

void readFIFO(uint32_t &red, uint32_t &ir) {
  Wire.beginTransmission(MAX_ADDR);
  Wire.write(0x07);  // FIFO_DATA register
  Wire.endTransmission(false);
  Wire.requestFrom(MAX_ADDR, 6);

  uint32_t rawData[2] = { 0 };
  for (int i = 0; i < 2; i++) {
    rawData[i] = ((uint32_t)Wire.read() << 16);
    rawData[i] |= ((uint32_t)Wire.read() << 8);
    rawData[i] |= Wire.read();
    rawData[i] &= 0x3FFFF;  // 18-bit mask
  }

  red = rawData[0];
  ir = rawData[1];
}

void init() {
  Wire.begin();

  // Reset
  writeRegister(0x09, 0x40);  // RESET = 1
  delay(100);

  // Clear FIFO pointers
  writeRegister(0x04, 0x00);  // FIFO_WR_PTR
  writeRegister(0x05, 0x00);  // OVF_COUNTER
  writeRegister(0x06, 0x00);  // FIFO_RD_PTR

  // FIFO config: no averaging, rollover enabled, almost full at 17
  writeRegister(0x08, 0x4F);

  // Mode config: SpO2 mode (Red + IR)
  writeRegister(0x09, 0x03);

  // SpO2 config: ADC range = 2048 nA, 100 Hz, 411 us pulse width (18 bits)
  writeRegister(0x0A, 0x27);

  // LED amplitudes: IR = Red = medium brightness
  writeRegister(0x0C, 0x24);  // LED1_PA (Red)
  writeRegister(0x0D, 0x24);  // LED2_PA (IR)

  // Multi-LED config: SLOT1 = Red, SLOT2 = IR
  writeRegister(0x11, 0x21);
}

uint32_t hr() {
  uint32_t red, ir;
  readFIFO(red, ir);
  return ir;  // Use IR for heart rate
}

uint32_t spo() {
  uint32_t red, ir;
  readFIFO(red, ir);
  // In a real application, you'd calculate SpO2 using a proper algorithm
  // Here we return IR and Red for further processing
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" IR: ");
  Serial.println(ir);
  return (ir << 18) | (red & 0x3FFFF);  // Pack into 32-bit (example only)
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  init();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setRotation(1);
  display.clearDisplay();
  display.display();
}

void loop() {
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("HR:");
  display.setTextSize(1);
  display.print(hr());
  display.print("bpm");

  display.setCursor(64, 0);
  display.setTextSize(2);
  display.println("SpO2:");
  display.setTextSize(1);
  display.print(spo());
  display.print("");
}