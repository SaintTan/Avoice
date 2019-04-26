#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static char const * const HTTP_200_FORMAT = "HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: %ld\r\n\r\n";
static char const * const HTTP_400 = "HTTP/1.1 400 Bad Request\r\n\
Content-Length: 0\r\n\r\n";
static int const HTTP_400_LENGTH = 47;
static char const * const HTTP_404 = "HTTP/1.1 404 Not Found\r\n\
Content-Length: 0\r\n\r\n";
static int const HTTP_404_LENGTH = 45;

typedef enum{
  GET,
  POST,
  UNKNOWN
} METHOD;

struct user{
  char* cookiestring;
  int current_page;
}

static bool handle_http_request(int sockfd);
char* get_cookie(char* curr);
METHOD get_method(char* curr, int sockfd);

int main(int argc, char* argv[]){
  int sockfd, port_num, maxfd;
  char *address;
  int const reuse = 1;
  fd_set masterfds;
  struct sockaddr_in serv_add, cli_add;

  if (argc < 3){
    fprintf(stderr, "usage: %s ip port \n", argv[0]);
    return 0;
  }

  port_num=atoi(argv[2]);
  address=argv[1];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if( sockfd < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) <0){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  bzero(&serv_add, sizeof(serv_add));
  serv_add.sin_family=AF_INET;
  serv_add.sin_addr.s_addr= inet_addr(address);
  serv_add.sin_port=htons(port_num);

  if(bind(sockfd, (struct sockaddr*) &serv_add,sizeof(serv_add))<0){
    perror("bind");
    exit(EXIT_FAILURE);
  }

  listen(sockfd, 5);

  FD_ZERO(&masterfds);
  FD_SET(sockfd, &masterfds);

  maxfd=sockfd;

  while(1){
    fd_set readfds = masterfds;
    if(select(FD_SETSIZE,&readfds, NULL, NULL, NULL) < 0){
      perror("select");
      exit(EXIT_FAILURE);
    }

    for(int i=0; i<=maxfd;i++){
      if(FD_ISSET(i,&readfds)){
        if(i==sockfd){
          socklen_t cli_len = sizeof(cli_add);
          int newsockfd = accept(sockfd,(struct sockaddr*)&cli_add, &cli_len);

          if(newsockfd<0){
            perror("accept");
          }
          else{
            FD_SET(newsockfd, &masterfds);
            if (newsockfd > maxfd){
              maxfd = newsockfd;
            }
            char ip[INET_ADDRSTRLEN];
            printf("new connection from %s on socket %d\n",
                inet_ntop(cli_add.sin_family, &cli_add.sin_addr, ip, INET_ADDRSTRLEN),
                newsockfd);
          }
        }
        else if (!handle_http_request(i)){
          close(i);
          FD_CLR(i, &masterfds);
        }
      }
    }
  }
}


static bool handle_http_request(int sockfd){
  char buff[2049];
  char *curr;
  METHOD meth;
  int n;

  n = read (sockfd, buff, 2049);
  if(n<=0){
    if(n<0){
      perror("read");
    }
    else{
      printf("socket %d close the connection\n", sockfd);
    }
  }

  buff[n]=0;
  curr=buff;
  meth=get_method(curr, sockfd);

  if(meth==UNKNOWN){
    return false;
  }

  while (*curr == '.' || *curr == '/'){
    ++curr;
  }

  if(*curr == ' '){
    if(meth==GET){
      struct stat st;

      stat("updated_html/1_intro.html", &st);
      n = sprintf(buff, HTTP_200_FORMAT, st.st_size);
      // send the header first
      if (write(sockfd, buff, n) < 0){
          perror("write");
          return false;
      }
      // send the file
      int filefd = open("updated_html/1_intro.html", O_RDONLY);

      n = sendfile(sockfd, filefd, NULL, 2048);
      while(n>0){
        n = sendfile(sockfd, filefd, NULL, 2048);
      }

      if (n < 0){
          perror("sendfile");
          close(filefd);
          return false;
      }
      close(filefd);
    }
  }
  if(meth==POST){

  }
}

METHOD get_method(char* curr, int sockfd){
  char* newcurr=curr;
  if(strncmp(curr,"GET ", 4)==0){
    newcurr+=4;
    strcpy(curr, newcurr);
    return GET;
  }
  else if (strncmp(curr, "POST ", 5) == 0){
    newcurr+=5;
    strcpy(curr, newcurr);
    return POST;
  }
  else if (write(sockfd, HTTP_400, HTTP_400_LENGTH) < 0){
    perror("write");
    return UNKNOWN;
  }
}

char* get_cookie(char* curr){
  char* new
}
