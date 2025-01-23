#ifndef _IR_H_
#define _IR_H_
#include <src/ir/include/ir_learn.h>

#define IR_RESOLUTION_HZ                1000000 // 1MHz resolution, 1 tick = 1us
#define IR_TX_GPIO_NUM                  GPIO_NUM_39
#define IR_RX_GPIO_NUM                  GPIO_NUM_38
#define IR_RX_CTRL_GPIO_NUM             GPIO_NUM_44
#define IR_TX_DETECT_IO                 GPIO_NUM_0

class IR {
    public:
        void setup() {

        }
        void startLearn(int learnCount);
        void stopLearn();
        void send(struct ir_learn_sub_list_head *rmt_out);
    private:

};

#endif