#pragma once

#ifndef _SERVER_H
#define _SERVER_H

struct server {
  int WRITE_ME;
};

struct server* server_new(void);
void* server_thread(void* srv);
void server_stop(void);
void server_destroy(struct server*);

#endif /* _SERVER_H */
