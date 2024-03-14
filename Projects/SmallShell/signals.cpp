#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlCHandler(int sig_num) {
  std::cout << "smash: got ctrl-C" << endl;
  int pid=SmallShell::getInstance().fgPid;
  if(pid>0)
  {
      if(kill(pid,SIGKILL) == -1){
          perror("smash error: kill failed");
          return;
      }else{
          cout << "smash: process " << pid << " was killed" << endl;
          SmallShell::getInstance().fgPid=-1;
      }
  }
}

//void alarmHandler(int sig_num) {
//  // TODO: Add your implementation
//}

