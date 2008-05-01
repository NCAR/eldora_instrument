#include "Window.hh"
#include "Mouse.hh"
#include "aircraft.h"

extern "C" {
void mtst(void);
#include "stdioLib.h"
extern void wcio(int select, char *c, unsigned char n);
};

void mtst(void)
{
    GraphicController agc((void *)0xa0000000,1280,1024,2048,250);

    Window wdw0(&agc,0,0,0,1280,1024,0,0);

    Point a;

    a.x = 0;
    a.y = 0;

    wdw0.frect(a,1280,1024,BLACK);

    wdw0.expose();

    wdw0.display();

    a.x = 640;
    a.y = 512;

    Mouse mouse((void *)0xa0000000,a,aircraft,250);

    Point b = mouse.getXY();

    printf("%d, %d\n",b.x,b.y);

    printf("Waiting: ");
    getchar();
    getchar();

    wcio(1,"a",0xf7);

    int button = mouse.getButtonState();

    printf("Button state = 0x%x\n",button);

    unsigned char *ptr = (unsigned char *)0xa10003fe;

    printf("Int state = %d\n",*ptr & 0xff);


    int reason = mouse.waitOnInterrupt();

    printf("reason = 0x%x\n",reason);

}
