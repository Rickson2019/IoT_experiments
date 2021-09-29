#include "utils/ap_sta_utils.h"
#include "utils/deepsleep_utils.h"

void setup()
{
  setup_ap_sta();
}

void loop()
{

  // 如果设置了deepsleep 那么就进入睡眠模式
  // If deepsleep is required
  // then we put it into the sleep mode
  if ((get_deepsleep_minutes() != 0) && (get_deepsleep_seconds() != 0))
  {
    deepsleep_for_minutes(get_deepsleep_minutes());
    deepsleep_for_seconds(get_deepsleep_seconds());
  }
}
