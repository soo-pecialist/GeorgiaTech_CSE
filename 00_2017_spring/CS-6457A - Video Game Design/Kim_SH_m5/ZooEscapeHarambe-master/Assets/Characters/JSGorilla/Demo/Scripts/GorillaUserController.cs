using UnityEngine;
using System.Collections;

public class GorillaUserController : MonoBehaviour {
	GorillaCharacter gorillaCharacter;
    public GameObject caughtCanvas;
    private bool enableControl = true;
	void Start () {
		gorillaCharacter = GetComponent < GorillaCharacter> ();
	}
	
	void Update () {
        if (!enableControl) return;

		if (Input.GetKeyDown (KeyCode.F)) {
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
		if (Input.GetKeyDown (KeyCode.LeftShift)) {
			gorillaCharacter.Gallop();
		}				
		if (Input.GetKeyUp (KeyCode.LeftShift)) {
			gorillaCharacter.Walk();
		}		
		if (Input.GetKeyDown (KeyCode.U)) {
			gorillaCharacter.Drum();
		}	

		gorillaCharacter.forwardSpeed= gorillaCharacter.walkMode*Input.GetAxis ("Vertical");
		gorillaCharacter.turnSpeed= Input.GetAxis ("Horizontal");
	}

    public void CaughtByZookeeper()
    {
        if (caughtCanvas != null)
        {
            gorillaCharacter.Death();
            enableControl = false;
            caughtCanvas.SetActive(true);
        }
    }
}
