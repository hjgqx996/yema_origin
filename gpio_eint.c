#include "ql_oe.h"
#include "ql_eint.h"
#include "general.h"

#define WAKEUP_GPIO			PINNAME_GPIO3

extern bool real_wakeup;

static Enum_PinName m_GpioPin = WAKEUP_GPIO;

static void eint_callback(Enum_PinName PinName, int level)
{
        Log(__FUNCTION__,"pin: %d level: %d\n", PinName, level);
		real_wakeup = true;
}

void my_gpio_eint_init(void)
{	
	Enum_EintType senseType = EINT_SENSE_FALLING;
	
	Ql_GPIO_SetPullSelection(m_GpioPin,  PINPULLSEL_PULLUP);
	Ql_EINT_Enable(m_GpioPin, senseType, eint_callback);

    return;
}
