/*
   quaternion.hpp : Support for treating quaternion as a sensor
   
   Supports IMUs like EM7180 SENtral Sensor Fusion solution, where 
   quaternion is computed in hardware, and simulation platforms like
   UE4, where quaternion is provided by physics engine. For other IMUs 
   and simulators, you can use quaternion-filter classes in filters.hpp.

   Copyright (c) 2018 Simon D. Levy

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

#include <math.h>

#include "sensors/surfacemount.hpp"

namespace hf {

    class Quaternion : public SurfaceMountSensor {

        friend class Hackflight;

        private:

            float _qw = 0;
            float _qx = 0;
            float _qy = 0;
            float _qz = 0;

        protected:

            Quaternion(void)
            {
                _qw = 0;
                _qx = 0;
                _qy = 0;
                _qz = 0;
            }

            virtual void modifyState(state_t & state, float time) override
            {
                (void)time;

                computeEulerAngles(_qw, _qx, _qy, _qz, state.eulerAngles);

                // Convert heading from [-pi,+pi] to [0,2*pi]
                if (state.eulerAngles[2] < 0) {
                    state.eulerAngles[2] += 2*M_PI;
                }
            }

            virtual bool ready(float time) override
            {
                (void)time;

                return board->getQuaternion(_qw, _qx, _qy, _qz);
            }

        public:

            // We make this public so we can use it in different sketches
            static void computeEulerAngles(float qw, float qx, float qy, float qz, float euler[3])
            {
                euler[0] = atan2(2.0f*(qw*qx+qy*qz),qw*qw-qx*qx-qy*qy+qz*qz);
                euler[1] =  asin(2.0f*(qx*qz-qw*qy));
                euler[2] = atan2(2.0f*(qx*qy+qw*qz),qw*qw+qx*qx-qy*qy-qz*qz);

                euler[0] = int(euler[0]*1000)/1000.0;
                euler[1] = int(euler[1]*1000)/1000.0;
                euler[2] = int(euler[2]*1000)/1000.0;
            }

    };  // class Quaternion

} // namespace hf
