#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <csignal>
#include "Commands.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(std::string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h

SmallShell::SmallShell():prompt("smash"){
    jobs=new JobsList();
    fgPid=-1;
}


/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
/**
 * Dont forget to add the pipe
 */
    string cmd_s = _trim(string(cmd_line));
    string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

    if(cmd_s.find("|")!=string::npos)
    {
        return new PipeCommand(cmd_line); /** what if | existed but it wasn't a legit command **/
    }
    else if (cmd_s.find( ">") != string::npos || cmd_s.find( ">>") != string::npos ) {
        return new RedirectionCommand(cmd_line);
    }
    else if (firstWord.compare("chprompt") == 0){
        return new chpromptCommand(cmd_line);
    }
    else if (firstWord.compare("pwd") == 0) {
        return new GetCurrDirCommand(cmd_line);
    }
    else if (firstWord.compare("showpid") == 0) {
        return new ShowPidCommand(cmd_line);
    }
    else if (firstWord.compare("cd") == 0) {
        return new ChangeDirCommand(cmd_line, nullptr);
    }
    else if (firstWord.compare("jobs") == 0) {
        return new JobsCommand(cmd_line, nullptr);
    }
    else if (firstWord.compare("fg") == 0){
        return new ForegroundCommand(cmd_line, nullptr);
    }
    else if (firstWord.compare("quit") == 0) {
        return new QuitCommand(cmd_line, nullptr);
    }
    else if(!firstWord.compare("kill"))
        return  new KillCommand(cmd_line, nullptr);
    else if(!firstWord.compare("chmod"))
        return new ChmodCommand(cmd_line);
    else {
        return new ExternalCommand(cmd_line);
        //return new ExternalCommand(cmd_line);
    }

    //return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    getList()->removeFinishedJobs();
    Command* cmd = CreateCommand(cmd_line);
    if (cmd != nullptr) {
        cmd->execute();
    }
}


Command::Command(const char *cmd_line): m_cmdLine(cmd_line){}


BuiltInCommand::BuiltInCommand(const char *cmd_line) : Command(cmd_line) {
    if(_isBackgroundComamnd(cmd_line))
        _removeBackgroundSign(&(this->m_cmdLine[0]));
    char* args2[COMMAND_MAX_ARGS];
    _parseCommandLine(m_cmdLine.c_str() , args2);
    int i=0;
    while(args2[i] != nullptr)
    {
        args.push_back(args2[i]);
        //  free(args2[i]);
        i++;
    }
}

chpromptCommand::chpromptCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void chpromptCommand::execute() {
    if(args.size()==1)
        SmallShell::getInstance().setPrompt("smash");
    else
        SmallShell::getInstance().setPrompt(args[1]);
}

ShowPidCommand::ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void ShowPidCommand::execute() {

    /**
     * We should use a syscall not getPid func
     * ok? SmallShell::getInstance().
     */
    std::cout << "smash pid is " << getpid() << endl;
}

GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line): BuiltInCommand(cmd_line) {}

void GetCurrDirCommand::execute() {
    char * buffer=new char [COMMAND_ARGS_MAX_LENGTH+1];
    std::cout << getcwd(buffer , COMMAND_ARGS_MAX_LENGTH+1) << endl;
    delete[] buffer;
}

ChangeDirCommand::ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line) {}

/*
void JobsList::removeJobById(int jobId) {
    JobsList list = SmallShell::getInstance().getList();
    JobEntry job=list.m_jobs.front();
    for(int i=0 ; list.m_jobs!=list.)
}
*/

void ChangeDirCommand::execute() {
    if(args.size()>2)
    {
        std::cerr << "smash error: cd: too many arguments" << endl;
        return;
    }
    if(args[1] == "-" && SmallShell::getInstance().getLastDir().empty())
    {
        std::cerr << "smash error: cd: OLDPWD not set" << endl;
        return;
    }
    char * buffer= new char [COMMAND_ARGS_MAX_LENGTH+1];
    if(!getcwd(buffer , COMMAND_ARGS_MAX_LENGTH+1))
        perror("smash error: getcwd failed");

    if (args[1] == "-")
    {
        if(chdir(SmallShell::getInstance().getLastDir().c_str()) == -1) {
            perror("smash error: chdir failed");
            return;
        }

    }
    else if (chdir(args[1].c_str()) == -1)
    {
        perror("smash error: chdir failed");
        return;
    }
    SmallShell::getInstance().setLastDir(buffer);
    delete[] buffer;
}

