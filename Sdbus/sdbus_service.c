#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<systemd/sd-bus.h>


static int checker(sd_bus_message *m, void * , sd_bus_error *);
static int exiting(sd_bus_message *m, void * , sd_bus_error *);
//static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error);
static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "xx", &x, &y);
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Reply with the response */
        return sd_bus_reply_method_return(m, "x", x * y);
}
// The vtable of our little object, implements the interface 
static const sd_bus_vtable service_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("S_START", "s", "x", checker, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("EXIT_SERV","x","x", exiting,SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Multiply", "xx", "x", method_multiply, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};

int main(int argc, char * argv[])
{
	sd_bus_slot *slot = NULL;
	sd_bus *bus = NULL;
	int prt;

	// Connecting to the user bus
	prt = sd_bus_open_user(&bus);
	if(prt<0)
	{
		fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-prt));
		goto end;
	}
	else
		fprintf(stderr,"Connected to bus\n");

	//Installing the object
	prt = sd_bus_add_object_vtable(bus,
								   &slot,
								   "/home/mybus/Sdbus",		//Object Path
								   "com.mohit.dbus",		//Interface name
								   service_vtable,
								   NULL);
	if(prt<0){
		fprintf(stderr, "Failed to issue method call %s \n",strerror(-prt));
		goto end;
	}						
	fprintf(stderr,"Issued method call\n");

	//Taking a well known service name
	prt = sd_bus_request_name(bus,"com.mohit.dbus",0);
	if(prt<0)
	{
		fprintf(stderr, "Failed to acquire service name: %s\n", strerror(-prt));
                goto end;
	}
	fprintf(stderr,"Got a service name\n");


	while(1)
	{
		prt = sd_bus_process(bus,NULL);
		if(prt<0){
			  fprintf(stderr, "Failed to process bus: %s\n", strerror(-prt));
              goto end;
		}

		if(prt>0)
		{
			fprintf(stderr,"Got Request \n");
			continue;
		}

		prt=sd_bus_wait(bus,(uint64_t) -1);
		if(prt<0)
		{
			fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-prt));
         	goto end;
		}
	}

end:
	sd_bus_slot_unref(slot);
	sd_bus_unref(bus);

	return prt < 0 ? EXIT_FAILURE : EXIT_SUCCESS ;
}

int checker(sd_bus_message *m, void *userdata, sd_bus_error *ret_error)
{
	char com[20];
	if(sd_bus_message_read(m,"s",&com))
	{
		printf("Error in read in checker \n");
		return -1;
	}
	char command[70];
    snprintf(command,sizeof(command),"systemctl start %s",com);
    //system(command);
    int flag=0;
    FILE * fp;
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        flag = -1;
        exit(1);
         }
    pclose(fp);

    return sd_bus_reply_method_return(m,"x",flag);

}

static int exiting(sd_bus_message *m, void *userdata , sd_bus_error *ret_error)
{
	uint64_t a;
	int pr;
	pr = sd_bus_message_read(m, "x", &a);
        if (pr < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-pr));
                return pr;
        }

	printf("Exiting with status %" PRIu64 " \n",a);

	sd_bus_reply_method_return(m,"x",a);
	sd_bus_slot_unref(slot);
	sd_bus_unref(bus);
	exit(a);
}

