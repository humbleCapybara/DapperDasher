# 🎩 Dapper Dasher
Dapper Dasher is a 2D side-scrolling runner game inspired by the classic Google Dino Game — but with more style, animations, and sound! Built in C++ using the raylib library, it’s a fun and fast-paced project showcasing animation handling, collision detection, and simple physics.

## 🕹 Features
-> Animated main character (Scarfy) with smooth running and jumping <br>
-> Multiple obstacles (Nebulae) with animated sprites <br>
-> Parallax scrolling background layers for depth and immersion <br>
-> Gravity and jump physics for realistic motion <br>
-> Victory and loss conditions with replay options<br>
-> Integrated sound effects and background music<br>

## ⚙️ Technical Highlights
-> Uses a custom AnimData struct to manage animations<br>
-> Frame-based update logic running at 60 FPS<br>
-> Efficient collision detection using rectangle overlap checks<br>
-> Randomized obstacle placement for varied runs<br>
-> Audio management with jump, victory, and loss sounds<br>
-> Clean resource management (textures, sounds, music)<br>

## 💾 How to Run
### 1️⃣ Clone the repository:
```
git clone https://github.com/your-username/dapper-dasher.git
```
### 2️⃣ Make sure you have raylib installed.
You can install it via your package manager or build from source:<br>
https://github.com/raysan5/raylib

### 3️⃣ Compile the code (example using g++):
```
g++ main.cpp -o dapper-dasher -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
```
### 4️⃣ Run the game:
```
./dapper-dasher
```

### 🎶 Assets & Credits
Textures: Scarfy and Nebula sprite sheets (from raylib examples)<br>
Sounds: Custom jump, victory, and loss sound effects<br>
Background Music: Looping track for atmosphere<br>

## 🚀 Future Improvements
-> Add power-ups and bonuses<br>
-> Introduce multiple levels or biomes<br>
-> High score tracking and leaderboard<br>

