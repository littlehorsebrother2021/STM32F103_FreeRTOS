#ifndef  KEY_H
#define KEY_H

#include <stdint.h>

#define KEYUP_GPIO_PORT GPIOA
#define KEYUP_GPIO_PIN GPIO_PIN_0

#define KEYDOWN_GPIO_PORT GPIOE
#define KEYDOWN_GPIO_PIN GPIO_PIN_3

#define KEYLEFT_GPIO_PORT GPIOE
#define KEYLEFT_GPIO_PIN GPIO_PIN_4

#define KEYRIGHT_GPIO_PORT GPIOE
#define KEYRIGHT_GPIO_PIN GPIO_PIN_2

typedef enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
}KEY_TYPE;



#endif // KEY_H
