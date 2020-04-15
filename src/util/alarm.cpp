
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../hal/timer.h"

#include "../util/alarm.h"

static struct alarm* alarmList = NULL;

void alarmInit(struct alarm* alarm, const struct alarmProperties* properties)
{
  // Initialize all the parts of the alarm
  alarm->set = false;
  alarm->suppressed = false;
  alarm->properties = properties;
  alarm->next = NULL;
  
  // Insert into the list of all alarms in the system
  struct alarm** current = &alarmList;
  while (*current) {
    current = &((*current)->next);
  }
  *current = alarm;
  
  return;
}

void alarmSet(struct alarm* alarm)
{
  // Check if the alarm is currently begin suppressed
  if ((alarm->suppressed) &&
      (timerHalRun(&(alarm->suppressionTimer)) == HAL_IN_PROGRESS)) {
    return;
  } else if (alarm->suppressed) {
    // If the suppression timer has just expired, unsuppress the alarm now
    alarm->suppressed = false;
  }

  alarm->set = true;
  
  return;
}

bool alarmGet(struct alarm* alarm)
{
  return alarm->set;
}

void alarmSupress(struct alarm* alarm)
{
  // Clear and suppress the alarm; beginning the suppression timer
  alarm->suppressed = true;
  alarm->set = false;
  timerHalBegin(&(alarm->suppressionTimer),
                alarm->properties->suppressionTimeout);

  return;
}

bool alarmCheckAll(struct alarmProperties* properties)
{
  struct alarm* current = alarmList;
  bool allClear = true;
  
  // Set the priority to below the "lowest" possible value
  properties->priority = ALARM_PRIORITY_LOW + 1;

  // Loop through all registered alarms, checking if any changes need to be made
  // to their state, setting the return flags if any conditions are met
  while (current) {
    if ((current->suppressed) &&
        (timerHalRun(&(current->suppressionTimer)) != HAL_IN_PROGRESS)) {
      // This alarm should no longer be suppressed
      current->suppressed = false;
    }
    
    if (current->set) {
      allClear = false;
      if (current->properties->preventWatchdog) {
        properties->preventWatchdog = true;
      }
      if (current->properties->priority < properties->priority) {
        properties->priority = current->properties->priority;
      }
    }
    
    current = current->next;
  }
  
  return !allClear;
}