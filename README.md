#

## Features
- [Easy]
  - Minimap
  - Camera Movement
- [Medium]
  - Skill System
  - Funny Skills
  - 
- [Hard] 
  - Chat AI Usage (maybe)

## Easy Manual
- Control in game
  | **Action**         | **Primary Key**  | **Alternative Key** | **Description**                                 |
  | ------------------ | ---------------- | ------------------- | ----------------------------------------------- |
  | **Move Up**        | `W`              | `↑` (Up Arrow)      | Moves the character forward or upward.          |
  | **Move Down**      | `S`              | `↓` (Down Arrow)    | Moves the character backward or downward.       |
  | **Move Left**      | `A`              | `←` (Left Arrow)    | Moves the character to the left.                |
  | **Move Right**     | `D`              | `→` (Right Arrow)   | Moves the character to the right.               |
  | **Attack**         | Left Mouse Click | —                   | Performs a primary attack (e.g., shoot, slash). |
  | **Interact**       | `F`              | —                   | Interacts with objects like home or store.      |
  | **Skill Use**      | `1 ~ 6`          | —                   | Double press the number key can summon skill.   |
  | **Change Weapon**  | `Q`, `E`         | —                   | `Q` to switch to previous, `E` to switch to next|


## Individual Contribution 
- 113062208 黃柏睿 (): 
  - 地圖圖片、奇怪的圖片
  - 角色動畫/碰撞/移動......(all character.cpp/hpp)
  - 物品/技能欄、所有技能系統、商店物品解鎖/升級、Drone(skill中召喚物)
  - Camera、關於角色的顯示GUI(HP/POWER/SHIELD bars…)、怪物血條
  - 魔改引擎(支援切割的Sprite/支持更改背景顏色或圖片)、ColoredRectangle Component
  - AI Chat、Chat Scene
  - Debug


113062306 王博彥 (): 
角色圖片分鏡
武器製作(近戰武器、遠程武器)
敵人製作(移動邏輯、敵人武器與玩家之間的互動、三個boss)
音效素材

113062XXX 陳⼩美 (70%): Procedural generated level, Map editor
