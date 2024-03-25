/**
 * author: Aleksandr Shevchenko
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "ssd1306.h"
#include "driver/i2c.h"
#include "apds9960.h"
#include "font8x8_basic.h"
#include "cJSON.h"
#include "weather.h"

// I2C Configuration
#define I2C_MASTER_SCL_IO 22	    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21	    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

// MQTT Configuration
#define MQTT_BROKER "mqtt://broker.emqx.io:1883" /*!< MQTT broker URL*/
#define MQTT_SSID "sanek"				 /*!< Wi-Fi name*/
#define MQTT_PASS "brnobrno"				 /*!< Wi-Fi password*/
#define TOPIC "IMPproj"					 /*!< MQTT topic*/

// Tags for logging
#define tag "SSD1306"
static const char *TAG = "MeteoApp";

// MQTT setup and event handler
esp_mqtt_client_handle_t client;

// I2C bus handle
static i2c_bus_handle_t i2c_bus = NULL;

// APDS-9960 gesture sensor handle
static apds9960_handle_t apds9960_dev = NULL;

// Gesture variables
static volatile int gesture_changes = 0;
static volatile uint8_t gesture = -1;

// Changing weather data only when the user asked for it
static volatile int can_change_data = 0;

// Default weather data
char city_gl[17] = "Brno";
char weather_gl[10] = "rainy";
int temperature_gl = -25;

// Comparing the received city with the selected city
static volatile char selected_city[17];

// SPI display handle
SSD1306_t dev;

// Functions prototypes
i2c_config_t i2c_bus_init();
void apds9960_init();
void gesture_task(void *pvParameter);
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
					 int32_t event_id, void *event_data);
void wifi_init_sta();
void handle_incoming_mqtt_data(const char *mqtt_data);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_init();
void flash_init();
void display_config();
void display_title_page();
void display_weather_page(char *city, char *weather, int temperature);
void display_waiting_screen();
void display_city_selection_screen();
void display_task(void *pvParameters);

/**
 * function: i2c_bus_init
 * Initialize the I2C bus
 * @return: i2c_config_t - the I2C configuration
 */
i2c_config_t i2c_bus_init()
{
	// I2C configuration
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;			  // Master mode
	conf.sda_io_num = I2C_MASTER_SDA_IO;	  // SDA pin
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;	  // Enable pullup on SDA
	conf.scl_io_num = I2C_MASTER_SCL_IO;	  // SCL pin
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;	  // Enable pullup on SCL
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ; // CLK frequency
	return conf;
}

/**
 * function: apds9960_init
 * Initialize the APDS-9960 gesture sensor
 */
void apds9960_init()
{
	apds9960_dev = apds9960_create(i2c_bus, APDS9960_I2C_ADDRESS); // Create the APDS-9960 sensor
	esp_err_t ret = apds9960_gesture_init(apds9960_dev);		   // Initialize the gesture sensor
	if (ret != ESP_OK)
	{
		ESP_LOGE(TAG, "Failed to initialize APDS-9960 sensor");
		return;
	}
	if (apds9960_enable_gesture_engine(apds9960_dev, true) != ESP_OK)
	{
		ESP_LOGE(TAG, "Failed to enable gesture engine");
		return;
	}
}

/**
 * function: gesture_task
 * Task for handling the gesture sensor
 * @param: void *pvParameter - the parameter
 */
