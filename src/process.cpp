#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int my_id) : pid_(my_id) {
    //Find the UID associated with the process
    string uid = LinuxParser::Uid(pid_);
    //Find the user corresponding to that UID
    user_ = LinuxParser::User(uid);
    //parse and load ram
    ram_ = stoi(LinuxParser::str_check(LinuxParser::Ram(pid_)))/1000;
    //retrieve process start time
    up_time_  = LinuxParser::UpTime(pid_); 
    //get the command that started the process
    command_ = LinuxParser::Command(pid_);
    //Get cpu utilization
    cpu_util_ = Cpu_Util_calc();
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_util_;}

// TODO: Return the command that generated this process
string Process::Command() const { return command_;}

// TODO: Return this process's memory utilization
string Process::Ram() const { return to_string(ram_); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return up_time_; }

//Calculate cpu utiization
float Process::Cpu_Util_calc(){
    long sys_uptime = LinuxParser::UpTime(); //system uptime
    long active = LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK); //totaltime
    long start_time = LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK); //start time of the process
    return ((float) active /LinuxParser::UpTime(pid_));
}
// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 

    return a.cpu_util_ < this->cpu_util_;
    
}
