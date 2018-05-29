//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//
//public class Falling : MonoBehaviour {
//
//	public GameObject bulkie;
//
//	float dist = 0f; 
//	Animator bulkieAni;
//
//	void Start () {
//		bulkieAni = bulkie.GetComponent <Animator> ();
//	}
//
//	void Update () {
//		if (bulkieAni.GetBool ("isFalling")) {
//			dist += Time.deltaTime;
//			bulkieAni.SetFloat ("Dist", dist * 11);
//		} else if (!bulkieAni.GetBool ("isFalling")) {
//			dist = 0f;
//		}
//
//	}

//	void OnTriggerExit(Collider col) {   // Check if "player" is in the air or not
//		if (col.gameObject.name == "Bulkie"){
//			GameObject player = col.gameObject;
//
//			Animator anim = player.GetComponent <Animator> ();
//			anim.SetBool ("isFalling", true);
//			//print (col + "I'm out");
//		}
//	}
//
//	void OnTriggerStay(Collider col){	// Check if "player" is in the air or not
//		if(col.gameObject.name == "Bulkie"){
//			GameObject player = col.gameObject;
//
//			Animator anim = player.GetComponent <Animator> ();
//			anim.SetBool ("isFalling", false);
//			//print ("i'm in");
//		}
//	}

//}
