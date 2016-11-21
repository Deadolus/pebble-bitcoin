#include <pebble.h>

#define KEY_FINAL_BALANCE 0
#define KEY_FINAL_BALANCE_COMMA 4
#define KEY_REFRESH_INTERVAL 2
#define KEY_BTC_NAME_1 3

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_wallet_layer;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static GFont s_time_font;
static GFont s_wallet_font;
static ActionBarLayer *s_action_bar;
static GBitmap *s_select_bitmap;
static char name1[40];

//static void tick_handler(struct tm *tick_time, TimeUnits units_changed);

static void removeTrailingZeroes(char* buffer) {
      int i;
      for(i=strlen(buffer); i > 0; i--) {
      if(buffer[i] == '0') {
        buffer[i] = '\0';
      }
      if(buffer[i-1] != '0')
        break;
      }
}

static void updateBalance(int32_t balance, int32_t balance_comma)
{
    static char balance_buffer[76];
    static char comma_buffer [9];

    //static char wallet_layer_buffer[60];
    if(strcmp(name1, "") == 0 ) {
      snprintf(balance_buffer, sizeof(balance_buffer), "Please configure in settings");
    } else {
      if( (balance >= 0) && (balance_comma >= 0) ) {
        snprintf(comma_buffer, sizeof(comma_buffer), "%08ld", (long int) balance_comma);
        removeTrailingZeroes(comma_buffer);
        snprintf(balance_buffer, sizeof(balance_buffer), "%s\n%d.%s", name1, (int)(balance), comma_buffer);

      }
      else
        snprintf(balance_buffer, sizeof(balance_buffer), "%s\nERROR", name1);
    }

    //snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);


    // Assemble full string and display
    //snprintf(wallet_layer_buffer, sizeof(wallet_layer_buffer), "%s", balance_buffer);
    text_layer_set_text(s_wallet_layer, balance_buffer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Store incoming information

    static char conditions_buffer[32];


    // Read tuples for data
    Tuple *balance_tuple = dict_find(iterator, KEY_FINAL_BALANCE);
     Tuple *balance_comma_tuple = dict_find(iterator, KEY_FINAL_BALANCE_COMMA);
    Tuple *refresh_tuple = dict_find(iterator, KEY_REFRESH_INTERVAL);
    Tuple *name1_tuple = dict_find(iterator, KEY_BTC_NAME_1);

    if(name1_tuple) {
        snprintf(name1, sizeof(name1), "%s", name1_tuple->value->cstring);
        persist_write_string(KEY_BTC_NAME_1, name1);
    }
    // If all data is available, use it
    if(balance_tuple && balance_comma_tuple) {
        int32_t balance = balance_tuple->value->int32;
      int32_t balance_comma = balance_comma_tuple->value->int32;
        persist_write_int(KEY_FINAL_BALANCE, balance);
      persist_write_int(KEY_FINAL_BALANCE_COMMA, balance_comma);
        updateBalance(balance, balance_comma);
    }

    s_select_bitmap = gbitmap_create_with_resource(RESOURCE_ID_REFRESH);
    action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_select_bitmap);

    /*if(refresh_tuple) {
      tick_timer_service_subscribe(MINUTE_UNIT*refresh_tuple->value->int32, tick_handler);
      }*/
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

static void sendMessageToPhone() {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
    s_select_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ELLIPSIS);
    action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_select_bitmap);
    // Send the message!
    app_message_outbox_send();
}
/*static void update_time() {
// Get a tm structure
time_t temp = time(NULL); 
struct tm *tick_time = localtime(&temp);

// Write the current hours and minutes into a buffer
static char s_buffer[8];
strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
"%H:%M" : "%I:%M", tick_time);

// Display this time on the TextLayer
text_layer_set_text(s_time_layer, s_buffer);
}*/

/*static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

// Get wallet update every 30 minutes
if(tick_time->tm_min % 30 == 0) {
// Begin dictionary
DictionaryIterator *iter;
app_message_outbox_begin(&iter);

// Add a key-value pair
dict_write_uint8(iter, 0, 0);

// Send the message!
app_message_outbox_send();
text_layer_set_text(s_wallet_layer, "Sent");
}
}*/

static void main_window_load(Window *window) {
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    static char text_buffer[80];

    // Create GBitmap
    /*s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

    // Create BitmapLayer to display the GBitmap
    s_background_layer = bitmap_layer_create(bounds);

    // Set the bitmap onto the layer and add to the window
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));*/

    // Create the TextLayer with specific bounds
    /*s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

    // Improve the layout to be more like a watchface
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_text(s_time_layer, "00:00");
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);*/

    // Create GFont
    //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));

    // Apply to TextLayer
    //text_layer_set_font(s_time_layer, s_time_font);

    // Add it as a child layer to the Window's root layer
    //layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

    // Create temperature Layer
    s_wallet_layer = text_layer_create(
            GRect(5, PBL_IF_ROUND_ELSE(5, 5), bounds.size.w-40, bounds.size.h));

    // Style the text
    text_layer_set_background_color(s_wallet_layer, GColorClear);
    text_layer_set_text_color(s_wallet_layer, GColorWhite);
    text_layer_set_text_alignment(s_wallet_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(s_wallet_layer, GTextOverflowModeWordWrap);
  
  
    snprintf(text_buffer, sizeof(text_buffer), "%s\nLoading...", name1);
    text_layer_set_text(s_wallet_layer, text_buffer);

    // Create second custom font, apply it and add to Window
    //s_wallet_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
    s_wallet_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    text_layer_set_font(s_wallet_layer, s_wallet_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_wallet_layer));
    text_layer_enable_screen_text_flow_and_paging(s_wallet_layer, 5);
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);

    // Unload GFont
    fonts_unload_custom_font(s_time_font);

    // Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);

    // Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);

    // Destroy wallet elements
    text_layer_destroy(s_wallet_layer);
    fonts_unload_custom_font(s_wallet_font);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    // A single click has just occured
    sendMessageToPhone();

}

static void click_config_provider(void *context) 
{
    window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);

}

static void loadValuesFromPersistentStorage(void) {
    if(persist_exists(KEY_BTC_NAME_1)) {
        persist_read_string(KEY_BTC_NAME_1, name1, sizeof(name1));
    }
}

static void init() {
    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set the background color
    window_set_background_color(s_main_window, GColorDarkGray);

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
            .load = main_window_load,
            .unload = main_window_unload
            });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    // Make sure the time is displayed from the start
    //update_time();

    // Register with TickTimerService
    //tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    // Register callbacks
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    s_select_bitmap = gbitmap_create_with_resource(RESOURCE_ID_REFRESH);

    // Create ActionBarLayer
    s_action_bar = action_bar_layer_create();
    action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
    // Set the icons
    action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_select_bitmap);
    // Add to Window
    action_bar_layer_add_to_window(s_action_bar, s_main_window);
    loadValuesFromPersistentStorage();
    updateBalance(0,0);
    // Open AppMessage
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
    // Destroy the ActionBarLayer
    action_bar_layer_destroy(s_action_bar);

    // Destroy the icon GBitmaps
    gbitmap_destroy(s_select_bitmap);
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
