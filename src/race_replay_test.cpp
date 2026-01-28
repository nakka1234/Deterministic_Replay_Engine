
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "event.h"


std::atomic<size_t> global_seq{0};
std::mutex mtx;
std::condition_variable cv;
int counter=0;

void worker(int id, const std::vector<Event>& events) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        //std::cout<<"Ok1"<<id<<"\n";

        // wait until it's this thread's turn or all events are done
        cv.wait(lock, [&] {
            return global_seq >= events.size() ||
                   events[global_seq].thread_id == id;
        });

        if (global_seq >= events.size()) break;

        const Event &ev = events[global_seq];
         //std::cout<<"Ok2"<<id<<"\n";
        // Replay the event
        if (int(ev.type) == 3) {
            int tmp = counter; (void)tmp;
        } else if (int(ev.type) == 6) {
            counter++;
            std::cout << counter << "\n";
        } else if (int(ev.type) == 5 ){
            counter=ev.object_id;
           
        }

        global_seq++;

        lock.unlock();
        cv.notify_all();  // wake other threads
    }
}



int main() {
    const std::vector<Event>events = parse_log("log_race.txt");
    //std::cout<<events.size()<<"\n";
    int n=events.size();
    for(int i=0;i<50;i++)
    {
        std::cout<<int(events[i].type)<<"\n";
    }
   

    std::vector<std::thread> threads;
    for (int i = 0; i < 2; i++)
        threads.emplace_back([i, &events] { worker(i, events); });

    for (auto &t : threads) t.join();
    std::cout << "Replay finished. Counter = " << counter << std::endl;
}
