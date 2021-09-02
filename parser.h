#include<http_parser.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<time.h>

// static http_parser *parser;

int on_message_begin(http_parser* _) {
    (void)_;
    printf("\n***Message Begin***\n\n");
    return 0;
}

int on_headers_complete(http_parser* _) {
    (void)_;
    printf("\n***Headers complete***\n\n");
    return 0;
}

int on_message_complete(http_parser* _) {
    (void)_;
    printf("\n***Message complete***\n\n");
    return 0;
}

int on_url(http_parser* _, const char* at, size_t length) {
    (void)_;
    printf("Url: %.*s\n", (int)length, at);
    return 0;
}

int on_header_field(http_parser* _, const char* at, size_t length) {
    (void)_;
    printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

int on_header_value(http_parser* _, const char* at, size_t length) {
    (void)_;
    printf("Header value: %.*s\n", (int)length, at);
    return 0;
}

int on_body(http_parser* _, const char* at, size_t length) {
    (void)_;
    printf("Body: %.*s", (int)length, at);
    return 0;
}

static http_parser_settings settings_null = {
    .on_message_begin = on_message_begin,
    .on_url = on_url,
    .on_status = 0,
    .on_header_field = on_header_field,
    .on_header_value = on_header_value,
    .on_headers_complete = on_headers_complete,
    .on_body = on_body,
    .on_message_complete = on_message_complete
};