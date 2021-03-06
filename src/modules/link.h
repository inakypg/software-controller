// TODO: Document what this module does
#ifndef __LINK_MODULE_H__
#define __LINK_MODULE_H__

#include <stdint.h>
#include <stdbool.h>


#define ALARM_ECU_POWER_LOSS              0x01
#define ALARM_ECU_LOW_BATTERY             0x02 
#define ALARM_ECU_LOSS_BREATH_INTEGRITY   0x04
#define ALARM_ECU_HIGH_AIRWAY_PRESSURE    0x08
#define ALARM_ECU_LOW_AIRWAY_PRESSURE     0x10
#define ALARM_ECU_LOW_TIDAL_VOL_DELIVERED 0x20
#define ALARM_ECU_APNEA                   0x40
// bits 7 - 15 --
#define ALARM_CRC_ERROR                   0x00010000
#define ALARM_DROPPED_PACKET              0x00020000
#define ALARM_SERIAL_COMM                 0x00040000
#define ALARM_PACKET_VERSION              0x00080000
// bits 20 - 23 --
#define ALARM_UI_MODE_MISMATCH            0x01000000
#define ALARM_UI_RESP_RATE_MISMATCH       0x02000000
#define ALARM_UI_TIDAL_MISMATCH           0x04000000
#define ALARM_UI_IE_RATIO_MISMATCH        0x08000000
// bits 28 - 31 --

// bit mask
#define MODE_NON_ASSIST  0x00
#define MODE_ASSIST      0x01
#define MODE_SIM         0x02

// toggle bit for start stop
#define MODE_START_STOP  0x80


#define PACKET_VERSION 2

typedef struct __attribute__((packed)) {
  uint16_t sequence_count;            // bytes 0 - 1 - rpi unsigned short int
  uint8_t packet_version;             // byte 2      - rpi unsigned char
  uint8_t mode_value;                 // byte 3      - rpi unsigned char
  uint32_t respiratory_rate_measured; // bytes 4 - 7 - rpi unsigned int
  uint32_t respiratory_rate_set;      // bytes 8 - 11
  int32_t tidal_volume_measured;      // bytes 12 - 15
  int32_t tidal_volume_set;           // bytes 16 - 19
  uint32_t ie_ratio_measured;         // bytes 20 - 23
  uint32_t ie_ratio_set;              // bytes 24 - 27
  int32_t peep_value_measured;        // bytes 28 - 31
  int32_t peak_pressure_measured;     // bytes 32 - 35
  int32_t plateau_value_measurement;  // bytes 36 - 39
  int32_t pressure_measured;          // bytes 40 - 43
  int32_t flow_measured;              // bytes 44 - 47
  int32_t volume_in_measured;         // bytes 48 - 51
  int32_t volume_out_measured;        // bytes 52 - 55
  int32_t volume_rate_measured;       // bytes 56 - 59
  uint8_t control_state;              // byte 60       - rpi unsigned char
  uint8_t battery_level;              // byte 61
  uint16_t reserved;                  // bytes 62 - 63 - rpi unsigned int
  uint32_t alarm_bits;                // bytes 64 - 67
  uint16_t crc;                       // bytes 68 - 69 - rpi unsigned short int
} data_packet_def;

typedef struct __attribute__((packed)) {
  uint16_t sequence_count;            // bytes 0 - 1 - rpi unsigned short int
  uint8_t packet_version;             // byte 2      - rpi unsigned char
  uint8_t mode_value;                 // byte 3      - rpi unsigned char
  uint32_t respiratory_rate_set;      // bytes 4 - 7 - rpi unsigned int
  int32_t tidal_volume_set;           // bytes 8 - 11
  uint32_t ie_ratio_set;              // bytes 12 - 15
  uint32_t alarm_bits;                // bytes 16 - 19
  uint16_t crc;                       // bytes 20 - 21 - rpi unsigned short int  
} command_packet_def;

struct link {
  // Variables
  uint8_t  startVentilation;
  uint8_t  ventilationMode;
  uint32_t volumeRequested;
  uint32_t respirationRateRequested;
  uint32_t ieRatioRequested;
  
  // Alarms
  int8_t   droppedPacketAlarm;
  int8_t   crcErrorAlarm;
  int8_t   unsupportedPacketVersionAlarm;
};

// Public Variables
extern struct link comm;

// TODO: Doc
int linkModuleInit(void);

// TODO: Doc
int linkModuleRun(void);

#endif /* __LINK_MODULE_H__ */
