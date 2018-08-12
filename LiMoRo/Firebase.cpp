/**
 * Firebase.cpp
 * Copyright (c) 2018 takjn  All right reserved.
 */

#include "Firebase.h"

Firebase::Firebase(NetworkInterface *wifi, char *command_url, char *photo_url, char *message_url)
{
    _wifi = wifi;
    _command_url = command_url;
    _photo_url = photo_url;
    _message_url = message_url;
}

Firebase::~Firebase()
{
}

void Firebase::dump_response(HttpResponse *res)
{
    mbedtls_printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

    mbedtls_printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++)
    {
        mbedtls_printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    mbedtls_printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

string Firebase::get_command(void)
{
    string ret = "NONE";

    // printf("\r\n----- HTTPS GET request -----\r\n");
    HttpsRequest *get_req = new HttpsRequest(_wifi, SSL_CA_PEM, HTTP_GET, _command_url);
    // get_req->set_debug(true);

    HttpResponse *get_res = get_req->send();
    if (!get_res)
    {
        printf("HttpRequest failed (error code %d)\r\n", get_req->get_error());
        return "ERROR";
    }

    // コマンドの解析
    string command = get_res->get_body_as_string();
    if (_previous_command != command)
    {
        mbedtls_printf("%s\r\n", command.c_str());

        if (command.find("ON") != string::npos)
        {
            ret = "ON";
        }
        else if (command.find("OFF") != string::npos)
        {
            ret = "OFF";
        }
        else if (command.find("PHOTO") != string::npos)
        {
            ret = "PHOTO";
        }
        else if (command.find("TV") != string::npos)
        {
            ret = "TV";
        }
        else if (command.find("AIRCON") != string::npos)
        {
            ret = "AIRCON";
        }

        _previous_command = command;
    }
    delete get_req;

    return ret;
}

int Firebase::post_message(const char *message, size_t size)
{
    // printf("\r\n----- HTTPS GET request -----\r\n");
    HttpsRequest *post_req = new HttpsRequest(_wifi, SSL_CA_PEM, HTTP_POST, _message_url);
    // post_req->set_debug(true);
    post_req->set_header("Content-Type", "text/html");

    HttpResponse *post_res = post_req->send(message, size);

    if (!post_res)
    {
        printf("HttpRequest failed (error code %d)\n", post_req->get_error());
        return 1;
    }

    delete post_req;

    return 0;
}

int Firebase::post_photo(void *body, size_t size)
{
    // printf("\r\n----- HTTPS GET request -----\r\n");
    HttpsRequest *post_req = new HttpsRequest(_wifi, SSL_CA_PEM, HTTP_POST, _photo_url);
    // post_req->set_debug(true);
    post_req->set_header("Content-Type", "image/jpeg");

    HttpResponse *post_res = post_req->send(body, size - 1);

    if (!post_res)
    {
        printf("HttpRequest failed (error code %d)\n", post_req->get_error());
        return 1;
    }

    delete post_req;

    return 0;
}