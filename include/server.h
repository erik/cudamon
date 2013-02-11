#pragma once

#ifndef _SERVER_H
#define _SERVER_H

#include "mongoose.h"

struct monitor;

struct server {
  struct monitor* mon;

  struct mg_callbacks callbacks;
  struct mg_context *ctx;
};

struct server* server_new(struct monitor*);
void server_start(struct server* srv);
void server_destroy(struct server*);

#endif /* _SERVER_H */
