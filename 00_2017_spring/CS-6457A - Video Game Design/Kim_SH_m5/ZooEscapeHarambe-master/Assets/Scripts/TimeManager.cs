using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
public class TimeManager : MonoBehaviour {

	public Text timeText;
	public Text GoText;

	int time = 0;
	int totaltime = 180;
	int temptime = 0;
	int s=0;
	// Use this for initialization
	void Start () {
		
		timeText.text = "0";
		//time = (int)Time.timeSinceLevelLoad;
	}
	
	// Update is called once per frame
	void Update () {
		//time = (int)Time.time;
		time =(int)Time.timeSinceLevelLoad;
		if(Time.timeSinceLevelLoad > temptime)
		{
			temptime++;
			totaltime--;
			timeText.text = "Time: " + totaltime.ToString()+" s";
		}

		if(totaltime <=0 )
		{
			Time.timeScale = 0;
			timeText.text = "Time Over";
			GoText.text = "Game Completed!";
			Time.timeScale = 0.0f;
			Debug.Log ("Gamecmpltd");
		}

	}

		

}
