using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RagDollCon : MonoBehaviour {
	public GameObject player;

	private Animator anim;
	private CharacterController characterController;
	private Respawn reSpawn;
	Rigidbody rigidBody;
	bool isDead = false;

	// Use this for initialization
	void Start () {
		anim = GetComponent<Animator> ();
		characterController = GetComponent<CharacterController> ();
		reSpawn = GetComponent<Respawn> ();
		rigidBody = player.GetComponent<Rigidbody> ();
	}
	
	// Update is called once per frame
	void Update () {

		if (anim.GetFloat ("vely") != 0) {
			isDead = false;
		} else {
			isDead = true;
		}

		if(!isDead){
			Physics.IgnoreLayerCollision (8, 9, false);
		} else if (isDead){
			Physics.IgnoreLayerCollision (8, 9, true);

		}

		if (Input.GetKey ("k")) {
//			player.GetComponent<Rigidbody> ().isKinematic = true;
//			Physics.IgnoreLayerCollision (8, 9, true);
//			Invoke ("EnableRagdoll", 1f);

			rigidBody.velocity = Vector3.zero;
			rigidBody.angularVelocity = Vector3.zero;


			EnableRagdoll ();
			Invoke ("DisableRagDoll", 3f);
		}

//		if (Input.GetKey ("l")) {
////			player.GetComponent<Rigidbody> ().isKinematic = false;
//			DisableRagDoll ();
//		}

	}

	void EnableRagdoll(){

		if (anim.enabled) {
			anim.SetBool ("isRagDoll", true);


			foreach (Rigidbody rb in player.GetComponentsInChildren<Rigidbody>()) {
				//				if (rb.name == "mixamorig:LeftLeg" || rb.name == "mixamorig:RightLeg") {
//				Debug.Log(rb.name);
				rb.isKinematic = false; 
//				if (rb.name == "mixamorig:Hips") {
				if (rb.name == "mixamorig:Hips") {
					rb.constraints = RigidbodyConstraints.FreezePositionX | RigidbodyConstraints.FreezePositionZ; 
				}
//				rb.detectCollisions = true;
//				if (rb.GetComponent<CapsuleCollider> ()) {
//					rb.GetComponent<CapsuleCollider> ().enabled = false;
//				}
			}

//			characterController.enabled = false;
			anim.enabled = false;

//			player.GetComponent<Rigidbody> ().isKinematic = false;
		}
	}

	void DisableRagDoll(){
		
		if (!anim.enabled) {
			anim.SetBool ("isRagDoll", false);
//			Physics.IgnoreLayerCollision (8, 9, false);

			transform.position = GameObject.Find ("mixamorig:Hips").transform.position;

			foreach (Rigidbody rb in player.GetComponentsInChildren<Rigidbody>()) {
				rb.isKinematic = false;
				if (rb.name == "mixamorig:Hips") {
					rb.constraints = RigidbodyConstraints.None; 
				}

//				rb.detectCollisions = false;
//				if (rb.GetComponent<CapsuleCollider> ()) {
//					rb.GetComponent<CapsuleCollider> ().enabled = true;
//				}
			}

//			player.GetComponent<Rigidbody> ().isKinematic = false;
//			characterController.enabled = true;
			anim.enabled = true;

			reSpawn.RespawnPlayer();
//			player.GetComponent<Rigidbody> ().isKinematic = false;
		}
	}

}
