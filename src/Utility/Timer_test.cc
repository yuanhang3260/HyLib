#include <unistd.h>
#include <chrono>
#include <string>

#include "UnitTest/UnitTest.h"
#include "Base/Utils.h"
#include "Base/Log.h"
#include "Base/Ptr.h"

#include "Utility/Timer.h"

namespace Utility {

class TimerTest: public UnitTest {
 public:
  TimerTest() :
      timer_(std::chrono::seconds(2),
             std::bind(&TimerTest::Alert, this)) {
  }

  void Alert() {
    printf("time out, alerting\n");
  }

  void test_Timeout() {
    timer_.Start();
    usleep(5 * 1000 * 1000);
  }

  void test_Pause_Resume() {
    timer_.Start();
    usleep(1 * 1000 * 1000);  // 1s

    printf("Pause for 3s\n");
    timer_.Pause();
    usleep(3 * 1000 * 1000);

    printf("Resume\n");
    timer_.Resume();

    usleep(2 * 1000 * 1000);
  }

  void test_Restart() {
    timer_.Start();
    for (int i = 0; i < 5; i++) {
      usleep(1 * 1000 * 1000);  // 1s
      printf("Restart %d\n", i);
      timer_.Restart();
    }

    usleep(5 * 1000 * 1000);
    timer_.Start();
    usleep(5 * 1000 * 1000);
  }

  void test_Repeat() {
    timer_.SetRepeat(true);
    timer_.Start();

    usleep(5 * 1000 * 1000);

    printf("Pause for 5s\n");
    timer_.Pause();
    usleep(5 * 1000 * 1000);

    printf("Resume\n");
    timer_.Resume();
    usleep(5 * 1000 * 1000);

    timer_.Pause();
  }

 protected:
  Timer timer_;
};

}  // namespace net_stack

int main() {
  Utility::TimerTest test;
  test.setup();
  // test.test_Timeout();
  // test.test_Pause_Resume();
  // test.test_Restart();
  test.test_Repeat();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}
