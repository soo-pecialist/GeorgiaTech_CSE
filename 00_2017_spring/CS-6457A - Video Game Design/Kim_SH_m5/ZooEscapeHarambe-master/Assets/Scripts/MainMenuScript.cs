//Team: A-Team
//Team Members: David Burke, Kara Stolze, Shuangke Li, Marvin Cangcianno, Soo Hyeon Kim

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainMenuScript : MonoBehaviour {

    public GameObject CreditsCanvas;
    public GameObject MainCanvas;
    public Camera MainCamera;
	public Camera CreditsCamera;
	public Camera SceneCamera;

	private Vector3 init;
	private Quaternion initQ;
    // Use this for initialization
    void Start () {
		init = MainCamera.transform.position;
		initQ = MainCamera.transform.rotation;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void ShowCredits()
    {
        MainCamera.enabled = false;
        CreditsCamera.enabled = true;
        MainCanvas.SetActive(false);
        CreditsCanvas.SetActive(true);
    }

	public void TransitionScene1() 
	{
		MainCamera.enabled = false;
		MainCamera.GetComponent<Animator> ().enabled = false;
		CreditsCamera.enabled = false;
		SceneCamera.enabled = true;
		GameObject harambe = GameObject.FindGameObjectWithTag ("Player");
		harambe.GetComponent<GorillaUserController> ().enabled = true;
		MainCanvas.SetActive (false);
	}
}
