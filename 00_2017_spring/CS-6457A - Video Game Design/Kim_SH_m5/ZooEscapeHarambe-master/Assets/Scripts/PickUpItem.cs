using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PickUpItem : MonoBehaviour {

	public GameObject key;
	public GameObject harambe;


	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		float dist = Vector3.Distance (key.transform.position, harambe.transform.position);
		if (dist < 1 && key.GetComponent<MeshRenderer> ().enabled) {
			HelperHUDManager.text = "Press 'E' to pick up item.";
			OpenGate.test = true;
			if (Input.GetKeyDown(KeyCode.E)) {
				GorillaCharacter.addItem ("key");
				key.GetComponent<MeshRenderer> ().enabled = false;
			}
		} else {
			HelperHUDManager.text = "";
		}
	}
}
