# Haptic Cutting Knife (ME327 Group 16, 2025)

This repository contains all the source files, mechanical designs, and code for our project: **Haptic Cutting Knife**, developed for Stanford's ME327: Design and Control of Haptic Systems (Spring 2025).

📍 **Project website**: [https://charm.stanford.edu/ME327/2025-Group16](https://charm.stanford.edu/ME327/2025-Group16)

---

## 📁 Repository Contents

| File | Description |
|------|-------------|
| `ME327_Haptic_Cutting_G16_2025.step` | 3D CAD model of the full haptic knife system, including motor mount, capstan pulley, and structural parts. |
| `haptic_cutting_vFinal.ino` | Final Arduino sketch for controlling the motor, reading encoder values, and handling user input. |
| `hapticscutting.blend` | Blender file used for visual design and rendering of the knife and balloon-based haptic mechanism. |
| `parts_list.csv` | Tabulated list of all components used in the project, including specifications and cost notes. |
| `README.md` | This documentation file. Describes project overview, file structure, and cost breakdown. |

---

## 💸 Bill of Materials

| Component | Specification | Quantity | Unit Price (USD) | Notes |
|-----------|---------------|----------|------------------|-------|
| Maxon DC Motor | 148877 with Encoder 110513 (Part# 199554) | 1 |  | Borrowed from lab - Used in capstan transmission |
| Motor Driver | BTS7960 | 1 | 12 | High current motor driver (purchased) |
| Microcontroller | Hapkit board (Arduino Uno based) | 1 |  | Borrowed from lab - Used for control and sensing |
| Encoder | Integrated with Maxon motor | 1 |  | Borrowed from lab - Included with motor |
| Power Supply | DC Regulated (12V 10A) | 1 |  | Borrowed from lab - Used for motor and system power |
| Capstan Pulley | Custom with threaded guide and 3D printed | 1 |  | Borrowed from lab - 3D printed part |
| Fishing Line | High-strength | 2 m |  | Borrowed from lab - For capstan drive |
| Knife Handle | 3D printed | 1 |  | Borrowed from lab - Custom design |
| Base & Stand | 3D printed | 1 |  | Borrowed from lab - Support structure |
| Balloon (for jamming) | Latex with sand-filled or quinoa-filled | 4 |  | Borrowed from lab - For tactile grasping |
| Syringe | 60 ml | 1 |  | Borrowed from lab - To control particle jamming |
| VR Headset | Meta Quest 2 | 1 |  | Borrowed from lab - For VR experience |
| Push Buttons | Momentary switch | 3 |  | Borrowed from lab - Mode selection |
| Wires & Connectors | Generic jumper wires 1 set | 1 set |  | Borrowed from lab - For all wiring |
| Breadboard | Small breadboard | 2 |  | Borrowed from lab - Signal routing |
| Screws & Fasteners | Miscellaneous 1 set | 1 set |  | Borrowed from lab - For mechanical assembly |
| **Total Cost** |  |  | **12** | Only one component purchased |
