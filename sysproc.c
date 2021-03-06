#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "signal.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}

int sys_register_signal_handler(void){

  int signum;
  int hand;
  int tramp;

  if(argint(0, &signum) < -1) {
    return -1;
  }
  if(argint(1, &hand) < -1) {
    return -1;
  }
    if(argint(2, &tramp) < -1) {
    return -1;
  }
    //cprintf("MY SIGNAL IS: %d...\n", signum);
  sighandler_t handler = (sighandler_t)hand;
  if(signum != -1){
    proc->handlers[signum] = handler;
    proc->trampoline = (uint)tramp;
  }
  return 0;

}

int sys_alarm(void)
{
  int seconds;
  argint(0, &seconds);
  if(argint(0, &seconds) < 0){return -1;}
  proc->alarmed = seconds+1;
  return 0;
}

int sys_skip(void){

cprintf("We are setting the skip flag");
proc->skip = 1;
return 0;
}
