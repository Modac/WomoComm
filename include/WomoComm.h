#ifndef WOMOCOMM_h
#define WOMOCOMM_h

#include <Arduino.h>
#include <inttypes.h>

namespace WomoComm {
// 8 bits: category, 17 bits: id, 4 bit: sender
// "0b00000000'00000000000000000'0000"
    namespace CAN_ID {
        constexpr uint32_t ValveCommand 0b00001000000000000000010000000 | CAN_EFF_FLAG;
        constexpr uint32_t TrittCommand 0b00001000000000000000010010000 | CAN_EFF_FLAG;
        constexpr uint32_t GasLevelConfig 0b00001000000000000001000010000 | CAN_EFF_FLAG;
        constexpr uint32_t GasLevel 0b00001000000000000001000100000 | CAN_EFF_FLAG;
        constexpr uint32_t WasteLevelConfig 0b00001000000000000001001000000 | CAN_EFF_FLAG;
        constexpr uint32_t WasteLevel 0b00001000000000000001001010000 | CAN_EFF_FLAG;
        
        constexpr uint32_t KitchenLight = 0b00000010000000000000000010000 | CAN_EFF_FLAG;
        constexpr uint32_t MainLight 0b00000010000000000000000100000 | CAN_EFF_FLAG;

    }
    namespace Sender_ID {
        constexpr uint8_t Kitchen = 0x01;
        constexpr uint8_t Table = 0x02;
        constexpr uint8_t Bed = 0x03;
        constexpr uint8_t FreshWater = 0x04;

    }
}

#endif