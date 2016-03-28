#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

static int start;
static int end;
static int traps;

void dummy(void){}


void handle_signal(siginfo_t info)
{
     traps++;
     if(traps == 10000000){
         end = uptime();
         stop();
     }
}

int main(int argc, char *argv[])
{       
        traps = 0;
        start = uptime();
        int x = 5;
        int y = 0;
	signal(SIGFPE, handle_signal);
	x = x/y;
	printf(1, "Traps Performed: %d\n", traps);
	printf(1, "Total Elapsed Time: %d\n", start - end);
	printf(1, "Average Time Per Trap: %d\n", (start - end)/traps);

	exit();
}
