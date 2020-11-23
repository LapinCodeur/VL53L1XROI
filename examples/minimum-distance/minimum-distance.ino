/*******************************************************************************

The API files are from the STSW-IMG007 VL53L1X API from
ST, available here:

http://www.st.com/content/st_com/en/products/embedded-software/proximity-sensors-software/stsw-img007.html

It is also using fonctions from the ultra lite driver and implement STSW-IMG017 following the AN5191 application note.

********************************************************************************

COPYRIGHT(c) 2017 STMicroelectronics
COPYRIGHT(c) 2018 Pololu Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. Neither the name of STMicroelectronics nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/

#include <Wire.h>
#include "VL53L1X-ROI.h"

VL53L1X sensor;

void setup()
{
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
  delay(2000);
  if (!sensor.init()) Serial.println("A mistake was made");
}

void loop()
{
  //int exectime = millis();
  VL53L1X_coordinate coord[13];

  sensor.readVerticalScan(coord, FULL_13);

  for(int zonetoprint = 0; zonetoprint < NumOfZonesPerSensor; zonetoprint++)
  {
    Serial.print(coord[zonetoprint].x);
    Serial.print(" , ");
    Serial.print(coord[zonetoprint].y);
    Serial.print("\t");
  }
  Serial.print("Minimum distance : ");
  VL53L1X_coordinate distance = sensor.readMinimum();
  Serial.print(distance.x);
  Serial.print(" , ");
  Serial.print(distance.y);
  /*exectime = millis() - exectime;
  Serial.print("time : ");
  Serial.print(exectime);
  Serial.println(" ms");*/
  Serial.println();
}
