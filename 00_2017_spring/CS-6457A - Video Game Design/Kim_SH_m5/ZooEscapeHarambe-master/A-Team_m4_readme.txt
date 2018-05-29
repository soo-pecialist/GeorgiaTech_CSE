
Steps to Demonstrate:
1. Open the project with Unity. **A build is also included if you wish to run that instead.
2. Open the Assets/Scenes/MainMenu scene if it's not already open.
3. Click play to run the scene
4. Note the custom font and buttons, The game title, and the team name
5. Note the bloom lighting, water reflections, and slight film grain effects.
6. Note the rotating camera (animated with Mechanim)
7. Use the keyboard arrow keys to demonstrate keyboard selection of the two buttons, selection highlighted
by bananas
8. Click or use the arrow keys and enter to select the credits button
9. Note the camera change to in front of Harambe, + the blur effect
10. Note the credits scrolling vertically across the screen
11. Note the credits contain information on assets and team member contributions

First and only scene is Assets/Scenes/MainMenu.


-- General mini-game driving scene info:For 3 minutes (180 seconds), if you don’t hit any obstacles, and drive safe, then you win!Drive Safe, it’s very hard to control. —- Pass through the arrow on the screen then next arrow will show up, and repeat until you get on the main road. -- Control keys:1) Arrow keys (Up/Dn/Left/Right) or W/S/AD : Forward ,Backword, Left, Right move of Gorrila and also for the Car.2) Extra Keys for Gorilla:‘W(or Up) + R’ to run3)K for kill (die) L for Rebornleft shift for RunH for Hit U for Clapping 4) Space key: Brake when driving-- Script Explanation	1.Time Manager:Controls the countdown time of the game.180 seconds are allowed for this level.2.MyGameController:Transfers control of the Game from gorilla to the car when gorilla enters car.This also manages what happen if the car collides any obstacle the game overs.3.MyGorillaCameraScript:This script manages camera of the scene and give best view of the game.Updates the view of the game when character moves.4.MyGorillaCharacter:Basically a model class for all the gorilla animation and movements.5.MyGorillaUserControllerThis script uses the "MyGorillaCharacter" script to perform the gorilla actions.It takes inputs from user and call appropriate function from "MyGorillaCharacter" script.6.SimpleCarController:This script is responsible for car movements.7.ArrowGenerate:It handles the indication arrows when the game starts.8.ArrowRotate:This script controls the rotations of the indication arrows.