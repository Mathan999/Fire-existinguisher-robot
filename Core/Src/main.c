#include "main.h"

// Flame sensor
#define FLAME_SENSOR_PIN    GPIO_PIN_5
#define FLAME_SENSOR_PORT   GPIOA

// Motor pins
#define IN1 GPIO_PIN_0
#define IN2 GPIO_PIN_1
#define IN3 GPIO_PIN_2
#define IN4 GPIO_PIN_3
#define MOTOR_PORT GPIOA

// Relay for water pump
#define RELAY_PIN   GPIO_PIN_0
#define RELAY_PORT  GPIOB

// Servo pin (bit-bang)
#define SERVO_PIN   GPIO_PIN_6
#define SERVO_PORT  GPIOA

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void move_forward(void);
void stop_motor(void);
void spray_water(void);
void set_servo_angle(int angle); // Bit-bang PWM for servo

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  stop_motor(); // Motors off
  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET); // Pump OFF

  while (1)
  {
    int fire_count = 0;

    for (int i = 0; i < 20; i++)
    {
      if (HAL_GPIO_ReadPin(FLAME_SENSOR_PORT, FLAME_SENSOR_PIN) == GPIO_PIN_RESET)
        fire_count++;
      HAL_Delay(5);
    }

    if (fire_count >= 5 && fire_count <= 13)
    {
      move_forward(); // Medium fire
      HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET); // Pump OFF
    }
    else if (fire_count > 13)
    {
      stop_motor(); // Close fire
      HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_SET); // Pump ON
      spray_water(); // Spray using servo
      HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET); // Pump OFF
    }
    else
    {
      stop_motor(); // No fire
      HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET); // Pump OFF
    }

    HAL_Delay(100); // Loop delay
  }
}

void move_forward(void)
{
  HAL_GPIO_WritePin(MOTOR_PORT, IN1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(MOTOR_PORT, IN2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(MOTOR_PORT, IN3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(MOTOR_PORT, IN4, GPIO_PIN_SET);
}

void stop_motor(void)
{
  HAL_GPIO_WritePin(MOTOR_PORT, IN1 | IN2 | IN3 | IN4, GPIO_PIN_RESET);
}

void spray_water(void)
{
  set_servo_angle(90);  // Spray direction
  HAL_Delay(500);
  set_servo_angle(0);   // Back to idle
  HAL_Delay(500);
}

// Bit-banged PWM for SG90 servo
void set_servo_angle(int angle)
{
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;

  uint32_t pulse_width_us = 500 + ((angle * 2000) / 180);

  for (int i = 0; i < 50; i++)  // ~1 second movement
  {
    HAL_GPIO_WritePin(SERVO_PORT, SERVO_PIN, GPIO_PIN_SET);
    for (volatile int j = 0; j < pulse_width_us * 8; j++);
    HAL_GPIO_WritePin(SERVO_PORT, SERVO_PIN, GPIO_PIN_RESET);
    HAL_Delay(20); // 20 ms cycle
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Motor pins
  GPIO_InitStruct.Pin = IN1 | IN2 | IN3 | IN4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Flame sensor
  GPIO_InitStruct.Pin = FLAME_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Servo pin
  GPIO_InitStruct.Pin = SERVO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Relay pin
  GPIO_InitStruct.Pin = RELAY_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Set everything LOW by default
  HAL_GPIO_WritePin(GPIOA, IN1 | IN2 | IN3 | IN4 | SERVO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, RELAY_PIN, GPIO_PIN_RESET);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
