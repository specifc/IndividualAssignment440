#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>


char *QOTD;
const char *qotdFile;
pthread_mutex_t quoteLock=PTHREAD_MUTEX_INITIALIZER;
pthread_t checkForNewDayThread, connectionHandlerThread;

//This segment will link to the quote file
int lineNumber ( const char* quoteFile ){
    int count = 0;
    int current;
    FILE* fds=fopen( quoteFile,"r" );
    if( fds == NULL ){
        perror( "Fail to open the file" );
        exit( EXIT_FAILURE );
    }
    while(true){
        current=fgetc ( fds );
        switch ( current ){
            case '\n':
			{
                count++;
                break;
            }
            case EOF:
			{
                fclose( fds );
                return count;
            }
            default:
                continue;
        }
    }
}

//This segment is to find the quote file and will sent to buffer address
char* read_file ( const char* filePath )
{
   int quotes_no = lineNumber ( filePath );
   int lineNumberOfQOTD = rand( ) % quotes_no;
   int count = 0;
   char* lineptr = NULL; 
   size_t n = 0; 
   FILE* fds = fopen( filePath, "r" );
    if ( fds == NULL )
	{
        perror( "Oh no, we encounter error to open the Quote file" );
        exit ( EXIT_FAILURE );
    }
   while ( count < lineNumberOfQOTD )
   { 
       if( fgetc ( fds ) == '\n' ) count++;
   }
    getline( &lineptr, &n, fds ); 
    fclose( fds );
    return lineptr;
}

//This segment is to handle connections 
void * qotdConnection( void* port_ptr )
{
    struct sockaddr_in address;
    int server_fds, new_socket, opt = 1, addrlen = sizeof(address), port=* ( ( int* ) port_ptr);

    free(port_ptr);

    //This if statement is to create socket file connections
    if ((server_fds = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
    {
        perror( "socket failed" );
        exit( EXIT_FAILURE );
    }

    //This if statement is to assign ip address and port 17
    if (setsockopt(server_fds, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))==-1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (1717);

    //This if statement is to bind the socket with port 17
    if (bind(server_fds, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fds, 100) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening to port %i\n", port);

    while(1){ 
        if ((new_socket = accept(server_fds, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0)
	{
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_lock(&quoteLock);
        send(new_socket, QOTD, strlen(QOTD), 0);
        pthread_mutex_unlock(&quoteLock);
        close(new_socket);
    }
}

int main(int argc, char const *argv[])
{
    
    int* port=malloc(sizeof(int));

    

    time_t timeToExecute=time(NULL); 
    struct tm tm = *localtime(&timeToExecute);
    int day=tm.tm_mday; 
	
	switch(argc){ 
        case 1:{ 
            *port=1717;
            qotdFile=strdup("quote.txt");
            break;
        }
        case 2:{ 
            *port=atoi(argv[2]);
            qotdFile=argv[1];
            break;
        }
        
        

    }
    

	printf( "Hello %s !", getlogin() );
	printf( "\n" );
	printf( "This server contains %i quotes\n", lineNumber( qotdFile ) );
	{
        srand( timeToExecute ); 
	}

    QOTD = read_file(qotdFile); 
    qotdConnection(port);

    return 0;
}
