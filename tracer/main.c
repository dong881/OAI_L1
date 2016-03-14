#include <stdio.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define T_ID(x) x
#include "../T_IDs.h"
#include "../T_defs.h"

#ifdef T_USE_SHARED_MEMORY

T_cache_t *T_cache;
int T_busylist_head;
int T_pos;

static inline int GET(int s, void *out, int count)
{
  if (count == 1) {
    *(char *)out = T_cache[T_busylist_head].buffer[T_pos];
    T_pos++;
    return 1;
  }
  memcpy(out, T_cache[T_busylist_head].buffer + T_pos, count);
  T_pos += count;
  return count;
}

#else /* T_USE_SHARED_MEMORY */

#define GET read

#endif /* T_USE_SHARED_MEMORY */

int get_connection(char *addr, int port)
{
  struct sockaddr_in a;
  socklen_t alen;
  int s, t;

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) { perror("socket"); exit(1); }
  t = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int)))
    { perror("setsockopt"); exit(1); }

  a.sin_family = AF_INET;
  a.sin_port = htons(port);
  a.sin_addr.s_addr = inet_addr(addr);

  if (bind(s, (struct sockaddr *)&a, sizeof(a))) { perror("bind"); exit(1); }
  if (listen(s, 5)) { perror("bind"); exit(1); }
  alen = sizeof(a);
  t = accept(s, (struct sockaddr *)&a, &alen);
  if (t == -1) { perror("accept"); exit(1); }
  close(s);
  return t;
}

void get_string(int s, char *out)
{
  while (1) {
    if (GET(s, out, 1) != 1) abort();
    if (*out == 0) break;
    out++;
  }
}

void get_message(int s)
{
#define S(x, y) do { \
    char str[1024]; \
    get_string(s, str); \
    printf("["x"]["y"] %s", str); \
  } while (0)

  int m;
#ifdef T_USE_SHARED_MEMORY
  T_pos = 0;
