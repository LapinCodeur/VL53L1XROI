#include "VL53L1X-ROI.h"

VL53L1X::VL53L1X()
{

}

bool VL53L1X::init()
{
  bool done = true;

  vl53base.setTimeout(500);
  done = vl53base.init();

  vl53base.setDistanceMode(VL53L1X_BASE::Short);
  vl53base.setMeasurementTimingBudget(MEASUREMENT_BUDGET_MS * 1000);
  vl53base.startContinuous(INTER_MEASUREMENT_PERIOD_MS);

  return done;
}

void VL53L1X::readScan(VL53L1X_coordinate *posxy, uint8_t *zone_center, uint8_t width, uint8_t height, uint8_t incr, bool blocked)
{
  //static uint8_t Zone = 0;
  static uint8_t i = 0;

  if (Zone >= NumOfZonesPerSensor)
  {
    Zone = 0;
    i = 0;
  }

  vl53base.ClearInterrupt();
  vl53base.SetROI(width, height);
  vl53base.SetROICenter(zone_center[Zone]);

  stockData(Zone, posxy, i);
  Zone += incr;
  i++;

  if ((blocked) && (Zone < NumOfZonesPerSensor))  readScan(posxy, zone_center, width, height, incr, blocked);
}

bool VL53L1X::isScanComplete(void)
{
  if (Zone >= NumOfZonesPerSensor)  return true;
  else return false;
}

void VL53L1X::readHorizontalScan(VL53L1X_coordinate *posxy, uint8_t incr, bool blocked)
{
  readScan(posxy, zone_center_x, WidthOfSPADsPerZone, TotalWidthOfSPADS, incr, blocked);
}

void VL53L1X::readVerticalScan(VL53L1X_coordinate *posxy, uint8_t incr, bool blocked)
{
  readScan(posxy, zone_center_y, TotalWidthOfSPADS, WidthOfSPADsPerZone, incr, blocked);
}

VL53L1X_coordinate VL53L1X::readMinimum()
{
  return minimum;
}

//Acquire angle and distance for the current zone
void VL53L1X::stockData(uint8_t currentzone, VL53L1X_coordinate *pos, uint8_t counter)
{
  double PartZoneAngle;
  uint16_t Distance = 0;

  Distance = vl53base.read();

  if (Distance > 60000)
  {
    Distance = 0;
  }
  PartZoneAngle = (StartingZoneAngle + ZoneFOVChangePerStep*currentzone) - (HorizontalFOVofSensor / 2.0);
  pos[counter].angle = PartZoneAngle;
  pos[counter].distance = Distance;
  pos[counter].y = cos((PartZoneAngle / 180) * PI) * Distance;
  pos[counter].x = sin((PartZoneAngle / 180) * PI) * Distance;

  if (currentzone == 0)
  {
    minimum = pos[counter];
  }else if ((minimum.x > pos[counter].x) && (pos[counter].x > 0)) {
    minimum = pos[counter];
  }
}

uint16_t VL53L1X::readSimpleRanging(uint8_t FOV)
{
  uint16_t Distance = 0;

  vl53base.ClearInterrupt();
  vl53base.SetROI(FOV, FOV);
  vl53base.SetROICenter(FOVCenter);

  Distance = vl53base.read();

  if (Distance > 60000)
  {
    Distance = 0;
  }
  return Distance;
}
