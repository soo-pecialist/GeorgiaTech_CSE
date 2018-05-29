using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {

	private Animator anim;


	void Start () {
		anim = GetComponent<Animator> ();
		Time.timeScale = 1;
	}
	

	void Update () {
			
		if (Input.GetAxis ("Vertical") == 0) {  // when idle, turn - 
			if(Input.GetKeyDown("a")){  		// hard turn left
				anim.SetTrigger("turnLeft");
			}
			
			if(Input.GetKeyDown("d")){ 			// hard turn right
				anim.SetTrigger("turnRight");
			}

			if (Input.GetKeyDown ("q")) {		// light turn left
				anim.SetTrigger ("lightLeft");
			}

			if (Input.GetKeyDown ("e")) {		// light turn right
				anim.SetTrigger ("lightRight");
			}
//			if (Input.GetKeyDown ("s")) {
//				anim.SetTrigger ("turn180");
//			}
		}
		Landing ();
	}

	void Landing(){								// get a hold of distance between "player" and "ground" -> trigger "landing animation"
		RaycastHit hit;
		float theDistance;

		Vector3 downward = transform.TransformDirection (-Vector3.up) * 10;
		//Debug.DrawRay (transform.position, downward, Color.green);  // To visually check the distance beween player and the ground

		if (Physics.Raycast (transform.position, (downward), out hit)) {
			theDistance = hit.distance;

			//print (theDistance + ": " + hit.collider.gameObject.name);
			if (theDistance < 1.1f && theDistance > 0.95f && anim.GetBool("isMoving") == false) {
				anim.SetTrigger ("landTrigger");
				//anim.SetBool("isLanding", true);
			} 
		}
		//anim.SetBool ("isLanding", false);


	}
}

//TODO make landing devided into several level (soft - medium - hard landing) - Dist!!
