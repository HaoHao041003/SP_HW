/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		server.sin_family = AF_INET;
		/* Fill in code. */
		host = gethostbyname(resource);
		server.sin_port = htons(PORT);
		memcpy((char *)&server.sin_addr, host->h_addr_list[0], host->h_length);

		/* Allocate socket.
		 * Fill in code. */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			DIE("socket");
		}

		/* Connect to the server.
		 * Fill in code. */
		if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
		{
			DIE("connect");
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if (write(sockfd, sought, sizeof(Dictrec)) != sizeof(Dictrec))
	{
    	DIE("write");
	}
	if (read(sockfd, sought, sizeof(Dictrec)) != sizeof(Dictrec))
	{
		DIE("read");
	}

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
