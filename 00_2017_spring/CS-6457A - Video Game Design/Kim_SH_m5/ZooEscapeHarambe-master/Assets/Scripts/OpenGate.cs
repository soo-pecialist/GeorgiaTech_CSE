using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenGate : MonoBehaviour {

	private Transform transform;

	private bool openGate = false;
	private bool collide = false;
	private Quaternion start;
	private Quaternion target;
	public static bool test = true;
	// Use this for initialization
	void Start () {
		transform = GetComponent<Transform> ();
		start = Quaternion.identity;
		target = Quaternion.identity;
		start.eulerAngles = new Vector3 (0, 0, 0);
		start.y = transform.rotation.y;
		target.eulerAngles = new Vector3 (0, -90, 0);
	}
	
	// Update is called once per frame
	void Update () {
		if (!test)
			HelperHUDManager.text = "Find the key first!";
		if (openGate) {
			transform.rotation = Quaternion.Slerp (start, target, Time.time);
		} else if (collide) {
			if (test) 
				HelperHUDManager.text = "Press 'E' to open the gate";
			if (Input.GetKeyDown (KeyCode.E)) {
				bool hasKey = false;
				for (int i = 0; i < GorillaCharacter.inventory.Length; i++) {
					//Debug.Log (GorillaCharacter.inventory [i]);
					if (GorillaCharacter.inventory [i] == "key")
						hasKey = true;
				}
				if (hasKey) {
					openGate = true;
				} else {
					test = false;
				}
			}
		}
	}

	void OnCollisionStay(Collision col) {
		GameObject thing = col.gameObject;
		if (thing.CompareTag ("Player")) {
			collide = true;
		}
	}

	void OnCollisionExit(Collision col) {
		if (col.gameObject.CompareTag ("Player")) {
			collide = false;
		}
	}
}
