/**
 *  Interface class beteen C++ and C code for the VL53L1X
 */

#ifndef VL53L1X_ROI_h
#define VL53L1X_ROI_h

#include "Arduino.h"
#include "VL53L1X_base.h"

// Timing budget set through VL53L1_SetMeasurementTimingBudgetMicroSeconds().
#define MEASUREMENT_BUDGET_MS 20//20

// Interval between measurements, set through
// VL53L1_SetInterMeasurementPeriodMilliSeconds(). According to the API user
// manual (rev 2), "the minimum inter-measurement period must be longer than the
// timing budget + 4 ms."
#define INTER_MEASUREMENT_PERIOD_MS 24//33
#define TotalWidthOfSPADS           16
#define WidthOfSPADsPerZone           4
#define NumOfSPADsShiftPerZone          1
#define HorizontalFOVofSensor         19.09
#define SingleSPADFOV             (HorizontalFOVofSensor/TotalWidthOfSPADS)
#define NumOfZonesPerSensor           (((TotalWidthOfSPADS - WidthOfSPADsPerZone) / NumOfSPADsShiftPerZone) + 1)
#define StartingZoneAngle           (WidthOfSPADsPerZone / 2 * SingleSPADFOV)
#define ZoneFOVChangePerStep          (SingleSPADFOV * NumOfSPADsShiftPerZone)
#define FOVCenter 199
#define FOV_27 16
#define FOV_20 8
#define FOV_15 4
#define FULL_13 1
#define HALF_7 2
#define QUART_5 3
#define MIN_3 6

/**
 * A new type to handle data from the VL53L1X
 * Only use with data from scanning functions
 */
typedef struct {
  int x;
  int y;
  uint16_t distance;
  float angle;
}VL53L1X_coordinate;

class VL53L1X
{
public:
  /**
 * A constructor.
 */
  VL53L1X();
  /**
 * A method to initialize our sensor.
 * Only use after the declaration of the wire library.
 * @return a boolean that should be equal to zero, otherwise you could have some trouble getting data back
 */
  bool init();
  /**
 * A method to get values for an horizontal scanning.
 * @param *posxy a pointer to a table of type VL53L1X_coordinate, the table is used to send back data from the method
 * @param incr an uint8_t use to set the number of shift and therefor, the number of values send back by the method
 */
  void readHorizontalScan(VL53L1X_coordinate *posxy, uint8_t incr = NumOfSPADsShiftPerZone, bool blocked = true);
  /**
 * A method to get values for a vertical scanning.
 * @param *posxy a pointer to a table of type VL53L1X_coordinate, the table is used to send back data from the method
 * @param incr an uint8_t use to set the number of shift and therefor, the number of values send back by the method
 */
  void readVerticalScan(VL53L1X_coordinate *posxy, uint8_t incr = NumOfSPADsShiftPerZone, bool blocked = true);
  /**
 * A method to get values for a vertical or an horizontal scanning.
 * @param *posxy a pointer to a table of type VL53L1X_coordinate, the table is used to send back data from the method
 * @param *zone_center a pointer to a table of type uint8_t that contains values of ROI's center
 * @param width an uint8_t use to set the width of our ROI
 * @param heigth an uint8_t use to set the height of our ROI
 * @param incr an uint8_t use to set the number of shift and therefor, the number of values send back by the method
 * @param blocked a boolean use to set if the sensor need to do one measure at the time or all the sensor in one go
 */
  void readScan(VL53L1X_coordinate *posxy, uint8_t *zone_center, uint8_t width, uint8_t height, uint8_t incr, bool blocked);

  /**
  * A method to get if the measurement of readHorizontalScan of readVerticalScan is complete
  * @return a boolean equal to true if a scan has been complete.
  */
  bool isScanComplete(void);
  /**
 * A method to get the position of the closest point.
 * Should be called after a readHorizontalScan or a readVerticalScan or a readScan
 * @return the closest point in type VL53L1X_coordinate
 */
  VL53L1X_coordinate readMinimum();
  /**
 * A method to get one measurement.
 * @param FOV an uint8_t use to set the angle of view
 * @return an uint16_t of the measured distance in mm
 */
  uint16_t readSimpleRanging(uint8_t FOV = FOV_27);
  /**
  * An enum to help setting the desired FOV.
  */
  //enum FOV : uint8_t {FOV_27 = 16, FOV_20 = 8, FOV_15 = 4};
  /**
  * An enum to help setting the new increment to match the desired number of values send back.
  */
  //enum increment : uint8_t {FULL_13 = 1, HALF_7 = 2, QUART_5 = 3, MIN_3 = 6};
  VL53L1X_BASE vl53base;

private:
  void stockData(uint8_t currentzone, VL53L1X_coordinate *pos, uint8_t counter);
  VL53L1X_coordinate minimum;//pos[NumOfZonesPerSensor];
  uint8_t zone_center_x[NumOfZonesPerSensor] = {247,239,231,223,215,207,199,191,183,175,167,159,151};
  uint8_t zone_center_y[NumOfZonesPerSensor] = {193,194,195,196,197,198,199,63,62,61,60,59,58};
  uint8_t Zone = 0;
};
#endif
