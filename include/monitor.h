#pragma once

#ifndef _MONITOR_H
#define _MONITOR_H

struct monitor {
  int WRITE_ME;
};

struct monitor* monitor_new(void);
void* monitor_start(void* _);
void monitor_stop(void);

#endif /* _MONITOR_H */