JobsCommand::JobsCommand(const char *cmd_line, JobsList *jobs): BuiltInCommand(cmd_line){}

void JobsCommand::execute() {
    JobsList* list = SmallShell::getInstance().getList();
    list->removeFinishedJobs();
    list->printJobsList();
}

ForegroundCommand::ForegroundCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line) {}

void ForegroundCommand::execute(){
    int jobId;
    JobsList* list=SmallShell::getInstance().getList();
    JobsList::JobEntry job;
    if (list->m_jobs.size()!=0)
         job = list->m_jobs.back();

    if(args.size() >= 2){
        bool valid = false;
        try {
            jobId = stoi(args[1]); /// check if stoi failed?
        }
        catch (...)
        {
            std::cerr << "smash error: fg: invalid arguments" << endl;
            return;
        }
        //list->removeFinishedJobs();
        for(const auto & m_job : list->m_jobs)
        {
            if(m_job.getId() == jobId)
            {
                valid = true;
                job = m_job;
                break;
            }
        }
        if (!valid)
        {
            std::cerr << "smash error: fg: job-id " << jobId << " does not exist" << endl;
            return;
        }
    }
    if (list->m_jobs.empty())
    {
        std::cerr << "smash error: fg: jobs list is empty" << endl;
        return;
    }
    jobId = job.getId();
    if(args.size()>2)
    {
        std::cerr << "smash error: fg: invalid arguments" << endl;
        return;
    }
    /// should check the jobs list before
    std::cout << job.getCmd() << " " << job.getPid() << endl;
    /// adding the running code for the job (using the smash code)
    SmallShell::getInstance().fgPid=job.getPid();
    int status ; ///should be initialized
    if(waitpid(job.getPid(), &status , 0 ) == -1)
    {
        perror("smash error: waitpid failed");
        return;
    }
    else
        list->removeJobById(jobId);
    SmallShell::getInstance().fgPid=-1;
}

QuitCommand::QuitCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line){}

void QuitCommand::execute() {

    if (args.size() != 1){
        JobsList* list=SmallShell::getInstance().getList();
        list->removeFinishedJobs();
        cout << "smash: sending SIGKILL signal to " << list->m_jobs.size() << " jobs:" << endl;
        for (JobsList::JobEntry& job : list->m_jobs) {
            cout << job.getPid() << ": " << job.getCmd() << endl;
            kill(job.getPid(), SIGKILL);
        }
        ///  should check the jobs list (clean it because of the kill)
        list->removeFinishedJobs();
    }
    exit(0);
}

KillCommand::KillCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line) {}

void KillCommand::execute() {
    JobsList* list=SmallShell::getInstance().getList();
    JobsList::JobEntry job;

    int jobId;
    if(args.size() >= 3 )
    {
        bool valid = false;
        try {
            jobId = stoi(args[2]); /// check if stoi failed?
        }
        catch (...)
        {
            std::cerr << "smash error: kill: invalid arguments" << endl;
            return;
        }
        //list->removeFinishedJobs();
        for(const auto & m_job : list->m_jobs)
        {
            if(m_job.getId() == jobId)
            {
                valid = true;
                job = m_job;
                //std::cout << "hi"  << endl;
                break;
            }
        }
        if (!valid) {
            std::cerr << "smash error: kill: job-id " << jobId << " does not exist" << endl;
            return;
        }
    }
    if(list->m_jobs.size()!=0)
        job = list->m_jobs.back();
    else {
        std::cerr << "smash error: kill: invalid arguments" << endl;
        return;
    }
    if(args.size() == 3 ){
        try {
            jobId = stoi(args[2]); /// check if stoi failed?
        }
        catch (...)
        {
            std::cerr << "smash error: kill: invalid arguments" << endl;
            return;
        }
        //list->removeFinishedJobs();
        for(const auto & m_job : list->m_jobs)
        {
            if(m_job.getId() == jobId)
            {
                job = m_job;
                break;
            }
        }
        if(args[1][0] != '-')
        {
            std::cerr << "smash error: kill: invalid arguments" << endl;
            return;
        }
        int signalNum;
        try {
            signalNum = (-1) * stoi(args[1]);
        }
        catch (...)
        {
            std::cerr << "smash error: kill: invalid arguments" << endl;
            return;
        }
//        if(signalNum > 31 || signalNum < 0 ){
//            std::cerr<<"smash error: kill: invalid arguments"<<std::endl;
//            return;
//        }
//        //cout << job.getPid() << endl;
        if(kill(job.getPid(), signalNum)== -1)
        {

            perror("smash error: kill failed");
            return;
        }
        std::cout<<"signal number " << signalNum << " was sent to pid " <<job.getPid() <<std::endl;
        //list->removeJobById(jobId);
        return;
    }
        std::cerr << "smash error: kill: invalid arguments" << endl;
}

