#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>



void enterQuote( int sock_entrance )
{
	char in[ 100 ];
	int i;
	bzero( in, sizeof( in ) );
	printf( "Please enter (I need quote) : " );
	i = 0;
	if( ( in[ i++ ] = getchar() ) != '\n' )
	{
		write( sock_entrance, in, sizeof( in ) );
		bzero( in, sizeof( in ) );
		read( sock_entrance, in, sizeof( in ) );
		printf( "\nQuote of the day : %s\n", in );
	}
}

int main( ) {

	int sock_entrance, connfd;
	struct sockaddr_in server, client;
	
	// create socket
	
	sock_entrance = socket( AF_INET, SOCK_STREAM, 0);
	if ( sock_entrance == -1 ) {
		printf( "Oh no, you fail to create Socket :((\n" );
		
	}
	
	// create socket
	

	// assign IP, PORT
	
	server.sin_addr.s_addr = inet_addr( "192.168.13.7" ); // Server IP
	server.sin_family = AF_INET;
	server.sin_port = htons( 1717 ); // QOTD port
	
	// assign IP, PORT

	// connect to remote server
	
	if ( connect ( sock_entrance, ( struct sockaddr* ) &server, sizeof ( server ) ) != 0) {
		printf( "Failed to connect with server...\n" );
		return 1;
	}
	
	// connect to remote server
	
	
	printf( "==================================================================================================\n" );
	printf( "Succesfully Connected to the server \n" );
	
	
	// function for chat
	
	enterQuote( sock_entrance );
	
	// function for chat

	// close the socket
	
	close( sock_entrance );
	
	// close the socket
}
