// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "esp_http_server.h"
#include "esp_timer.h"
#include "Arduino.h"

#include <mutex> // std::mutex, std::lock

#include "http_server.h"

std::mutex stream_lock;

static HttpServer *server = NULL;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

const char *GET_RESP = "Camera Busy!";


static esp_err_t stream_handler(httpd_req_t *req)
{
    if (server)
    {
        if (server->server_stream(req))
            return ESP_OK;
    }
    return ESP_ERR_HTTPD_INVALID_REQ;
}

bool HttpServer::setup()
{

    camera.setup();

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL};

    config.server_port += 1;
    config.ctrl_port += 1;
    Serial.printf("Starting stream server on port: '%d'\n", config.server_port);
    if (httpd_start(&stream_httpd, &config) == ESP_OK)
    {
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }

    server = this;
    return true;
}

bool HttpServer::server_stream(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[64];
    Serial.println("Incoming Stream Req!");
    if (stream_lock.try_lock())
    {
        res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
        if (res != ESP_OK)
        {
            return res;
        }

        httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

        while (true)
        {
            if (!camera.capture_image(&_jpg_buf_len, &_jpg_buf))
                res = ESP_ERR_CAMERA_BASE;
            if (res == ESP_OK)
            {
                size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
                res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
            }
            if (res == ESP_OK)
            {
                res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
            }
            if (res == ESP_OK)
            {
                res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
            }
            camera.try_free_frame_buff(&_jpg_buf);

            if (res != ESP_OK)
            {
                break;
            }
        }
        httpd_resp_send(req, GET_RESP, strlen(GET_RESP));
        stream_lock.unlock();
    }
    else
    {
        Serial.println("Couldn't aquire stream lock");
        //timeout resp
        httpd_resp_send(req, GET_RESP, strlen(GET_RESP));
        return -1;
    }
    return res;
}