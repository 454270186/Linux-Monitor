#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int);                            // Constructor
  int Pid() const;                               // TODO: See src/process.cpp
  string User()const ;                      // TODO: See src/process.cpp
  string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  string Ram()const ;                       // TODO: See src/process.cpp
  long int UpTime() const;                       // TODO: See src/process.cpp
  float Cpu_Util_calc();            //For cpu Util
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
    int pid_{0};
    int ram_{0};
    long int up_time_{1};
    float cpu_util_{0.0};
    string uid_{string()}, user_{string()}, command_{string()};
};

#endif