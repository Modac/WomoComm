#ifndef WOMOCOMM_h
#define WOMOCOMM_h

#include <Arduino.h>
#include <mcp2515.h>
#include <inttypes.h>

namespace WomoComm {
// 8 bits: category, 17 bits: id, 4 bit: sender
// "0b00000000'00000000000000000'0000"
    namespace CAN_ID {
        constexpr canid_t ValveCommand = 0b00001000000000000000010000000 | CAN_EFF_FLAG;
        constexpr canid_t TrittCommand = 0b00001000000000000000010010000 | CAN_EFF_FLAG;
        constexpr canid_t GasLevelConfig = 0b00001000000000000001000010000 | CAN_EFF_FLAG;
        constexpr canid_t GasLevel = 0b00001000000000000001000100000 | CAN_EFF_FLAG;
        constexpr canid_t WasteLevelConfig = 0b00001000000000000001001000000 | CAN_EFF_FLAG;
        constexpr canid_t WasteLevel = 0b00001000000000000001001010000 | CAN_EFF_FLAG;

        constexpr canid_t KitchenLight = 0b00000010000000000000000010000 | CAN_EFF_FLAG;
        constexpr canid_t MainLight = 0b00000010000000000000000100000 | CAN_EFF_FLAG;

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