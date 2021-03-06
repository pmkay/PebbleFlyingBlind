/**
 * This code is provided under the Apache 2.0 license.
 * Please read the LICENSE.md file for more information
 * 
 * Copyright (c) 2013 Matthew Congrove (http://github.com/mcongrove)
 * Modified and improved by PK -> to emulate the DURRR watch by vibrating every 5 minutes
 */
#include <pebble.h>

static char THEME[6] = "dark";
static int VIBRATE = 0;

Window *window;
InverterLayer *inverter_layer;
TextLayer *label_layer_time;
char timeText[] = "00:00";
static AppTimer *timer;

enum {
	KEY_THEME = 0x0,
	KEY_VIBRATE = 0x1
};

static void set_theme() {
	if (persist_exists(KEY_THEME)) {
		persist_read_string(KEY_THEME, THEME, 6);
	}
	
	bool hide = strcmp(THEME, "light") == 0 ? true : false;
	
	layer_set_hidden(inverter_layer_get_layer(inverter_layer), hide);
	
	APP_LOG(APP_LOG_LEVEL_INFO, "SELECTED THEME: %s", THEME);
}

static void set_vibrate() {
	if (persist_exists(KEY_VIBRATE)) {
		VIBRATE = persist_read_int(KEY_VIBRATE);
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "SELECTED VIBRATE: %d", VIBRATE);
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *theme_tuple = dict_find(iter, KEY_THEME);
	Tuple *vibrate_tuple = dict_find(iter, KEY_VIBRATE);
	
	theme_tuple ? persist_write_string(KEY_THEME, theme_tuple->value->cstring) : false;
	vibrate_tuple ? persist_write_int(KEY_VIBRATE, vibrate_tuple->value->uint8) : false;

	set_theme();
	set_vibrate();
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	
}

static void timer_callback(void *data) {
	layer_set_hidden(text_layer_get_layer(label_layer_time), true);
}

static void handle_tap(AccelAxisType axis, int32_t direction) {
	layer_set_hidden(text_layer_get_layer(label_layer_time), false);
	
	timer = app_timer_register(3000, timer_callback, NULL);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	char *time_format;
	
	// Set the current time display
	if(clock_is_24h_style()) {
		time_format = "%R";
	} else {
		time_format = "%I:%M";
	}
	
	strftime(timeText, sizeof(timeText), time_format, t);
	
	text_layer_set_text(label_layer_time, timeText);
	
	if (t->tm_min == 0) {
		vibes_long_pulse();
	} else if ((VIBRATE == 1 && t->tm_min == 30) || (VIBRATE == 2 && (t->tm_min == 15 || t->tm_min == 30 || t->tm_min == 45)) || (VIBRATE == 3 && (t->tm_min == 5 || t->tm_min == 10 || t->tm_min == 15 || t->tm_min == 20 || t->tm_min == 25 || t->tm_min == 30 || t->tm_min == 35 || t->tm_min == 40 || t->tm_min == 45 || t->tm_min == 50 || t->tm_min == 55 ))) {
		if (t->tm_min == 5) {
			static const uint32_t const segments5[] = { 100, 100 };
			VibePattern pat5 = { .durations = segments5, .num_segments = ARRAY_LENGTH(segments5) };
			vibes_enqueue_custom_pattern(pat5);
		}
    		if (t->tm_min == 10) {
			static const uint32_t const segments10[] = { 100, 100 };
			VibePattern pat10 = { .durations = segments10, .num_segments = ARRAY_LENGTH(segments10) };
			vibes_enqueue_custom_pattern(pat10);
		}
    		if (t->tm_min == 15) {
			static const uint32_t const segments15[] = { 100, 100, };
			VibePattern pat15 = { .durations = segments15, .num_segments = ARRAY_LENGTH(segments15) };
			vibes_enqueue_custom_pattern(pat15);
		}
		    if (t->tm_min == 20) {
			static const uint32_t const segments20[] = { 100, 100 };
			VibePattern pat20 = { .durations = segments20, .num_segments = ARRAY_LENGTH(segments20) };
			vibes_enqueue_custom_pattern(pat20);
		}
    		if (t->tm_min == 25) {
			static const uint32_t const segments25[] = { 100, 100 };
			VibePattern pat25 = { .durations = segments25, .num_segments = ARRAY_LENGTH(segments25) };
			vibes_enqueue_custom_pattern(pat25);
		}
			if (t->tm_min == 30) {
			static const uint32_t const segments30[] = { 100, 100 };
			VibePattern pat30 = { .durations = segments30, .num_segments = ARRAY_LENGTH(segments30) };
			vibes_enqueue_custom_pattern(pat30);
		}
    		if (t->tm_min == 35) {
			static const uint32_t const segments35[] = { 100, 100 };
			VibePattern pat35 = { .durations = segments35, .num_segments = ARRAY_LENGTH(segments35) };
			vibes_enqueue_custom_pattern(pat35);
		}
			if (t->tm_min == 40) {
			static const uint32_t const segments40[] = { 100, 100 };
			VibePattern pat40 = { .durations = segments40, .num_segments = ARRAY_LENGTH(segments40) };
			vibes_enqueue_custom_pattern(pat40);
		}
			if (t->tm_min == 45) {
			static const uint32_t const segments45[] = { 100, 100 };
			VibePattern pat45 = { .durations = segments45, .num_segments = ARRAY_LENGTH(segments45) };
			vibes_enqueue_custom_pattern(pat45);
		}
    		if (t->tm_min == 50) {
			static const uint32_t const segments50[] = { 100, 100 };
			VibePattern pat50 = { .durations = segments50, .num_segments = ARRAY_LENGTH(segments50) };
			vibes_enqueue_custom_pattern(pat50);
		}
    		if (t->tm_min == 55) {
			static const uint32_t const segments55[] = { 100, 100 };
			VibePattern pat55 = { .durations = segments55, .num_segments = ARRAY_LENGTH(segments55) };
			vibes_enqueue_custom_pattern(pat55);
		}
	}
}

static void init() {
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_open(64, 0);
	
	window = window_create();
	window_set_background_color(window, GColorWhite);
	window_stack_push(window, true);
	
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	
	// Add time layer
	label_layer_time = text_layer_create(GRect(0, 65, 144, 30));
	text_layer_set_text_color(label_layer_time, GColorBlack);
	text_layer_set_background_color(label_layer_time, GColorClear);
	text_layer_set_text_alignment(label_layer_time, GTextAlignmentCenter);
	text_layer_set_font(label_layer_time, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	layer_set_hidden(text_layer_get_layer(label_layer_time), true);
	layer_add_child(window_layer, text_layer_get_layer(label_layer_time));
	
	// Create the inverter layer
	inverter_layer = inverter_layer_create(bounds);
	layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
	accel_tap_service_subscribe(handle_tap);
	
	set_theme();
	set_vibrate();
}

static void deinit() {
	window_destroy(window);
	inverter_layer_destroy(inverter_layer);
	
	tick_timer_service_unsubscribe();
	accel_tap_service_unsubscribe();
	app_message_deregister_callbacks();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}