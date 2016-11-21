#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 128

typedef struct rwbuf
{
   int fd;              // file descriptor
   char ubuf[BUFSIZE];  // write buffer
   int ubuf_len;        // number of bytes currently in the buffer
} rwbuf_t;

/*! This function reads....*/
int buf_read(rwbuf_t *rb, char *data, int data_len)
{
   int rlen, copy_len, total_len;

   for (total_len = 0; data_len > 0; total_len += copy_len)
   {
      // check if buffer is empty
      if (rb->ubuf_len == 0)
      {
         if ((rlen = read(rb->fd, rb->ubuf, sizeof(rb->ubuf))) == -1)
            return -1;
         // check for EOF
         if (!rlen)
            break;
         rb->ubuf_len += rlen;
      }

      // check for number of bytes to copy to user buffer
      if (data_len <= rb->ubuf_len)
         copy_len = data_len;
      else
         copy_len = rb->ubuf_len;

      // copy data
      memcpy(data, rb->ubuf, copy_len);
      data += copy_len;
      data_len -= copy_len;

      // move remaining data in internal buffer to the beginning
      rb->ubuf_len -= copy_len;
      memmove(rb->ubuf, rb->ubuf + copy_len, rb->ubuf_len);
   }
   return total_len;
}

int buf_flush(rwbuf_t *wb)
{
   int wlen;

   if ((wlen = write(wb->fd, wb->ubuf, wb->ubuf_len)) == -1)
      return -1;
   wb->ubuf_len -= wlen;
   return wlen;
}


/*! This function writes data to a file using an intermediate buffer.
 * @param wb Pointer to rwbuf_t structure.
 * @param data Pointer to data which shall be written.
 * @param data_len Length of data buffer (number of bytes to write).
 * @return The function returns the number of bytes copied to through
 * the buffer (which typically will be data_len), or -1 in case of
 * error.
 */
int buf_write(rwbuf_t *wb, const char *data, int data_len)
{
   int copy_len, wlen, total_len;

   for (total_len = 0; data_len > 0; total_len += copy_len)
   {
      // check if data fits into the buffer
      if (data_len <= sizeof(wb->ubuf) - wb->ubuf_len)
         // yes
         copy_len = data_len;
      else
         // no
         copy_len = sizeof(wb->ubuf) - wb->ubuf_len;

      // copy data from user buffer to internal buffer
      memcpy(wb->ubuf + wb->ubuf_len, data, copy_len);
      wb->ubuf_len += copy_len;

      // "remove" data from user buffer
      data += copy_len;
      data_len -= copy_len;

      // check if buffer is full
      if (wb->ubuf_len >= sizeof(wb->ubuf))
      {
         if ((wlen = write(wb->fd, wb->ubuf, sizeof(wb->ubuf))) == -1)
            return -1;  //FIXME: improve error handling
         // decrement buffer fill counter...
         wb->ubuf_len -= wlen;
         // move data which was not written to the beginning
         memmove(wb->ubuf, wb->ubuf + wlen, wb->ubuf_len);
      }
   }
   return total_len;
}


int main()
{
   rwbuf_t wb; // output buffer (write)
   rwbuf_t rb; // input buffer (read)

   char buf[20];
   int len;

   wb.fd = 1;
   wb.ubuf_len = 0;
   rb.fd = 0;
   rb.ubuf_len = 0;

   for (;;)
   {
      if ((len = buf_read(&rb, buf, sizeof(buf))) <= 0)
         break;

      buf_write(&wb, buf, len);
   }

   buf_flush(&wb);

   return 0;
}


