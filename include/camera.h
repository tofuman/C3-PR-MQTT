#ifndef CAMERA_H
#define CAMERA_H

#include "esp_camera.h"

class Camera {
    public:
        Camera();
        bool setup();
        bool capture_image(size_t *jpg_buf_len, uint8_t **jpg_buf);
        void try_free_frame_buff(uint8_t **jpg_buf);

    private:
        camera_config_t config;
        camera_fb_t *frame_buffer;

};


#endif