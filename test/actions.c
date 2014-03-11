/*
 * actions.c
 *
 *  Created on: Feb 22, 2014
 *      Author: mribeiro
 */

#include <qk_program.h>
#include <qk_debug.h>

#define ACT_BUF_SIZE 2

enum
{
  ACT_BOOL,
  ACT_INT
};

qk_action act_buf[ACT_BUF_SIZE];

void action_callback(qk_action_id id)
{
  if(id == ACT_BOOL)
  {
    hal_setLED(qk_actionValueB(id));
  }
  else if(id == ACT_INT)
  {
    int delay_ms = qk_actionValueI(id);
    if(delay_ms < 1) delay_ms = 1;
    hal_blinkLED(2, delay_ms);
  }
}

void qk_setup()
{
  qk_setBoardName("qk actions test");


  qk_setActionBuffer(act_buf, ACT_BUF_SIZE);

  qk_setActionType(ACT_BOOL, QK_ACTION_TYPE_BOOL);
  qk_setActionLabel(ACT_BOOL, "ACT_BOOL");
  qk_setActionValueB(ACT_BOOL, false);

  qk_setActionType(ACT_INT, QK_ACTION_TYPE_INT);
  qk_setActionLabel(ACT_INT, "ACT_INT");
  qk_setActionValueI(ACT_INT, 250);

  qk_setActionCallback(action_callback);
}

int main()
{
  return qk_main();
}
