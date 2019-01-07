#include<stdbool.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include"create_user.h"

#define FILE_USER "/home/ankit/Desktop/Term1/System Software/mini_project/users.txt"
#define FILE_TRANS "/home/ankit/Desktop/Term1/System Software/mini_project/trans.txt"

bool deposit(char *username,char *pass,int balance)
{
	struct user *user_entry = (struct user *)malloc(sizeof(struct user));
	struct trans *trans_entry = (struct trans *)malloc(sizeof(struct trans));
	struct flock fl;

		fl.l_pid = getpid();

	int user_file,trans_file,fcntl_rc;
	int size = sizeof(struct user);
	user_file = open(FILE_USER,O_RDWR,0777);
	trans_file = open(FILE_TRANS,O_RDWR,0777);
	if(user_file<2)
	{
		printf("error in file opening\n");
		return false;
	}
	lseek(user_file,0,SEEK_SET);
	// printf("%d",read(user_file,user_entry,sizeof(struct user)));
	int read_size = read(user_file,user_entry,size);
	while(read_size>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive)
		{
			trans_entry->account_no = user_entry->account_no;		// for trans
			strcpy(trans_entry->trans_type,"Deposit");
			trans_entry->old_balance = user_entry->balance;
			
			user_entry->balance = user_entry->balance + balance;

			trans_entry->new_balance = user_entry->balance;

			lseek(user_file,-size,SEEK_CUR);
			fl.l_whence = SEEK_CUR;
			fl.l_start = 0;
			fl.l_len = size;
			// fl.l_type = F_UNLCK;
			// fcntl_rc = fcntl(user_file,F_SETLK,&fl);
			fl.l_type = F_WRLCK;
			fcntl(user_file,F_GETLK,&fl);

			//if(fl.l_type==F_WRLCK)
                fl.l_type = F_UNLCK;
			fcntl_rc = fcntl(user_file,F_SETLKW,&fl);
			write(user_file,user_entry,size);
			write(trans_file,trans_entry,sizeof(struct trans));
			fl.l_type = F_UNLCK;
			fcntl(user_file,F_SETLK,&fl);

			//transaction entry

			return true;
		}
		read_size = read(user_file,user_entry,sizeof(struct user));
	}

	printf("Error in deposit\n");
	return false;
}

bool withdraw(char *username,char *pass,int balance)
{
	struct user *user_entry = (struct user *)malloc(sizeof(struct user));
	struct trans *trans_entry = (struct trans *)malloc(sizeof(struct trans));
	struct flock fl;
		fl.l_type = F_WRLCK;
		fl.l_pid = getpid();
	int user_file,trans_file;
	int size = sizeof(struct user);
	user_file = open(FILE_USER,O_RDWR,0744);
	if(user_file<2)
	{
		printf("error in file opening\n");
		return false;
	}
	int read_size = read(user_file,user_entry,sizeof(struct user));
	//printf("%d\n",read_size);
	while(read_size>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive)
		{
			trans_entry->account_no = user_entry->account_no;		// for trans
			strcpy(trans_entry->trans_type,"withdraw");
			trans_entry->old_balance = user_entry->balance;

			user_entry->balance = user_entry->balance - balance;

			trans_entry->new_balance = user_entry->balance;
			
			if((user_entry->balance)<0)
			{
				printf("insufficient balance");
				return false;
			}
			lseek(user_file,-size,SEEK_CUR);
			fl.l_whence = SEEK_CUR;
			fl.l_start = 0;
			fl.l_len = size;
			fcntl(user_file,F_SETLKW,&fl);
			write(user_file,user_entry,size);
			fl.l_type = F_UNLCK;
			fcntl(user_file,F_SETLK,&fl);
			return true;
		}
		read_size = read(user_file,user_entry,sizeof(struct user));
	}

	printf("Error in withdraw\n");
	return false;
}

