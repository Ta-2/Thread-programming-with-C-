#include <utility>
#include <iostream>
#include <string.h>
#include <conio.h>
#include <boost/thread.hpp>
#define arr_size 10
boost::mutex stdmtx;

class MemoryManager {
  double *front_arr = nullptr, *back_arr = nullptr;
  boost::shared_mutex mtx_m;
public:
  MemoryManager (int size){
    front_arr = new double[size]();
    back_arr = new double[size]();
  }
  double *GetFrontArr (){
    boost::shared_lock<boost::shared_mutex> lock(mtx_m);
    return front_arr;
  }
  double *GetBackArr (){
    boost::shared_lock<boost::shared_mutex> lock(mtx_m);
    return back_arr;
  }
  double *SwapArr (){
    boost::upgrade_lock<boost::shared_mutex> lock(mtx_m);
    {
      boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(lock);
      boost::lock_guard<boost::mutex> guard(stdmtx);
      std::cout << "arr swaped" << std::endl;
      std::swap(front_arr, back_arr);
      return back_arr;
    }
  }
};

 class Analyzer {
 private:
   int size_m = -1;
   double *target_arr = nullptr;
   MemoryManager *mm_m;
public:
  Analyzer (int size, MemoryManager *mm){
    size_m = size;
    mm_m = mm;
    target_arr = mm_m->GetBackArr();
  }
  void Analyze (){
    for(int i = 0; i < size_m; i++)target_arr[i] += 1;
  }
    void SetArr (){ target_arr = mm_m->GetBackArr(); }
    void UpdateArr (){ target_arr = mm_m->SwapArr(); }
};

class Sender {
private:
  int size_m = -1, step = 0;
  double *target_arr = nullptr;
  MemoryManager *mm_m;
public:
  Sender (int size, MemoryManager *mm){
    size_m = size;
    mm_m = mm;
    target_arr = mm_m->GetBackArr();
  }
  void Send () {
    boost::lock_guard<boost::mutex> guard(stdmtx);
    std::cout << step << ": ";
    for(int i = 0; i < size_m; i++) std::cout << *(target_arr+i);
    std::cout << std::endl << std::endl;
    step++;
  }
  void SetArr (){ target_arr = mm_m->GetFrontArr(); }
};

void AnalyzerThread (MemoryManager *mm, bool *flag){
  Analyzer a(arr_size, mm);
  a.SetArr();
  while(*flag){
    a.Analyze();
    a.UpdateArr();
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
  }
  return;
}

void SenderThread (MemoryManager *mm, bool *flag){
  Sender s(arr_size, mm);
  s.SetArr();
  while(*flag){
    //fprintf(stdout, "t2\n");
    s.Send();
    s.SetArr();
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
  }
  return ;
}

void InputThread (bool *flag){
  std::string input;
  while(true){
    std::cin >> input;
    if(input == "q"){
      *flag = false;
      break;
    }
  }
  return;
}

int main(){
  bool flag = true;
  MemoryManager *mm = new MemoryManager(arr_size);
  boost::thread AT( boost::bind(AnalyzerThread, mm, &flag));
  boost::thread ST( boost::bind(SenderThread, mm, &flag));
  boost::thread IT( boost::bind(InputThread, &flag));

  IT.join();
  fprintf(stdout, "IT finished\n");
  AT.join();
  fprintf(stdout, "AT finished\n");
  ST.join();
  fprintf(stdout, "ST finished\n");

  return 0;
}
