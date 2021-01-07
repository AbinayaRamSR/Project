#include <vector>
#include <string>
#include <fstream>
#include<iostream>
#include<sstream>
#include<queue>
#include<iomanip>

using namespace std;

class Process
{
public:
  int pid;
  int arrivalclockTime;
  int burstclockTime;
  int burstclockTimeOrginal;
  int timeSliceLeft;
  int turnAroundTime;
  int waitingTime;

};

class ProcessCreator
{
public:
  vector<Process> processes;
  void createProcessInclockTime(queue<Process> &readyQueue,int clockTime)
  {
    for(int i=0; i<processes.size(); i++)
    {

      if (processes[i].arrivalclockTime==clockTime)
      {
        readyQueue.push(processes[i]);
      }
    }
  }
};

void PrintQueue(queue<Process> addingQueue)
{
  while(!addingQueue.empty())
  {
    cout << addingQueue.front().pid << ",";
    addingQueue.pop();
  }
  cout << "\n" ;
}

class Scheduler
{
public:
  int timeQuantum=1;

  void scheduleFromReadyQueue(queue<Process> &readyQueue, queue<Process> &runningQueue)
  {
    if(!readyQueue.empty() && runningQueue.empty())
    {
      Process u=readyQueue.front();
      u.timeSliceLeft=timeQuantum;
      readyQueue.pop();
      runningQueue.push(u);
    }
  }

  void checkRunningQueue(queue<Process> &readyQueue, queue<Process> &runningQueue, int clockTime, vector<Process> &finishedProcesses)
  {
    if(!runningQueue.empty())
    {
      Process u = runningQueue.front();
      runningQueue.pop();
      if(u.burstclockTime==0)
      {
        int exitTime=clockTime;
        u.turnAroundTime=exitTime-u.arrivalclockTime;
        u.waitingTime=u.turnAroundTime-u.burstclockTimeOrginal;
        finishedProcesses.push_back(u);
        // if burst time is 0 exit the process.
        return;
      }
      else if(u.timeSliceLeft==0 )
      {
        // if time is 0 and has burstTime then put in ready queue
        readyQueue.push(u);
      }
      else
      {
        // if there is burst time and time left then put in running queue
        runningQueue.push(u);
      }
    }
  }


};


class CPU
{
public:
  void runProcessFromRunningQueue(queue<Process> &runningQueue)
  {
    Process tmp;
    if(!runningQueue.empty())
    {
      Process tmp=runningQueue.front();
      runningQueue.pop();
      tmp.timeSliceLeft--;
      tmp.burstclockTime--;
      runningQueue.push(tmp);
    }
    /*
    *timeSliceLeft changes time when process enters runningQueue
    *first TimesliceLeft is 3
    *when process enters runningQueue timeSlice reduces by 1.
    */

  }

};

void readInputFile(vector<Process> &processes, string inputFileName)
{

  fstream fin;
  fin.open(inputFileName , ios::in);
  string row, column;
  while ( fin >> row)
  {
    stringstream sRow(row);
    Process proc;

    getline(sRow, column, ',');
    //pid
    proc.pid = stoi(column);
    cout << proc.pid << ",";

    getline(sRow,column,',');
    //arrival clockTime
    proc.arrivalclockTime = stoi(column);
    cout << proc.arrivalclockTime << ",";

    getline(sRow,column,',');
    //burst clockTime
    proc.burstclockTime = stoi(column);
    proc.burstclockTimeOrginal = proc.burstclockTime;
    cout << proc.burstclockTime <<"\n";

    processes.push_back(proc);
  }
}



int main()
{
  int clockTime=0;
  queue<Process> readyQueue;
  queue<Process> runningQueue;
  ProcessCreator creator;
  Scheduler schedule;
  CPU cpu;
  vector<Process> finishedProcesses;

  readInputFile(creator.processes, "inputfile.txt");
  for(int clockTime=0; clockTime<=19; clockTime++ )
  {
    cout << " Clock Time: " << clockTime << "\n" ;
    creator.createProcessInclockTime(readyQueue, clockTime);
    //schedule the processes from ready queue
    cout << " Ready Queue: " ;
    PrintQueue(readyQueue);
    schedule.scheduleFromReadyQueue(readyQueue, runningQueue);
    cout << " Running Queue : " ;
    PrintQueue(runningQueue);
    cpu.runProcessFromRunningQueue(runningQueue);
    schedule.checkRunningQueue(readyQueue, runningQueue, clockTime, finishedProcesses);
  }

cout << " Process : " << setw(5) << " Turn Around Time : " << setw(5) << " Waiting Time : " << " \n " ;
for(int i=0; i<finishedProcesses.size(); i++)
{
  cout << finishedProcesses[i].pid << setw(20);
  cout << finishedProcesses[i].turnAroundTime << setw(20);
  cout << finishedProcesses[i].waitingTime;
  cout << endl;
}

int total_waitingtime = 0;
int total_turnAroundTime = 0;

for(int i=0; i<finishedProcesses.size(); i++)
{
  total_waitingtime = total_waitingtime + finishedProcesses[i].waitingTime;
  total_turnAroundTime = total_turnAroundTime + finishedProcesses[i].turnAroundTime;
}
cout << " Average of Waiting Time : " << total_waitingtime/4 << "\n" ;
cout << " Average of Turn Around Time : " << total_turnAroundTime/4 ;

}