#endif
  if (GET(s, &m, sizeof(int)) != sizeof(int)) abort();
  switch (m) {
  case T_first: {
    char str[1024];
    get_string(s, str);
    printf("%s", str);
    break;
  }
  case T_LEGACY_MAC_INFO: S("MAC", "INFO"); break;
  case T_LEGACY_MAC_ERROR: S("MAC", "ERROR"); break;
  case T_LEGACY_MAC_WARNING: S("MAC", "WARNING"); break;
  case T_LEGACY_MAC_DEBUG: S("MAC", "DEBUG"); break;
  case T_LEGACY_MAC_TRACE: S("MAC", "TRACE"); break;
  case T_LEGACY_PHY_INFO: S("PHY", "INFO"); break;
  case T_LEGACY_PHY_ERROR: S("PHY", "ERROR"); break;
  case T_LEGACY_PHY_WARNING: S("PHY", "WARNING"); break;
  case T_LEGACY_PHY_DEBUG: S("PHY", "DEBUG"); break;
  case T_LEGACY_PHY_TRACE: S("PHY", "TRACE"); break;
  case T_LEGACY_S1AP_INFO: S("S1AP", "INFO"); break;
  case T_LEGACY_S1AP_ERROR: S("S1AP", "ERROR"); break;
  case T_LEGACY_S1AP_WARNING: S("S1AP", "WARNING"); break;
  case T_LEGACY_S1AP_DEBUG: S("S1AP", "DEBUG"); break;
  case T_LEGACY_S1AP_TRACE: S("S1AP", "TRACE"); break;
  case T_LEGACY_RRC_INFO: S("RRC", "INFO"); break;
  case T_LEGACY_RRC_ERROR: S("RRC", "ERROR"); break;
  case T_LEGACY_RRC_WARNING: S("RRC", "WARNING"); break;
  case T_LEGACY_RRC_DEBUG: S("RRC", "DEBUG"); break;
  case T_LEGACY_RRC_TRACE: S("RRC", "TRACE"); break;
  case T_LEGACY_RLC_INFO: S("RLC", "INFO"); break;
  case T_LEGACY_RLC_ERROR: S("RLC", "ERROR"); break;
  case T_LEGACY_RLC_WARNING: S("RLC", "WARNING"); break;
  case T_LEGACY_RLC_DEBUG: S("RLC", "DEBUG"); break;
  case T_LEGACY_RLC_TRACE: S("RLC", "TRACE"); break;
  case T_LEGACY_PDCP_INFO: S("PDCP", "INFO"); break;
  case T_LEGACY_PDCP_ERROR: S("PDCP", "ERROR"); break;
  case T_LEGACY_PDCP_WARNING: S("PDCP", "WARNING"); break;
  case T_LEGACY_PDCP_DEBUG: S("PDCP", "DEBUG"); break;
  case T_LEGACY_PDCP_TRACE: S("PDCP", "TRACE"); break;
  case T_LEGACY_ENB_APP_INFO: S("ENB_APP", "INFO"); break;
  case T_LEGACY_ENB_APP_ERROR: S("ENB_APP", "ERROR"); break;
  case T_LEGACY_ENB_APP_WARNING: S("ENB_APP", "WARNING"); break;
  case T_LEGACY_ENB_APP_DEBUG: S("ENB_APP", "DEBUG"); break;
  case T_LEGACY_ENB_APP_TRACE: S("ENB_APP", "TRACE"); break;
  case T_LEGACY_SCTP_INFO: S("SCTP", "INFO"); break;
  case T_LEGACY_SCTP_ERROR: S("SCTP", "ERROR"); break;
  case T_LEGACY_SCTP_WARNING: S("SCTP", "WARNING"); break;
  case T_LEGACY_SCTP_DEBUG: S("SCTP", "DEBUG"); break;
  case T_LEGACY_SCTP_TRACE: S("SCTP", "TRACE"); break;
  case T_LEGACY_UDP__INFO: S("UDP", "INFO"); break;
  case T_LEGACY_UDP__ERROR: S("UDP", "ERROR"); break;
  case T_LEGACY_UDP__WARNING: S("UDP", "WARNING"); break;
  case T_LEGACY_UDP__DEBUG: S("UDP", "DEBUG"); break;
  case T_LEGACY_UDP__TRACE: S("UDP", "TRACE"); break;
  case T_LEGACY_NAS_INFO: S("NAS", "INFO"); break;
  case T_LEGACY_NAS_ERROR: S("NAS", "ERROR"); break;
  case T_LEGACY_NAS_WARNING: S("NAS", "WARNING"); break;
  case T_LEGACY_NAS_DEBUG: S("NAS", "DEBUG"); break;
  case T_LEGACY_NAS_TRACE: S("NAS", "TRACE"); break;
  case T_LEGACY_HW_INFO: S("HW", "INFO"); break;
  case T_LEGACY_HW_ERROR: S("HW", "ERROR"); break;
  case T_LEGACY_HW_WARNING: S("HW", "WARNING"); break;
  case T_LEGACY_HW_DEBUG: S("HW", "DEBUG"); break;
  case T_LEGACY_HW_TRACE: S("HW", "TRACE"); break;
  case T_LEGACY_EMU_INFO: S("EMU", "INFO"); break;
  case T_LEGACY_EMU_ERROR: S("EMU", "ERROR"); break;
  case T_LEGACY_EMU_WARNING: S("EMU", "WARNING"); break;
  case T_LEGACY_EMU_DEBUG: S("EMU", "DEBUG"); break;
  case T_LEGACY_EMU_TRACE: S("EMU", "TRACE"); break;
  case T_LEGACY_OTG_INFO: S("OTG", "INFO"); break;
  case T_LEGACY_OTG_ERROR: S("OTG", "ERROR"); break;
  case T_LEGACY_OTG_WARNING: S("OTG", "WARNING"); break;
  case T_LEGACY_OTG_DEBUG: S("OTG", "DEBUG"); break;
  case T_LEGACY_OTG_TRACE: S("OTG", "TRACE"); break;
  case T_LEGACY_OCG_INFO: S("OCG", "INFO"); break;
  case T_LEGACY_OCG_ERROR: S("OCG", "ERROR"); break;
  case T_LEGACY_OCG_WARNING: S("OCG", "WARNING"); break;
  case T_LEGACY_OCG_DEBUG: S("OCG", "DEBUG"); break;
  case T_LEGACY_OCG_TRACE: S("OCG", "TRACE"); break;
  case T_LEGACY_OMG_INFO: S("OMG", "INFO"); break;
  case T_LEGACY_OMG_ERROR: S("OMG", "ERROR"); break;
  case T_LEGACY_OMG_WARNING: S("OMG", "WARNING"); break;
  case T_LEGACY_OMG_DEBUG: S("OMG", "DEBUG"); break;
  case T_LEGACY_OMG_TRACE: S("OMG", "TRACE"); break;
  case T_LEGACY_GTPU_INFO: S("GTPU", "INFO"); break;
  case T_LEGACY_GTPU_ERROR: S("GTPU", "ERROR"); break;
  case T_LEGACY_GTPU_WARNING: S("GTPU", "WARNING"); break;
  case T_LEGACY_GTPU_DEBUG: S("GTPU", "DEBUG"); break;
  case T_LEGACY_GTPU_TRACE: S("GTPU", "TRACE"); break;
  case T_LEGACY_TMR_INFO: S("TMR", "INFO"); break;
  case T_LEGACY_TMR_ERROR: S("TMR", "ERROR"); break;
  case T_LEGACY_TMR_WARNING: S("TMR", "WARNING"); break;
  case T_LEGACY_TMR_DEBUG: S("TMR", "DEBUG"); break;
  case T_LEGACY_TMR_TRACE: S("TMR", "TRACE"); break;
  case T_LEGACY_OSA_INFO: S("OSA", "INFO"); break;
  case T_LEGACY_OSA_ERROR: S("OSA", "ERROR"); break;
  case T_LEGACY_OSA_WARNING: S("OSA", "WARNING"); break;
  case T_LEGACY_OSA_DEBUG: S("OSA", "DEBUG"); break;
  case T_LEGACY_OSA_TRACE: S("OSA", "TRACE"); break;
  case T_LEGACY_component_INFO: S("XXX", "INFO"); break;
  case T_LEGACY_component_ERROR: S("XXX", "ERROR"); break;
  case T_LEGACY_component_WARNING: S("XXX", "WARNING"); break;
  case T_LEGACY_component_DEBUG: S("XXX", "DEBUG"); break;
  case T_LEGACY_component_TRACE: S("XXX", "TRACE"); break;
  case T_LEGACY_componentP_INFO: S("XXX", "INFO"); break;
  case T_LEGACY_componentP_ERROR: S("XXX", "ERROR"); break;
  case T_LEGACY_componentP_WARNING: S("XXX", "WARNING"); break;
  case T_LEGACY_componentP_DEBUG: S("XXX", "DEBUG"); break;
  case T_LEGACY_componentP_TRACE: S("XXX", "TRACE"); break;
  case T_LEGACY_CLI_INFO: S("CLI", "INFO"); break;
  case T_LEGACY_CLI_ERROR: S("CLI", "ERROR"); break;
  case T_LEGACY_CLI_WARNING: S("CLI", "WARNING"); break;
  case T_LEGACY_CLI_DEBUG: S("CLI", "DEBUG"); break;
  case T_LEGACY_CLI_TRACE: S("CLI", "TRACE"); break;
  default: printf("unkown message type %d\n", m); abort();
  }

