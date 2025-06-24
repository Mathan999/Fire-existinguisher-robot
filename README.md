# 🔥 Fire Extinguisher Robot using STM32 (STM32CubeIDE)

This project is a simple yet effective fire-fighting robot built using an STM32F446RE microcontroller. The robot can detect fire using an IR flame sensor, move toward the fire, and automatically extinguish it using a water pump controlled via a relay and servo motor.

---

## 🧾 Project Overview

The Fire Extinguisher Robot is designed to detect fire using flame sensors and autonomously move toward the source of the fire. Once the fire is close enough, it activates a water spraying mechanism using a servo motor and a water pump to extinguish the flame.

The project is programmed using **STM32CubeIDE**, and runs on an **STM32F446RE** development board.

---

## 🔧 Components Used

| Component            | Quantity | Description                                        |
|----------------------|----------|----------------------------------------------------|
| STM32F446RE MCU      | 1        | Main microcontroller board                         |
| L298N Motor Driver   | 1        | Drives 4 DC motors                                 |
| DC Geared Motors     | 4        | Enables robot movement                             |
| Wheels               | 4        | Connected to DC motors                             |
| IR Flame Sensor      | 1–4      | Detects fire by IR radiation                       |
| SG90 Servo Motor     | 1        | Rotates to spray water from the pump               |
| Water Pump           | 1        | Used to extinguish the fire                        |
| Relay Module         | 1        | Controls the pump power                            |
| LM2596 Buck Converter| 1        | Regulates power supply                             |
| Battery Pack         | 2x 3.7V  | Powers motors and other components                 |
| Jumper Wires         | Many     | For circuit connections                            |
| Chassis              | 1        | Holds all the components                           |

---

## ✨ Features

- Detects fire using IR flame sensors.
- Automatically moves toward the fire when detected.
- Stops close to the fire to prevent damage.
- Activates water spray using:
  - SG90 Servo Motor to position nozzle.
  - Water pump via Relay Module.
- Simple logic using GPIO control in STM32CubeIDE.
- Uses software PWM (bit-banging) for servo control (no hardware timers needed).

---

## ⚡ Circuit Connections

### 🔌 Motor Driver (L298N)
| L298N Pin       | STM32 Pin       |
|----------------|-----------------|
| IN1            | PA0             |
| IN2            | PA1             |
| IN3            | PA2             |
| IN4            | PA3             |
| ENA/ENB        | Connected to +5V (enabled) |
| VCC            | +12V from battery |
| GND            | Common Ground    |

### 🔥 Flame Sensor
| Flame Sensor Pin | STM32 Pin |
|------------------|-----------|
| OUT              | PA5       |
| VCC              | 3.3V/5V   |
| GND              | GND       |

### 💧 Relay Module (Pump)
| Relay Pin | STM32 Pin |
|-----------|-----------|
| IN        | PB0       |
| VCC       | 5V        |
| GND       | GND       |

### 🔁 SG90 Servo Motor
| Servo Pin | STM32 Pin |
|-----------|-----------|
| Signal    | PA6       |
| VCC       | 5V        |
| GND       | GND       |

### 🔋 Power
- Motors powered via L298N using 7.4V (2 x 3.7V Li-ion)
- STM32 board powered via USB or regulated 5V from LM2596

---

## 🧠 Logic Summary

- **On Boot:** Robot stays still.
- **Fire Detected (medium range):** Robot moves forward.
- **Fire Very Close:** Robot stops.
- **Spray Activation:** Servo motor rotates, water pump is turned ON via relay.

---

## 📁 Folder Structure (Suggested)

