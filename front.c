// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

void switch_fn_client(char user_fn,int sock);
void switch_fn_client_admin(char user_fn,int sock);
int main(void)
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";
    char msg[20];
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    	// puts("enter the login");
    	// scanf("%s",msg);
	    // if((send(sock , msg , strlen(msg)+1 , 0 ))>0)
	    // printf("Hello message sent\n");
     //    else
     //        printf("error in sending\n");
	    // valread = read( sock , buffer, 5);
	    // printf("%s\n",buffer );

    // recv. welcome msg

        char wel_msg[1024],user_sp_choice[1024],username[100],pass[100];
        //memset(wel_msg,'\0',sizeof(wel_msg));
        // send the welcome message
        if((read(sock,wel_msg,1024))<=0)
            exit(EXIT_FAILURE);
        printf("%s",wel_msg);

        char type_user;
        scanf("%c",&type_user);
        if((write(sock,&type_user,1))<=0)
            printf("Error in reading type of user");

        // check username and password
        char auth_success;
        while(1)
        {
            read(sock,username,100);
            printf("%s",username);
            memset(username,'\0',sizeof(username));
            scanf("%s",username);
            write(sock,username,100);
            read(sock,pass,100);
            printf("%s",pass);
            memset(pass,'\0',sizeof(pass));
            scanf("%s",pass);
            write(sock,pass,100);
            read(sock,&auth_success,1);
            if(auth_success=='y')
            {
                break;
            }
        }
        //read user type

           // exit(EXIT_FAILURE);

        // receive user function
        //char user_fn[1024];
        char user_fn;
        int user_fn_int;
        switch(type_user)
        {
                case '1':
                //while(1)
                //{
                    // read trans type
                    while(1)
                    {
                        if((read(sock,user_sp_choice,1024))<=0);
                            //exit(EXIT_FAILURE);
                        printf("%s",user_sp_choice);
                        memset(user_sp_choice,'\0',sizeof(user_sp_choice));
                        scanf("%d",&user_fn_int);
                        user_fn = user_fn_int + '0';
                        if((write(sock,&user_fn,1))<=0)
                            exit(EXIT_FAILURE);
                        if(user_fn_int==6)
                            break;
                        // if(user_fn=='6')
                        //     exit(EXIT_FAILURE);
                        switch_fn_client(user_fn,sock);
                    }
                    break;
                case '2':
                    while(1)
                    {
                        if((read(sock,user_sp_choice,1024))<=0);
                            //exit(EXIT_FAILURE);
                        printf("%s",user_sp_choice);
                        memset(user_sp_choice,'\0',sizeof(user_sp_choice));
                        scanf("%d",&user_fn_int);
                        user_fn = user_fn_int + '0';
                        if((write(sock,&user_fn,1))<=0)
                            exit(EXIT_FAILURE);
                        if(user_fn_int==6)
                            break;
                        // if(user_fn=='6')
                        //     exit(EXIT_FAILURE);
                        switch_fn_client(user_fn,sock);
                    }
                    break;
                case '3':
                    while(1)
                    {
                        if((read(sock,user_sp_choice,1024))<=0);
                            //exit(EXIT_FAILURE);
                        printf("%s",user_sp_choice);
                        scanf("%d",&user_fn_int);
                        user_fn = user_fn_int + '0';
                        if((write(sock,&user_fn,1))<=0)
                            exit(EXIT_FAILURE);
                        if(user_fn_int==5)
                            break;
                        switch_fn_client_admin(user_fn,sock);
                    }
                break;

                //}
        }


        // else
        //     printf("Error in receieving wel msg\n");


   close(sock);
    return 0;
}

void switch_fn_client(char user_fn,int sock)
{
    char c[200],amt[20];
    char new_pass[100];
    switch(user_fn)
    {
        case '1':
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",amt);
            write(sock,amt,20);
            memset(c,'\0',sizeof(c));
            read(sock,c,200);
            printf("%s",c);
            break;
        case '2':
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",amt);
            write(sock,amt,20);
            memset(c,'\0',sizeof(c));
            read(sock,c,200);
            printf("%s",c);
            break;
        case '3':
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
            break;
        case '4':
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",new_pass);
            write(sock,new_pass,100);
            memset(c,'\0',sizeof(c));
            read(sock,c,200);
            printf("%s",c);
            break;
        case '5':
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
            break;
        default:
            read(sock,c,200);
            printf("%s",c);
            memset(c,'\0',sizeof(c));
    }
}
void switch_fn_client_admin(char user_fn,int sock)
{
    char c[200],user_check_rc;
    memset(c,'\0',sizeof(c));
    switch(user_fn)
    {
        case '1':               //add user-admin
            // type
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // name
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // username
            while(1)
            {
                read(sock,c,200);
                printf("%s\n",c);
                memset(c,'\0',sizeof(c));
                scanf("%s",c);
                write(sock,c,strlen(c));
                memset(c,'\0',sizeof(c));
                read(sock,&user_check_rc,1);
                if(user_check_rc=='y')
                {
                    break;
                }
            }
            // pass
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // account no.
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // balance
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));

            // return count
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));

            break;
        case '2':
        //delete user
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            //rc
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));

            break;
        case '3':
        //Modify user
            // account no.
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // name
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // username
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            // pass
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));

            //rc
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));

            break;
        case '4':
             //search user
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            scanf("%s",c);
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            //rc
            read(sock,c,200);
            printf("%s\n",c);
            memset(c,'\0',sizeof(c));
            break;


    }
}
