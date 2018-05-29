using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChange : MonoBehaviour {

	public void LoadLevelMainMenu() {
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("MainMenu");
		//Application.LoadLevel("MainMenu");
	}

	public void LoadLevelEnclosure() {
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("Enclosure");
        //Application.LoadLevel("Enclosure");
	}

	public void LoadLevelMaze() {
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("SneakMaze");
        //Application.LoadLevel("SneakMaze");
	}

    public void LoadLevelParking()
    {
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("Parking");
        //Application.LoadLevel("Parking");
    }
}
