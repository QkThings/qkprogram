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
    hal_setLED(qk_action_get_value_b(id));
  }
  else if(id == ACT_INT)
  {
    int delay_ms = qk_action_get_value_i(id);
    if(delay_ms < 1) delay_ms = 1;
    hal_blinkLED(2, delay_ms);
  }
}

void qk_setup()
{
  qk_board_set_name("qk actions test");


  qk_action_set_buffer(act_buf, ACT_BUF_SIZE);

  qk_action_set_type(ACT_BOOL, QK_ACTION_TYPE_BOOL);
  qk_action_set_label(ACT_BOOL, "ACT_BOOL");
  qk_action_set_value_b(ACT_BOOL, false);

  qk_action_set_type(ACT_INT, QK_ACTION_TYPE_INT);
  qk_action_set_label(ACT_INT, "ACT_INT");
  qk_action_set_value_i(ACT_INT, 250);

  qk_set_action_callback(action_callback);
}

int main()
{
  return qk_main();
}