void gesture_task(void *pvParameter)
{
	while (1)
	{
		uint8_t read_gesture = apds9960_read_gesture(apds9960_dev); // Read the gesture
		if (read_gesture == APDS9960_DOWN || read_gesture == APDS9960_UP || read_gesture == APDS9960_LEFT || read_gesture == APDS9960_RIGHT)
		{
			gesture_changes = 1;
			gesture = read_gesture;
			switch (gesture)
			{
			case APDS9960_DOWN:
				ESP_LOGI(TAG, "UP");
				break;
			case APDS9960_UP:
				ESP_LOGI(TAG, "DOWN");
				break;
			case APDS9960_LEFT:
				ESP_LOGI(TAG, "RIGHT");
				break;
			case APDS9960_RIGHT:
				ESP_LOGI(TAG, "LEFT");
				break;
			default:
				break;
			}
		}

		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

/**
 * function: wifi_event_handler
 * Event handler for Wi-Fi events
 * @param: void *arg - the argument
 * @param: esp_event_base_t event_base - the event base
 * @param: int32_t event_id - the event ID
 * @param: void *event_data - the event data
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
					 int32_t event_id, void *event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
	{
		ESP_LOGI(TAG, "Wi-Fi started");
		esp_wifi_connect();
	}
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
	{
		ESP_LOGI(TAG, "Wi-Fi connected");
	}
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		ESP_LOGE(TAG, "Wi-Fi disconnected, trying to reconnect...");
		esp_wifi_connect();
	}
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	{
		ESP_LOGI(TAG, "Got an IP address");
	}
}

/**
 * function: wifi_init_sta
 * Initialize the Wi-Fi station
 */
void wifi_init_sta()
{
	// Initialize TCP/IP stack
	ESP_ERROR_CHECK(esp_netif_init());
	// Create the default event loop
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	// Create the default Wi-Fi station
	esp_netif_create_default_wifi_sta();
	// Initialize the Wi-Fi station
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	// Initialize the Wi-Fi driver
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	// Register the Wi-Fi event handler
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
	// Register the IP event handler
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

	// Configure the Wi-Fi station
	wifi_config_t wifi_config = {
	    .sta = {
		  .ssid = MQTT_SSID,
		  .password = MQTT_PASS,
	    },
	};

	// Set the Wi-Fi mode to station
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	// Set the Wi-Fi configuration
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	// Start the Wi-Fi station
	ESP_ERROR_CHECK(esp_wifi_start());
	ESP_LOGI(TAG, "wifi_init_sta finished.");
	ESP_LOGI(TAG, "connect to ap SSID:%s password:%s", MQTT_SSID, MQTT_PASS);
}

/**
 * function: handle_incoming_mqtt_data
 * Handle the incoming MQTT data
 * @param: const char *mqtt_data - the incoming data
 */
void handle_incoming_mqtt_data(const char *mqtt_data)
{
	// Parse the JSON
	cJSON *json = cJSON_Parse(mqtt_data);
	if (json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			ESP_LOGE(TAG, "Error before: %s", error_ptr);
		}
		return;
	}

	// Get the city
	cJSON *city = cJSON_GetObjectItemCaseSensitive(json, "city");
	if (cJSON_IsString(city) && (city->valuestring != NULL))
	{
		// Compare the received city with the selected city
		char *temp_city = (char *)selected_city;
		if (strcmp(temp_city, city->valuestring) != 0)
		{
			ESP_LOGI(TAG, "Received city does not match selected city, exiting");
			return;
		}
		// If the received city matches the selected city, update the global city variable
		if (can_change_data)
		{
			strncpy(city_gl, city->valuestring, sizeof(city_gl) - 1);
		}
		ESP_LOGI(TAG, "City: %s", city->valuestring);
	}

	// Get the weather
	cJSON *weather = cJSON_GetObjectItemCaseSensitive(json, "weather");
	if (cJSON_IsString(weather) && (weather->valuestring != NULL))
	{
		if (can_change_data)
		{
			strncpy(weather_gl, weather->valuestring, sizeof(weather_gl) - 1);
		}
		ESP_LOGI(TAG, "Weather: %s", weather->valuestring);
	}

	// Get the temperature
	cJSON *temperature = cJSON_GetObjectItemCaseSensitive(json, "temperature");
	if (cJSON_IsNumber(temperature))
	{
		if (can_change_data)
		{
			temperature_gl = temperature->valueint;
		}
		ESP_LOGI(TAG, "Temperature: %d", temperature->valueint);
	}

	can_change_data = 0;
	cJSON_Delete(json);
}

/**
 * function: mqtt_event_handler
 * Event handler for MQTT events
 * @param: void *handler_args - the handler arguments
 * @param: esp_event_base_t base - the event base
 * @param: int32_t event_id - the event ID
 * @param: void *event_data - the event data
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
	esp_mqtt_event_handle_t event = event_data;
	esp_mqtt_client_handle_t client = event->client;

	switch (event->event_id)
	{
	case MQTT_EVENT_CONNECTED: // If connected, subscribe to the topic
		ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
		esp_mqtt_client_subscribe(client, TOPIC, 0);
		break;

	case MQTT_EVENT_DATA: // If data is received, handle it
		ESP_LOGI(TAG, "MQTT_EVENT_DATA");
		printf("Topic: %.*s, Data: %.*s\n", event->topic_len, event->topic, event->data_len, event->data);
		if (event->data[0] != '{') // If the first character is not '{', it's not JSON
		{
			break;
		}
		handle_incoming_mqtt_data(event->data);
		break;

	default:
		break;
	}
}

/**
 * function: mqtt_init
 * Initialize the MQTT client
 */
void mqtt_init()
{
	// Configure the MQTT client
	esp_mqtt_client_config_t mqtt_cfg = {
	    .broker.address.uri = MQTT_BROKER,
	};
	// Create the MQTT client
	client = esp_mqtt_client_init(&mqtt_cfg);
	// Register the MQTT event handler
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
	// Start the MQTT client
	esp_mqtt_client_start(client);
}

/**
 * function: flash_init
 * Initialize the flash memory
 */
void flash_init()
{
	// Initialize the flash memory
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		// If NVS partition was truncated, erase it and retry
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}

/**
 * function: display_config
 * Configure the display
 */
void display_config()
{
	ESP_LOGI(tag, "INTERFACE is SPI");
	ESP_LOGI(tag, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
	ESP_LOGI(tag, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
	ESP_LOGI(tag, "CONFIG_CS_GPIO=%d", CONFIG_CS_GPIO);
	ESP_LOGI(tag, "CONFIG_DC_GPIO=%d", CONFIG_DC_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);

	ESP_LOGI(tag, "Panel is 128x64");
	// Initialize the display
	ssd1306_init(&dev, 128, 64);
}

/**
 * function: display_title_page
 * Display the title page
 */
void display_title_page()
{
	gesture_changes = 0;
	ssd1306_display_text_x3(&dev, 0, "METEO", 5, false);
	ssd1306_display_text_x3(&dev, 3, " APP", 4, false);

	// Make the screen blink
	while (1)
	{
		ssd1306_display_text(&dev, 6, "  any gesture", 13, false);
		ssd1306_display_text(&dev, 7, "   to start", 11, false);

		if (gesture_changes > 0)
		{
			break;
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);

		ssd1306_clear_line(&dev, 6, false);
		ssd1306_clear_line(&dev, 7, false);

		if (gesture_changes > 0)
		{
			break;
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

/**
 * function: display_weather_page
 * Display the weather page
 * @param: char *city - the city
 * @param: char *weather - the weather
 * @param: int temperature - the temperature
 */
void display_weather_page(char *city, char *weather, int temperature)
{
	ssd1306_clear_screen(&dev, false);
	gesture_changes = 0;
	while (1)
	{
		// Display the city name at the top, centered
		char city_text[17]; // 16 characters + null terminator
		int city_padding = (16 - strlen(city)) / 2;
		memset(city_text, ' ', city_padding);
		strncpy(city_text + city_padding, city, 16 - city_padding);
		city_text[16] = '\0'; // Ensure the string is null-terminated
		ssd1306_display_text(&dev, 0, city_text, strlen(city_text), false);

		// Display the weather icon in the middle
		if (strcmp(weather, "sunny") == 0)
		{
			ssd1306_bitmaps(&dev, 44, 12, sunny_icon, 40, 40, false);
		}
		else if (strcmp(weather, "rainy") == 0)
		{
			ssd1306_bitmaps(&dev, 44, 12, rainy_icon, 40, 40, false);
		}
		else if (strcmp(weather, "snowy") == 0)
		{
			ssd1306_bitmaps(&dev, 44, 12, snowy_icon, 40, 40, false);
		}
		else if (strcmp(weather, "windy") == 0)
		{
			ssd1306_bitmaps(&dev, 44, 12, windy_icon, 40, 40, false);
		}

		// Display the temperature at the bottom, centered
		char temp_text[17]; // 16 characters + null terminator
		sprintf(temp_text, "%d C", temperature);
		char temp_padded_text[17];
		int temp_padding = (16 - strlen(temp_text)) / 2;
		memset(temp_padded_text, ' ', temp_padding);
		strncpy(temp_padded_text + temp_padding, temp_text, 16 - temp_padding);
		temp_padded_text[16] = '\0'; // Ensure the string is null-terminated
		ssd1306_display_text(&dev, 7, temp_padded_text, strlen(temp_padded_text), false);

		// If the user swipes left or right, exit the weather page
		if (gesture_changes > 0)
		{
			break;
		}
	}
}

/**
 * function: display_waiting_screen
 * Display the waiting screen
 */
void display_waiting_screen()
{
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	// allow the change of data
	can_change_data = 1;

	// Make the screen blink for 30 seconds
	for (int i = 0; i < 30; i++)
	{
		ssd1306_display_text(&dev, 3, "  WAITING FOR", 13, false);
		ssd1306_display_text(&dev, 4, "    DATA ...", 12, false);

		if (can_change_data == 0)
		{
			break;
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);

		ssd1306_clear_line(&dev, 3, false);
		ssd1306_clear_line(&dev, 4, false);

		if (can_change_data == 0)
		{
			break;
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

/**
 * function: display_city_selection_screen
 * Display the city selection screen
 */
void display_city_selection_screen()
{
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	gesture_changes = 0;
	int selected_city_index = 0;
	while (1)
	{
		if (gesture_changes > 0)
		{
			switch (gesture)
			{
			case APDS9960_LEFT:
				char *temp_city = (char *)selected_city;
				strncpy(temp_city, cities[selected_city_index], sizeof(selected_city) - 1);
				selected_city[sizeof(selected_city) - 1] = '\0'; // Ensure null termination
				// Publish the selected city to the MQTT broker
				int msg_id = esp_mqtt_client_publish(client, TOPIC, cities[selected_city_index], 0, 0, 0);
				if (msg_id >= 0)
				{
					ESP_LOGI(TAG, "Successfully published message, msg_id=%d", msg_id);
				}
				else
				{
					ESP_LOGE(TAG, "Failed to publish message");
				}
				display_waiting_screen();
				return;
				break;
			case APDS9960_RIGHT: // return to the weather page
				return;
			case APDS9960_DOWN:
				selected_city_index = selected_city_index == 0 ? 0 : selected_city_index - 1;
				break;
			case APDS9960_UP:
				selected_city_index = selected_city_index == sizeof(cities) / sizeof(cities[0]) - 1 ? sizeof(cities) / sizeof(cities[0]) - 1 : selected_city_index + 1;
				break;
			default:
				break;
			}

			gesture_changes = 0;
		}
		// Display "SELECT CITY" at the top
		ssd1306_display_text(&dev, 0, "SELECT CITY", strlen("SELECT CITY"), false);

		// Display the cities in the array
		for (int i = 0; i < sizeof(cities) / sizeof(cities[0]); i++)
		{
			if (i == selected_city_index)
			{

				ssd1306_display_text(&dev, i + 1, cities[i], strlen(cities[i]), true);
			}
			else
			{
				ssd1306_display_text(&dev, i + 1, cities[i], strlen(cities[i]), false);
			}
		}
	}
}

/**
 * function: display_task
 * Task for handling the display
 * @param: void *pvParameters - the parameter
 */
void display_task(void *pvParameters)
{
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);

	display_title_page();

	// display task shows the weather page in loop and handles the gestures
	while (1)
	{
		display_weather_page(city_gl, weather_gl, temperature_gl);
		if (gesture_changes > 0 && (gesture == APDS9960_LEFT || gesture == APDS9960_RIGHT))
		{
			ssd1306_clear_screen(&dev, false);
			ssd1306_contrast(&dev, 0xff);
			switch (gesture)
			{
			case APDS9960_LEFT:
				display_city_selection_screen();
				break;
			case APDS9960_RIGHT:
				display_title_page(); // return to the title page
				break;
			default:
				break;
			}

			gesture_changes = 0;
		}

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

/**
 * function: app_main
 * The main function
 */
void app_main(void)
{
	flash_init();

	wifi_init_sta();

	esp_log_level_set("*", ESP_LOG_INFO);

	mqtt_init();

	i2c_config_t conf = i2c_bus_init();

	i2c_bus = i2c_bus_create(I2C_MASTER_NUM, &conf);

	apds9960_init();

	xTaskCreate(gesture_task, "gesture_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);

	display_config();

	xTaskCreate(display_task, "display_task", configMINIMAL_STACK_SIZE * 20, NULL, 5, NULL);
}