#ifdef T_USE_SHARED_MEMORY
  T_cache[T_busylist_head].busy = 0;
  T_busylist_head++;
  T_busylist_head &= T_CACHE_SIZE - 1;
#endif
}

#ifdef T_USE_SHARED_MEMORY

void wait_message(void)
{
  while (T_cache[T_busylist_head].busy == 0) usleep(1000);
}

void init_shm(void)
{
  int s = shm_open(T_SHM_FILENAME, O_RDWR | O_CREAT, 0666);
  if (s == -1) { perror(T_SHM_FILENAME); abort(); }
  if (ftruncate(s, T_CACHE_SIZE * sizeof(T_cache_t)))
    { perror(T_SHM_FILENAME); abort(); }
  T_cache = mmap(NULL, T_CACHE_SIZE * sizeof(T_cache_t),
                 PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (T_cache == NULL)
    { perror(T_SHM_FILENAME); abort(); }
  close(s);
}

#endif /* T_USE_SHARED_MEMORY */

int main(void)
{
  int s;
  int l;
  char t;
#ifdef T_USE_SHARED_MEMORY
  init_shm();
#endif
  s = get_connection("127.0.0.1", 2020);
  /* send the first message - activate all traces */
  t = 0;
  if (write(s, &t, 1) != 1) abort();
  l = T_NUMBER_OF_IDS;
  if (write(s, &l, sizeof(int)) != sizeof(int)) abort();
  for (l = 0; l < T_NUMBER_OF_IDS; l++)
    if (write(s, &l, sizeof(int)) != sizeof(int)) abort();
  /* read messages */
  while (1) {
#ifdef T_USE_SHARED_MEMORY
    wait_message();
#endif
    get_message(s);
  }
  return 0;
}
