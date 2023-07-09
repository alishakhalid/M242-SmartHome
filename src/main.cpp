#include "view.h"
#include "networking.h"
#include "sideled.h"


void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event);
void event_handler_button(struct _lv_obj_t * obj, lv_event_t event);
void init_gui_elements();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

unsigned long next_lv_task = 0;

lv_obj_t * led;

lv_obj_t * livingroom_checkbox;
lv_obj_t * bedroom_checkbox;
lv_obj_t * attic_checkbox;

lv_obj_t * off_checkbox;
//if one color chosen new options appear: red, green, blue
lv_obj_t * one_color_checkbox;
lv_obj_t * blink_checkbox;
lv_obj_t * party_checkbox;

//options shown to light up room in just one of these color
lv_obj_t * red_checkbox;
lv_obj_t * green_checkbox;
lv_obj_t * blue_checkbox;


lv_obj_t * apply_button;

// void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event) {
//   if(event == LV_EVENT_VALUE_CHANGED ) {
//     if(
//       (obj == red_checkbox || obj == blue_checkbox) &&
//       (lv_checkbox_is_checked(red_checkbox) || lv_checkbox_is_checked(blue_checkbox))
//       ) {
//       lv_checkbox_set_checked(blue_checkbox, obj == blue_checkbox ? lv_checkbox_is_checked(blue_checkbox) : false);
//       lv_checkbox_set_checked(red_checkbox, obj == red_checkbox ? lv_checkbox_is_checked(red_checkbox) : false);
//     }
//     if(
//       (obj == off_checkbox || obj == on_checkbox || obj == blink_checkbox) &&
//       (lv_checkbox_is_checked(off_checkbox) || lv_checkbox_is_checked(on_checkbox) || lv_checkbox_is_checked(blink_checkbox))
//       ) {
//       lv_checkbox_set_checked(off_checkbox, obj == off_checkbox ? lv_checkbox_is_checked(off_checkbox) : false);
//       lv_checkbox_set_checked(on_checkbox, obj == on_checkbox ? lv_checkbox_is_checked(on_checkbox) : false);
//       lv_checkbox_set_checked(blink_checkbox, obj == blink_checkbox ? lv_checkbox_is_checked(blink_checkbox) : false);
//     }
//     bool is_ready = (
//         ((
//         (lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blue_checkbox)) ||
//         (!lv_checkbox_is_checked(red_checkbox) && lv_checkbox_is_checked(blue_checkbox))
//       ) &&
//       (
//         (!lv_checkbox_is_checked(off_checkbox) && lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox)) ||
//         (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && lv_checkbox_is_checked(blink_checkbox))
//       )) || (
//         (lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox))
//       ));
//     lv_obj_set_click(apply_button, is_ready);
//     lv_obj_set_click(right_button, is_ready);
//     lv_obj_set_state(apply_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
//     lv_obj_set_state(right_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
//   } 
// }

void event_handler_checkbox(struct _lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        if ((obj == livingroom_checkbox || obj == bedroom_checkbox || obj == attic_checkbox) &&
            (lv_checkbox_is_checked(livingroom_checkbox) || lv_checkbox_is_checked(bedroom_checkbox) || lv_checkbox_is_checked(attic_checkbox))) {
            lv_checkbox_set_checked(livingroom_checkbox, obj == livingroom_checkbox ? lv_checkbox_is_checked(livingroom_checkbox) : false);
            lv_checkbox_set_checked(attic_checkbox, obj == attic_checkbox ? lv_checkbox_is_checked(attic_checkbox) : false);
        }
        if ((obj == off_checkbox || obj == one_color_checkbox || obj == blink_checkbox || obj == party_checkbox) &&
            (lv_checkbox_is_checked(off_checkbox) || lv_checkbox_is_checked(one_color_checkbox) ||
             lv_checkbox_is_checked(blink_checkbox) || lv_checkbox_is_checked(party_checkbox))) {
            lv_checkbox_set_checked(off_checkbox, obj == off_checkbox ? lv_checkbox_is_checked(off_checkbox) : false);
            lv_checkbox_set_checked(one_color_checkbox, obj == one_color_checkbox ? lv_checkbox_is_checked(one_color_checkbox) : false);
            lv_checkbox_set_checked(blink_checkbox, obj == blink_checkbox ? lv_checkbox_is_checked(blink_checkbox) : false);
            lv_checkbox_set_checked(party_checkbox, obj == party_checkbox ? lv_checkbox_is_checked(party_checkbox) : false);
        }
        bool is_ready = (((lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blue_checkbox)) ||
                          (!lv_checkbox_is_checked(red_checkbox) && lv_checkbox_is_checked(blue_checkbox))) &&
                         ((!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(one_color_checkbox) &&
                           lv_checkbox_is_checked(one_color_checkbox) && !lv_checkbox_is_checked(blink_checkbox)) ||
                          (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(one_color_checkbox) &&
                           !lv_checkbox_is_checked(one_color_checkbox) && lv_checkbox_is_checked(blink_checkbox))) ||
                         (lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(one_color_checkbox) &&
                          !lv_checkbox_is_checked(one_color_checkbox) && !lv_checkbox_is_checked(blink_checkbox)));
        lv_obj_set_click(apply_button, is_ready);
        lv_obj_set_state(apply_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
    }
}


