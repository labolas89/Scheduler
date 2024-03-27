#include <iostream>

#include "Scheduler.h"
#include <avl_array.h>

void message(const std::string &s) {
  std::cout << s << std::endl;
}

template<typename mapTy, typename keyTy>
void testPrint(mapTy map, keyTy key) {
  printf("upper_bound(%d) %c\n", key, map.end() != map.upper_bound(key) ? *map.upper_bound(key) : '-');
  printf("lower_bound(%d) %c\n", key, map.end() != map.lower_bound(key) ? *map.lower_bound(key) : '-');
  printf("find(%d) %c\n\n", key, map.end() != map.find(key) ? *map.find(key) : '-');
}

int main() {

  avl_array<int, char, uint8_t, 255> tstMap;

  char cin = 'a';
  tstMap.insert(1,  cin++); // a
  tstMap.insert(2,  cin++); // b
  tstMap.insert(4,  cin++); // c
  tstMap.insert(4,  cin++); // d
  tstMap.insert(6,  cin++); // e
  tstMap.insert(7,  cin++); // f
  tstMap.insert(7,  cin++); // g
  tstMap.insert(9,  cin++); // h
  tstMap.insert(10, cin++); // i
  tstMap.insert(13, cin++); // j

  testPrint(tstMap, 2);
  testPrint(tstMap, 3);
  testPrint(tstMap, 4);
  testPrint(tstMap, 5);
  testPrint(tstMap, 6);
  testPrint(tstMap, 7);
  testPrint(tstMap, 8);
  testPrint(tstMap, 9);





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