void JobsList::addJob( std::string cmd, pid_t pid , bool isStopped ) {
    // JobsList jobs=SmallShell::getInstance().getList();
    int id;
    /**if(m_jobs.size()>100)
        return;**/
    if(m_jobs.empty())
        id=1;
    else
        id=m_jobs.back().getId()+1;
    m_jobs.push_back(JobEntry(id,pid,cmd));
}

void JobsList::printJobsList() {
    for(const auto & m_job : m_jobs)
    {
        cout << "[" << m_job.getId() <<"] " << m_job.getCmd() << endl;
    }
}

void JobsList::killAllJobs() {
    for(auto it = this->m_jobs.begin() ; it!= this->m_jobs.end(); ++it)
    {
        if(kill((*it).getPid(), 9)== -1)
        {
            perror("smash error: kill failed");
            return;
        }
        m_jobs.erase(it);
    }
    removeFinishedJobs();
}

void JobsList::removeFinishedJobs() {
    for(auto it = this->m_jobs.begin() ; it!= this->m_jobs.end(); ++it)
    {
        int status;
        int ret_wait = waitpid((*it).getPid(), &status, WNOHANG);
        if (ret_wait == (*it).getPid()) { //ret_wait == -1
            m_jobs.erase(it);
            --it;
        }
//        else if(ret_wait == -1)
//        {
//            perror("smash error: waitpid failed");
//            --it;
//            return;
//        }
    }
}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    for(auto it = this->m_jobs.begin() ; it!= this->m_jobs.end(); ++it)
    {
        if((*it).getId()==jobId)
            return &(*it);
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    for(auto it = this->m_jobs.begin() ; it!= this->m_jobs.end(); ++it)
    {
        if((*it).getId()==jobId) {
            m_jobs.erase(it);
            return;
        }
    }
}

JobsList::JobsList() {
    m_jobs = deque<JobEntry>();
}

/**
JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    return nullptr;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    return nullptr;
}
**/

ExternalCommand::ExternalCommand(const char *cmd_line) : Command(cmd_line) {

}

void ExternalCommand::execute() {
    bool bg = _isBackgroundComamnd(m_cmdLine.c_str());
    std::string cleaned=m_cmdLine;
    if(bg)
        _removeBackgroundSign(&(cleaned[0]));
    bool isComplex = false;
    for(char c : cleaned)
    {
        if(c == '*' || c == '?')
        {
            isComplex=true;
            break;
        }
    }
    pid_t pid=fork();
    if(pid<0)
    {
        perror("smash error: fork failed");
        return;
    }
    JobsList* list=SmallShell::getInstance().getList();
    if(pid>0)
    {
        if(bg)
        {
            string str_cmd(m_cmdLine);
            list->addJob(m_cmdLine,pid);
        }
        else{
            SmallShell::getInstance().fgPid=pid;
            int status;
            if(waitpid(pid, &status , 0 ) == -1)
            {
                perror("smash error: waitpid failed");
                return;
            }
            SmallShell::getInstance().fgPid=-1;
        }
    }
    else
    {
        if (setpgrp() == -1) {
            perror("smash error: setpgrp failed");
            return; //
        }
        if(isComplex)
        {
            /** should we change curr pid also here???
             * **/
            execlp("/bin/bash","/bin/bash","-c",cleaned.c_str(), nullptr);
        }
        else
        {
            char* args[COMMAND_MAX_ARGS];
            _parseCommandLine(cleaned.c_str() , args);
            execvp(args[0], args);

            perror("smash error: execvp failed");
            exit(1);
        }
    }
}

RedirectionCommand::RedirectionCommand(const char *cmd_line) : Command(cmd_line) {}

