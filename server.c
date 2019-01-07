#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include"trans.h"

#define PORT 8080


void *connection_handler(void *);
void print_normal_choice(int sock);
char* print_joint_choice();
void print_admin_choice(int sock);
void switch_fn(char read_choice,int sock,char* username,char* pass);
void switch_fn_admin(char read_choice,int sock);
int main()
{
	int serv_fd,client_sock,*new_sock;
	struct sockaddr_in serv_addr,client_addr;
	int addrlen = sizeof(serv_addr);
	int opt = 1;
	char buf[1024];
	//char *hello = "Hello from server";
	if((serv_fd=socket(AF_INET, SOCK_STREAM,0))==0)
	{
		perror("error in socket creation");
		exit(EXIT_FAILURE);
	}
	printf("server fd is %d\n",serv_fd);
	if(setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if(bind(serv_fd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(serv_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    puts("waiting for incoming Connection");
    pthread_t thread_id;
    while ((client_sock = accept(serv_fd, (struct sockaddr *)&client_addr,(socklen_t*)&addrlen)))
    {
    	printf("Connection accepted from client IP: %s , client port : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        new_sock = malloc(sizeof *new_sock);
        *new_sock = client_sock;

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    // valread = read(new_socket,buf, 1024);
    // printf("%s\n",buf );
    // send(new_socket , hello , strlen(hello) , 0 );
    // printf("Hello message sent\n");
}
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char wel_message[1024],client_message[2000],write_choice,read_choice;

    // welcome message

    // wel_message = "Enter your choice\n"
    //               "1. Normal user!\n"
    //               "2. Joint user\n"
    //               "3. Administrator\n";
    strcpy(wel_message,"1.Normal User\n2.Joint user\n3.Administrator\nEnter your choice\n");
    if(write(sock,wel_message,1024)<=0)
    {
        //printf("Welcome msg sent\n");
        exit(EXIT_FAILURE);
    }
    if((read(sock,&write_choice,1))<=0)
        exit(EXIT_FAILURE);
    //printf("%c",write_choice);
    // check username and password
        char username[100],pass[100];
        int user_type;
        char auth_success;
        //memset(username,'\0',sizeof(username));
        while(1){
            memset(username,'\0',sizeof(username));
            strcpy(username,"Enter your username\n");
            write(sock,username,100);
            memset(username,'\0',sizeof(username));
            read(sock,username,100);
            //memset(pass,'\0',sizeof(pass));
            memset(pass,'\0',sizeof(pass));
            strcpy(pass,"Enter your password\n");
            write(sock,pass,100);
            memset(pass,'\0',sizeof(pass));
            read(sock,pass,100);
            user_type = authenticate(username,pass,write_choice);
            if(user_type!=-1)
                {
                    auth_success = 'y';
                    write(sock,&auth_success,1);
                    break;
                }
            else
                {
                    auth_success = 'n';
                    write(sock,&auth_success,1);
                }
            }


    //char *username = "ankitr774";
    //char *pass = "ank";
    char *user_fn;
    switch(write_choice)
    {
        case '1':
            //while(read_choice!='6')
            //{
                while(1)
                {
                print_normal_choice(sock);
                //if((write(sock,user_fn,sizeof(user_fn)))<=0)
                    //exit(EXIT_FAILURE);
                // read trans type
                if((read(sock,&read_choice,1))<=0);
                    //exit(EXIT_FAILURE);
                if(read_choice=='6')
                    break;
                switch_fn(read_choice,sock,username,pass);
                //printf("choice enter is %c\n",read_choice);
            //}
                }

            break;
        case '2':
            while(1)
                {
                print_normal_choice(sock);
                //if((write(sock,user_fn,sizeof(user_fn)))<=0)
                    //exit(EXIT_FAILURE);
                // read trans type
                if((read(sock,&read_choice,1))<=0);
                    //exit(EXIT_FAILURE);
                if(read_choice=='6')
                    break;
                switch_fn(read_choice,sock,username,pass);
                //printf("choice enter is %c\n",read_choice);
            //}
                }
                break;
        case '3':
            while(1)
            {
                print_admin_choice(sock);
                 if((read(sock,&read_choice,1))<=0);
                        //exit(EXIT_FAILURE);
                if(read_choice=='5')
                        break;
                switch_fn_admin(read_choice,sock);
            }
            break;
        default:
            user_fn = "Wrong choice";
            if((write(sock,user_fn,strlen(user_fn)))<=0)
                exit(EXIT_FAILURE);
            break;

    }


        //printf("welcome sent");

    // //Send some messages to the client
    // message = "Greetings! I am your connection handler\n";
    // write(sock , message , strlen(message));

    // message = "Now type something and i shall repeat what you type \n";
    // write(sock , message , strlen(message));

    //Receive a message from client
  //   while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
  //   {
  //       //end of string marker
		// client_message[read_size] = '\0';

		// //Send the message back to client
  //       write(sock , client_message , strlen(client_message));

		// //clear the message buffer
		// memset(client_message, 0, 2000);
  //   }

    /*read_size = recv(sock , client_message , 2000 , 0);
    if(read_size>0)
    {
        client_message[read_size]='\0';
        printf("%s\n",client_message);
    }

    write(sock,"hello",5);
    */

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    free(socket_desc);
    close(sock);
    pthread_exit(NULL);
}

void print_normal_choice(int sock)
{
    char c[1024];
    memset(c,'\0',sizeof(c));
    strcpy(c,"1. Deposit\n2. Withdraw\n3. Balance Enquiry\n4. Password change\n5. view Details\n6. Exit\nEnter your choice\n");
    if((write(sock,c,sizeof(c)))<=0);
}

char* print_joint_choice()
{
    return NULL;
    //print_normal_choice();
}

void print_admin_choice(int sock)
{
    char c[1024];
    memset(c,'\0',sizeof(c));
    strcpy(c,"1. Add user\n2. Delete user\n3. Modify user\n4. Search user\n5. Exit\n");
    if((write(sock,c,sizeof(c)))<=0);
}

void switch_fn(char read_choice,int sock,char* username,char* pass)
{
    //char *c=(char*)malloc(1024);
        char c[200];
        memset(c,'\0',sizeof(c));
        char amt[20];
        memset(amt,'\0',sizeof(amt));
        char new_pass[100];
        char *view_detail;
        int bal;
        int amt_int;
    switch(read_choice)
    {
        case '1':
            strcpy(c,"Enter the amount to be deposited\n");
            write(sock,c,strlen(c));
            read(sock,amt,20);
            amt_int = atoi(amt);
            memset(c,'\0',sizeof(c));
            if(deposit(username,pass,atoi(amt)))
                strcpy(c,"Amount is deposited successfully\n");
            else
                strcpy(c,"Error in deposit\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            break;
        case '2':
            strcpy(c,"Enter the amount for Withdraw\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,amt,20);
            if(withdraw(username,pass,atoi(amt)))
                strcpy(c,"Amount is Withdraw successfully\n");
            else
                strcpy(c,"Error in Withdraw\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            break;
        case '3':
            bal = bal_enquiry(username,pass);
            if(bal>0)
            {
                sprintf(amt,"%d",bal);
                strcpy(c,"Your current balance is ");
                strcat(c,amt);
                strcat(c,"\n");
            }
            else
                strcpy(c,"Error in retrieving balance\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            break;

        case '4':
            strcpy(c,"Enter the new Password \n");
            write(sock,c,strlen(c));
            read(sock,new_pass,100);
            if(pass_change(username,pass,new_pass))
            {
                strcpy(c,"Your password has changed successfully\n");
            }
            else
                strcpy(c,"Error in changing password\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            break;
        case '5':
            view_detail = view_user_details(username,pass);
            write(sock,view_detail,strlen(view_detail));
            break;
        default:
            strcpy(c,"Wrong choice entered\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
    }
}
void switch_fn_admin(char read_choice,int sock)
{
    char c[200];
    char name[200],username[200],pass[200],bal_char[20],acnt_char[20],type_char;
    int balance,type,account_no;
    int rc;
    char *view_detail,user_check_rc;
    memset(c,'\0',sizeof(c));
    switch(read_choice)
    {
        case '1':                               //add user admin
            strcpy(c,"Enter the type for the user(0.Normal 1.Joint user)");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,&type_char,1);
            if(type_char=='0')
                strcpy(c,"Enter the name of the user");
            if(type_char=='1')
            {
               strcpy(c,"Enter the name of the both the user separted by \',\'");
            }
            type = type_char - '0';
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,name,200);
            while(1)
            {
            strcpy(c,"Enter the username of the user");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,username,200);
            if(!check_username_exist(username))
                {
                    user_check_rc = 'y';
                    write(sock,&user_check_rc,1);
                    memset(c,'\0',sizeof(c));
                    break;
                }
            else
                {
                    user_check_rc = 'n';
                    write(sock,&user_check_rc,1);
                    //strcpy(c,"Username already exists please enter different username\n");
                    //write(sock,c,strlen(c));
                    memset(c,'\0',sizeof(c));
                }
            }
            strcpy(c,"Enter the password for the user");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,pass,200);
            strcpy(c,"Enter the account no. for the user");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,acnt_char,200);
            account_no = atoi(acnt_char);
            strcpy(c,"Enter the balance for the user");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,bal_char,20);
            balance = atoi(bal_char);
            rc = create_new_users(name,username,pass,balance,type,account_no);
            if(rc!=-1)
                strcpy(c,"User has been added successfully\n");
            else
                strcpy(c,"Error in adding the user\n");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));

            break;

        case '2':
            strcpy(c,"Enter the account no.for the user which you want to delete");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,acnt_char,200);
            account_no = atoi(acnt_char);
            if(delete_user(account_no))
            {
                strcpy(c,"User has deleted successfully");
            }
            else
            {
                strcpy(c,"Error in deleting the user");
            }
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));


            break;
        case '3':
            strcpy(c,"Enter the account no. for the user which you want to modify");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,acnt_char,200);
            account_no = atoi(acnt_char);
            strcpy(c,"Enter the modified name");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,name,200);
            strcpy(c,"Enter the modified username");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,username,200);
            strcpy(c,"Enter the modified password");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,pass,200);
            if(modify_user(name,username,pass,account_no))
            {
                strcpy(c,"User has been modified successfully");
            }
            else
            {
                strcpy(c,"Error in modifying the user");
            }
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            break;

        case '4':
            strcpy(c,"Enter the account no.for the user which you want to search");
            write(sock,c,strlen(c));
            memset(c,'\0',sizeof(c));
            read(sock,acnt_char,200);
            account_no = atoi(acnt_char);
            view_detail = print_user_details(account_no);
            write(sock,view_detail,strlen(view_detail));
            memset(view_detail,'\0',sizeof(view_detail));
            break;

    }
}
