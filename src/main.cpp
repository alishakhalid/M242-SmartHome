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
lv_obj_t * blink_checkbox;
lv_obj_t * party_checkbox;

//options shown to light up room in just one of these color
lv_obj_t * red_checkbox;
lv_obj_t * white_checkbox;


lv_obj_t * apply_button;

void event_handler_checkbox(struct _lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        if ((obj == livingroom_checkbox || obj == bedroom_checkbox || obj == attic_checkbox) &&
            (lv_checkbox_is_checked(livingroom_checkbox) || lv_checkbox_is_checked(bedroom_checkbox) || lv_checkbox_is_checked(attic_checkbox))) {
            lv_checkbox_set_checked(livingroom_checkbox, obj == livingroom_checkbox ? lv_checkbox_is_checked(livingroom_checkbox) : false);
            lv_checkbox_set_checked(attic_checkbox, obj == attic_checkbox ? lv_checkbox_is_checked(attic_checkbox) : false);
        }
        if ((obj == off_checkbox || obj == red_checkbox || obj == blink_checkbox || obj == party_checkbox || obj == white_checkbox)  &&
            (lv_checkbox_is_checked(off_checkbox) || lv_checkbox_is_checked(red_checkbox) ||
             lv_checkbox_is_checked(blink_checkbox) || lv_checkbox_is_checked(party_checkbox) || lv_checkbox_is_checked(white_checkbox))) {
            lv_checkbox_set_checked(off_checkbox, obj == off_checkbox ? lv_checkbox_is_checked(off_checkbox) : false);
            lv_checkbox_set_checked(red_checkbox, obj == red_checkbox ? lv_checkbox_is_checked(red_checkbox) : false);
            lv_checkbox_set_checked(blink_checkbox, obj == blink_checkbox ? lv_checkbox_is_checked(blink_checkbox) : false);
            lv_checkbox_set_checked(party_checkbox, obj == party_checkbox ? lv_checkbox_is_checked(party_checkbox) : false);
            lv_checkbox_set_checked(white_checkbox, obj == white_checkbox ? lv_checkbox_is_checked(white_checkbox) : false);
        }

        bool is_ready = (
        ((
        (lv_checkbox_is_checked(livingroom_checkbox) && !lv_checkbox_is_checked(bedroom_checkbox) && !lv_checkbox_is_checked(attic_checkbox)) ||
        (!lv_checkbox_is_checked(livingroom_checkbox) && !lv_checkbox_is_checked(attic_checkbox) && lv_checkbox_is_checked(bedroom_checkbox)) ||
        (lv_checkbox_is_checked(attic_checkbox) && !lv_checkbox_is_checked(livingroom_checkbox) && !lv_checkbox_is_checked(bedroom_checkbox))
      ) &&
      (
        (!lv_checkbox_is_checked(off_checkbox) && lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blink_checkbox) && !lv_checkbox_is_checked(party_checkbox) && !lv_checkbox_is_checked(white_checkbox)) ||
        (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(party_checkbox) && lv_checkbox_is_checked(blink_checkbox) && !lv_checkbox_is_checked(white_checkbox)) ||
        (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(red_checkbox) && lv_checkbox_is_checked(party_checkbox) && !lv_checkbox_is_checked(blink_checkbox) && !lv_checkbox_is_checked(white_checkbox)) ||
        (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(party_checkbox) && !lv_checkbox_is_checked(blink_checkbox) && lv_checkbox_is_checked(white_checkbox)) 

       ) 
      ) || (
        (lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blink_checkbox) && !lv_checkbox_is_checked(party_checkbox) && !lv_checkbox_is_checked(white_checkbox))
      ));
        lv_obj_set_click(apply_button, is_ready);
        lv_obj_set_state(apply_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
    }
}

void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    Serial.println("pressed");
    uint8_t led_start = 0;
    uint8_t led_end = 0;
        // Check which room is selected
    if (lv_checkbox_is_checked(livingroom_checkbox)) {
      // Room 1 selected
      led_start = 0;
      led_end = 15;
    } else if (lv_checkbox_is_checked(bedroom_checkbox)) {
      // Room 2 selected
      led_start = 16;
      led_end = 26;
    } else if (lv_checkbox_is_checked(attic_checkbox)) {
      // Room 3 selected
      led_start = 27;
      led_end = 30;
    }
  
    CRGB color = lv_checkbox_is_checked(red_checkbox) ? CRGB::Red ? lv_checkbox_is_checked(white_checkbox) : CRGB::White : CRGB::White;
    uint8_t state = SIDELED_STATE_OFF;

    if (lv_checkbox_is_checked(red_checkbox) || lv_checkbox_is_checked(white_checkbox)) state = SIDELED_STATE_ON;
    if (lv_checkbox_is_checked(blink_checkbox)) state = SIDELED_STATE_BLINK;
    if (lv_checkbox_is_checked(party_checkbox)) state = SIDELED_STATE_FADE;
    if (lv_checkbox_is_checked(off_checkbox)) state = SIDELED_STATE_OFF;
      
     set_sideled_color(led_start, led_end, color);
     set_sideled_state(led_start, led_end, state);
}}

void init_gui_elements() {
  add_label("Choose room", 13, 11);
  livingroom_checkbox = add_checkbox("Livingroom", 13, 31, event_handler_checkbox);
  bedroom_checkbox = add_checkbox("Bedroom", 156, 31, event_handler_checkbox);
  attic_checkbox = add_checkbox("Attic", 16, 64, event_handler_checkbox);

  add_label("2. Select Lightning Mode", 11, 100);
  off_checkbox = add_checkbox("Off", 12, 152, event_handler_checkbox);
  white_checkbox = add_checkbox("White", 12, 120, event_handler_checkbox);
  blink_checkbox = add_checkbox("Blink", 146, 123, event_handler_checkbox);
  party_checkbox = add_checkbox("Party", 229, 121, event_handler_checkbox);
  red_checkbox = add_checkbox("Red", 86, 152, event_handler_checkbox);

  apply_button = add_button("Apply", event_handler_button, 180, 169, 120, 60);
  
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
