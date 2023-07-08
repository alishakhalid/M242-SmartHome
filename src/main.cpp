#include "view.h"
#include "networking.h"
#include "sideled.h"


void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event);
void event_handler_button(struct _lv_obj_t * obj, lv_event_t event);
void init_gui_elements();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

unsigned long next_lv_task = 0;

lv_obj_t * led;

lv_obj_t * red_checkbox;
lv_obj_t * blue_checkbox;

lv_obj_t * off_checkbox;
lv_obj_t * on_checkbox;
lv_obj_t * blink_checkbox;

lv_obj_t * left_button;
lv_obj_t * right_button;

void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED ) {
    if(
      (obj == red_checkbox || obj == blue_checkbox) &&
      (lv_checkbox_is_checked(red_checkbox) || lv_checkbox_is_checked(blue_checkbox))
      ) {
      lv_checkbox_set_checked(blue_checkbox, obj == blue_checkbox ? lv_checkbox_is_checked(blue_checkbox) : false);
      lv_checkbox_set_checked(red_checkbox, obj == red_checkbox ? lv_checkbox_is_checked(red_checkbox) : false);
    }
    if(
      (obj == off_checkbox || obj == on_checkbox || obj == blink_checkbox) &&
      (lv_checkbox_is_checked(off_checkbox) || lv_checkbox_is_checked(on_checkbox) || lv_checkbox_is_checked(blink_checkbox))
      ) {
      lv_checkbox_set_checked(off_checkbox, obj == off_checkbox ? lv_checkbox_is_checked(off_checkbox) : false);
      lv_checkbox_set_checked(on_checkbox, obj == on_checkbox ? lv_checkbox_is_checked(on_checkbox) : false);
      lv_checkbox_set_checked(blink_checkbox, obj == blink_checkbox ? lv_checkbox_is_checked(blink_checkbox) : false);
    }
    bool is_ready = (
        ((
        (lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blue_checkbox)) ||
        (!lv_checkbox_is_checked(red_checkbox) && lv_checkbox_is_checked(blue_checkbox))
      ) &&
      (
        (!lv_checkbox_is_checked(off_checkbox) && lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox)) ||
        (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && lv_checkbox_is_checked(blink_checkbox))
      )) || (
        (lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox))
      ));
    lv_obj_set_click(left_button, is_ready);
    lv_obj_set_click(right_button, is_ready);
    lv_obj_set_state(left_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
    lv_obj_set_state(right_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
  } 
}

void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
  if(event == LV_EVENT_PRESSED) {
    uint8_t led_start = (obj == right_button ? 0 : 5);
    uint8_t led_end = (obj == right_button ? 5 : 10);
    CRGB color = lv_checkbox_is_checked(blue_checkbox) ? CRGB::Blue : CRGB::Red;
    uint8_t state = SIDELED_STATE_OFF;
    if(lv_checkbox_is_checked(on_checkbox)) state = SIDELED_STATE_ON;
    if(lv_checkbox_is_checked(blink_checkbox)) state = SIDELED_STATE_BLINK;
    set_sideled_color(led_start,led_end, color);
    set_sideled_state(led_start,led_end, state);
  }
}

// void init_gui_elements() {
//   add_label("1. Select Color", 10, 10);
//   red_checkbox = add_checkbox("Red", 10, 50, event_handler_checkbox);
//   blue_checkbox = add_checkbox("Blue", 120, 40, event_handler_checkbox);
//   add_label("2. Select Mode", 10, 70);
//   off_checkbox = add_checkbox("Off", 10, 100, event_handler_checkbox);
//   on_checkbox = add_checkbox("On", 120, 100, event_handler_checkbox);
//   blink_checkbox = add_checkbox("Blink", 200, 100, event_handler_checkbox);
//   add_label("3. Apply Color to side:", 10, 140);
//   left_button = add_button("Apply Left", event_handler_button, 0, 170, 150, 50);
//   right_button = add_button("Apply Right", event_handler_button, 160, 170, 150, 50);

//   lv_obj_set_click(left_button, false);
//   lv_obj_set_click(right_button, false);
//   lv_obj_set_state(left_button, LV_STATE_DISABLED);
//   lv_obj_set_state(right_button, LV_STATE_DISABLED);
// }

lv_obj_t *livingroom_checkbox;
lv_obj_t *bedroom_checkbox;
lv_obj_t *attic_checkbox;
lv_obj_t *green_checkbox;

