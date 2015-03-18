#include <Wire.h>
#include <VirtualWire.h>

#define LED_PIN 9
#define RX_PIN 14
#define I2C_SLAVE_ADDRESS 0x43

volatile uint8_t rf_buffer[VW_MAX_MESSAGE_LEN];
volatile int rf_buffer_index = -1;
volatile uint8_t rf_buffer_size = 0;

void requestEvent()
{  
  
  if (rf_buffer_index == -1) {
    Wire.write(rf_buffer_size);
    
    if (rf_buffer_size > 0) {
      rf_buffer_index++;
    }
    
  } else {
    
    Wire.write(rf_buffer[rf_buffer_index]);
    rf_buffer_index++;
    if (rf_buffer_index >= rf_buffer_size) {
      rf_buffer_index = -1;
      rf_buffer_size = 0;
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(9600);
    
  vw_set_rx_pin(RX_PIN);
  vw_setup(1000);                
  vw_rx_start();
  
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  
}

void loop () {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t i;
  
  if (vw_get_message(buf, &buflen)) {
    
    //Copy data from buffer to rf_buffer
    for (i = 0; i < buflen; i++) {
      rf_buffer[i] = buf[i];
    } 
    
    rf_buffer_size = buflen;
    rf_buffer_index = -1;
    
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
  }
}