int bal_enquiry(char *username,char *pass)
{
	struct user *user_entry = (struct user *)malloc(sizeof(struct user));
	int user_file;
	int size = sizeof(struct user);
	struct flock fl;
		fl.l_type = F_WRLCK;
		fl.l_pid = getpid();
	user_file = open(FILE_USER,O_CREAT|O_RDONLY,0744);
	if(user_file<2)
	{
		printf("error in file opening");
		return -1;
	}
	int read_size = read(user_file,user_entry,sizeof(struct user));
	while(read_size>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive)
		{

			return user_entry->balance;
		}
		read_size = read(user_file,user_entry,sizeof(struct user));
	}

	printf("Error in display balance\n");
	return -1;
}

bool pass_change(char *username,char *pass,char *new_pass)
{
	struct user *user_entry = (struct user *)malloc(sizeof(struct user));
	int user_file;
	int size = sizeof(struct user);
	struct flock fl;
		fl.l_type = F_WRLCK;
		fl.l_pid = getpid();
	user_file = open(FILE_USER,O_RDWR,0744);
	if(user_file<2)
	{
		printf("error in file opening\n");
		return false;
	}
	int read_size = read(user_file,user_entry,sizeof(struct user));
	while(read_size>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive)
		{
			fl.l_whence = SEEK_CUR;
			fl.l_start = 0;
			fl.l_len = size;
			fcntl(user_file,F_SETLKW,&fl);
			strcpy(user_entry->pass,new_pass);
			lseek(user_file,-size,SEEK_CUR);
			write(user_file,user_entry,size);
			fl.l_type = F_UNLCK;
			fcntl(user_file,F_SETLK,&fl);
			return true;
		}
		read_size = read(user_file,user_entry,sizeof(struct user));
	}

	printf("Error in changing the pass\n");
	return false;
}

struct user *search_user_trans(char* username,char* pass)
{
	int user_file;
	int size;
	size = sizeof(struct user);
	//struct user user_entry;
	struct user *user_entry = (struct user *)malloc(size);
	user_file = open(FILE_USER,O_RDONLY,0744);
	if(user_file<2)
	{
		//perror("error in file opening please enter again\n");
		printf("Error in file opening\n");
		return NULL;
	}
	//printf("%d\n",(read(user_file,&user_entry,sizeof(struct user))));
	int read_size = read(user_file,user_entry,sizeof(struct user));
	while(read_size>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive)
		{
			//*user_entry2 = user_entry;
			return user_entry;
		}
		read_size = read(user_file,user_entry,sizeof(struct user));
	}
	printf("User not found\n");
	return NULL;
}
char* view_user_details(char* username,char* pass)
{
    struct user *srch_user;
    srch_user = search_user_trans(username,pass);
    char *c = (char*)malloc(1024);
    char acnt[50],bal[20];
	if(srch_user->isactive==true)
	{
        strcat(c,"Name: ");
		strcat(c,srch_user->name);
		strcat(c,"\nUsername: ");
		strcat(c,srch_user->user_name);
		strcat(c,"\nAccount No.: ");
		sprintf(acnt,"%d",srch_user->account_no);
		strcat(c,acnt);
		strcat(c,"\nBalance: ");
		sprintf(bal,"%d",srch_user->balance);
		strcat(c,bal);
		strcat(c,"\n");
		//printf("Account No.: %d\n", srch_user->account_no);
		//printf("Balance: %d\n", srch_user->balance);
		//printf("pass: %s\n",srch_user->pass);
	}
	else
		//printf("User doesn't exist\n");
		strcat(c,"User doesn't exist");
    return c;
}

int authenticate(char *username,char *pass,char write_choice)
{
	int user_file;
	int size = sizeof(struct user);
	int type_int = write_choice - '0';
	type_int = type_int - 1;
	struct user *user_entry = (struct user *)malloc(size);
	user_file = open(FILE_USER,O_RDONLY,0744);
	if(user_file<2)
	{
		printf("error in file opening\n");
		return -1;
	}
	while((read(user_file,user_entry,sizeof(struct user)))>0)
	{
		if(strcmp(username,user_entry->user_name)==0 && strcmp(pass,user_entry->pass)==0 && user_entry->isactive && user_entry->type == type_int)
		{
			return user_entry->type;
		}
	}

	printf("Error in authenticate\n");
	return -1;
}







