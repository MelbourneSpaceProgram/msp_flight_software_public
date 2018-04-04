#ifndef SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_
#define SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_

#include <external/sgp4/sgp4.h>
#include <src/messages/Tle.pb.h>

class LocationEstimator {
   public:
    LocationEstimator();
    void StoreTle(Tle tle);
    void UpdateLocation(double tsince_millis);
    double GetLattitudeGeodeticDegrees() const;
    double GetLongitudeDegrees() const;
    double GetAltitudeAboveEllipsoidKm() const;

   private:
    elsetrec satrec;
    double lattitude_geodetic_degrees;
    double longitude_degrees;
    double altitude_above_ellipsoid_km;
};

#endif  // SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_
