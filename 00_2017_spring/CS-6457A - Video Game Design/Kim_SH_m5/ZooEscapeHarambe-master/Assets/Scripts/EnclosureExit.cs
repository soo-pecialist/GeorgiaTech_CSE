using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class EnclosureExit : MonoBehaviour
{
    public string NextLevel;
    public GameObject winGameCanvas;

    void OnTriggerEnter(Collider col)
    {
        if (col.gameObject.tag == "Player")
        {
            //might be an optional message to show before going to main menu
            if (NextLevel == "MainMenu" && winGameCanvas != null)
            {
                Time.timeScale = 0;
                winGameCanvas.SetActive(true);
            }
            else {
                SceneManager.LoadScene(NextLevel);
            }
        }
    }
}