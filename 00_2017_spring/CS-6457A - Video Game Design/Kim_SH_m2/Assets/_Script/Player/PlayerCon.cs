using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCon : MonoBehaviour {

	public ParticleSystem[] particles;
	private Rigidbody rigidBody;
	private Animator anim;
	private float dist = 0f; 
	private CharacterController controller;

	private FootstepMaster_Curves FstCurve;

	void Start () {
		anim = GetComponent<Animator> ();
		Time.timeScale = 1;
		controller = GetComponent<CharacterController> ();
		FstCurve = GetComponent<FootstepMaster_Curves> ();
		rigidBody = GetComponent<Rigidbody> ();
	}
	

	void Update () {
	
		FallingDistance ();


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

	void FallingDistance (){

		if (anim.GetBool ("isFalling")) {
			dist += Time.deltaTime;
			//Debug.Log (dist * 6.5f);
			anim.SetFloat ("Dist", dist * 6.5f);
		} else if (anim.GetBool("isMoving") && !anim.GetBool ("isFalling")) {
			dist = 0f;
		}

		//try invoke in 1 second
	}

	void Landing(){								// get a hold of distance between "player" and "ground" -> trigger "landing animation"
		RaycastHit hit;
		float theDistance;

		Vector3 downward = transform.TransformDirection (-Vector3.up) * 20;
//			Debug.DrawRay (transform.position, downward, Color.green);  // To visually check the distance beween player and the ground

		if (Physics.Raycast (transform.position, (downward), out hit)) {
			theDistance = hit.distance;

			if (theDistance > 1.5f && !controller.isGrounded) {
				Invoke ("FallingInOneSec", 0.3f); // considering the speed of walking (too slow to initiate immediately)
			}	else if (theDistance <= 0.5f && controller.isGrounded) {
				anim.SetBool ("isFalling", false);
				anim.SetBool ("isLanding", false);
			}  

			//print (theDistance + ": " + hit.collider.gameObject.name);
			if (theDistance < 1.5f && theDistance > 0.8f && !controller.isGrounded) {
				float v3Velocity = rigidBody.velocity.magnitude;
				if (v3Velocity <= 0.5f) {
					anim.SetBool ("isLanding", true);
					Invoke ("CallingLandingSound", 0.2f);
				}
			}
		}
	}

	void FallingInOneSec (){
		anim.SetBool ("isFalling", true);
	}


	void CallingLandingSound(){
		FstCurve.LandingSound ();
	}
}