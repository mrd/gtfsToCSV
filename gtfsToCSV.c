#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<error.h>
#include"gtfs-realtime.pb-c.h"

#define DEFAULT_BUFSIZE 32768

static size_t read_buffer (FILE *fp, uint8_t **out)
{
  size_t cur_len = 0, buf_length = DEFAULT_BUFSIZE, nread;
  uint8_t *buf;

  buf = (uint8_t *) malloc(sizeof (uint8_t) * buf_length);
  if(buf == NULL) {
    perror ("malloc");
    return 0;
  }

  while ((nread=fread(buf + cur_len, 1, buf_length - cur_len, fp)) != 0) {
    cur_len += nread;

    if (cur_len >= buf_length) {
      /* double or nothing */
      uint8_t *buf2 = (uint8_t *) realloc(buf, sizeof (uint8_t) * buf_length * 2);
      if(buf2 == NULL) {
        perror ("realloc");
        free (buf);
        *out = NULL;
        return 0;
      }
      buf = buf2;
      buf_length <<= 1;
    }
    
  }

  *out = buf;
  return cur_len;
}

int main(int argc, const char * argv[])
{
  TransitRealtime__FeedMessage *msg;
  uint8_t *buf;
  size_t msg_len,sz;
  int i;
  FILE *fp;

  msg_len = read_buffer (stdin, &buf);
  if(msg_len == 0) {
    fprintf(stderr, "read_buffer failed\n");
    return 1;
  }
  // Deserialize the serialized input
  msg = transit_realtime__feed_message__unpack (NULL, msg_len, buf);
  if (msg == NULL) {
    fprintf(stderr, "error unpacking incoming message\n");
    if(buf) free(buf);
    return 1;
  }

  printf ("routeid, timestamp, vid, tripid, geo, status, stop_sequence, stop_id\n");
  for (i = 0; i < msg->n_entity; i++) {      
    uint64_t ts = msg->entity[i]->vehicle->timestamp;
    struct tm tm;
    char tstr[26];
    gmtime_r(&ts, &tm);
    strftime(tstr, 26, "%Y-%m-%d %H:%M:%S+00", &tm);
    printf("'%s', '%s', '%s', '%s', %f, %f, %d, %d, '%s'\n",
            msg->entity[i]->vehicle->trip->route_id,
            tstr,                    
            msg->entity[i]->vehicle->vehicle->id,
            msg->entity[i]->vehicle->trip->trip_id,
            msg->entity[i]->vehicle->position->longitude,
            msg->entity[i]->vehicle->position->latitude,
            msg->entity[i]->vehicle->current_status,
            msg->entity[i]->vehicle->current_stop_sequence,
            msg->entity[i]->vehicle->stop_id);
  }

  transit_realtime__feed_message__free_unpacked(msg,NULL); // Free the message from unpack()
  if(buf) free(buf);
  return 0;
}
