#include <pebble.h>
//declare windows
static Window *menu_window;
static Window *temperature_window;
static Window *distance_window;

//declare layers
static MenuLayer *menu_layer;
static TextLayer *temperature_layer;
static TextLayer *distance_layer;

//global variable
static char msg[100];

void out_sent_handler(DictionaryIterator *sent, void *context) {
  // outgoing message was delivered — do nothing
}
void out_failed_handler(DictionaryIterator *failed,
AppMessageResult reason, void *context) {
// outgoing message failed
text_layer_set_text(temperature_layer, "Error out! Check internet connection."); 
}
void in_received_handler(DictionaryIterator *received, void *context) {
// incoming message received 
// looks for key #0 in the incoming message 
int key = 1;
Tuple *text_tuple = dict_find(received, key); 
if (text_tuple) {
if (text_tuple->value) {
// put it in this global variable 
 strcpy(msg, text_tuple->value->cstring);
}
else strcpy(msg, "no value!");
text_layer_set_text(temperature_layer, msg); }
else {
text_layer_set_text(temperature_layer, "no message!");
}
}
void in_dropped_handler(AppMessageResult reason, void *context) { 
  // incoming message dropped  
text_layer_set_text(temperature_layer, "Error in!");
}

void distance_select_click_handler(ClickRecognizerRef recognizer, void *context) {
DictionaryIterator *iter; 
app_message_outbox_begin(&iter);
int key = 1;
// send the message "hello?" to the phone, using key #0 
Tuplet value = TupletCString(key, "Distance"); 
dict_write_tuplet(iter, &value); 
app_message_outbox_send();
//text_layer_set_text(hello_layer, "Refresh!"); 
}

/* This is called when the select button is clicked */
void temperature_select_click_handler(ClickRecognizerRef recognizer, void *context) {
DictionaryIterator *iter; 
app_message_outbox_begin(&iter);
int key = 1;
// send the message "hello?" to the phone, using key #0 
Tuplet value = TupletCString(key, "Celsius"); 
dict_write_tuplet(iter, &value); 
app_message_outbox_send();
//text_layer_set_text(hello_layer, "Refresh!"); 
}

void temperature_select_click_handler_up(ClickRecognizerRef recognizer, void *context){
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 1;
  // send the message "hello?" to the phone, using key #0 
  Tuplet value = TupletCString(key, "Farenheit"); 
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
}

void temperature_select_click_handler_down(ClickRecognizerRef recognizer, void *context){
  DictionaryIterator *iter; 
  app_message_outbox_begin(&iter);
  int key = 1;
  // send the message "hello?" to the phone, using key #0 
  Tuplet value = TupletCString(key, "Standby"); 
  dict_write_tuplet(iter, &value); 
  app_message_outbox_send();
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, 
                                      uint16_t section_index, void *context) {
  const uint16_t num_rows = 3;
  return num_rows;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, 
                                        MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  if((int)cell_index->row==0){
    snprintf(s_buff, sizeof(s_buff), "Temperature");
  }
  else if((int)cell_index->row==1){
    snprintf(s_buff, sizeof(s_buff), "Distance");
  }
  else{
    snprintf(s_buff, sizeof(s_buff), "Time");
  }

  // Draw this row's index
  menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, 
                                        MenuIndex *cell_index, void *context) {
  const int16_t cell_height = 44;
  return cell_height;
}

static void select_callback(struct MenuLayer *menu_layer, 
                                        MenuIndex *cell_index, void *context) {
  // Do something in response to the button press
  if((int)cell_index->row==0){
    window_stack_push(temperature_window, true);
  }
  else if((int)cell_index->row==1){
    window_stack_push(distance_window,true);
  }
   
}

/* this registers the appropriate function to the appropriate button */ 
void temperature_config_provider(void *context) {
window_single_click_subscribe(BUTTON_ID_SELECT, temperature_select_click_handler);
window_single_click_subscribe(BUTTON_ID_UP, temperature_select_click_handler_up);
window_single_click_subscribe(BUTTON_ID_DOWN, temperature_select_click_handler_down);
  
}

void distance_config_provider(void *context) {
window_single_click_subscribe(BUTTON_ID_SELECT, distance_select_click_handler);
// window_single_click_subscribe(BUTTON_ID_UP, temperature_select_click_handler_up);
// window_single_click_subscribe(BUTTON_ID_DOWN, temperature_select_click_handler_down);
}

static void distance_window_load(Window *window) {
Layer *window_layer = window_get_root_layer(window); 
GRect bounds = layer_get_bounds(window_layer);
distance_layer = text_layer_create(bounds); 
text_layer_set_text(distance_layer, "Distance!");
text_layer_set_overflow_mode(distance_layer,GTextOverflowModeWordWrap);
text_layer_set_text_alignment(distance_layer, GTextAlignmentCenter); 
layer_add_child(window_layer, text_layer_get_layer(distance_layer));
}

static void distance_window_unload(Window *window) { 
  text_layer_destroy(distance_layer);
  // Destroy the MenuLayer
}

static void temperature_window_load(Window *window) {
Layer *window_layer = window_get_root_layer(window); 
GRect bounds = layer_get_bounds(window_layer);
temperature_layer = text_layer_create(bounds); 
text_layer_set_text(temperature_layer, "Temperature!");
text_layer_set_overflow_mode(temperature_layer,GTextOverflowModeWordWrap);
text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter); 
layer_add_child(window_layer, text_layer_get_layer(temperature_layer));
}

static void temperature_window_unload(Window *window) { 
  text_layer_destroy(temperature_layer);
  // Destroy the MenuLayer
}

void menu_window_load(Window *window) {
Layer *window_layer = window_get_root_layer(menu_window);
GRect bounds = layer_get_bounds(window_layer);
// Create the MenuLayer
menu_layer = menu_layer_create(bounds);

// Let it receive click events
menu_layer_set_click_config_onto_window(menu_layer, window);

// Set the callbacks for behavior and rendering
menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows_callback,
    .draw_row = draw_row_callback,
    .get_cell_height = get_cell_height_callback,
    .select_click = select_callback,
});

// Add to the Window
layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void menu_window_unload(Window *window) { 
 // Destroy the MenuLayer
 menu_layer_destroy(menu_layer);
}

static void init(void) {
  //window create
  menu_window = window_create(); 
  temperature_window = window_create();
  distance_window = window_create();
  
  //set window handlers
  window_set_window_handlers(menu_window, (WindowHandlers) {
  .load = menu_window_load,
  .unload = menu_window_unload, });
  
  window_set_window_handlers(temperature_window, (WindowHandlers) {
  .load = temperature_window_load,
  .unload = temperature_window_unload, });
  
  window_set_window_handlers(distance_window, (WindowHandlers) {
  .load = temperature_window_load,
  .unload = temperature_window_unload, });
  
  // need this for adding the listener
  window_set_click_config_provider(temperature_window, temperature_config_provider);
  window_set_click_config_provider(distance_window, distance_config_provider);
  
  // for registering AppMessage handlers 
  app_message_register_inbox_received(in_received_handler); 
  app_message_register_inbox_dropped(in_dropped_handler); 
  app_message_register_outbox_sent(out_sent_handler); 
  app_message_register_outbox_failed(out_failed_handler); 
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64; 
  app_message_open(inbound_size, outbound_size);
  const bool animated = true;
  window_stack_push(menu_window, animated); 
}

static void deinit(void) { 
  window_destroy(menu_window);
  window_destroy(temperature_window);
  window_destroy(distance_window);
}

int main(void) { 
  init();
  app_event_loop();
  deinit(); 
}