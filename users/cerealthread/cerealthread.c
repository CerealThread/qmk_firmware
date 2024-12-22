#include "cerealthread.h"

int CheckLayer(void) {
    if(IS_LAYER_ON(_CSP)) {
        return 6;
    } else if(IS_LAYER_ON(_BLEND)) {
        return 9;
    } else { return 0; }
}

// void
//     if (record->event.pressed) {
//       if (get_mods() && MOD_MASK_SHIFT && !checkBlendLayer) {
//         register_code(KC_0);
//       } else {
//         register_code(KC_P0);
//       }
//     } else {
//         unregister_code(KC_0);
//         unregister_code(KC_P0);
//     }
//     return false;
//     break;
