using UnityEngine;
using System.Collections;

public class MyGorillaUserController : MonoBehaviour {
	MyGorillaCharacter gorillaCharacter;
	
	void Start () {
		gorillaCharacter = GetComponent < MyGorillaCharacter> ();
	}
	
	void Update () {	
		if (Input.GetKeyDown(KeyCode.P)) {
			gorillaCharacter.Attack();
		}
		if (Input.GetButtonDown ("Jump")) {
			gorillaCharacter.Jump();
		}
		if (Input.GetKeyDown (KeyCode.H)) {
			gorillaCharacter.Hit();
		}
		if (Input.GetKeyDown (KeyCode.K)) {
			gorillaCharacter.Death();
		}
		if (Input.GetKeyDown (KeyCode.L)) {
			gorillaCharacter.Rebirth();
		}		
		if (Input.GetKeyDown (KeyCode.R)) {
			gorillaCharacter.Gallop();
		}				
		if (Input.GetKeyUp (KeyCode.R)) {
			gorillaCharacter.Walk();
		}		
		if (Input.GetKeyDown (KeyCode.U)) {
			gorillaCharacter.Drum();
		}
        gorillaCharacter.Gallop();
        gorillaCharacter.forwardSpeed= gorillaCharacter.walkMode*Input.GetAxis ("Vertical");
		gorillaCharacter.turnSpeed= Input.GetAxis ("Horizontal");
	}



}
