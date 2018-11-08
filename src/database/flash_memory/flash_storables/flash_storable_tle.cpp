#include <src/database/flash_memory/flash_storables/flash_storable_tle.h>

FlashStorableTle::FlashStorableTle(Tle &tle)
    : tle_data.mean_motion(tle.mean_motion),
    tle_data.mean_anomaly(tle.mean_anomaly), tle_data.inclination(
                                                 tle.inclination),
    tle_data.raan(tle.raan), tle_data.bstar_drag(tle.bstar_drag),
    tle_data.epoch(tle.epoch), tle_data.eccentricity_1e7(tle.eccentricity_1e7),
    tle_data.argument_of_perigee(tle.argument_of_perigee) {}

void FlashStorableTle::ConvertToFlashStorableStruct(
    FlashStorableTleStruct flash_storable_tle_struct) {
    // TODO(hugorilla): make this function just return tle_data variable - no
    // need to pass in a flash_storable_struct like this.
    // This struct assignment could be bad.
    flash_storable_tle_struct = tle_data;
}

void FlashStorableTle::UpdateFromFlashStorableStruct(
    FlashStorableTleStruct flash_storable_tle_struct) {
    // TODO(hugorilla): as above, address this struct assignment
    tle_data = flash_storable_tle_struct;
}
