#include <stdlib.h>

#include "server.h"

struct server* server_new(void)
{
  struct server* serv = malloc(sizeof(struct server));

  return serv;
}

// Run in a new thread
void* server_thread(void* srv)
{
  return srv;
}

void server_stop(void)
{
}

void server_destroy(struct server* srv)
{
  free(srv);
}