// void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
//   if(event == LV_EVENT_PRESSED) {
//     uint8_t led_livingroom = (obj == apply_button ? 0 : 15);
//     uint8_t led_bedroom = (obj == apply_button ? 15 : 26);
//     uint8_t led_lattic = (obj == apply_button ? 26 : 30);
//     CRGB color = lv_checkbox_is_checked(blue_checkbox) ? CRGB::Blue ? lv_checkbox_is_checked(red_checkbox) : CRGB::Red : CRGB::Green;
//     uint8_t state = SIDELED_STATE_OFF;
//     if(lv_checkbox_is_checked(one_color_checkbox)) state = SIDELED_STATE_ON;
//     if(lv_checkbox_is_checked(blink_checkbox)) state = SIDELED_STATE_BLINK;
//     if(lv_checkbox_is_checked(party_checkbox)) state = SIDELED_STATE_FADE;
//     if(lv_checkbox_is_checked(off_checkbox)) state = SIDELED_STATE_OFF;

//     set_sideled_color(led_livingroom,led_lattic, color);
//     set_sideled_state(led_livingroom,led_lattic, state);
//   }
// }

void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    uint8_t led_livingroom = (obj == apply_button ? 0 : 15);
    uint8_t led_bedroom = (obj == apply_button ? 15 : 26);
    uint8_t led_lattic = (obj == apply_button ? 26 : 30);
    CRGB color = lv_checkbox_is_checked(blue_checkbox) ? CRGB::Blue :
                 (lv_checkbox_is_checked(red_checkbox) ? CRGB::Red : CRGB::Green);
    uint8_t state = SIDELED_STATE_OFF;
    if (lv_checkbox_is_checked(one_color_checkbox)) state = SIDELED_STATE_ON;
    if (lv_checkbox_is_checked(blink_checkbox)) state = SIDELED_STATE_BLINK;
    if (lv_checkbox_is_checked(party_checkbox)) state = SIDELED_STATE_FADE;
    if (lv_checkbox_is_checked(off_checkbox)) state = SIDELED_STATE_OFF;

    set_sideled_color(led_livingroom, led_bedroom,led_lattic, color);
    set_sideled_state(led_livingroom, led_bedroom,led_lattic, state);
  }
}


// void init_gui_elements() {
//   add_label("Choose room", 13, 11);
//   livingroom_checkbox = add_checkbox("Livingroom", 13, 31, event_handler_checkbox);
//   bedroom_checkbox = add_checkbox("Bedroom", 156, 31, event_handler_checkbox);
//   attic_checkbox = add_checkbox("Attic", 16, 64, event_handler_checkbox);

//   add_label("2. Select Lightning Mode", 11, 100);
//   off_checkbox = add_checkbox("Off", 12, 252, event_handler_checkbox);
//   one_color_checkbox = add_checkbox("One Color", 12, 120, event_handler_checkbox);
//   blink_checkbox = add_checkbox("Blink", 146, 123, event_handler_checkbox);
//   party_checkbox = add_checkbox("Party", 229, 121, event_handler_checkbox);

//   if(lv_checkbox_is_checked(one_color_checkbox)){
//     add_label("Choose color", 10, 70);
//     red_checkbox = add_checkbox("Red", 13, 207, event_handler_checkbox);
//     blue_checkbox = add_checkbox("Blue", 172, 206, event_handler_checkbox);
//     green_checkbox = add_checkbox("Green", 82, 207, event_handler_checkbox);
//   }

