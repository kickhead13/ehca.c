#include "ptt.h"
#include <unistd.h>
#include <fcntl.h>

ptth_response_t set_func(ptth_request_t req, ptth_shared_t *__) {
  char *key   = ptth_map_get_value(*(req.mpath), "key");
  char *value = ptth_map_get_value(*(req.mpath), "keyvalue");

  int fd = open(key, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  write(fd, value, strlen(value));

  close(fd);
  
  PTTH_RESPONDE_HTML(key)
}

ptth_response_t get_func(ptth_request_t req, ptth_shared_t *__) {
  char *key   = ptth_map_get_value(*(req.mpath), "rkey");

  char value[100];

  int fd = open(key, O_RDONLY);
  int bytes_read = read(fd, value, 100);

  close(fd);
  value[bytes_read] = '\0';
  PTTH_RESPONDE_HTML(value)
}

int main() {
  ptth_server_t server;
  ptth_init_server(&server);
  ptth_add_service(&server, PTTH_GET, "/set/{key}/{value}/", &set_func);
  ptth_add_service(&server, PTTH_GET, "/get/{rkey}/", &get_func);
  ptth_bind_server(&server, "127.0.0.1", 8081);
  ptth_start_single(server);
}
