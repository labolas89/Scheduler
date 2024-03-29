#include <iostream>

#include "Scheduler.h"
#include <avl_array.h>

void message(const std::string &s) {
  std::cout << s << std::endl;
}

template<typename mapTy, typename keyTy>
void testPrint(mapTy map, keyTy key) {
  // printf("upper_bound(%d) %c\n", key, map.end() != map.upper_bound(key) ? *map.upper_bound(key) : '-');
  // printf("lower_bound(%d) %c\n", key, map.end() != map.lower_bound(key) ? *map.lower_bound(key) : '-');
  printf("find(%d) %c\n\n", key, map.end() != map.find(key) ? *map.find(key) : '-');
}

int main() {

  // setvbuf(stdout, NULL, _IOLBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  std::map<int, char> cmap;


  avl_array<int, char, uint8_t, 255> tstMap;

#if 0
  std::pair<int, char> testArr[] = {
    {10,'a'},{10,'b'},{20,'c'},{40,'d'},{40,'e'},{60,'f'},{70,'g'},{70,'h'},{90,'i'},{100,'j'},{130,'k'}};
#else
  std::pair<int, char> testArr[] = {
    {130,'a'},{100,'b'},{90,'c'},{70,'d'},{70,'e'},{60,'f'},{40,'g'},{40,'h'},{20,'i'},{10,'j'},{10,'k'}};
#endif
  for (auto& pair : testArr)
  {
    auto it = cmap.insert(pair);

    bool success = it.second;
    bool isFront = it.first == cmap.begin();

    printf("%s %s %d %c begin:%c\n", 
      success ? "ok" : "no", 
      isFront ? "FR" : "NF", 
      pair.first, 
      pair.second, 
      cmap.begin()->second);
  }
  printf("\n\n");

  for (auto& pair : testArr)
  {
    auto it = tstMap.insert(
      std::forward<int>(pair.first), 
      std::forward<char>(pair.second)); 

    bool success = it != tstMap.end();
    bool isFront = it == tstMap.begin();

    printf("%s %s %d %c begin:%c\n", 
      success ? "ok" : "no", 
      isFront ? "FR" : "NF", 
      pair.first, 
      pair.second, 
      *tstMap.begin());
  }
  
  int i = 0;
  for (auto it = tstMap.begin(); it != tstMap.end(); it++)
  {
    auto val = *it;
    printf("[%d] %c\n", i++, *it);
  }
  
  testPrint(tstMap, 20);
  testPrint(tstMap, 30);
  testPrint(tstMap, 40);
  testPrint(tstMap, 50);
  testPrint(tstMap, 60);
  testPrint(tstMap, 70);
  testPrint(tstMap, 80);
  testPrint(tstMap, 90);
  testPrint(tstMap, 100);

  return 0;



  // number of tasks that can run simultaneously
  // Note: not the number of tasks that can be added,
  //       but number of tasks that can be run in parallel
  unsigned int max_n_threads = 12;

  // Make a new scheduling object.
  // Note: s cannot be moved or copied
  Bosma::Scheduler s(max_n_threads);

  // every second call message("every second")
  s.every(std::chrono::seconds(1), message, "every second");

  // in one minute
  s.in(std::chrono::minutes(1), []() { std::cout << "in one minute" << std::endl; });

  // in 5 seconds
  s.in(std::chrono::seconds(5), []() { std::cout << "in 5 seconds" << std::endl; });

  // run lambda, then wait a second, run lambda, and so on
  // different from every in that multiple instances of the function will never be run concurrently
  s.interval(std::chrono::seconds(1), []() {
      std::cout << "right away, then once every 6s" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(5));
  });

  // https://en.wikipedia.org/wiki/Cron
  s.cron("* * * * *", []() { std::cout << "top of every minute" << std::endl; });

  // Time formats supported:
  // %Y/%m/%d %H:%M:%S, %Y-%m-%d %H:%M:%S, %H:%M:%S
  // With only a time given, it will run tomorrow if that time has already passed.
  // But with a date given, it will run immediately if that time has already passed.
  s.at("2017-04-19 12:31:15", []() { std::cout << "at a specific time." << std::endl; });

  s.cron("5 0 * * *", []() { std::cout << "every day 5 minutes after midnight" << std::endl; });

  // destructor of Bosma::Scheduler will cancel all schedules but finish any tasks currently running
  std::this_thread::sleep_for(std::chrono::minutes(10));
}