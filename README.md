# Projeto utilizando comunicação I2C para controlar o display SSD1306

## 1. Propósito
O objetivo desse projeto manipular o display SSD1306, utilizando a comunicação I2C em conjunto com a matriz de LEDs WS2812, enviar mensagens pela UART, tratar interrupções e bounce.


## 2. Vídeo de demonstração
https://www.dropbox.com/scl/fi/g21anp5iluvngydx7iihu/V-deo-do-WhatsApp-de-2025-02-09-s-19.45.42_c4013f80.mp4?rlkey=z8k6m6mx4rs9kw5qy3nlyul6q&st=yjxgs8ni&dl=0


## 3. Funcionalidades
1. Ao pressionar o botão A alterna o estado do LED RGB verde, informa o estado tanto pela Serial Monitor quanto pelo display SSD1306.
2. Ao pressionar o botão B alterna o estado do LED RGB azul, informa o estado tanto pela Serial Monitor quanto pelo display SSD1306.
3. Ao digitar um caractere de A-Z/a-z (maiúsculo ou minúsculo) no Serial Monitor ele será exibido no display SSD1306.
4. Ao digitar um número no Serial Monitor ele será exibido na matriz de LEDs 5x5 WS2812 e no display SSD1306.
5. Ao pressionar o botão JOY (botão do joystick) irá habilitar o modo BootSel (gravação).



## 4. Pré-requisitos
1. Ter o [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado na máquina;
2. Ter o [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) instalado;
3. Ter o [Visual Studio Code](https://code.visualstudio.com/download) instalado;
4. Ter este repositório clonado na sua máquina;
5. Ter as seguintes extensões instaladas no seu VS Code para o correto funcionamento:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake);
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools);
- [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico);

##  5. Como executar o projeto e simular:
1. Clonar o repositório: https://github.com/Astorolus-11/embarcatech-interfaces-de-comunicacao-serial
2. Abrir a pasta clonada através do import project:

   ![image](https://github.com/user-attachments/assets/9ea528e1-0253-4cf8-b6c6-8532be0fc1b4)

4. Para executar na placa clique em Run que está localizada no rodapé do vscode (A placa precisa já está conectada e com o BootSel ativado):

   ![image](https://github.com/user-attachments/assets/36b14dce-1309-4f0c-a7f3-3cd7edb2b336)
   
  # A placa utilizada foi a BitDogLab, com o Raspberry Pi Pico W RP2040, disponibilizada pela Embarcatech.
  ![WhatsApp Image 2025-02-09 at 18 50 34](https://github.com/user-attachments/assets/b61bbb99-e33a-4585-aeee-581cc2e549b7)



  # Pronto! já está tudo pronto para testar o projeto!