void RedirectionCommand::execute() {
    int originalCout = dup(STDOUT_FILENO);
    if (originalCout == -1) {
        perror("smash error: dup failed");
        return;
    }
    if(close(STDOUT_FILENO)==-1)
    {
        perror("smash error: close failed");
        return;
    }
    int fd;
    string file;
    if (m_cmdLine.find(">>") != string::npos) {
        file = _trim(m_cmdLine.substr(m_cmdLine.find_first_of(">>") + 2));
        fd = open(file.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0655);
        if (fd == -1) {
            if (dup2(originalCout, STDOUT_FILENO) == -1) { //// may use original
                perror("smash error: dup2 failed");
            } else {
                perror("smash error: open failed");
            }
            return;

        }
    }
    else
    {
        file = _trim(m_cmdLine.substr(m_cmdLine.find_first_of(">") + 1));
        fd = open(file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0655); //what is 0655
        if (fd == -1) {
            if (dup2(originalCout, STDOUT_FILENO) == -1) { //// may use original
                perror("smash error: dup2 failed");
            } else {
                perror("smash error: open failed");
            }
            return;
        }
    }

    SmallShell::getInstance().executeCommand(_trim(m_cmdLine.substr(0,
                                                                    m_cmdLine.find_first_of(">"))).c_str());
    if (close(fd) == -1){
        perror("smash error: close failed");
        return;
    }
    if (dup2(originalCout, STDOUT_FILENO) == -1) {
        perror("smash error: dup2 failed");
        return;
    }
}

ChmodCommand::ChmodCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

void ChmodCommand::execute() {
    if(args.size()!=3 || access(args[2].c_str(),F_OK)==-1){
        std::cerr << "smash error: chmod: invalid arguments" << endl;
        return;
    }
    int mode;
    try {
        mode=std::stoi(args[1]);
    }
    catch (...)
    {
        cerr << "smash error: chmod: invalid arguments" << endl;
        return;
    }
    int checkOctal=mode;
    while(checkOctal)
    {
        if(checkOctal%10 > 7 || checkOctal%10 < 0)
        {
            cerr << "smash error: chmod: invalid arguments" << endl;
            return;
        }
        checkOctal%=10;
    }
    int modeAsDecimal;
    try {
        modeAsDecimal=std::stoi(args[1],nullptr,8);
    }
    catch (...)
    {
        cerr << "smash error: chmod: invalid arguments" << endl;
        return;
    }
    if(chmod(args[2].c_str(),modeAsDecimal)==-1)
        perror("smash error: chmod failed");
}

PipeCommand::PipeCommand(const char *cmd_line) : Command(cmd_line) {}

void PipeCommand::execute() {
    std::string cmd1 , cmd2;
    int fd;
    SmallShell& shell = SmallShell::getInstance();
    if(m_cmdLine.find("|&")!=string::npos)
    {
        cmd1 = _trim(m_cmdLine.substr(0,m_cmdLine.find("|&")));
        cmd2 =_trim(m_cmdLine.substr(m_cmdLine.find("|&")+2));
        fd=STDERR_FILENO;
    }
    else
    {
        cmd1 = _trim(m_cmdLine.substr(0,m_cmdLine.find("|")));
        cmd2 =_trim(m_cmdLine.substr(m_cmdLine.find("|")+1));
        fd=STDOUT_FILENO;
    }
    int my_pipe[2];
    if(pipe(my_pipe)==-1)
    {
        perror("smash error: pipe failed");
        return;
    }
    else
    {
        int pid = fork();
        if (pid == -1)
        {
            perror("smash error: fork failed");
            return;
        }
        else if(pid>0)
        {
            int file= dup(fd);
            dup2(my_pipe[1],fd);
            close(my_pipe[0]);
            close(my_pipe[1]);
            shell.CreateCommand(cmd1.c_str())->execute();
            close(fd);
            dup(file);
            close(file);
            if(waitpid(pid,nullptr,0)==-1)
            {
                perror("smash error: waitpid failed");
                return;
            }
        }
        else
        {
            if (setpgrp()==-1){
                perror("smash error: setpgrp failed");
                return;
            }
            dup2(my_pipe[0],STDIN_FILENO);
            close(my_pipe[0]);
            close(my_pipe[1]);
            shell.CreateCommand(cmd2.c_str())->execute();
            exit(1);
        }
    }
}


