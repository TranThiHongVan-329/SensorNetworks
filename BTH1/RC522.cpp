#include <Arduino.h>
#include "TinyRC522.h"

#define SDA_PIN    2
#define BUS_SPEED  1000000
#define TAG_SIZE   64

RC522 g_rc522;

void dump_tag_data(uint8_t* buffer, uint16_t buffer_size)
{
  for(uint16_t i=0; i<buffer_size; i+=16)
    Serial.printf("%03d> %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
      i,
      buffer[i+0],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+6],buffer[i+7],
      buffer[i+8],buffer[i+9],buffer[i+10],buffer[i+11],buffer[i+12],buffer[i+13],buffer[i+14],buffer[i+15]);
}

void setup()
{
  delay(150);
  Serial.begin(9600);
  Serial.println("\n");
  Serial.println("****** Read/write application for TinyRC522 Library ******");

  g_rc522.initialize(SDA_PIN,BUS_SPEED);
  g_rc522.set_antenna_gain(ANTENNA_GAIN_18_DB);
}

void loop()
{
  Tag* p_tag=g_rc522.get_tag();
  if (p_tag)
  {
    Serial.printf("\nTag %08X found\n",p_tag->get_uid());

    uint8_t data_buffer[TAG_SIZE];
    if (p_tag->read(data_buffer,TAG_SIZE))
    {
      dump_tag_data(data_buffer,TAG_SIZE);

      randomSeed(millis());
      for(uint8_t i=0; i<TAG_SIZE; i++)
        data_buffer[i]=random(256);

      if (p_tag->write(data_buffer,TAG_SIZE))
      {
        uint8_t control_buffer[TAG_SIZE];
        if (p_tag->read(control_buffer,TAG_SIZE))
        {
          dump_tag_data(data_buffer,TAG_SIZE);

          bool result=true;
          for (uint8_t i=0; i<TAG_SIZE; i++)
            if (data_buffer[i]!=control_buffer[i])
              result=false;

          Serial.println(result?"Test succeeded":"/!\\ Test failed !");
        }
      }
    }
    p_tag->release();
  }
  delay(50);
}
