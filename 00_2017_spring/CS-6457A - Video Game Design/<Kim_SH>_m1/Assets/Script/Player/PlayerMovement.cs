using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour {


	private Animator anim;
	private Rigidbody rigidBody;
	private float speed = 1/10f;
	private float dir = 0f;

	// Use this for initialization
	void Start () {
		anim = GetComponent<Animator> ();
		rigidBody = GetComponent<Rigidbody> ();
	}
	
	// Update is called once per frame
	void FixedUpdate () {

		GetInput (); 

		rigidBody.AddForce (transform.position);
	}

	void GetInput ()
	{
		float h = Input.GetAxis ("Horizontal");
		// [-1, 1]
		float v = Input.GetAxis ("Vertical");
		// [-1, 1]
		if (Input.GetButtonDown ("Jump")) {
			anim.SetBool ("isJumping", true);
			Invoke ("StopJumping", 0.1f);
		}
		if (Input.GetKey ("1")) {
			speed = 1 / 10f;
		}
		else
			if (Input.GetKey ("2")) {
				speed = 2 / 10f;
			}
			else
				if (Input.GetKey ("3")) {
					speed = 3 / 10f;
				}
				else
					if (Input.GetKey ("4")) {
						speed = 4 / 10f;
					}
					else
						if (Input.GetKey ("5")) {
							speed = 5 / 10f;
						}
						else
							if (Input.GetKey ("6")) {
								speed = 6 / 10f;
							}
							else
								if (Input.GetKey ("7")) {
									speed = 7 / 10f;
								}
								else
									if (Input.GetKey ("8")) {
										speed = 8 / 10f;
									}
									else
										if (Input.GetKey ("9")) {
											speed = 9 / 10f;
										}
										else
											if (Input.GetKey ("0")) {
												speed = 10 / 10f;
											}
		if (Input.GetKey ("q")) {
			dir = -0.5f;
		}
		else
			if (Input.GetKey ("e")) {
				dir = 0.5f;
			}
			else {
				dir = 0f;
			}
		anim.SetFloat ("velx", h + dir);
		anim.SetFloat ("vely", v * speed);

		if ((v*speed) != 0) {anim.SetBool ("isMoving", true);}  // if hit "w" or "s", "player" starts to move. 
			else {anim.SetBool("isMoving", false);}

	}

	void StopJumping (){
		anim.SetBool ("isJumping", false);
	}

}
