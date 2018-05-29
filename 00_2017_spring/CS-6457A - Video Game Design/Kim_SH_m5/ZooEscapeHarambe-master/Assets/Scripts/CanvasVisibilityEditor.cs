using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CanvasVisibilityEditor : MonoBehaviour {

	public GameObject canvasElements;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void HideUserMessageMaze() {
		canvasElements.SetActive(false);
	}
}
