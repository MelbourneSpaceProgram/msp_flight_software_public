#include <src/database/flash_memory/flash_storables/flash_storable_tle.h>

FlashStorableTle::FlashStorableTle(Tle& tle)
    : FlashStorable<FlashStorableTleStruct>(
          kFlashStorableTleFlashStorageAddress) {
    tle_data.mean_motion = tle.mean_motion;
    tle_data.mean_anomaly = tle.mean_anomaly;
    tle_data.inclination = tle.inclination;
    tle_data.raan = tle.raan;
    tle_data.bstar_drag = tle.bstar_drag;
    tle_data.epoch = tle.epoch;
    tle_data.eccentricity_1e7 = tle.eccentricity_1e7;
    tle_data.argument_of_perigee = tle.argument_of_perigee;
}

void FlashStorableTle::ConvertToFlashStorableStruct(
    FlashStorableTleStruct* flash_storable_tle_struct_ptr) {
    flash_storable_tle_struct_ptr->mean_motion = tle_data.mean_motion;
    flash_storable_tle_struct_ptr->mean_anomaly = tle_data.mean_anomaly;
    flash_storable_tle_struct_ptr->inclination = tle_data.inclination;
    flash_storable_tle_struct_ptr->raan = tle_data.raan;
    flash_storable_tle_struct_ptr->bstar_drag = tle_data.bstar_drag;
    flash_storable_tle_struct_ptr->eccentricity_1e7 = tle_data.eccentricity_1e7;
    flash_storable_tle_struct_ptr->argument_of_perigee =
        tle_data.argument_of_perigee;
}

void FlashStorableTle::UpdateFromFlashStorableStruct(
    FlashStorableTleStruct* flash_storable_tle_struct_ptr) {
    tle_data.mean_motion = flash_storable_tle_struct_ptr->mean_motion;
    tle_data.mean_anomaly = flash_storable_tle_struct_ptr->mean_anomaly;
    tle_data.inclination = flash_storable_tle_struct_ptr->inclination;
    tle_data.raan = flash_storable_tle_struct_ptr->raan;
    tle_data.bstar_drag = flash_storable_tle_struct_ptr->bstar_drag;
    tle_data.eccentricity_1e7 = flash_storable_tle_struct_ptr->eccentricity_1e7;
    tle_data.argument_of_perigee =
        flash_storable_tle_struct_ptr->argument_of_perigee;
}
