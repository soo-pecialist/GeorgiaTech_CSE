using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Falling : MonoBehaviour {

	void OnTriggerExit(Collider col) {   // Check if "player" is in the air or not
		if (col.gameObject.name == "Bulkie"){
			GameObject player = col.gameObject;

			Animator anim = player.GetComponent <Animator> ();
			anim.SetBool ("isFalling", true);
			print (col + "I'm out");
		}
	}

	void OnTriggerStay(Collider col){	// Check if "player" is in the air or not
		if(col.gameObject.name == "Bulkie"){
			GameObject player = col.gameObject;

			Animator anim = player.GetComponent <Animator> ();
			anim.SetBool ("isFalling", false);
			//print ("i'm in");
		}
	}

}
