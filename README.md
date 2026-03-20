# Caro Game Project

A classic Caro (Gomoku) game built with C++ and Raylib. 


## How to Install and Setup in Visual Studio
I have included Raylib library in this Repository, so you don't have to download it

If you are having trouble setting up the Raylib library in Visual Studio, you can watch this tutorial:
[How to setup Raylib with Visual Studio 2022 (YouTube)](https://www.youtube.com/watch?v=O96m52ZxQcw)
### 1. Clone the repository
```bash
git clone https://github.com/nhtnam232/CaroGameProject.git
```
### 2. Install Raylib in VS
1. Open .sln in Visual Studio
2. Right-click CaroGame project in the Solution Explorer and select Properties.
3. Set the configuration at the top to All Configurations and All Platforms.
4. Include Directories
- Go to C/C++ -> General -> Additional Include Directories
- Add the path to your Raylib ***include*** folder (e.g., C:\raylib\include).
- Add the path to the ***include*** directory in this repository
5. Library Directories
- Go to Linker -> General -> Additional Library Directories.
- Add the path to your Raylib ***lib*** folder (e.g., C:\raylib\lib).
6. Input Dependencies:
- Go to Linker -> Input -> Additional Dependencies.
- Add raylib.lib;winmm.lib;

Now you can Build and Run the game!
   
