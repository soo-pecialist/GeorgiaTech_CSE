<Read Me> 
0. complied executable file(.unity) is located at “<Kim_SH>_m3/Assets/HW3/Scene/“ 
My scene is “HW3”.

1. Soo Hyeon Kim 

2. I developed my game on OS X 

3. You can open “HW3” in “Scene” folder

4. 
a) You can see my name on the screen (HUD)

b) My project meets the requirement 1 - state machine, camera, field design.
My NPC is controlled with 5 personal states, and 4 team states.
NPC will be randomly assigned attribute (offensive / defensive).
-> states explanation
	- XX: NPC no ball, and enemy no ball
	- XO: NPC no ball, and enemy has a ball
	- OX: NPC has a ball, and enemy no ball
	- OO: NPC and enemy both have a ball. 
-> NPC states transition:
	- XX: if NPC picks a ball, it changes to OX states; if the enemy picks a ball, it changes to XO
	- XO: if NPC picks a ball, changes to OO states; if enemy picks a ball, XO; if enemy throws a ball, XO or XX
	- OX: if NPC throws a ball, changes to XX; if enemy picks a ball, OO.
	- OO: if NPC throws a ball, changes to XO; if enemy throws a ball, OO or OX
-> NPC play style
	- Offensive(OFF) / Defensive(DEF)
	- To determine the team status
		- OFF: if more than 25% of teammates are alive, it thinks of situation as safe; otherwise, dangerous
		- DEF: if more than 50% of teammates are alive, it thinks of situation as safe; otherwise, dangerous.
	- Based on team status, together with NPC status, NPC will act accordingly.
-> Team Status
	1. LL: both team are small (A)
	2. LM: my team is small, and other team is big (B)
	3. ML: my team is big, and other team is small (C)
	4. MM: both team are big (D)
	5. team status is renewed every time NPCs are dead or revived. 

c) My project meets the requirement 2 - picking up the ball from spawn locations. 
balls are spawning at 2 spots in each team’s court randomly every 2-3 seconds. 
If the player gets the ball, you can see its head get bigger. 

d) My project meets the requirement 3 - throwing a ball to enemies (current position) 

e) My project meets the requirement 4 - hiding behind the wall in danger
- depending on player game style (OFF / DEF) default hiding tendency is a bit different. 

f) My project meets the requirement 5 - using nevmesh and making NPC navigate. 
I use 7 scripts: Ball.cs; GameAIBot.cs; GameAINavSteeringController.cs; GameManager.cs; Gameplayer.cs; RespawnArea.cs; Robot.cs
Especially, I modified AINavSteeringController.cs, creating GameAINavSteeringController.cs.

g) My project meets the requirement 6 - getting hit by a ball, moving to prison back and forth. 
if NPC gets hit by a ball in court, it walks out along the side line to prison. and if it get hit by a ball in prison which its team has thrown, it will come back to the court along the sideline. 

f) My project meets the requirement 7) - informative sound effects. 
you can hear audio effects when NPC hit by a ball, picking up a ball, throwing a ball, and  the ball spawned at spawning points. 

4-1) general info of game. 
once you starts the game, you will input the number(1-9) of players in one team. 
Let’s say you input 4, then 8 NPCs (4 each in one team) will be spawned and starts playing dodgeball. If you want to restart the game, no matter when you can input player numbers and click start, then it’ll restart. 
Also, you can see each NPC’s status (Playing/SideLine/in Prison)
Once NPC pick up the ball, then it will check if there’s an enemy exposed on the spot, if there is, then aim at the player and if not, checking if there’s a teammate in prison. If there’s a teammate in person then throw a ball to the player to help it revive. once it detect an enemy, it’ll move toward it. 

5. Extra credit
You can join the team as a human player. 
if you hit “p”, the player will join the game. 
you can control it with arrows (up/down/right/left) or AWSD for movement. 
and it throws a ball when you click “left shift” 
also, if you get hit by a ball, your character will go to prison. 


6. 
a) I referenced the project that Jeff shared through Git. 
b) I downloaded my audio sources at http://www.freesound.org/

