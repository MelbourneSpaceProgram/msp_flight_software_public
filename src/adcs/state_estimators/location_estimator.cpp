#include <external/nanopb/pb_decode.h>
#include <external/sgp4/sgp4.h>
#include <external/sgp4/sgp4_utils.h>
#include <math.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/satellite.h>
#include <src/messages/LocationReading.pb.h>
#include <src/util/message_codes.h>
#include <src/util/physical_constants.h>
#include <ti/sysbios/BIOS.h>
#include <src/util/satellite_time_source.h>

Mailbox_Handle LocationEstimator::tle_update_uplink_mailbox_handle;
Mailbox_Params LocationEstimator::tle_update_uplink_mailbox_params;

LocationEstimator::LocationEstimator()
    : satrec(),
      lattitude_geodetic_degrees(0),
      longitude_degrees(0),
      altitude_above_ellipsoid_km(0),
      tle_epoch_offset_ms(0),
      tle_is_current(false) {
  // TODO (rskew) read TLE from flash memory
}

void LocationEstimator::SetTleUpdateUplinkMailboxHandle(
    Mailbox_Handle tle_update_uplink_mailbox_handle) {
    LocationEstimator::tle_update_uplink_mailbox_handle =
        tle_update_uplink_mailbox_handle;
}

bool LocationEstimator::CheckForUpdatedTle() {
    Tle tle;
    bool new_tle_received =
        Mailbox_pend(tle_update_uplink_mailbox_handle, &tle, BIOS_NO_WAIT);
    if (new_tle_received) {
        StoreTle(tle);
    }
    return new_tle_received;
}

void LocationEstimator::RequestTleFromDebugClient() {
    Tle tle = RequestNanopbFromSimMacro(Tle, kTleRequestCode);
    StoreTle(tle);
}

bool LocationEstimator::StoreTle(Tle tle) {
    Time current_time = SatelliteTimeSource::GetTime();
    if (current_time.is_valid) {
      Sgp4::InitialisePropagator(tle, satrec);
      tle_epoch_offset_ms = tle.time_since_epoch_ms - current_time.timestamp_ms;
      // TODO (rskew) store TLE in flash, along with tle_epoch_time_ms
      tle_is_current = true;
      return true;
    }
    return false;
}

bool LocationEstimator::UpdateLocation() {
    Time current_time = SatelliteTimeSource::GetTime();
    if (!current_time.is_valid) {
        return false;
    }
    if (current_time.timestamp_ms + tle_epoch_offset_ms > kTleShelfLifeMs) {
      tle_is_current = false;
    }
    if (!tle_is_current) {
      return false;
    }
    double tsince_millis = (double)(current_time.timestamp_ms + tle_epoch_offset_ms);
    double position_true_equator_mean_equinox[3];
    double velocity_true_equator_mean_equinox[3];
    double tsince_minutes = tsince_millis / (1000 * 60);
    Sgp4::Propagate(satrec, tsince_minutes, position_true_equator_mean_equinox,
                    velocity_true_equator_mean_equinox);

    double julian_day_whole = satrec.jdsatepoch;
    // converting tsince_minutes to days
    double julian_day_fraction =
        satrec.jdsatepochF + (tsince_minutes / kMinutesPerDay);

    // TODO (rskew) add comment with reason for adding half a day
    double julian_day =
        julian_day_whole + julian_day_fraction + ((0.5) / (60.0 * 60.0 * 24));

    // convert from TEME to PEF
    double position_psuedo_earth_fixed[3];
    double velocity_psuedo_earth_fixed[3];
    Sgp4Utils::TrueEquatorMeanEquinoxToPsuedoEarthFixed(
        position_true_equator_mean_equinox, velocity_true_equator_mean_equinox,
        julian_day, position_psuedo_earth_fixed, velocity_psuedo_earth_fixed);

    double lattitude_geocentric_radians;
    double lattitude_geodetic_radians;
    double longitude_radians;
    // Using PEF rather then ECEF. This introduces error due to not
    // accounting for polar motion, which is ~20m.
    Sgp4Utils::EcefXyzToLatLong(position_psuedo_earth_fixed,
                                lattitude_geocentric_radians,
                                lattitude_geodetic_radians, longitude_radians,
                                altitude_above_ellipsoid_km);
    lattitude_geodetic_degrees = lattitude_geodetic_radians * (180 / kPi);
    longitude_degrees = longitude_radians * (180 / kPi);
}

double LocationEstimator::GetLattitudeGeodeticDegrees() const {
    return lattitude_geodetic_degrees;
}

double LocationEstimator::GetLongitudeDegrees() const {
    return longitude_degrees;
}

double LocationEstimator::GetAltitudeAboveEllipsoidKm() const {
    return altitude_above_ellipsoid_km;
}

elsetrec LocationEstimator::GetSatrec() const { return satrec; }
