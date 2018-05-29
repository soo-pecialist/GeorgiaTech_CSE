using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventTriggerConsistant : MonoBehaviour {
//	PlasticBarrierPlayer plasticBarrier;
//
//	void Start(){
//		plasticBarrier = GetComponent<PlasticBarrierPlayer> ();
//	}
	// Update is called once per frame
	void Update () {

		//constant sounds---------
		EventManager.TriggerEvent ("Rolling");
		EventManager.TriggerEvent ("Wind");

		//collision sounds---------
		EventManager.TriggerEvent ("PlasticBar");
		EventManager.TriggerEvent ("Crate");
		EventManager.TriggerEvent ("PlasticCone");
	
	}
}
