# WarriorCombat

A third-person action combat demo developed with **Unreal Engine 5.5.4**.

---

## 🧾 Description

**WarriorCombat** is an Unreal Engine 5 project focused on building responsive melee combat mechanics with modern design patterns. This prototype explores core gameplay systems such as combo attacks, lock-on targeting, animation-driven combat, and enemy AI.

Developed by **Xiaoyu Zhang** and inspired by the tutorial from [Vince Petrelli](https://www.udemy.com/course/unreal-engine-5-advanced-action-rpg/), this project also serves as a study and demonstration of Unreal's Gameplay Ability System (GAS), character states, and animation integration in a C++ driven workflow.

---

## ✨ Features

- 🎮 **Third-person melee combat**
  - Light / heavy attacks and combo chaining
  - Directional dodging and sprint mechanics
- 🎯 **Lock-on targeting system**
  - Smooth camera tracking of selected enemy
- 🤖 **Basic enemy AI**
  - Detect, chase, strafe and engage player
- 🧠 **State machine architecture**
  - Modular behavior and animation transitions
- ⚙️ **Gameplay Ability System (GAS)**
  - Abilities with tags, effects, and cooldowns
- 🎞️ **Montage-based animation system**
  - Syncing gameplay logic with root motion animations

---

## 🚀 Getting Started

### Requirements
- Unreal Engine **5.5.4**
- Windows 10/11

### How to Run

1. **Clone the repository**

   ```bash
   git clone https://github.com/PanAkatsuki/WarriorCombat.git
   ```
   
2. Open the .uproject file in Unreal Engine 5.5.4.

3. Compile the project (if not pre-built):

   - Open in Visual Studio 2022

   - Right-click the solution → Build

4. Run in Editor or play in "Selected Viewport".

---

## 📁 Source Code Structure

All C++ source code is located in the `Source/Combat/` directory, organized following Unreal Engine’s best practices with `Public/` (header files) and `Private/` (implementation files) separation.

### 🔧 Major Modules

| Folder              | Description |
|---------------------|-------------|
| `AI/`               | Enemy behavior logic, including AIController and Behavior Trees integration. |
| `AbilitySystem/`    | Gameplay Ability System (GAS) setup — abilities, attribute sets, gameplay effects. |
| `AnimInstances/`    | Custom animation instance classes handling animation state and montages. |
| `Characters/`       | Player and enemy character base classes, movement, input, and states. |
| `CombatTypes/`      | Common combat types, enums, and helper structs (e.g., damage types, combat states). |
| `Components/`       | Modular components like health, targeting, ability queues, etc. |
| `Controllers/`      | PlayerController and AIController logic. |
| `DataAssets/`       | Data-driven configs for abilities, items, enemies, weapons, etc. |
| `GameModes/`        | GameMode and GameState classes for managing the overall game flow. |
| `Interfaces/`       | Blueprint and C++ interfaces used across systems for abstraction. |
| `Items/`            | Item definitions, inventory logic, and equipment functionality. |
| `SaveGame/`         | Save/load system using `USaveGame` objects. |
| `Widgets/`          | UI elements such as HUD, health bars, lock-on indicator, etc. |

This modular design allows for easy navigation, maintainability, and future scalability of the combat system.

---

## 📽️ Demo Video

> 📺 [Click here to watch the gameplay demo](https://drive.google.com/file/d/15AV7upSPgC-2KwE8FYTRmf1fMd0WODi0/view?usp=sharing)

---

## 🚧 Project Status

This project is under active development and intended primarily for learning and prototyping purposes.

**Planned additions:**
- [x] Basic combo and animation system
- [x] Lock-on targeting and camera control
- [x] Blocking / Parry system
- [x] Skill ability system with cooldown
- [x] Boss fight AI behavior
- [ ] Ragdoll death and visual effects

---

## 👤 Author

**Xiaoyu Zhang**  
 
🔗 [GitHub](https://github.com/PanAkatsuki)

---

## 📜 License

Copyright © Xiaoyu Zhang
All rights reserved.
This project is intended for personal, educational, and demonstration purposes only.

---

## 🙏 Acknowledgments

- Vince Petrelli for the UE5 tutorial.

---