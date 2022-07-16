#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;
using std::stof;
using LinuxParser::CPUStates;

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() { 
    //Reference = https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux 
 
    std::vector<string> cpu_info = LinuxParser::CpuUtilization();
    
    float Idle = stof(LinuxParser::str_check(cpu_info[CPUStates::kIdle_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kIOwait_]));

    float NonIdle = stof(LinuxParser::str_check(cpu_info[CPUStates::kUser_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kNice_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSystem_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kIRQ_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSoftIRQ_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSteal_]));

    float Total = Idle + NonIdle;
    
    return (Total - Idle)/Total; 
    
}