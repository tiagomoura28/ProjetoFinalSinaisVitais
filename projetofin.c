#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22     // GPIO para botão do Joystick
#define Botao_A 5          // GPIO para botão A

// Inicialização do display SSD1306
ssd1306_t ssd;
void init_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
}

// Função para configurar os LEDs RGB (PWM)
void init_leds() {
    // Configura os pinos para PWM
    gpio_set_function(11, GPIO_FUNC_PWM); // LED vermelho
    gpio_set_function(12, GPIO_FUNC_PWM); // LED verde
    gpio_set_function(13, GPIO_FUNC_PWM); // LED azul

    // Obtém os canais PWM para os pinos GPIO
    uint slice_num_red = pwm_gpio_to_slice_num(11);
    uint slice_num_green = pwm_gpio_to_slice_num(12);
    uint slice_num_blue = pwm_gpio_to_slice_num(13);

    // Configura os canais PWM para a frequência padrão
    pwm_set_clkdiv(slice_num_red, 16.0);   // Frequência padrão para o LED vermelho
    pwm_set_clkdiv(slice_num_green, 16.0); // Frequência padrão para o LED verde
    pwm_set_clkdiv(slice_num_blue, 16.0);  // Frequência padrão para o LED azul
        // Inicializa o PWM com o valor mínimo (apagado)
    
        pwm_set_gpio_level(11, 0); // LED vermelho apagado
        pwm_set_gpio_level(12, 0); // LED verde apagado
        pwm_set_gpio_level(13, 0); // LED azul apagado
        

    pwm_set_enabled(slice_num_red, true);
    pwm_set_enabled(slice_num_green, true);
    pwm_set_enabled(slice_num_blue, true);

    

    
}

// Leitura dos valores do joystick (simulação)
int read_joystick_x() {
    adc_select_input(0); // Pino 26 (X)
    return adc_read(); // Valor de 0 a 4095
}

int read_joystick_y() {
    adc_select_input(1); // Pino 27 (Y)
    return adc_read(); // Valor de 0 a 4095
}

// Função para ajustar os LEDs conforme os valores dos sensores simulados
void update_leds(int temperature, int air_quality) {
    // LED vermelho para temperatura (simulada)
    if (temperature > 30) {
        pwm_set_gpio_level(13, 255); // LED vermelho acende
       // pwm_set_gpio_level(12, 0);
       // pwm_set_gpio_level(13, 0);
    } else {
        pwm_set_gpio_level(13, 0); // LED vermelho apaga
    }

    // LED verde para qualidade do ar (simulada)
    if (air_quality > 56) {
        pwm_set_gpio_level(11, 255); // LED verde acende
      //  pwm_set_gpio_level(11, 0);
       // pwm_set_gpio_level(11, 0);
    } else {
        pwm_set_gpio_level(11, 0); // LED verde apaga
    }

    // LED azul pode ser usado como indicador de sistema ativo
    //pwm_set_gpio_level(11, 50); // LED azul fraco aceso
   // pwm_set_gpio_level(12, 50);
   // pwm_set_gpio_level(13, 50);
}

// Função para exibir os valores no display
void display_values(int temperature, int air_quality) {
    char temp_str[10], air_quality_str[10];
    sprintf(temp_str, "Temp: %d C", temperature);
    sprintf(air_quality_str, "Air: %d", air_quality);

    ssd1306_fill(&ssd, false);  // Limpa o display
    ssd1306_draw_string(&ssd, temp_str, 10, 10); // Exibe temperatura
    ssd1306_draw_string(&ssd, air_quality_str, 10, 30); // Exibe qualidade do ar
    ssd1306_send_data(&ssd); // Atualiza o display
}

int main() {
    stdio_init_all();

    // Inicialização dos sensores e display
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    init_display();
    init_leds();

    while (true) {
        // Lê os valores do joystick e simula os sensores
        int joystick_x = read_joystick_x();
        int joystick_y = read_joystick_y();


        //if (joystick_x >= 2048 && joystick_x <= 2128) {
         //   pwm_set_gpio_level(11, 0);
         //   pwm_set_gpio_level(12, 0);
      //  }
        //if (joystick_y >= 2040 && joystick_y <= 2090) {
        //    pwm_set_gpio_level(11, 0);
        //    pwm_set_gpio_level(13, 0);
      //  }

        
        // Simulação dos valores de temperatura e qualidade do ar
        // Ajustando os valores do joystick para um intervalo mais realista
        int temperature = (joystick_x * 50) / 4095;  // Mapeia de 0 a 50°C
        int air_quality = (joystick_y * 100) / 4095;  // Mapeia de 0 a 100

        // Atualiza LEDs conforme valores
        update_leds(temperature, air_quality);

        // Exibe os valores no display
        display_values(temperature, air_quality);

        // Delay
        sleep_ms(200);  // Atualiza a cada 200ms
    }

    return 0;
}