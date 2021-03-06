/*
   Parent class for STM4F boards

   Copyright (C) 2019 Simon D. Levy 

   This file is part of Hackflight.

   Hackflight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Hackflight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Hackflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <boards/realboard.hpp>
#include <boards/softquat.hpp>

#include <MPU6xx0.h>
#include "support/motors.hpp"

// Cleanflight drivers
extern "C" {
#include "platform.h"
#include "drivers/system.h"
#include "drivers/timer.h"
#include "drivers/time.h"
#include "drivers/pwm_output.h"
#include "drivers/serial.h"
#include "drivers/serial_uart.h"
#include "drivers/serial_usb_vcp.h"
#include "io/serial.h"
#include "target.h"


    // We put this outside the class to make it available to static Board::outbuf() below
    static serialPort_t * _serial0;

} // extern "C"

class Stm32FBoard : public hf::RealBoard, public hf::SoftwareQuaternionBoard  {

    private:

        MPU6xx0 * _mpu = NULL;

    protected: 

        Stm32FBoard(serialPort_t * serial0)
        {
            _serial0 = serial0;
        }

        void checkImuError(MPUIMU::Error_t errid)
        {
            switch (errid) {

                case MPUIMU::ERROR_IMU_ID:
                    error("Bad device ID");
                    break;
                case MPUIMU::ERROR_SELFTEST:
                    error("Failed self-test");
                    break;
                default:
                    break;
            }
        }

        // Board class overrides

        void writeMotor(uint8_t index, float value)
        {
            motor_write(index, value);
        }

        void reboot(void)
        {
            systemResetToBootloader();
        }

        bool getQuaternion(float & qw, float & qx, float & qy, float & qz)
        {
            return SoftwareQuaternionBoard::getQuaternion(qw, qx, qy, qz, getTime());
        }

        bool getGyrometer(float & gx, float & gy, float & gz)
        {
            return SoftwareQuaternionBoard::getGyrometer(gx, gy, gz);
        }

        uint8_t serialNormalAvailable(void)
        {
            return serialRxBytesWaiting(_serial0);
        }

        uint8_t serialNormalRead(void)
        {
            return serialRead(_serial0);
        }

        void serialNormalWrite(uint8_t c)
        {
            serialWrite(_serial0, c);
        }

        virtual void setLed(bool isOn) override;

}; // class Stm32FBoard

void hf::Board::outbuf(char * buf)
{
    for (char *p=buf; *p; p++)
        serialWrite(_serial0, *p);
}
