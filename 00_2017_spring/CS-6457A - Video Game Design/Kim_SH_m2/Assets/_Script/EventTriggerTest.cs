using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventTriggerTest : MonoBehaviour {

	void Update () {
		if (Input.GetKeyDown ("o")) {
			EventManager.TriggerEvent ("test");
		}	

		if (Input.GetKeyDown ("p")) {
			
		}
	}
}