//   //add_label("3. Apply Settings:", 10, 140);
//   apply_button = add_button("Apply", event_handler_button, 244, 197, 70, 30);
//   lv_obj_set_click(apply_button, false);
//   lv_obj_set_state(apply_button, LV_STATE_DISABLED);
// }

void init_gui_elements() {
  add_label("Choose room", 13, 11);
  livingroom_checkbox = add_checkbox("Livingroom", 13, 31, event_handler_checkbox);
  lv_obj_set_event_cb(livingroom_checkbox, event_handler_checkbox);  // Assign event handler

  bedroom_checkbox = add_checkbox("Bedroom", 156, 31, event_handler_checkbox);
  lv_obj_set_event_cb(bedroom_checkbox, event_handler_checkbox);  // Assign event handler

  attic_checkbox = add_checkbox("Attic", 16, 64, event_handler_checkbox);
  lv_obj_set_event_cb(attic_checkbox, event_handler_checkbox);  // Assign event handler

  add_label("2. Select Lightning Mode", 11, 100);
  off_checkbox = add_checkbox("Off", 12, 252, event_handler_checkbox);
  lv_obj_set_event_cb(off_checkbox, event_handler_checkbox);  // Assign event handler

  one_color_checkbox = add_checkbox("One Color", 12, 120, event_handler_checkbox);
  lv_obj_set_event_cb(one_color_checkbox, event_handler_checkbox);  // Assign event handler

  blink_checkbox = add_checkbox("Blink", 146, 123, event_handler_checkbox);
  lv_obj_set_event_cb(blink_checkbox, event_handler_checkbox);  // Assign event handler

  party_checkbox = add_checkbox("Party", 229, 121, event_handler_checkbox);
  lv_obj_set_event_cb(party_checkbox, event_handler_checkbox);  // Assign event handler

  if (lv_checkbox_is_checked(one_color_checkbox)) {
    add_label("Choose color", 10, 70);
    red_checkbox = add_checkbox("Red", 13, 207, event_handler_checkbox);
    lv_obj_set_event_cb(red_checkbox, event_handler_checkbox);  // Assign event handler

    blue_checkbox = add_checkbox("Blue", 172, 206, event_handler_checkbox);
    lv_obj_set_event_cb(blue_checkbox, event_handler_checkbox);  // Assign event handler

    green_checkbox = add_checkbox("Green", 82, 207, event_handler_checkbox);
    lv_obj_set_event_cb(green_checkbox, event_handler_checkbox);  // Assign event handler
  }

  apply_button = add_button("Apply", event_handler_button, 244, 197, 70, 30);
  lv_obj_set_event_cb(apply_button, event_handler_button);  // Assign event handler
  lv_obj_set_click(apply_button, false);
  lv_obj_set_state(apply_button, LV_STATE_DISABLED);
}


// ----------------------------------------------------------------------------
// MQTT callback
// ----------------------------------------------------------------------------

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Parse Payload into String
  char * buf = (char *)malloc((sizeof(char)*(length+1)));
  memcpy(buf, payload, length);
  buf[length] = '\0';
  String payloadS = String(buf);
  payloadS.trim();

  if(String(topic) == "example") {
    if(payloadS == "on") {
      lv_led_on(led);
    }
    if(payloadS == "off") {
      lv_led_off(led);
    }
  }
}


// ----------------------------------------------------------------------------
// UI event handlers
// ----------------------------------------------------------------------------

String buffer = "";

void event_handler_num(struct _lv_obj_t * obj, lv_event_t event) {

}

lv_obj_t * mbox;


// ----------------------------------------------------------------------------
// MAIN LOOP
// ----------------------------------------------------------------------------

void loop() {
  if(next_lv_task < millis()) {
    lv_task_handler();
    next_lv_task = millis() + 5;
  }

  // Uncomment the following lines to enable MQTT
  // mqtt_loop();
}

// ----------------------------------------------------------------------------
// MAIN SETUP
// ----------------------------------------------------------------------------

void setup() {
  init_m5();
  init_display();
  Serial.begin(115200);
  // Uncomment the following lines to enable WiFi and MQTT
  lv_obj_t * wifiConnectingBox = show_message_box_no_buttons("Connecting to you...");
  lv_task_handler();
  delay(5);
  setup_wifi();
  mqtt_init(mqtt_callback);
  close_message_box(wifiConnectingBox);
  init_gui_elements();
  init_sideled();
}
