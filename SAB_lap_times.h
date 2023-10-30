
#pragma once
#include <string.h>
#include <stdint.h>
#ifdef HT_DEBUG_EN
    // Arduino.h should inlcude the above, so might want to use it
    // in debugging cases
    #include "Arduino.h"
#endif

#pragma pack(push,1)
// LAP TIME PACKET STRUCTURE
// 1) Identifier
//  [4 bits: clock / message state][4 bits: time 1 type]
// 2) Identifier
//  [4 bits: time 2 type][4 bits: empty (for now)]
// 3) Time 1
// 4) Time 1
// 5) Time 1
// 6) Time 2
// 7) Time 2
// 8) Time 2

class SAB_lap_times{
public:

    // default constructor 
  SAB_lap_times() = default;

    // constructor that initializes the class with a CAN packet
  SAB_lap_times(uint8_t buf[]) { load(buf); }

    // load() takes in a buffer, and copies the buffer to the class's member
    // data. (This is usually used with the FlexCAN or STM32CAN CAN_message_t.buf)
    // This effectively loads the CAN message.
  inline void load(uint8_t buf[])         { memcpy(this, buf, sizeof(*this)); }
    
    // write() takes in a buffer, and copies the member data of the class to
    // the location of the buffer. This effectively writes the CAN message.
  inline void write(uint8_t buf[])  const { memcpy(buf, this, sizeof(*this)); }

    // getters
    // get state at first 4 bits of message
    inline uint8_t get_state()      const { return message & 0xF;}

    // get time 1 and 2 type at second/third 4 bits of message
    inline uint8_t get_time_1_type()    const{ return (message >> 0x4) & 0xF;}
    inline uint8_t get_time_2_type()    const{ return (message >> 0x8) & 0xF;}

    // get time 1 and 2 at last and second to last 6 bytes of message
    inline uint32_t get_time_1()        const{ return (message >> 0x10) & 0xFFFFFF;}
    inline uint32_t get_time_2()        const{ return message >> 0x28;}

    inline uint64_t get_message()       const{ return message;}


    // setters
    // Set state at first 4 bits of message
    // State will be used to start and stop the timer on dasboard.
    // It may also (in the future) be used to set this message into text mode
    // to send messages to the driver from SAB/live telem
    inline void set_state(uint8_t state) { message = (message & 0xFFFFFFFFFFFFFFF0) | (state & 0xF);}
    
    // set time 1/2 type at second/ third 4 bits of message
    // time type will be used to designate the type of the times in 
    // the last 6 bytes of the message (best lap, last lap, total time)
    inline void set_time_1_type(uint8_t type) { message = (message & 0xFFFFFFFFFFFFFF0F) | ((uint64_t)(type & 0xF) << 0x4);}
    inline void set_time_2_type(uint8_t type) { message = (message & 0xFFFFFFFFFFFFF0FF) | ((uint64_t)(type & 0xF) << 0x8);}

    // set times at the last and second to last 6 bytes of message
    // times are in milliseconds and is designated by the time type
    inline void set_time_1(uint32_t time) {message = (message & 0xFFFFFF000000FFFF) | ((uint64_t)(time & 0xFFFFFF) << 0x10);}
    inline void set_time_2(uint32_t time) {message = (message & 0x000000FFFFFFFFFF) | ((uint64_t)(time & 0xFFFFFF) << 0x28);}

    // private variables that hold CAN buffer data
private:
    
    uint64_t message;
  
};

#pragma pack(pop)
