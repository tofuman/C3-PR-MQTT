#include "esp_camera.h"
#include "Arduino.h"
#include "img_converters.h"

#define CAMERA_MODEL_AI_THINKER

// Select camera module used on the board
#define CAMERA_MODULE_OV2640

#include "camera_pins.h"
#include "camera.h"

Camera::Camera()
{
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
}

bool Camera::setup()
{
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID)
  {
    s->set_vflip(s, 1);       //flip it back
    s->set_brightness(s, 1);  //up the brightness just a bit
    s->set_saturation(s, -2); //lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_SXGA);

  return true;
}

bool Camera::capture_image(size_t *jpg_buf_len, uint8_t **jpg_buf)
{
  frame_buffer = esp_camera_fb_get();
  if (!frame_buffer)
  {
    Serial.println("Camera capture failed");
    return false;
  }
  else
  {
    if (frame_buffer->format != PIXFORMAT_JPEG)
    {
      bool jpeg_converted = frame2jpg(frame_buffer, 80, jpg_buf, jpg_buf_len);
      esp_camera_fb_return(frame_buffer);
      if (!jpeg_converted)
      {
        Serial.println("JPEG compression failed");
        return false;
      }
    }
    else
    {
      *jpg_buf_len = frame_buffer->len;
      *jpg_buf = frame_buffer->buf;
    }
  }
  return true;
}

void Camera::try_free_frame_buff(uint8_t **jpg_buf)
{
  if (frame_buffer)
  {
    esp_camera_fb_return(frame_buffer);
    frame_buffer = NULL;
    *jpg_buf = NULL;
  }
  else if (*jpg_buf)
  {
    free(*jpg_buf);
    *jpg_buf = NULL;
  }
}