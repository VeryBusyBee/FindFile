#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <experimental/filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::experimental::filesystem::recursive_directory_iterator;
//namespace fs = std::experimental::filesystem;

const int MAX_THREADS {8};
bool stop_threads {false};
std::vector<std::thread> threads;
std::mutex mtx;
std::condition_variable cv;
int count = MAX_THREADS-1;

void find_in_dir(string path, string sname) {
    string fpath, filename;
cout << "Looking in " << path << endl;
    for (const auto & file : recursive_directory_iterator(path)) {
       if (stop_threads) //exit if notified to stop
        return;
       fpath = file.path();
cout << fpath << endl;
       if (is_regular_file(file.status())) {
//cout << fpath << endl;
            filename = fpath.substr(fpath.find_last_of("/\\") + 1);
            if (filename == sname) {
                cout << "Found:" << fpath << endl;
                stop_threads = true;
            }
        }
        else {
            if (is_directory(file.status())) {
                if (!fpath.empty() ) {

                    std::unique_lock<std::mutex> lock(mtx);
                    while(count == 0) {
                        //wait on the mutex until one of threads finished
                        cv.wait(lock);
                    }
                    --count;
                    threads.push_back(std::thread(find_in_dir, fpath, sname));//RECURSION!!!
cout << "Adding a new thread " << count << " for " << fpath  << endl;
                }
            }
        }
cout << "Next ";
    }
    std::unique_lock<std::mutex> lock(mtx);
    if (count < MAX_THREADS-1) ++count;
cout << "Thread " << count << " for " << path << " deleted,  " <<  endl;
}

void run_search(string path, string sname) {

     find_in_dir(path, sname);

     for (auto &th : threads) { //wait for all threads to finish
        th.join();
     }
}
