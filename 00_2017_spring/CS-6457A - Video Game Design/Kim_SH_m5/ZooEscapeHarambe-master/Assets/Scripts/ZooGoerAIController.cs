using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZooGoerAIController : MonoBehaviour {

    Rigidbody rb;
	// Use this for initialization
	void Start () {
        rb = GetComponent<Rigidbody>();
	}
	
	// Update is called once per frame
	void Update () {
        var vel = rb.velocity;
	}
}
