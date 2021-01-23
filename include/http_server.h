#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "camera.h"
#include "esp_http_server.h"

class HttpServer {
    public:
        HttpServer() :camera() {};
        bool setup();
        bool server_stream(httpd_req_t *req);
    private:
        Camera camera;
        httpd_handle_t stream_httpd = NULL;

};


#endif