#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void helloWorld() {
    cout << "Hello World!" << endl;
}

void helloWorldID(int ID) {
    cout << ID << " Hello World!" << endl;
}

int main()
{
    std::thread t1(helloWorld);
    t1.join();

    const int numThreads = 10;
    std::vector<std::thread> threadPool;
    for(int id = 0; id < numThreads; id++) {
        threadPool.push_back(std::thread(helloWorldID, id));
    }

    for(auto &t : threadPool) {
        t.join();
    }

    return 0;
}
