#include <iostream>
#include <filesystem> // C++17 feature
#include <cstdlib>
#include <string>
#include <pwd.h> // For home directory expansion
#include <unistd.h>

// Function to expand '~' to the home directory
std::string expandHomeDirectory(const std::string &path)
{
    if (!path.empty() && path[0] == '~')
    {
        const char *homeDir = getenv("HOME");
        if (homeDir || (homeDir = getpwuid(getuid())->pw_dir))
        {
            return std::string(homeDir) + path.substr(1);
        }
    }
    return path;
}

void listJobs(const std::string &jobsDir)
{
    std::string expandedDir = expandHomeDirectory(jobsDir);
    std::cout << "Listing jobs in directory: " << expandedDir << std::endl;
    for (const auto &entry : std::filesystem::directory_iterator(expandedDir))
    {
        if (entry.path().extension() == ".cpp")
        {
            std::cout << entry.path().filename() << std::endl;
        }
    }
}

void compileAndRun(const std::string &filePath)
{
    std::string expandedFilePath = expandHomeDirectory(filePath);

    // Extract the file name without the directory path
    std::string fileName = expandedFilePath.substr(expandedFilePath.find_last_of("/") + 1);

    // Compile the program and create the output in the current directory
    std::string compileCmd = "g++ -o " + fileName + ".out " + expandedFilePath;

    if (system(compileCmd.c_str()) == 0)
    {
        // Run the compiled program from the current directory
        std::string runCmd = "./" + fileName + ".out";
        system(runCmd.c_str());
    }
    else
    {
        std::cout << "Compilation failed!" << std::endl;
    }
}

void compileAndRunAll(const std::string &jobsDir)
{
    std::string expandedDir = expandHomeDirectory(jobsDir);
    for (const auto &entry : std::filesystem::directory_iterator(expandedDir))
    {
        if (entry.path().extension() == ".cpp")
        {
            compileAndRun(entry.path().string());
        }
    }
}

void setJobsDirectory(std::string &jobsDir)
{
    std::cout << "Enter new jobs directory: ";
    std::cin >> jobsDir;

    // Expand home directory if needed
    jobsDir = expandHomeDirectory(jobsDir);

    // Confirm that the directory has been set
    std::cout << "New jobs directory set to: " << jobsDir << std::endl;
}

void help()
{
    std::cout << "Available commands:\n";
    std::cout << "1. List jobs\n";
    std::cout << "2. Set jobs directory\n";
    std::cout << "3. Compile and run specific program\n";
    std::cout << "4. Compile and run all jobs\n";
    std::cout << "5. Shutdown\n";
    std::cout << "6. Help\n";
}

int main()
{
    std::string jobsDir = "~/batch/jobs"; // Initial directory
    int choice;

    while (true)
    {
        std::cout << "\nBatch Monitor Options:\n";
        std::cout << "1. List jobs\n";
        std::cout << "2. Set jobs directory\n";
        std::cout << "3. Compile and run specific program\n";
        std::cout << "4. Compile and run all jobs\n";
        std::cout << "5. Shutdown\n";
        std::cout << "6. Help\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            listJobs(jobsDir);
            break;
        case 2:
            setJobsDirectory(jobsDir);
            break;
        case 3:
        {
            std::string jobName;
            std::cout << "Enter the job file to compile and run (e.g. dots.cpp): ";
            std::cin >> jobName;
            compileAndRun(jobsDir + "/" + jobName);
            break;
        }
        case 4:
            compileAndRunAll(jobsDir);
            break;
        case 5:
            std::cout << "Shutting down...\n";
            return 0;
        case 6:
            help();
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
