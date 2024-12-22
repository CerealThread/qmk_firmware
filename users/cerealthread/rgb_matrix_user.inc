// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(transflag)
// RGB_MATRIX_EFFECT(my_cool_effect2)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// e.g: A simple effect, self-contained within a single method
static bool transflag(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  HSV hsv = rgb_matrix_config.hsv;
  hsv.h = 137;
  RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv);
  hsv.h = 244;
  RGB rgb2 = rgb_matrix_hsv_to_rgb(hsv);
  hsv.s = 0;
  RGB rgb3 = rgb_matrix_hsv_to_rgb(hsv);
  for (uint8_t i = led_min; i < led_max; i++) {

    if (HAS_FLAGS(g_led_config.flags[i], 0x80) || HAS_FLAGS(g_led_config.flags[i], 0x10) || HAS_FLAGS(g_led_config.flags[i], 0x20)) { // 0x01 == LED_FLAG_MODIFIER
        rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
    }
    else if (HAS_FLAGS(g_led_config.flags[i], 0x40)) { // 0x01 == LED_FLAG_MODIFIER
        rgb_matrix_set_color(i, rgb2.r, rgb2.g, rgb2.b);
    }
    else if (HAS_FLAGS(g_led_config.flags[i], 0x4)) { // 0x01 == LED_FLAG_MODIFIER
        rgb_matrix_set_color(i, rgb3.r, rgb3.g, rgb3.b);
    }

        // if(row % 5 == 0)
        //     rgb_matrix_set_color(col, rgb1.r, rgb1.g, rgb1.b);
        // }
        // else if((row-4) % 5 == 0)
        // (
        //     if (col != 0 && col != 1) {
        //         rgb_matrix_set_color(col, rgb1.r, rgb1.g, rgb1.b);
        //     }
        // )
        // else if((row-1) % 5 == 0 || (row-3) % 5 == 0) {
        //     rgb_matrix_set_color(col, rgb2.r, rgb2.g, rgb2.b);
        // }
        // else if((row-2) % 5 == 0) {
        //     rgb_matrix_set_color(col, rgb3.r, rgb3.g, rgb3.b);
        // }
  }
  return rgb_matrix_check_finished_leds(led_max);
}

// static bool transflag(effect_params_t* params) {
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   HSV hsv = rgb_matrix_config.hsv;
//   hsv.h = 231;
//   RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv);
//   hsv.h = 36;
//   RGB rgb2 = rgb_matrix_hsv_to_rgb(hsv);
//   hsv.h = 140;
//   RGB rgb3 = rgb_matrix_hsv_to_rgb(hsv);
//   for (uint8_t i = led_min; i < led_max; i++) {

//     if (HAS_FLAGS(g_led_config.flags[i], 0x80) || HAS_FLAGS(g_led_config.flags[i], 0x10) || HAS_FLAGS(g_led_config.flags[i], 0x20)) { // 0x01 == LED_FLAG_MODIFIER
//         rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
//     }
//     else if (HAS_FLAGS(g_led_config.flags[i], 0x40)) { // 0x01 == LED_FLAG_MODIFIER
//         rgb_matrix_set_color(i, rgb2.r, rgb2.g, rgb2.b);
//     }
//     else if (HAS_FLAGS(g_led_config.flags[i], 0x4)) { // 0x01 == LED_FLAG_MODIFIER
//         rgb_matrix_set_color(i, rgb3.r, rgb3.g, rgb3.b);
//     }

//         // if(row % 5 == 0)
//         //     rgb_matrix_set_color(col, rgb1.r, rgb1.g, rgb1.b);
//         // }
//         // else if((row-4) % 5 == 0)
//         // (
//         //     if (col != 0 && col != 1) {
//         //         rgb_matrix_set_color(col, rgb1.r, rgb1.g, rgb1.b);
//         //     }
//         // )
//         // else if((row-1) % 5 == 0 || (row-3) % 5 == 0) {
//         //     rgb_matrix_set_color(col, rgb2.r, rgb2.g, rgb2.b);
//         // }
//         // else if((row-2) % 5 == 0) {
//         //     rgb_matrix_set_color(col, rgb3.r, rgb3.g, rgb3.b);
//         // }
//   }
//   return rgb_matrix_check_finished_leds(led_max);
// }
// // // e.g: A more complex effect, relying on external methods and state, with
// // // dedicated init and run methods
// // static uint8_t some_global_state;
// // static void my_cool_effect2_complex_init(effect_params_t* params) {
// //   some_global_state = 1;
// // }
// // static bool my_cool_effect2_complex_run(effect_params_t* params) {
// //   RGB_MATRIX_USE_LIMITS(led_min, led_max);
// //   for (uint8_t i = led_min; i < led_max; i++) {
// //     rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
// //   }
// //   return rgb_matrix_check_finished_leds(led_max);
// // }
// // static bool my_cool_effect2(effect_params_t* params) {
// //   if (params->init) my_cool_effect2_complex_init(params);
// //   return my_cool_effect2_complex_run(params);
// // }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
