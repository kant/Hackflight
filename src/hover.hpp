/*
   hover.hpp : PID-based hover

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

#include "debug.hpp"
#include "datatypes.hpp"
#include "state.hpp"
#include "receiver.hpp"

namespace hf {


    class Hover {

        friend class Hackflight;

        public:

        Hover(float throttleScale)
        {
            _throttleScale = throttleScale;
        }

        protected:

        void modifyDemands(State & state, demands_t & demands) 
        {
            // Throttle
            demands.throttle = 
                ((abs(demands.throttle) > Receiver::THROTTLE_DEADBAND) ?  // Outside throttle deaband,
                 _throttleScale*demands.throttle :                        // allow throttle to raise/lower vehicle.
                 -state.variometer);                                      // Inside deadband, move to oppose variometer.

            // Pitch/roll
            //Debug::printf("forward: %+2.2f    rightward: %+2.2f\n", state.velocityForward, state.velocityRightward);
            demands.pitch -= state.velocityForward;
            demands.roll  -= state.velocityRightward;
        }

        private:

        float _throttleScale;

    };  // class Hover

} // namespace
