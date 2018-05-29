using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArrowGenerate : MonoBehaviour {

	public GameObject arrow1;
	public GameObject arrow2;
	public GameObject arrow3;
	public GameObject arrow4;
	public GameObject arrow5;
	public GameObject arrow6;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame

	void OnTriggerEnter (Collider other)
	{
		if (other.gameObject.tag == "Arrow1") {
			Destroy (arrow1.gameObject);
			arrow2.SetActive (true);
		}if (other.gameObject.tag == "Arrow2") {
			Destroy (arrow2.gameObject);
			arrow3.SetActive (true);
		}if (other.gameObject.tag == "Arrow3") {
			Destroy (arrow3.gameObject);
			arrow4.SetActive (true);
		}if (other.gameObject.tag == "Arrow4") {
			Destroy (arrow4.gameObject);
			arrow5.SetActive (true);
		}
//			if (other.gameObject.tag == "Arrow5") {
//				Destroy (arrow5.gameObject);
//				arrow6.SetActive (true);
//			}if (other.gameObject.tag == "Arrow6") {
//				Destroy (arrow6.gameObject);
//			}
		}
		

	void Update () {
		
	}
}
