#pragma once

#ifndef _SERVER_H
#define _SERVER_H

struct server {
  int WRITE_ME;
};

struct server* server_new(void);
void* server_start(void* _);
void server_stop(void);

#endif /* _SERVER_H */
