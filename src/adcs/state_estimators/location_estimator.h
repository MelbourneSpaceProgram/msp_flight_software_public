#ifndef SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_
#define SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_

#include <external/sgp4/sgp4.h>
#include <src/messages/Tle.pb.h>
#include <ti/sysbios/knl/Mailbox.h>

class LocationEstimator {
   public:
    LocationEstimator();
    void StoreTle(Tle tle);
    void UpdateLocation(double tsince_millis);
    double GetLattitudeGeodeticDegrees() const;
    double GetLongitudeDegrees() const;
    double GetAltitudeAboveEllipsoidKm() const;
    elsetrec GetSatrec() const;
    void RequestTleFromDebugClient();
    static void SetTleUpdateUplinkMailboxHandle(
        Mailbox_Handle tle_update_uplink_mailbox_handle);
    bool CheckForUpdatedTle();
    static Mailbox_Params tle_update_uplink_mailbox_params;

   private:
    elsetrec satrec;
    double lattitude_geodetic_degrees;
    double longitude_degrees;
    double altitude_above_ellipsoid_km;
    static Mailbox_Handle tle_update_uplink_mailbox_handle;
};

#endif  // SRC_ADCS_STATE_ESTIMATORS_LOCATION_ESTIMATOR_H_
