//Bibliotecas:
#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h" // Responsável por entrar no modo bootsel pelo botão joy
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "matriz_leds.pio.h" // arquivo .pio
#include "hardware/i2c.h" // Responsável pela comunicação I2C
#include "fonte.h" // Arquivo das fontes
#include "ssd1306.h" // Arquivo que controla o display

//Pinos:
#define pin_matrix 7
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
const uint led_verde = 11;
const uint led_azul = 12;
const uint botao_a = 5;
const uint botao_b = 6;
const uint botao_joy = 22;


//Protótipos das funções:
void setup(); // Inicializa os pinos
static void gpio_irq_handler(uint gpio, uint32_t events); // Trata a interrupção
void exibir_numero(int num);
uint32_t cores(double b, double r, double g);

//Variáveis globais:
static volatile uint32_t last_time = 0; // Para tratar o debounce
static bool estado_led_verde = false;
static bool estado_led_azul = false;
#define pixels 25
ssd1306_t ssd; //Estrutura do display

//PIO:
static PIO pio;
static uint sm;
static uint offset;
static uint32_t valor_led;

//Desenho dos números da matriz de leds:
double numeros[10][25]={
    {0.0, 0.2, 0.2, 0.2, 0.0,  // 0
     0.0, 0.2, 0.0, 0.2, 0.0,  
     0.0, 0.2, 0.0, 0.2, 0.0,  
     0.0, 0.2, 0.0, 0.2, 0.0,  
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.0, 0.2, 0.0, 0.0, // 1 
     0.0, 0.0, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 2 
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 3 
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.0, 0.2, 0.0, // 4 
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 5 
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 6 
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 7 
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 8 
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 9 
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0}

};

int main()
{   
    pio=pio0;
    bool ok;

    ok=set_sys_clock_khz(128000,false);// definindo o clock do sistema

    printf("Iniciando a transmissao PIO:\n");
    if(ok){ // Se o clock de 128000 foi habilitado com sucesso
        printf("Clock definido: %ld\n",clock_get_hz(clk_sys));
    }

    //CONFIGURAÇÂO DA PIO:--------------------------------------------------------------------------------------------------
    offset = pio_add_program(pio,&matriz_leds_program); 
    sm = pio_claim_unused_sm(pio,true); // Utiliza uma máquina de estado que não está ocupada 
    matriz_leds_program_init(pio, sm, offset, pin_matrix);
    //-----------------------------------------------------------------------------------------------------------------------
    //CONFIGURAÇÃO DO I2C:---------------------------------------------------------------------------------------------------
    i2c_init(I2C_PORT, 400 * 1000); //Inicializa a porta i2c e define a frequência de 400khz
    
    //Habilita a função i2c da porta:
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
    //Habilita o pull-up:
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); //Inicializa o display
    ssd1306_config(&ssd); //Configura o display
    ssd1306_send_data(&ssd); //Envia os dados para o display
    //Inicia com os pixels apagados:
    ssd1306_fill(&ssd,false);
    ssd1306_send_data(&ssd);



    //-----------------------------------------------------------------------------------------------------------------------

    setup();
    stdio_init_all();


    //Interrupções:------------------------------------------------------------------------------------------------------
    gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler); //Interrupção para o botão A
    gpio_set_irq_enabled_with_callback(botao_b,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler); //Interrupção para o botão B
    gpio_set_irq_enabled_with_callback(botao_joy,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler); //Interrupção para o botão do joystick
    //-------------------------------------------------------------------------------------------------------------------
    
    while (true) {
        if(stdio_usb_connected()){
            char c;
            scanf("%c",&c);
            
            if(c>='0' && c<='9'){
                int numero = c - '0'; //Converte para número
                exibir_numero(numero); //Desenha o número na matriz 5x5
                ssd1306_draw_char(&ssd,c,55,25); //Configura para exibir o caractere
                ssd1306_send_data(&ssd); //Envia os dados pro display

            }
            else{
                ssd1306_draw_char(&ssd,c,55,25); //Desenha as letras do alfabeto
                ssd1306_send_data(&ssd); //Envia os dados pro display
            }
            

        }
        
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
    gpio_init(botao_joy);
    
    //Define como entrada/saida:
    gpio_set_dir(led_verde,GPIO_OUT);
    gpio_set_dir(led_azul,GPIO_OUT);
    gpio_set_dir(botao_a,GPIO_IN);
    gpio_set_dir(botao_b,GPIO_IN);
    gpio_set_dir(botao_joy,GPIO_IN);

    //Habilita o pull-up dos botões:
    gpio_pull_up(botao_a);
    gpio_pull_up(botao_b);
    gpio_pull_up(botao_joy);

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

            if(estado_led_verde==1){
                printf("LED verde ligado!\n"); //Informa o estado do LED verde pelo serial monitor
                //Limpa a tela e manda a mensagem no display
                ssd1306_fill(&ssd,false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd,"LED VERDE LIGADO",0,0);
                ssd1306_send_data(&ssd);
            } 
            else { 
                printf("LED verde desligado!\n");
                //Limpa a tela e manda a mensagem no display
                ssd1306_fill(&ssd,false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd,"LED VERDE DESLIGADO",0,0);
                ssd1306_send_data(&ssd);
                
            }
            


        }
        else if(gpio_get(botao_b)==0){ //Botão responsável por alterar o estado do LED azul

            estado_led_azul=!estado_led_azul; //Inverte o estado do LED azul
            gpio_put(led_azul,estado_led_azul);

            if(estado_led_azul==1){
                printf("LED azul ligado\n"); //Informa o estado do LED azul pelo serial monitor
                //Limpa a tela e manda a mensagem no display
                ssd1306_fill(&ssd,false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd,"LED AZUL LIGADO",0,45);
                ssd1306_send_data(&ssd);
            } 
            else { 
                printf("LED azul desligado!\n");
                //Limpa a tela e manda a mensagem no display
                ssd1306_fill(&ssd,false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd,"LED AZUL DESLIGADO",0,45);
                ssd1306_send_data(&ssd);
            }


        }
        else if(gpio_get(botao_joy)==0){ //Botão responsável por iniciar o modo BootSel
            printf("Habilitando o modo gravação!\n");
            reset_usb_boot(0,0);

        }

    }
}
uint32_t cores(double b, double r, double g){
    unsigned char R,G,B;
    R=r*255;
    G=g*255;
    B=b*255;
    return(G<<24) | (R<<16) | (B<<8); // para cada 8 bits recebe uma cor
}
void exibir_numero(int num){

    for(uint i=0;i<pixels;i++){ // Descobre qual o digito e desenha
        valor_led=cores(0.0,numeros[num][24-i],0.0);
        pio_sm_put_blocking(pio,sm,valor_led);
       }
}

