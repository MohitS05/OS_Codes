#include<stdio.h>
#include<stdlib.h>
#include<systemd/sd-bus.h>

//void unrefdbus(sd_bus_message * , sd_bus *);

//int main(int argc,char argv[])
int main()
{
	//printf("Code started");
	sd_bus_error error = SD_BUS_ERROR_NULL;
	sd_bus_message *msg1 = NULL;
	sd_bus *bus = NULL;
	//const char *path ;
	int ptr;

	ptr = sd_bus_open_user(&bus);
	if(ptr < 0){
                fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-ptr));
                goto end;
                //unrefdbus(msg,bus);
	}
	int a=50,b=6;

	/* Parse the response message */
    int64_t res;
   /* ptr = sd_bus_message_read(msg1, "x", &res);
    if (ptr < 0) {
            fprintf(stderr, "Failed to parse response message: %s\n", strerror(-ptr));
            
            goto end;
    }*/

	//int r;
	ptr = sd_bus_call_method(bus,
                               "com.mohit.dbus",           /* service to contact */
                               "/home/mybus/Sdbus",          /* object path */
                               "com.mohit.dbus",   /* interface name */
                               "Multiply",                          /* method name */
                               &error,                               /* object to return error in */
                               &msg1,                                   /* return message on success */
                               "xx",                                 /* input signature */
                               a,                       /* first argument */
                               b);                           /* second argument */
    if (ptr < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto end;
    }
	
    ptr = sd_bus_message_read(msg1, "x", &res);
    if (ptr < 0) {
            fprintf(stderr, "Failed to parse response message: %s\n", strerror(-ptr));
            
            goto end;
        }
	if(ptr < 0){
                fprintf(stderr, "Failed to Parse response: %s\n", strerror(-ptr));
                //fprintf(stderr, "Failed to parse response message: %s\n", strerror(ptr));
                fprintf(stderr,"OHHH YEEAHH\n");
                goto end;
	}
	printf("res1 =%ld\n", res);
	/* Parse the response message */


	/* To Check service is running or not, otherwise start the service by informing other program. */

	FILE *fp;
    char path[1035];
    char servss[] = "whoopsie";
    char command[70];
    snprintf(command,sizeof(command),"systemctl status %s |grep Active",servss);
  /* Open the command for reading. */
 //   fp = popen("systemctl status whoopsie| grep Active", "r");
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
         }
        char *ded = "dead";
    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path), fp) != NULL) {
        
        if(strstr(path, ded) != NULL)
        {
            printf("Service inactive :D \n");
            
            pclose(fp);
            goto dones;


        }
    else
    {
      printf("service is running, no action needed, going to next \n");
      pclose(fp);
      goto next;
      break;
    } }

    dones:
	
	ptr = sd_bus_call_method(bus,
                               "com.mohit.dbus",           /* service to contact */
                               "/home/mybus/Sdbus",          /* object path */
                               "com.mohit.dbus",   /* interface name */
                               "S_START",                          /* method name */
                               &error,                               /* object to return error in */
                               &msg1,                                   /* return message on success */
                               "s",                                 /* input signature */
                               servss);                           /* second argument */
	if (ptr < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto end;
    }
    ptr = sd_bus_message_read(msg1, "x", &res);
    if (ptr < 0) {
            fprintf(stderr, "Failed to parse response message: %s\n", strerror(-ptr));
            
            goto end;
        }
    printf("res2 =%ld\n", res);

    next:


/* Code to exit the service with given status */

    ptr = sd_bus_call_method(bus,
                               "com.mohit.dbus",           /* service to contact */
                               "/home/mybus/Sdbus",          /* object path */
                               "com.mohit.dbus",   /* interface name */
                               "EXIT_SERV",                          /* method name */
                               &error,                               /* object to return error in */
                               &msg1,                                   /* return message on success */
                               "x",                                 /* input signature */
                               1);
    if (ptr < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto end;
    }
    ptr = sd_bus_message_read(msg1, "x", &res);
    if (ptr < 0) {
            fprintf(stderr, "Failed to parse response message: %s\n", strerror(-ptr));
            
            goto end;
        }
    printf("Server exited with status =%ld\n", res);

	//printf("Service job is in queue as %s .\n",path);
	
	/*
	ptr =  sd_bus_call_method(bus,
							"com.mohit.dbusserv",	//Service to connect
							"/home/mybus/Sdbus",	//Object path
							"com.mohit.dbus",		//Interface name
							"Start",				//Method name
							&err,					//Error return locations
							"s",					//input signature
							cos);			//First argument aka service to start
	if(ptr < 0){
                fprintf(stderr, "Failed to Parse response: %s\n", strerror(-ptr));
                goto end;
                //unrefdbus(msg,bus);
                return EXIT_FAILURE;
	}*/
	/*ptr =  sd_bus_call_method(bus,
							"com.mohit.dbusserv",	//Service to connect
							"/home/mybus/Sdbus",	//Object path
							"com.mohit.dbus",		//Interface name
							"Start",				//Method name
							&err,					//Error return locations
							"xx",					//input signature
							a,
							b);			//First argument aka service to start)*/
	
	//Taking response now
	//ptr = sd_bus_message_read(msg1,"x",res);
	//printf("Service job is in queue as %s .\n",path);
end:
	sd_bus_error_free(&error);
    sd_bus_message_unref(msg1);
    sd_bus_unref(bus);
	return EXIT_SUCCESS;
}
/*
void unrefdbus(sd_bus_message * m, sd_bus * bus)
{
		
}*/