#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

class background_task {
public:
    void operator() () const {
        printHello();
    }

    void printHello() const {
        cout << "Hello World" << endl;
    }

};

struct func {
    int &i;

    func(int &i_) : i(i_) {}

    void operator() () {
        cout << "Child thread started ..." << endl;
        for(unsigned j = 0; j < 10000; j++) {
            cout << i + j << " "; // access to external variable
        }
        cout << endl;
        cout << "Child thread finished." << endl;
    }
};

void fun_oops() {
    cout << "Main thread started." << endl;
    int some_local_state = 0;
    func my_func(some_local_state);

    cout << "Creating thread.." << endl;
    thread my_thread(my_func);
    my_thread.detach();
    cout << "Child thread detached." << endl;

    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Main thread finished." << endl;
}

void chapter2_test1() {
    background_task f;
    thread my_thread(f);
    my_thread.join();

    //thread my_thread(background_task()); // denotes function declaration
    // correct way
    thread my_thread2((background_task()));
    thread my_thread3{background_task()};

    my_thread2.join();
    my_thread3.join();

    // lambda function
    thread my_thread4([] {
                      cout << "Lambda function - Hello World" << endl;
                      });
    my_thread4.join();
}

struct func_exception {
    int &i;

    func_exception(int &i_) : i(i_) {}

    void operator() () {
        cout << "Child thread started ..." << endl;
        try {
            for(unsigned j = 0; j < 10000; j++) {
                cout << i + j << " "; // access to external variable
            }
            cout << endl;
            throw "Throwing exception";
        } catch (...) {
            cout << "Exception caught." << endl;
        }
        cout << "Child thread finished." << endl;
    }
};

void fun_correct() {
    cout << "Main thread started." << endl;
    int some_local_state = 0;
    func_exception my_func(some_local_state);

    cout << "Creating thread.." << endl;
    thread t(my_func);
    try {
        this_thread::sleep_for(chrono::seconds(5));
    } catch (...) { // join thread in case of exception
        t.join();
        throw;
    }
    t.join();
    cout << "Main thread finished." << endl;
}

//thread_guard class for safe thread exiting
class thread_guard {
    thread &t;

public:
    explicit thread_guard(thread &t_): t(t_) {}

    ~thread_guard() {
        if(t.joinable()) { // make sure thread is still joinable
            t.join();
        }
    }

    thread_guard(thread_guard const&) = delete;
    thread_guard& operator= (thread_guard const&) = delete;
};

void fun_correct2() {

    cout << "Main thread started." << endl;
    int some_local_state = 0;
    func_exception my_func(some_local_state);

    cout << "Creating thread.." << endl;
    thread t(my_func);
    thread_guard g(t);

    t.join();
    this_thread::sleep_for(chrono::seconds(5));
    cout << "Main thread finished." << endl;

}

void chapter2_test2() {
    //fun_oops();
    //fun_correct();
    fun_correct2();
}
