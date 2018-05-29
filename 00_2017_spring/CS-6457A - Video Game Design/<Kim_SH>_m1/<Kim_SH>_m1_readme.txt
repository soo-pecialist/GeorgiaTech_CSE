<Read Me> 
0. complied executable file(.unity) is located at “<Kim_SH>_m1/Assets/_Scene/“ 

1. Soo Hyeon Kim 

2. 
a) 
> My character is a rigged 3d character model from Mixamo. 
> Main camera chases my character (Bulkie).
> Level you can utilize is “PlayLevel”
> My character(Bulkie) can turn left/right slightly/hard in idle state(standing) with keys of “a/d, q/e”
> My character starts to walk(or Run according at “speed” (1-9) that you will assign 
	- e.g., if you hit “0”, it will run at full speed; if “1”, it will run at walking speed; if “1-9”, it will fast walk/slow run. (* initial state is walking)
> My character can move in the direction you want it to go - if you hit “w+d”, “Bulkie” will run to the right narrowly; if “w+e”, “Bulkie” will run to the right widely etc. 
> My character can walk/run backward - hit “s” and assign the value from 1-9. 
> My character can walk/run backward to the rigsht or left
	- e.g., “s+a” = hard left backward;  “s+e” = light right backward
> My character can jump in any state —> simply hit space of the keyboard. 
> My character will turn to “falling” animation when it’s falling. 
> My character will turn to “landing” animation when it’s at a certain height from the ground. 

3. I used animation that I downloaded together with a Mixamo rigged character. (e.g. “walking animation”, “running animation” etc. 
> I downloaded “barrier” and “barrier_Stop” asset from asset store. 
	(link: https://www.assetstore.unity3d.com/en/#!/content/72966)

4. I developed my game on OS X

5. I first created my character, bulky, using mixamo and fuse, then I downloaded animations I needed from Mixamo webpage. and then, I planned animation blueprint
- first I add turning animation from idle; second, I add walk/running blend tree animation  and also add walk/running backward animations in bled tree as well; third, I created jumping animation from any state; and finally, i dealt with falling and landing loop animations. 

6. You can open “PlayLevel” in “_Scene” folder
