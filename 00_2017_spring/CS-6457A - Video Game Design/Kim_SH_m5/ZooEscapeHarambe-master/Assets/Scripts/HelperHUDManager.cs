using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HelperHUDManager : MonoBehaviour {

	public static string text;

	Text t;
	// Use this for initialization
	void Start () {
		t = GetComponent<Text> ();
		text = "";
	}
	
	// Update is called once per frame
	void Update () {
		t.text = text;
	}
}
