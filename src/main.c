#include <pebble.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

static Window *s_main_window;
//static TextLayer *s_time_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_date_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_battery_layer;

static GFont s_time_font;
static GFont s_date_font;
static GFont s_weather_font;
static GFont s_battery_font;

//static BitmapLayer *s_background_layer;
//static GBitmap *s_background_bitmap;

// Store incoming information
static char temperature_buffer[8];
static char conditions_buffer[32];
static char weather_layer_buffer[32];


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  //static char buffer[] = "00:00";
  //static char buffer[8];
  static char hour_layer_buffer[3];
  static char minute_layer_buffer[3];
  static char date_layer_buffer[32];
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24h hour format
    //strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
    strftime(hour_layer_buffer, sizeof(hour_layer_buffer), "%H", tick_time);
  } else {
    //Use 12 hour format
    //strftime(buffer, sizeof(buffer), "%I:%M", tick_time);
    strftime(hour_layer_buffer, sizeof(hour_layer_buffer), "%I", tick_time);
  }
  strftime(minute_layer_buffer, sizeof(minute_layer_buffer), "%M", tick_time);

  
  // Display this time on the TextLayer
  //text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_hour_layer, hour_layer_buffer);
  text_layer_set_text(s_minute_layer, minute_layer_buffer);
  
  // Write the current date into the buffer
  strftime(date_layer_buffer, sizeof(date_layer_buffer), "%a %d %b", tick_time);

  // Display this date on the TextLayer
  text_layer_set_text(s_date_layer, date_layer_buffer);

}

static void battery_handler(BatteryChargeState new_state) {
  // Write to buffer and display
  static char s_battery_buffer[32];
  
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  text_layer_set_text(s_battery_layer, s_battery_buffer);
}

static void main_window_load(Window *window) {
/*
  //Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_TUNNEL);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
*/
  
  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPENSANS_BOLD_64));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPENSANS_LIGHT_20));
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPENSANS_LIGHT_20));
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPENSANS_LIGHT_20));

  // Create date layer TextLayer
  s_date_layer = text_layer_create(GRect(0, 0, 139, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  text_layer_set_text(s_date_layer, "");
  text_layer_set_font(s_date_layer, s_date_font);

  /*
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorFromRGB(255, 0, 0));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  text_layer_set_text(s_time_layer, "");
  text_layer_set_font(s_time_layer, s_time_font);
*/
  
  // Create hour TextLayer
  s_hour_layer = text_layer_create(GRect(5, 10, 139, 70));
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorFromRGB(255, 0, 0));
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentRight);
  text_layer_set_text(s_hour_layer, "");
  text_layer_set_font(s_hour_layer, s_time_font);
  
  // Create minute TextLayer
  s_minute_layer = text_layer_create(GRect(5, 67, 139, 70));
  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorFromRGB(255, 0, 0));
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentRight);
  text_layer_set_text(s_minute_layer, "");
  text_layer_set_font(s_minute_layer, s_time_font);
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 130, 144, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
  text_layer_set_text(s_weather_layer, ""); 
  text_layer_set_font(s_weather_layer, s_weather_font);

  // Create battery TextLayer
  s_battery_layer = text_layer_create(GRect(0, 20, 144, 25));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
  text_layer_set_text(s_battery_layer, ""); 
  text_layer_set_font(s_battery_layer, s_battery_font);
  
  // Add layers as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
  
  // Get the current battery level
  battery_handler(battery_state_service_peek());

  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload GFonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_weather_font);

  //Destroy GBitmap
  //gbitmap_destroy(s_background_bitmap);

  //Destroy BitmapLayer
  //bitmap_layer_destroy(s_background_layer);
  
  // Destroy TextLayers
  text_layer_destroy(s_date_layer);
  //text_layer_destroy(s_time_layer);
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_battery_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
  
    // Send the message!
    app_message_outbox_send();
  }
  
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Specify Window color
  window_set_background_color(s_main_window, GColorBlack);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Subscribe to the Battery State Service
  battery_state_service_subscribe(battery_handler);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}