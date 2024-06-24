#ifndef WOMOCOMM_h
#define WOMOCOMM_h

#include <Arduino.h>
#include <mcp2515.h>
#include <inttypes.h>

namespace WomoComm {
// 8 bits: category, 17 bits: id, 4 bit: sender
// "0b00000000'00000000000000000'0000"
    namespace CAN_ID {
        namespace CATEGORY {
            // Category: 0b00001000 = 8: Unten
            constexpr canid_t Below = 0b00001000UL;
            constexpr canid_t Below_CANID = Below<<21;
            // Category: 0b00000010 = 2: Lighting
            constexpr canid_t Lighting = 0b00000010;
            constexpr canid_t Lighting_CANID = Lighting<<21;
            // Category: 0b00000011 = 3: Fresh Water
            constexpr canid_t FreshWater = 0b00000011;
            constexpr canid_t FreshWater_CANID = FreshWater<<21;
            // Category: 0b00000100 = 4: Electrical
            constexpr canid_t Electrical = 0b00000100;
            constexpr canid_t Electrical_CANID = Electrical<<21;
        }

        constexpr canid_t ValveCommand =        CATEGORY::Below_CANID|(0b00000000000001000UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t SlidestepCommand =    CATEGORY::Below_CANID|(0b00000000000001001UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 3
        // data[0]: reserved
        // (((uint16_t)data[1]) << 8) | data[2]: interval of level sending in ms, minimum of 1000 or 0 to disable
        constexpr canid_t GasLevelConfig =      CATEGORY::Below_CANID|(0b00000000000100001UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 3
        // data[0]: status of the measurement, 1=Error | 3=OK
        // data[1]: msbs of value
        // data[2]: lsbs of value
        constexpr canid_t GasLevel =            CATEGORY::Below_CANID|(0b00000000000100010UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 3
        // data[0]: reserved
        // (((uint16_t)data[1]) << 8) | data[2]: interval of level sending in ms, minimum of 1000 or 0 to disable
        constexpr canid_t WasteLevelConfig =    CATEGORY::Below_CANID|(0b00000000000100100UL<<4) | CAN_EFF_FLAG;
        // can_dlc=7
        // data[0]: status of the measurements, 1=Error | 3=OK, bit 0 & 1 for value 1, bit 2 & 3 for value 2, bit 4 & 5 for value 3
        // data[1]: msbs of value 1
        // data[2]: lsbs of value 1
        // data[3]: msbs of value 2
        // data[4]: lsbs of value 2
        // data[5]: msbs of value 3
        // data[6]: lsbs of value 3
        constexpr canid_t WasteLevel =          CATEGORY::Below_CANID|(0b00000000000100101UL<<4) | CAN_EFF_FLAG;
        // can_dlc=1
        // data[0] = 0: lost interest in the levels, 1: got interested in the levels
        constexpr canid_t LevelInterest =       CATEGORY::Below_CANID|(0b00000000000100110UL<<4) | CAN_EFF_FLAG;
        

        // Lighting action message
        // If can_dlc==0 the light should be toggled
        // If can_dlc>0 the pwm values should be updated from the data array. Every byte is one pwm value.
        constexpr canid_t KitchenLight =        CATEGORY::Lighting_CANID|(0b00000000000000001UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t MainLight =           CATEGORY::Lighting_CANID|(0b00000000000000010UL<<4) | CAN_EFF_FLAG;
        // Lighting status message
        // can_dlc==1
        // data[0] = 0: light is off, 1: light is on
        constexpr canid_t KitchenLightStatus =  CATEGORY::Lighting_CANID|(0b00000000000000011UL<<4) | CAN_EFF_FLAG;
        constexpr canid_t MainLightStatus =     CATEGORY::Lighting_CANID|(0b00000000000000100UL<<4) | CAN_EFF_FLAG;


        // If can_dlc = 0: Toggle pump
        // If can_dlc = 1:
        //   can_data[0] = 0: turn pump off / 1: turn pump on 
        constexpr canid_t FreshWaterPump =          CATEGORY::FreshWater_CANID|(0b00000000000000001UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 1: 
        //   can_data[0] = 0: pump is off / 1: pump is on
        constexpr canid_t FreshWaterPumpStatus =    CATEGORY::FreshWater_CANID|(0b00000000000000010UL<<4) | CAN_EFF_FLAG;
        // can_dlc = 2
        // data[0]: unsigned level in percent or UINT8_MAX (0xFF) if error or UINT8_MAX-1 (0xFE) if no level resistor
        // data[1]: uint8_t | remaining liter estimation
        constexpr canid_t FreshWaterLevel =         CATEGORY::FreshWater_CANID|(0b00000000000000011UL<<4) | CAN_EFF_FLAG;


        // can_dlc = 2
        // data[0]+data[1]: uint16_t battery voltage in mV
        constexpr canid_t BatteryVoltage =         CATEGORY::Electrical_CANID|(0b00000000000000001UL<<4) | CAN_EFF_FLAG;        

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

        /// @brief Check if CanId is from specified category
        /// @param canId The can message id to check
        /// @param category The category to check against, must already be shifted
        /// @return 
        inline boolean isCanIdCategory(canid_t canId, canid_t category){
            return (canId&(0b11111111UL<<21)) == (category);
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