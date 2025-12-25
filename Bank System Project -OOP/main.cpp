#include <iostream>
#include "clsCurrency.h"
#include "clsMainScreen.h"
#include "clsLoginScreen.h"


int main()

{
    while (true)
    {
        if (!clsLoginScreen::ShowLoginScreen())
            break;
    }

    return 0;
}

