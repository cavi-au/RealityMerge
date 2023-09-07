/**
* \author Jason A. Kankiewicz
*/
// third-party

// regional
#include <core/object/class_db.h>

// local
#include "reality_merge.h"
#include "register_types.h"


void initialize_reality_merge_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    GDREGISTER_CLASS(RMDocument);
}


void uninitialize_reality_merge_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}
