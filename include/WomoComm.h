#ifndef WOMOCOMM_h
#define WOMOCOMM_h

#include <Arduino.h>
#include <mcp2515.h>
#include <inttypes.h>

namespace WomoComm {
// 8 bits: category, 17 bits: id, 4 bit: sender
// "0b00000000'00000000000000000'0000"
    namespace CAN_ID {
        // Category: 0b00001000 = 8: Unten
        constexpr canid_t ValveCommand =        (0b00001000UL<<21)|(0b00000000000001000UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t SlidestepCommand =    (0b00001000UL<<21)|(0b00000000000001001UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t GasLevelConfig =      (0b00001000UL<<21)|(0b00000000000100001UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t GasLevel =            (0b00001000UL<<21)|(0b00000000000100010UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t WasteLevelConfig =    (0b00001000UL<<21)|(0b00000000000100100UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t WasteLevel =          (0b00001000UL<<21)|(0b00000000000100101UL<<4) | CAN_EFF_FLAG;
        // can_dlc=1
        // data[0] = 0: lost interest in the levels, 1: got interested in the levels
        constexpr canid_t LevelInterest =       (0b00001000UL<<21)|(0b00000000000100110UL<<4) | CAN_EFF_FLAG;
        

        // Category: 0b00000010 = 2: Lighting
        // Lighting action message
        // If can_dlc==0 the light should be toggled
        // If can_dlc>0 the pwm values should be updated from the data array. Every byte is one pwm value.
        constexpr canid_t KitchenLight =        (0b00000010UL<<21)|(0b00000000000000001UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t MainLight =           (0b00000010UL<<21)|(0b00000000000000010UL<<4) | CAN_EFF_FLAG;
        // Lighting status message
        // can_dlc==1
        // data[0] = 0: light is off, 1: light is on
        constexpr canid_t KitchenLightStatus =        (0b00000010UL<<21)|(0b00000000000000011UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t MainLightStatus =           (0b00000010UL<<21)|(0b00000000000000100UL<<4) | CAN_EFF_FLAG;

        // Category: 0b00000011 = 3: Fresh Water
        // If can_dlc = 0: Toggle pump
        // If can_dlc = 1:
        //   can_data[0] = 0: turn pump off / 1: turn pump on 
        constexpr canid_t FreshWaterPump =        (0b00000011UL<<21)|(0b00000000000000001UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 1: 
        //   can_data[0] = 0: pump is off / 1: pump is on
        constexpr canid_t FreshWaterPumpStatus =  (0b00000011UL<<21)|(0b00000000000000010UL<<4) | CAN_EFF_FLAG;

        // Clears the sender id bits from a CAN ID (4 lsb)
        inline canid_t stripSender(canid_t CAN_ID) {
            return CAN_ID & ~0xF;
        }

        // Extracts the sender id from a CAN_ID (4 lsb)
        // 0 if no sender 
        inline uint8_t getSender(canid_t CAN_ID) {
            return CAN_ID & 0xF;
        }

        // Adds the sender id to a CAN ID (4 lsb)
        // Clears the sender id first
        inline canid_t addSender(canid_t CAN_ID, uint8_t Sender_ID) {
            return stripSender(CAN_ID) | (Sender_ID & 0xF);
        }

    }

    namespace Sender_ID {
        constexpr uint8_t Kitchen = 0x01;
        constexpr uint8_t Table = 0x02;
        constexpr uint8_t Bed = 0x03;
        constexpr uint8_t FreshWater = 0x04;

    }
}

#endif