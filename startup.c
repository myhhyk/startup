#include <stdint.h>
//#include "stm32f103xb.h"
#include "Kiti.h"
//USART1 TX -> PA9	
//USART1 RX -> PA10
/*Импортируем функцию main*/
extern void main(void);
void Reset_Handler(void);
/*Создаём структуру для дальнейшего размещения векторов*/
typedef struct{
	const uint32_t* stack_top;
	void (*reset)(void);
	void (*NMI)(void);
	void (*staff_func[13])(void);
	void (*interrupt_vectors[68])(void);
}ISR_Vector_t;

void ADC1_Handler(void);

/*Импортируем адрес стека из файла линковки*/
extern const uint32_t __StackTop;
/*Выделяем память под структуру ISR_Vector, указываем,чтобы компановщик положил во входную секцию .isr_vector */
__attribute__((section(".isr_vector"),__unused__))/*аттрибуты указываются именно при создании объекта(когда выделяется память)*/
const ISR_Vector_t ISR_Vector = {&__StackTop,Reset_Handler,Reset_Handler,.interrupt_vectors[18] = setupADC1	};/*а не при объявлении*/

void Reset_Handler(void){
	
	setupRCC();
	setupUSART1();
	
	/*Импортируем адреса секций*/
	extern uint8_t _data_flash;
	extern uint8_t _data_sram_start;
	extern uint8_t _data_sram_end;
	
	
	/*Создаем указатели для инициализации переменных*/
	uint8_t*	dst;
	uint8_t* 	src;
	/*Присваиаем указателям адреса начала секций данных ROM и RAM*/
	src = &_data_flash;
	dst = &_data_sram_start;
	
	/*Переносим данные из ROM в RAM*/
	while(!(dst == &_data_sram_end))
	{
		*dst++ = *src++;
	}
	/*Экспортируем адреса начала и конца секции .bss*/
	extern uint8_t _bss_start;
	extern uint8_t _bss_end;
	/*Присваиваем указателю адрес начала .bss секции*/
	dst = &_bss_start;
	/*Заполняем нулями секцию .bss*/
	while(!(dst == &_bss_end)){
		*dst++ = 0;
	}

	/*Вызываем main()  (Переходим в main())*/
	main();
}


/*extern делать именно в той функции
 * где используется внешняя переменная*/
/*arm-none-eabi-nm --print-size test.elf*/
/*arm-none-eabi-nm --print-size test.elf*/
//arm-none-eabi-gcc  %f -g3 -c -I ./lib  -mcpu=cortex-m3  -o %e.o
//arm-none-eabi-ld -T map.ld -o %e.elf  %e.o
