#include <utility>
#include <iostream>
#include <boost/thread.hpp>
#define arr_size 10

class MemoryManager {
  double *front_arr = nullptr, *back_arr = nullptr;
public:
  MemoryManager (int size){
    front_arr = new double[size];
    back_arr = new double[size];
  }
  double *GetFrontArr (){ return front_arr; }
  double *GetBackArr (){ return back_arr; }
  double *SwapArr (){
    std::swap(front_arr, back_arr);
    return back_arr;
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
    for(int i = 0; i < size_m; i++) *(target_arr+i)=+1;
  }
    void SetArr (double *arr){ target_arr = arr; }
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
  void Send (){
    std::cout << step << ": ";
    for(int i = 0; i < size_m; i++) std::cout << *(target_arr+i);
    std::cout << std::endl << std::endl;
  }
  void SetArr (double *arr){ target_arr = arr; }
};

void AnalyzerThread (MemoryManager *mm){
  Analyzer a(arr_size, mm);

  return;
}

void SenderThread (MemoryManager *mm){
  Sender s(arr_size, mm);

  return ;
}

int main(){
  MemoryManager *mm = new MemoryManager(arr_size);
  boost::thread AT( boost::bind(AnalyzerThread, mm));
  boost::thread ST( boost::bind(SenderThread, mm));

  AT.join();
  ST.join();

  return 0;
}
