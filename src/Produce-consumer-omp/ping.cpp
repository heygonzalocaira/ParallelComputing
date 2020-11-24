
#include <condition_variable>
#include <iostream>
#include <thread>

using namespace std;
bool dataReady= false;                                   
mutex mutex_;
condition_variable condVar1;
condition_variable condVar2;

int counter=0;
int COUNTLIMIT=50;
void setTrue(){                                          

  while(counter <= COUNTLIMIT){
    std::unique_lock<std::mutex> lck(mutex_);
    condVar1.wait(lck, []{return dataReady == false;});  
    dataReady= true;
    ++counter;                                            
    std::cout << dataReady << std::endl;
    condVar2.notify_one();                               

  }
}

void setFalse(){                                          
  while(counter < COUNTLIMIT){                           
    unique_lock<std::mutex> lck(mutex_);
    condVar2.wait(lck, []{return dataReady == true;});
    dataReady= false;
    cout << dataReady << std::endl;
    condVar1.notify_one();
  }

}

int main(){

  cout << std::boolalpha << std::endl;
  cout << "Begin: " << dataReady << std::endl;
  thread t1(setTrue);
  thread t2(setFalse);
  t1.join();
  t2.join();
  
  dataReady= false;
  cout << "End: " << dataReady << std::endl;
  cout << std::endl;
    
}