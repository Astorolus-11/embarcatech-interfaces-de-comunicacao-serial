#include <stdio.h>
#include "pico/stdlib.h"

//Pinos:
const uint led_verde = 11;
const uint led_azul = 12;
const uint botao_a = 5;
const uint botao_b = 6;

//Protótipos das funções:
void setup(); // Inicializa os pinos
static void gpio_irq_handler(uint gpio, uint32_t events); // Trata a interrupção

//Variáveis globais:
static volatile uint32_t last_time = 0; // Para tratar o debounce
static bool estado_led_verde = false;
static bool estado_led_azul = false;

int main()
{   setup();
    stdio_init_all();


    //Interrupções:------------------------------------------------------------------------------------------------------
    gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler); //Interrupção para o botão A
    gpio_set_irq_enabled_with_callback(botao_b,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler); //Interrupção para o botão B
    //-------------------------------------------------------------------------------------------------------------------
    
    while (true) {
        
    }
}


//Campo das funções
//-----------------------------------------------------------------------------------------------------------------

void setup(){
    //Inicializa os pinos:
    gpio_init(led_verde);
    gpio_init(led_azul);
    gpio_init(botao_a);
    gpio_init(botao_b);
    
    //Define como entrada/saida:
    gpio_set_dir(led_verde,GPIO_OUT);
    gpio_set_dir(led_azul,GPIO_OUT);
    gpio_set_dir(botao_a,GPIO_IN);
    gpio_set_dir(botao_b,GPIO_IN);

    //Habilita o pull-up dos botões:
    gpio_pull_up(botao_a);
    gpio_pull_up(botao_b);

    //Os leds estão inicialmente desligados:
    gpio_put(led_verde,false);
    gpio_put(led_azul,false);
}
//-----------------------------------------------------------------------------------------------------------------
static void gpio_irq_handler(uint gpio, uint32_t events){ // Função de callback da interrupção
    uint32_t tempo_atual=to_us_since_boot(get_absolute_time()); //Pega o tempo desde o começo do sistema
   
    if(tempo_atual-last_time>200000){ //Debounce
        last_time=tempo_atual; //Atualiza o ultimo tempo

        if(gpio_get(botao_a)==0){ //Botão responsável por alterar o estado do LED verde
            estado_led_verde=!estado_led_verde; //Inverte o estado do LED verde
            gpio_put(led_verde,estado_led_verde);


        }
        else if(gpio_get(botao_b)==0){ //Botão responsável por alterar o estado do LED azul
            estado_led_azul=!estado_led_azul; //Inverte o estado do LED azul
            gpio_put(led_azul,estado_led_azul);


        }

    }
}