void set_decision(const char* room, const char* mode, const char* color)
{
    printf("Setting decision: Room: %s, Mode: %s", room, mode);
    
    if (strcmp(mode, "Color") == 0)
    {
        printf(", Color: %s\n", color);
        // Code to control the LED with the specified color
    }
    else
    {
        printf("\n");
        // Code to control the LED based on other modes (e.g., blink, party)
    }
}

void setup_gui()
{
    lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label1, "1. Select Room");
    lv_obj_set_pos(label1, 10, 10);

    livingroom_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(livingroom_checkbox, "Living Room");
    lv_obj_set_pos(livingroom_checkbox, 10, 50);

    bedroom_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(bedroom_checkbox, "Bedroom");
    lv_obj_set_pos(bedroom_checkbox, 120, 40);

    attic_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(attic_checkbox, "Attic");
    lv_obj_set_pos(attic_checkbox, 200, 40);

    lv_obj_t *label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label2, "2. Select Mode");
    lv_obj_set_pos(label2, 10, 90);

    blink_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(blink_checkbox, "Blink");
    lv_obj_set_pos(blink_checkbox, 10, 130);

    party_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(party_checkbox, "Party");
    lv_obj_set_pos(party_checkbox, 120, 130);

    color_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(color_checkbox, "Color");
    lv_obj_set_pos(color_checkbox, 220, 130);

    red_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(red_checkbox, "Red");
    lv_obj_set_pos(red_checkbox, 10, 170);

    green_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(green_checkbox, "Green");
    lv_obj_set_pos(green_checkbox, 120, 170);

    blue_checkbox = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(blue_checkbox, "Blue");
    lv_obj_set_pos(blue_checkbox, 220, 170);

    lv_obj_set_hidden(red_checkbox, true);
    lv_obj_set_hidden(green_checkbox, true);
    lv_obj_set_hidden(blue_checkbox, true);

    lv_obj_t *label3 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label3, "3. Apply Color to Side:");
    lv_obj_set_pos(label3, 10, 210);

    left_button = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(left_button, 0, 250);
    lv_obj_set_size(left_button, 150, 50);
    lv_btn_set_checkable(left_button, true);
    lv_obj_t *label_left = lv_label_create(left_button, NULL);
    lv_label_set_text(label_left, "Apply Left");

    right_button = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(right_button, 160, 250);
    lv_obj_set_size(right_button, 150, 50);
    lv_btn_set_checkable(right_button, true);
    lv_obj_t *label_right = lv_label_create(right_button, NULL);
    lv_label_set_text(label_right, "Apply Right");

    lv_obj_set_event_cb(left_button, handle_button);
    lv_obj_set_event_cb(right_button, handle_button);

    lv_obj_set_hidden(left_button, true);
    lv_obj_set_hidden(right_button, true);
}

static lv_res_t handle_button(lv_obj_t *button, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (lv_obj_get_state(left_button, LV_BTN_PART_MAIN) & LV_STATE_CHECKED)
        {
            // Get the selected room
            char room[15];
            if (lv_checkbox_is_checked(livingroom_checkbox))
            {
                strcpy(room, "Living Room");
            }
            else if (lv_checkbox_is_checked(bedroom_checkbox))
            {
                strcpy(room, "Bedroom");
            }
            else if (lv_checkbox_is_checked(attic_checkbox))
            {
                strcpy(room, "Attic");
            }

            // Get the selected mode
            char mode[10];
            if (lv_checkbox_is_checked(blink_checkbox))
            {
                strcpy(mode, "Blink");
            }
            else if (lv_checkbox_is_checked(party_checkbox))
            {
                strcpy(mode, "Party");
            }
            else if (lv_checkbox_is_checked(color_checkbox))
            {
                strcpy(mode, "Color");
            }

            // Get the selected color if mode is Color
            char color[10];
            if (strcmp(mode, "Color") == 0)
            {
                if (lv_checkbox_is_checked(red_checkbox))
                {
                    strcpy(color, "Red");
                }
                else if (lv_checkbox_is_checked(green_checkbox))
                {
                    strcpy(color, "Green");
                }
                else if (lv_checkbox_is_checked(blue_checkbox))
                {
                    strcpy(color, "Blue");
                }
            }

            // Set the decision and light up the LED accordingly
            if (strcmp(mode, "Color") == 0)
            {
                set_decision(room, mode, color);
            }
            else
            {
                set_decision(room, mode, "");
            }
        }
    }
    return LV_RES_OK;
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