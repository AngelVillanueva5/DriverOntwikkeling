#include <linux/io.h>

/* PIN */
#define PIN 19

/* base address */
#define GPIO1_ADDR		0x4804c000

/* register offsets in uint32_t sizes */
#define GPIO_OE		0x4D  // 0x134
#define GPIO_DATAIN		0x4E  // 0x138
#define GPIO_CLEARDATAOUT	0x64  // 0x190
#define GPIO_SETDATAOUT	0x65  // 0x194

/* max size in bytes */
#define GPIO_MAX		0x198

uint32_t* gpio1;
uint32_t oe;

/* output instellen */
int main(int argc, char* argv[])
{
	gpio1 = ioremap( GPIO1_ADDR, GPIO_MAX * sizeof(uint32_t) ); 
	barrier();
	oe = ioread32( gpio1 + GPIO_OE );
	rmb();
	iowrite32( (oe & (~(1<<PIN))), gpio1 + GPIO_OE );
	wmb(); // write memory barrier
	iounmap(gpio1);

	/* ledje aan en uit zetten */

	gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX); 
	barrier();
	iowrite32( (1<<PIN), gpio1 + GPIO_SETDATAOUT   ); // Pin 19 aan
	iowrite32( (1<<PIN), gpio1 + GPIO_CLEARDATAOUT ); // Pin 19 uit

	wmb(); // write memory barrier 
	iounmap(gpio1);
	return 0;
}
