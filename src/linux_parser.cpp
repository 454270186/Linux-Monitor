#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//泛型
//reference:Udacity project reviewer.
/*template <typename T> 
T findValueByKey(string const &keyfilter, string const &filename) {
  string line;
  string key;
  T value;
  std::ifstream filestream("/proc/" + filename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key)
      {
        if(key == keyfilter)
        {
          return value;
        }
      }
    }
  }
  return value;
};
  
template <typename T>
T getValueOfFile(string const &filename) {
  string line;
  T value;
  std::ifstream filestream( + filename);
  
  if(filestream.is_open())
  {
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  
  return value;
}; */

string LinuxParser::str_check(string my_string){
  if(my_string!="")
    return my_string;
  else
    return "0";
  
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(str_check(filename));
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string line;
  string key, value, unit;
  float tot_mem, free_mem;
  
  std::unordered_map<string, int> map;
  int i = 3;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (i>=0) {
        std::getline(filestream, line);
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> key >> value >> unit;
        map[key] =stoi(str_check(value));
        i--;
    }
  }
  filestream.close();
  
  tot_mem = float(map.at("MemTotal"));
  free_mem = float(map.at("MemFree"));
  
  return (tot_mem - free_mem) / tot_mem; 
  
  
  
  /*string mem_tot_target = "MemTotal";
  string mem_free_target = "MemFree";
  float mem_tot = findValueByKey<float>(mem_tot_target, kMeminfoFilename);
  float mem_free = finValueByKey<float>(mem_free_target, kMeminfoFilename);
  
  return (mem_tot - mem_free) / mem_tot; */
  
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime, idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  
  if(filestream.is_open())
  {
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return stol(str_check(uptime));
  }
  else
    return 0;
}


// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  vector<string> parser;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> line)
      {
        parser.push_back(value);
      }
      return stol(str_check(parser[13])) + stol(str_check(parser[14])) + stol(str_check(parser[15])) + stol(str_check(parser[16]));
    }
  }
  
  return 0;
}

// TODO: Read and return CPU utilization
//该函数解析/proc/stat文件的第一行，并返回一个向量及其所有对应字段
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string name;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open())
  {
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq
      >> steal >> guest >> guest_nice;
  }
  cpu = {name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  return cpu;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes")
        return stoi(str_check(value));
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running")
        return stoi(str_check(value));
    }
  }
  
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  
  if(filestream.is_open())
  {
    getline(filestream, line);
    if(line.length() > 40)
      return line.substr(0, 40) + "...";
    else
      return line;
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmData")
        return value;
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if(key == "Uid:")
        return uid;
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(string uid) {
  string line;
  string key, name, x;
  std::ifstream filestream(kPasswordPath);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      line.replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> x >> key;
      if(key == uid)
        return name;
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  vector<string> parser;
  string value;
  //reference:: Knowledge section
  long up_time, time_pid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> value)
      {
        parser.push_back(value);
      }
      
      up_time = LinuxParser::UpTime();
      time_pid = (stol(str_check(parser[21]))/sysconf(_SC_CLK_TCK));
      return up_time - time_pid;
    }
  }
  return 0;
}