#include "lwip/def.h"
#include "lwip/api.h"

#if LWIP_NETCONN

#define TASK_HTTP_STACK_SIZE    800
#define TASK_HTTP_PRIO          5

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_index_html[] = "<html><head><title>Congrats!</title></head><body><h1>This is Coocox OS LwIP demo !</h1><p>More information at www.coocox.org</body></html>";

/** HTTP server process function.
 */
static void http_process(struct netconn *conn) {
    struct netbuf *inbuf;
    char *buf;
    u16_t len;
    
    /* Read the data from the port, blocking if nothing yet there. 
    We assume the request (the part we care about) is in one netbuf */
    if (ERR_OK == netconn_recv(conn, &inbuf)) {
        netbuf_data(inbuf, (void **)&buf, &len);
        
        /* Is this an HTTP GET command? (only check the first 5 chars, since
        there are other formats for GET, and we're keeping it very simple )*/
        if (5 <= len && buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T' && buf[3] == ' ' && buf[4] == '/') {
            /* Send the HTML header 
            * subtract 1 from the size, since we dont send the \0 in the string
            * NETCONN_NOCOPY: our data is const static, so no need to copy it
            */
            netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
            
            /* Send our HTML page */
            netconn_write(conn, http_index_html, sizeof(http_index_html) - 1, NETCONN_NOCOPY);
        }
    }
    
    /* Close the connection (server closes in HTTP) */
    netconn_close(conn);
    
    /* Delete the buffer (netconn_recv gives us ownership,
    so we have to make sure to deallocate the buffer) */
    netbuf_delete(inbuf);
}

/** HTTP server task.
 */
static void http_server(void *pdata) {
    struct netconn *conn, *newconn;
    
    /* Create a new TCP connection handle */
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);
    
    /* Bind to port 80 (HTTP) with default IP address */
    netconn_bind(conn, NULL, 80);
    
    /* Put the connection into LISTEN state */
    netconn_listen(conn);
    
    while (1) {
        netconn_accept(conn, &newconn);
        http_process(newconn);
        netconn_delete(newconn);
    }
}

/** Initialize and start HTTP server.
 */
int http_init(void) {
    sys_thread_t err;
    
    err = sys_thread_new(NULL, 
                         http_server, 
                         NULL, 
                         TASK_HTTP_STACK_SIZE, 
                         TASK_HTTP_PRIO);
    
    if (E_CREATE_FAIL == err) {
        return -1;
    }
    
    return 0;	
}

#endif /* LWIP_NETCONN*/
